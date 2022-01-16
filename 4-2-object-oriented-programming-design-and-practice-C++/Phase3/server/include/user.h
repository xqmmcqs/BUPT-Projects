/**
 * @file      user.h
 * @brief     用户和用户管理
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中定义了用户类和用户管理类。
 *
 * 用户类包括一个用户基类，以及商家、消费者两个派生类。
 *
 * 用户类中保存用户余额。消费者类中还保存了购物车信息（商品ID、商品数量），并且定义了查询购物车中商品数量、添加到购物车、从购物车中移除的接口。
 *
 * 商品管理类中保存当前已经登录的用户，由于大多数操作都需要对用户信息进行鉴权，所以商品管理类中定义了系统中所有操作的接口，对于需要鉴权的操作，
 * 进行鉴权后执行相应的操作或调用商品管理类、订单管理类中的相应接口。
 *
 * 对于订单的操作中，付款、取消操作针对主订单，退款操作针对子订单，且退款操作需要由消费者先发起，之后由商家确认。
*/

#ifndef PHASE3_USER_H
#define PHASE3_USER_H

#include <QString>
#include <QJsonValue>

#include "database.h"
#include "commodity.h"

/// 用户基类
class User
{
protected:
    int balance; ///< 余额

public:
    enum USER_TYPE
    {
        MERCHANT, CUSTOMER
    };
    
    User(int _balance) : balance(_balance)
    {}
    
    virtual USER_TYPE getUserType() const = 0;
    
    int getBalance() const
    { return balance; }
    
    void addBalance(int num)
    { balance += num; }
    
    virtual ~User() = default;
};

/// 消费者用户子类
class Customer : public User
{
private:
    QMap<int, int> cart; ///< 购物车

public:
    Customer(int _balance) : User(_balance)
    {}
    
    USER_TYPE getUserType() const override
    { return CUSTOMER; };
    
    /**
     * @brief 查询购物车中某件商品的数量
     * @param id 商品ID
     * @return 商品的数量
     */
    int getItemNum(int id) const;
    
    /**
     * @brief 向购物车中添加指定数量的商品
     * @param id 商品ID
     * @param number 商品数量
     */
    void addToCart(int id, int number);
    
    /**
     * @brief 从购物车中移除指定数量的商品
     * @param id 商品ID
     * @param number 商品数量
     */
    void removeFromCart(int id, int number);
    
    /**
     * @brief 从购物车中移除商品
     * @param id 商品ID
     */
    void eraseFromCart(int id);
    
    const QMap<int, int> &getCart() const
    { return cart; }
};

/// 商家用户子类
class Merchant : public User
{
public:
    Merchant(int _balance) : User(_balance)
    {}
    
    USER_TYPE getUserType() const override
    { return MERCHANT; };
};

/**
 * @brief 用户管理类
 *
 * 成员函数接口中如果有token，表示一个凭据，格式应当为：
 * ```json
 * {
 *     "username": <用户名>,
 *     "iss": "Ziheng Mao"
 * }
 * ```
 * 如果操作正常完成，成员函数返回一个空串，否则返回错误信息。
 */
class UserManage
{
private:
    QMap<QString, QSharedPointer<User>> map; ///< 用户名向用户实例的映射，只有已登录的用户才会创建实例
    Database * database;
    CommodityManage * commodityManage;
    OrderManage * orderManage;
    
    /**
     * @brief 用户鉴权
     * @param token 凭据
     * @return 如果鉴权成功，返回用户名，否则返回空串
     */
    QString verify(const QJsonObject &token) const;
    
    /**
     * @brief 减少一个用户的余额，增加另一个用户的余额
     * @param token 第一个用户的凭据
     * @param balance 减少的余额
     * @param anotherUser 另一个用户的用户名
     * @return 如果修改余额成功，返回true
     */
    bool changeBalance(const QJsonObject &token, int balance, const QString &anotherUser) const;
    
    static void constructCommodityJson(const QSharedPointer<Commodity> &commodity, QJsonObject &object);

public:
    UserManage(Database * _database, CommodityManage * _commodityManage, OrderManage * _orderManage) :
            database(_database), commodityManage(_commodityManage), orderManage(_orderManage)
    {}
    
    QString registerUser(const QString &username, const QString &passwd, bool type) const;
    
    /**
     * @brief 登录
     * @param username 用户名
     * @param passwd 密码
     * @param ret 生成的凭据
     * @return 如果登录成功，返回空串，否则返回错误信息
     */
    QString login(const QString &username, const QString &passwd, QJsonObject &ret);
    
    QString changePasswd(const QJsonObject &token, const QString &passwd) const;
    
    /**
     * @brief 获取用户信息
     * @param token 凭据
     * @param ret 用户信息
     * @return 如果获取成功，返回空串，否则返回错误信息
     *
     * 用户信息的格式：
     * ```json
     * {
     *    "username": <字符串>,
     *    "type": <整数>,
     *    "balance": <整数>
     * }
     * ```
     */
    QString getInfo(const QJsonObject &token, QJsonObject &ret) const;
    
    QString changeBalance(const QJsonObject &token, int balance) const;
    
    QString logout(const QJsonObject &token);
    
