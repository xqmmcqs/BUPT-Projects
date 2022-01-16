/**
 * @file      client.cpp
 * @brief     客户端
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是客户端类接口的实现。
*/

#include "client.h"

#include <QJsonDocument>
#include <QNetworkDatagram>

QJsonObject Client::sendMessage(const QJsonObject &msg)
{
    QJsonObject ret;
    qDebug() << QByteArray(QJsonDocument(msg).toJson(QJsonDocument::Compact));
    if (socket.writeDatagram(QByteArray(QJsonDocument(msg).toJson(QJsonDocument::Compact)), QHostAddress::LocalHost,
                             port) == -1)
    {
        ret.insert("status", false);
        ret.insert("payload", "UDP socket错误");
        return ret;
    }
    socket.waitForReadyRead();
    QNetworkDatagram datagram = socket.receiveDatagram();
    qDebug() << QJsonDocument::fromJson(datagram.data()).object();
    return QJsonDocument::fromJson(datagram.data()).object();
}

QJsonObject Client::login(const QJsonObject &msg)
{
    QJsonObject req;
    req.insert("type", USER_LOGIN);
    req.insert("payload", msg);
    auto ret = sendMessage(req);
    if (ret["status"].toBool() == true)
    {
        token = ret["payload"].toString();
        ret.remove("token");
    }
    return ret;
}

void Client::logout()
{
    QJsonObject msg;
    msg.insert("token", token);
    QJsonObject req;
    req.insert("type", USER_LOGOUT);
    req.insert("payload", msg);
    sendMessage(req);
    token = "";
}

QJsonObject Client::request(RequestType type, QJsonObject &msg, bool needToken)
{
    if (needToken) msg.insert("token", token);
    QJsonObject req;
    req.insert("type", type);
    req.insert("payload", msg);
    return sendMessage(req);
}
