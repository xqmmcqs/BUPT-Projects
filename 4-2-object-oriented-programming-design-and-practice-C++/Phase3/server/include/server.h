/**
 * @file      server.h
 * @brief     服务端
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中定义了服务端类。
 *
 * 服务端类从本地端口中创建UDP socket，并从其中接收来自客户端的报文，分析报文类型后，将报文内容转化为JSON格式，调用相应的处理函数。
 *
 * 如有必要，处理函数首先分析JWT token的内容，进行初步鉴权，之后进行初步的参数处理并调用用户管理类中的相应接口。
 *
 * 处理完成后，处理函数将回复打包成字节流，并且通过UDP socket发回客户端。
 *
 * 服务端类中定义了JWT token编码、验证、提取信息的三个工具函数接口，以及三个打包返回信息的工具函数接口。
*/

#include <QObject>
#include <QUdpSocket>
#include <QJsonValue>

#include "user.h"

#ifndef PHASE3_SERVER_H
#define PHASE3_SERVER_H

/// 服务器类
class Server : public QObject
{
Q_OBJECT
public:
    Server(QObject * parent, quint16 _port, UserManage * _usermanage);

private:
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
    
    void messageHandler();
    
    /**
     * @brief 将凭据打包成JWT token字符串
     * @param payload 凭据
     * @param secret 密钥
     * @return JWT token字符串
     */
    static QString jwtEncoding(const QJsonObject &payload, const QByteArray &secret);
    
    /**
     * @brief 验证JWT token的指纹
     * @param jwt JWT token
     * @param secret 密钥
     * @return 如果验证成功，返回true
     */
    static bool jwtVerify(const QString &jwt, const QByteArray &secret);
    
    /**
     * @brief 从JWT token字符串中提取凭据
     * @param jwt JWT token
     * @return 凭据
     */
    static QJsonObject jwtGetPayload(const QString &jwt);
    
    /**
     * @brief 构造仅包含字段错误信息的回复
     * @param ret 回复
     */
    static void constructRet(QJsonObject &ret);
    
    /**
     * @brief 构造回复
     * @param ret 回复
     * @response 用户管理类的处理结果
     *
     * 如果response为空，则回复状态正常，否则回复状态异常
     */
    static void constructRet(QJsonObject &ret, const QString &response);
    
    /**
     * @brief 构造回复
     * @param ret 回复
     * @param response 用户管理类的处理结果
     * @param result 用户管理类的回复信息
     *
     * 如果response为空，则回复状态正常，并且把result附加到回复中，否则回复状态异常
     */
    static void constructRet(QJsonObject &ret, const QString &response, const QJsonValue &result);
    
    QByteArray userLoginHandler(const QJsonObject &payload) const;
    
    QByteArray userRegisterHandler(const QJsonObject &payload) const;
    
    QByteArray userLogoutHandler(const QJsonObject &payload) const;
    
    QByteArray userGetInfoHandler(const QJsonObject &payload) const;
    
    QByteArray userSetPasswdHandler(const QJsonObject &payload) const;
    
    QByteArray userAddMoneyHandler(const QJsonObject &payload) const;
    
    QByteArray userSubMoneyHandler(const QJsonObject &payload) const;
    
    QByteArray itemGetAllHandler() const;
    
    QByteArray itemGetMineHandler(const QJsonObject &payload) const;
    
    QByteArray itemGetDetailHandler(const QJsonObject &payload) const;
    
    QByteArray itemGetFilterHandler(const QJsonObject &payload) const;
    
    QByteArray itemSetHandler(const QJsonObject &payload) const;
    
    QByteArray itemAddHandler(const QJsonObject &payload) const;
    
    QByteArray itemDeleteHandler(const QJsonObject &payload) const;
    
    QByteArray itemDiscountHandler(const QJsonObject &payload) const;
    
    QByteArray cartGetHandler(const QJsonObject &payload) const;
    
    QByteArray cartAddHandler(const QJsonObject &payload) const;
    
    QByteArray cartSetHandler(const QJsonObject &payload) const;
    
    QByteArray cartDeleteHandler(const QJsonObject &payload) const;
    
    QByteArray orderAddHandler(const QJsonObject &payload) const;
    
    QByteArray orderGetMineHandler(const QJsonObject &payload) const;
    
    QByteArray orderGetDetailHandler(const QJsonObject &payload) const;
    
    QByteArray orderCancelHandler(const QJsonObject &payload) const;
    
    QByteArray orderPayHandler(const QJsonObject &payload) const;
    
    QByteArray orderRefundHandler(const QJsonObject &payload) const;
    
    UserManage * userManage;
    QUdpSocket socket;
    const QByteArray secret = "thisisasecret"; ///< JWT token 加密密钥
};

#endif // PHASE3_SERVER_H