    /**
     * @brief 添加商品
     * @param token 凭据
     * @param info 商品信息
     * @return 如果添加成功，返回空串，否则返回错误信息
     *
     * 商品信息的格式：
     * ```json
     * {
     *     "name": <字符串>,
     *     "intro": <字符串>,
     *     "type": <整数>,
     *     "price": <整数>,
     *     "amount": <整数>
     * }
     * ```
     */
    QString addCommodity(const QJsonObject &token, const QJsonObject &info) const;
    
    /**
     * @brief 修改商品
     * @param token 凭据
     * @param info 商品信息
     * @return 如果修改成功，返回空串，否则返回错误信息
     *
     * 商品信息的格式：
     * ```json
     * {
     *     "name": <字符串>,
     *     "intro": <字符串（可选）>,
     *     "type": <整数（可选）>,
     *     "price": <整数（可选）>,
     *     "amount": <整数（可选）>
     * }
     * ```
     */
    QString changeCommodity(const QJsonObject &token, const QJsonObject &info) const;
    
    /**
     * @brief 查询商品
     * @param info 查询信息
     * @param ret 查询结果
     * @return 如果查询成功，返回空串，否则返回错误信息
     *
     * 查询信息的格式分为四种：
     *
     * 1. 查询所有商品
     * ```json
     * {
     *     "TYPE": 0,
     * }
     * ```
     * 2. 按ID查询商品
     * ```json
     * {
     *     "TYPE": 1,
     *     "id": <整数>
     * }
     * ```
     * 3. 按筛选查询商品
     * ```json
     * {
     *     "TYPE": 2,
     *     "name": <字符串（可选）>,
     *     "type": <整数（可选）>,
     *     "min_price": <整数（可选）>,
     *     "max_price": <整数（可选）>,
     *     "remain": <整数（可选）>
     * }
     * ```
     * 4. 按商家查询商品
     * ```json
     * {
     *     "TYPE": 3,
     *     "token": <凭据>,
     * }
     * ```
     *
     * 查询结果的格式：
     * ```json
     * [
     *     {
     *         "id": <整数>,
     *         "name": <字符串>,
     *         "intro": <字符串>,
     *         "type": <整数>,
     *         "belong": <字符串>,
     *         "price": <整数>,
     *         "discount": <整数>,
     *         "amount": <整数>
     *     },
     *     ...
     * ]
     * ```
     */
    QString queryCommodity(const QJsonObject &info, QJsonArray &ret) const;
    
    static QString discount(int type, int discount);
    
    QString deleteCommodity(const QJsonObject &token, int id) const;
    
    /**
     * @brief 查询购物车中的商品
     * @param token 凭据
     * @param ret 查询结果
     * @return 如果查询成功，返回空串，否则返回错误信息
     *
     * 查询结果的格式：
     * ```json
     * [
     *     {
     *         "id": <整数>,
     *         "name": <字符串>,
     *         "type": <整数>,
     *         "price": <整数>,
     *         "discount": <整数>,
     *         "amount": <整数>
     *     },
     *     ...
     * ]
     * ```
     */
    QString viewCart(const QJsonObject &token, QJsonArray &ret) const;
    
    QString addToCart(const QJsonObject &token, int id, int number) const;
    
    QString modifyCart(const QJsonObject &token, int id, int number) const;
    
    QString removeFromCart(const QJsonObject &token, int id) const;
    
    /**
     * @brief 生成订单
     * @param token 凭据
     * @param info 订单信息
     * @return 如果生成成功，返回空串，否则返回错误信息
     *
     * 订单信息的格式：
     * ```json
     * [
     *     <整数>,
     *     ...
     * ]
     * ```
     */
    QString generateOrder(const QJsonObject &token, const QJsonArray &info) const;
    
    /**
     * @brief 通过用户名查询订单
     * @param token 凭据
     * @param ret 查询结果
     * @return 如果查询成功，返回空串，否则返回错误信息
     *
     * 查询结果的格式：
     * ```json
     * [
     *     {
     *         "id": <整数>,
     *         "total": <整数>
     *     },
     *     ...
     * ]
     * ```
     */
    QString viewOrderByName(const QJsonObject &token, QJsonArray &ret) const;
    
    /**
     * @brief 通过订单ID查询订单
     * @param token 凭据
     * @param id 订单ID
     * @param ret 查询结果
     * @return 如果查询成功，返回空串，否则返回错误信息
     *
     * 查询结果的格式：
     * ```json
     * {
     *     "id": <整数>,
     *     "total": <整数>,
     *     "status": <整数>,
     *     "order": [
     *         {
     *             "id": <整数>,
     *             "mainid": <整数>,
     *             "status": <整数>,
     *             "total": <整数>,
     *             "buyer": <字符串>,
     *             "seller": <字符串>
     *         },
     *         ...
     *     ],
     *     "commodity": [
     *         {
     *             "id": <整数>,
     *             "name": <字符串>,
     *             "type": <整数>,
     *             "amount": <整数>,
     *             "order": <整数>
     *         },
     *         ...
     *     ]
     * ```
     */
    QString viewOrderDetails(const QJsonObject &token, int id, QJsonObject &ret) const;
    
    QString cancelOrder(const QJsonObject &token, int id) const;
    
    QString payOrder(const QJsonObject &token, int id) const;
    
    QString refundOrder(const QJsonObject &token, int id) const;
};

#endif // PHASE3_USER_H
