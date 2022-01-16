/**
 * @file      server.cpp
 * @brief     服务端
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是服务端类的实现。
*/

#include <QNetworkDatagram>
#include <QJsonDocument>

#include "../include/server.h"

Server::Server(QObject * parent, quint16 port, UserManage * _usermanage)
        : QObject(parent), userManage(_usermanage), socket(this)
{
    socket.bind(QHostAddress::LocalHost, port);
    QObject::connect(&socket, &QUdpSocket::readyRead, this, &Server::messageHandler);
}

void Server::messageHandler()
{
    while (socket.hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket.receiveDatagram();
        QJsonObject json = QJsonDocument::fromJson(datagram.data()).object();
        QByteArray response;
        if (!json.contains("type") || !json.contains("payload"))continue;
        auto type = json["type"].toInt();
        if (type == USER_LOGIN)
            response = userLoginHandler(json["payload"].toObject());
        else if (type == USER_REGISTER)
            response = userRegisterHandler(json["payload"].toObject());
        else if (type == USER_LOGOUT)
            response = userLogoutHandler(json["payload"].toObject());
        else if (type == USER_GET_INFO)
            response = userGetInfoHandler(json["payload"].toObject());
        else if (type == USER_SET_PASSWD)
            response = userSetPasswdHandler(json["payload"].toObject());
        else if (type == USER_ADD_MONEY)
            response = userAddMoneyHandler(json["payload"].toObject());
        else if (type == USER_SUB_MONEY)
            response = userSubMoneyHandler(json["payload"].toObject());
        else if (type == ITEM_GET_ALL)
            response = itemGetAllHandler();
        else if (type == ITEM_GET_MINE)
            response = itemGetMineHandler(json["payload"].toObject());
        else if (type == ITEM_GET_DETAIL)
            response = itemGetDetailHandler(json["payload"].toObject());
        else if (type == ITEM_GET_FILTER)
            response = itemGetFilterHandler(json["payload"].toObject());
        else if (type == ITEM_SET)
            response = itemSetHandler(json["payload"].toObject());
        else if (type == ITEM_ADD)
            response = itemAddHandler(json["payload"].toObject());
        else if (type == ITEM_DELETE)
            response = itemDeleteHandler(json["payload"].toObject());
        else if (type == ITEM_DISCOUNT)
            response = itemDiscountHandler(json["payload"].toObject());
        else if (type == CART_GET)
            response = cartGetHandler(json["payload"].toObject());
        else if (type == CART_ADD)
            response = cartAddHandler(json["payload"].toObject());
        else if (type == CART_SET)
            response = cartSetHandler(json["payload"].toObject());
        else if (type == CART_DELETE)
            response = cartDeleteHandler(json["payload"].toObject());
        else if (type == ORDER_ADD)
            response = orderAddHandler(json["payload"].toObject());
        else if (type == ORDER_GET_MINE)
            response = orderGetMineHandler(json["payload"].toObject());
        else if (type == ORDER_GET_DETAIL)
            response = orderGetDetailHandler(json["payload"].toObject());
        else if (type == ORDER_CANCEL)
            response = orderCancelHandler(json["payload"].toObject());
        else if (type == ORDER_PAY)
            response = orderPayHandler(json["payload"].toObject());
        else if (type == ORDER_REFUND)
            response = orderRefundHandler(json["payload"].toObject());
        auto status = socket.writeDatagram(response, datagram.senderAddress(), datagram.senderPort());
        if (status == -1)
            qCritical() << "UDP socket出错";
    }
}

QString Server::jwtEncoding(const QJsonObject &payload, const QByteArray &secret)
{
    QJsonObject header;
    header.insert("alg", "HS256");
    header.insert("typ", "jwt");
    QByteArray header_encoded = QByteArray(QJsonDocument(header).toJson(QJsonDocument::Compact)).toBase64();
    QByteArray payload_encoded = QByteArray(QJsonDocument(payload).toJson(QJsonDocument::Compact)).toBase64();
    QByteArray sig = QMessageAuthenticationCode::hash(header_encoded + "." + payload_encoded, secret,
                                                      QCryptographicHash::Sha256).toBase64();
    return QString(header_encoded + '.' + payload_encoded + '.' + sig);
}

bool Server::jwtVerify(const QString &jwt, const QByteArray &secret)
{
    auto splited = jwt.split('.');
    auto header_encoded = splited[0].toUtf8();
    auto payload_encoded = splited[1].toUtf8();
    auto sig = splited[2].toUtf8();
    return sig == QMessageAuthenticationCode::hash(header_encoded + "." + payload_encoded, secret,
                                                   QCryptographicHash::Sha256).toBase64();
}

QJsonObject Server::jwtGetPayload(const QString &jwt)
{
    auto splited = jwt.split('.');
    auto payload = QByteArray::fromBase64(splited[1].toUtf8());
    return QJsonDocument::fromJson(payload).object();
}

void Server::constructRet(QJsonObject &ret)
{
    ret.insert("status", false);
    ret.insert("payload", "字段有误");
}

void Server::constructRet(QJsonObject &ret, const QString &response)
{
    if (response.isEmpty())
    {
        ret.insert("status", true);
    }
    else
    {
        ret.insert("status", false);
        ret.insert("payload", response);
    }
}

void Server::constructRet(QJsonObject &ret, const QString &response, const QJsonValue &result)
{
    if (response.isEmpty())
    {
        ret.insert("status", true);
        ret.insert("payload", result);
    }
    else
    {
        ret.insert("status", false);
        ret.insert("payload", response);
    }
}

