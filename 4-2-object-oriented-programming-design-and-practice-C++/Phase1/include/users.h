/**
 * @file      users.h
 * @brief     用户和用户管理
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中定义了用户类和用户管理类。
 *
 * 用户类包括一个用户基类，以及商家、消费者两个派生类。
 *
 * 用户类中保存用户余额。
 *
 * 商品管理类中保存当前已经登录的用户，由于大多数操作都需要对用户信息进行鉴权，所以商品管理类中定义了系统中所有操作的接口，对于需要鉴权的操作，
 * 进行鉴权后执行相应的操作或调用商品管理类中的相应接口。
*/

#ifndef PHASE1_USERS_H
#define PHASE1_USERS_H

#include <QString>
#include <QJsonValue>

#include "database.h"
#include "commodity.h"

class User
{
protected:
    int balance;

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

class Customer : public User
{
public:
    Customer(int _balance) : User(_balance)
    {}
    
    USER_TYPE getUserType() const override
    { return CUSTOMER; };
};

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
 *
 * - username 一个字符串
 * - iss Ziheng Mao
 *
 * 如果操作正常完成，成员函数返回一个空串，否则返回错误信息。
 */
class UserManage
{
private:
    QMap<QString, QSharedPointer<User>> map; ///< 用户名向用户实例的映射，只有已登录的用户才会创建实例
    Database * database;
    CommodityManage * commodityManage;
    
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
    UserManage(Database * _database, CommodityManage * _commodityManage) : database(_database),
                                                                           commodityManage(_commodityManage)
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
     *
     * - username 一个字符串
     * - type 一个整数
     * - balance 一个整数
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
     *
     * - name 一个字符串
     * - intro 一个字符串
     * - type 一个整数
     * - price 一个整数
     * - amount 一个整数
     */
    QString addCommodity(const QJsonObject &token, const QJsonObject &info) const;
    
    /**
     * @brief 修改商品
     * @param token 凭据
     * @param info 商品信息
     * @return 如果修改成功，返回空串，否则返回错误信息
     *
     * 商品信息的格式：
     *
     * - id 一个整数
     * - name 可选，一个字符串
     * - intro 可选，一个字符串
     * - price 可选，一个整数
     * - amount 可选，一个整数
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
     * 1. - TYPE 0 表示查询所有商品
     * 2. - TYPE 1 表示按ID查询商品
     *    - id 一个整数
     * 3. - TYPE 2 表示按筛选查询商品
     *    - name 可选，一个字符串
     *    - type 可选，一个整数
     *    - min_price 可选，一个整数
     *    - max_price 可选，一个整数
     *    - remain 可选，一个整数
     * 4. - TYPE 3 表示按商家查询商品
     *    - token 凭据
     *
     * 查询结果的格式：
     *
     * 一个列表，列表中的元素是一个字典：
     * - id 一个整数
     * - name 一个字符串
     * - intro 一个字符串
     * - type 一个整数
     * - belong 一个字符串
     * - price 一个整数
     * - discount 一个整数
     * - amount 一个整数
     */
    QString queryCommodity(const QJsonObject &info, QJsonArray &ret) const;
    
    static QString discount(int type, int discount);
    
    QString deleteCommodity(const QJsonObject &token, int id) const;
};

#endif // PHASE1_USERS_H
