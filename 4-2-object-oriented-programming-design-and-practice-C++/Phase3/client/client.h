/**
 * @file      client.h
 * @brief     客户端
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件定义了客户端类。客户端通过socket与服务端通信，客户端类中保存登录用户的凭据，如果有需要鉴权的操作，则将凭据附加在json报文中。
*/

#ifndef PHASE3_CLIENT_H
#define PHASE3_CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QJsonObject>

/// 客户端类
class Client : public QObject
{
Q_OBJECT
public:
    enum RequestType
    {
        USER_LOGIN,
        USER_REGISTER,
        USER_LOGOUT,
        USER_GET_INFO,
        USER_SET_PASSWD,
        USER_ADD_MONEY,
        USER_SUB_MONEY,
        ITEM_GET_ALL,
        ITEM_GET_MINE,
        ITEM_GET_DETAIL,
        ITEM_GET_FILTER,
        ITEM_SET,
        ITEM_ADD,
        ITEM_DELETE,
        ITEM_DISCOUNT,
        CART_GET,
        CART_ADD,
        CART_SET,
        CART_DELETE,
        ORDER_ADD,
        ORDER_GET_MINE,
        ORDER_GET_DETAIL,
        ORDER_CANCEL,
        ORDER_PAY,
        ORDER_REFUND
    };
    
    Client(QObject * parent, quint16 _port) : QObject(parent), socket(this), port(_port), token{}
    {}
    
    /// 登录
    QJsonObject login(const QJsonObject &msg);
    
    /// 登出
    void logout();
    
    /**
     * @brief 发送请求
     * @param type 请求种类
     * @param msg 报文
     * @param needToken 是否需要附加token
     * @return 服务端的返回报文
     */
    QJsonObject request(RequestType type, QJsonObject &msg, bool needToken = false);

private:
    /**
     * @brief 向服务端发送报文
     * @param msg 报文
     * @return 服务端的返回报文
     */
    QJsonObject sendMessage(const QJsonObject &msg);
    
    QUdpSocket socket; ///< 与服务端通信的socket
    quint16 port; ///< 服务端端口
    QString token; ///< 用户凭据
};

#endif // PHASE3_CLIENT_H