QByteArray Server::userLoginHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    QJsonObject token;
    if (!payload.contains("username") || !payload.contains("password"))
        constructRet(ret);
    else
    {
        auto response = userManage->login(payload["username"].toString(), payload["password"].toString(), token);
        if (response.isEmpty())
        {
            ret.insert("status", true);
            ret.insert("payload", jwtEncoding(token, secret));
        }
        else
        {
            ret.insert("status", false);
            ret.insert("payload", response);
        }
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::userRegisterHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("username") || !payload.contains("password") || !payload.contains("type"))
        constructRet(ret);
    else
    {
        auto response = userManage->registerUser(payload["username"].toString(), payload["password"].toString(),
                                                 payload["type"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::userLogoutHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret))
        constructRet(ret);
    else
    {
        auto response = userManage->logout(jwtGetPayload(payload["token"].toString()));
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::userGetInfoHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    QJsonObject result;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret))
        constructRet(ret);
    else
    {
        auto response = userManage->getInfo(jwtGetPayload(payload["token"].toString()), result);
        constructRet(ret, response, result);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::userSetPasswdHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("password"))
        constructRet(ret);
    else
    {
        auto response = userManage->changePasswd(jwtGetPayload(payload["token"].toString()),
                                                 payload["password"].toString());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::userAddMoneyHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("money"))
        constructRet(ret);
    else
    {
        auto response = userManage->changeBalance(jwtGetPayload(payload["token"].toString()), payload["money"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::userSubMoneyHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("money"))
        constructRet(ret);
    else
    {
        auto response = userManage->changeBalance(jwtGetPayload(payload["token"].toString()),
                                                  -payload["money"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemGetAllHandler() const
{
    QJsonObject ret;
    QJsonObject msg;
    QJsonArray result;
    msg.insert("TYPE", 0);
    auto response = userManage->queryCommodity(msg, result);
    constructRet(ret, response, result);
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemGetMineHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret))
        constructRet(ret);
    else
    {
        QJsonObject msg;
        QJsonArray result;
        msg.insert("TYPE", 3);
        msg.insert("token", jwtGetPayload(payload["token"].toString()));
        auto response = userManage->queryCommodity(msg, result);
        constructRet(ret, response, result);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemGetDetailHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    QJsonObject msg(payload);
    QJsonArray result;
    msg.insert("TYPE", 1);
    auto response = userManage->queryCommodity(msg, result);
    constructRet(ret, response, result);
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemGetFilterHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    QJsonObject msg(payload);
    QJsonArray result;
    msg.insert("TYPE", 2);
    auto response = userManage->queryCommodity(msg, result);
    constructRet(ret, response, result);
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemSetHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret))
        constructRet(ret);
    else
    {
        auto response = userManage->changeCommodity(jwtGetPayload(payload["token"].toString()),
                                                    payload["info"].toObject());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemAddHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret))
        constructRet(ret);
    else
    {
        auto response = userManage->addCommodity(jwtGetPayload(payload["token"].toString()),
                                                 payload["info"].toObject());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemDeleteHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id"))
        constructRet(ret);
    else
    {
        auto response = userManage->deleteCommodity(jwtGetPayload(payload["token"].toString()), payload["id"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::itemDiscountHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("type") || !payload.contains("discount"))
        constructRet(ret);
    else
    {
        auto response = userManage->discount(payload["type"].toInt(), payload["discount"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::cartGetHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    QJsonArray result;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret))
        constructRet(ret);
    else
    {
        auto response = userManage->viewCart(jwtGetPayload(payload["token"].toString()), result);
        constructRet(ret, response, result);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::cartAddHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id") ||
        !payload.contains("number"))
        constructRet(ret);
    else
    {
        auto response = userManage->addToCart(jwtGetPayload(payload["token"].toString()), payload["id"].toInt(),
                                              payload["number"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::cartSetHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id") ||
        !payload.contains("number"))
        constructRet(ret);
    else
    {
        auto response = userManage->modifyCart(jwtGetPayload(payload["token"].toString()), payload["id"].toInt(),
                                               payload["number"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::cartDeleteHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id"))
        constructRet(ret);
    else
    {
        auto response = userManage->removeFromCart(jwtGetPayload(payload["token"].toString()), payload["id"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::orderAddHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("info"))
        constructRet(ret);
    else
    {
        auto response = userManage->generateOrder(jwtGetPayload(payload["token"].toString()),
                                                  payload["info"].toArray());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::orderGetMineHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    QJsonArray result;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret))
        constructRet(ret);
    else
    {
        auto response = userManage->viewOrderByName(jwtGetPayload(payload["token"].toString()), result);
        constructRet(ret, response, result);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::orderGetDetailHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    QJsonObject result;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id"))
        constructRet(ret);
    else
    {
        auto response = userManage->viewOrderDetails(jwtGetPayload(payload["token"].toString()), payload["id"].toInt(),
                                                     result);
        constructRet(ret, response, result);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::orderCancelHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id"))
        constructRet(ret);
    else
    {
        auto response = userManage->cancelOrder(jwtGetPayload(payload["token"].toString()), payload["id"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::orderPayHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id"))
        constructRet(ret);
    else
    {
        auto response = userManage->payOrder(jwtGetPayload(payload["token"].toString()), payload["id"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}

QByteArray Server::orderRefundHandler(const QJsonObject &payload) const
{
    QJsonObject ret;
    if (!payload.contains("token") || !jwtVerify(payload["token"].toString(), secret) || !payload.contains("id"))
        constructRet(ret);
    else
    {
        auto response = userManage->refundOrder(jwtGetPayload(payload["token"].toString()), payload["id"].toInt());
        constructRet(ret, response);
    }
    return QByteArray(QJsonDocument(ret).toJson(QJsonDocument::Compact));
}
