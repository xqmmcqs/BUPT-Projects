/**
 * @file      clientmodel.h
 * @brief     客户端数据结构
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件定义了在Qml中使用到的数据结构，以及供Qml调用的操作的接口。
 *
 * 一般情况下，调用接口后，客户端需等到服务端的回复，如果没有错误，接口会返回一个空字符串；否则接口会返回报错信息。
*/

#ifndef PHASE3_CLIENTMODEL_H
#define PHASE3_CLIENTMODEL_H

#include<QHostAddress>
#include<QObject>

#include"client.h"

/// 用户数据结构
class UserModel : public QObject
{
Q_OBJECT
    Q_PROPERTY(QString username READ username CONSTANT)
    Q_PROPERTY(int type READ type CONSTANT)
    Q_PROPERTY(int balance READ balance CONSTANT)

public:
    UserModel(QString _username, int _type, int _balance) : m_username(std::move(_username)), m_type(_type),
                                                            m_balance(_balance)
    {}
    
    QString username() const
    { return m_username; }
    
    int type() const
    { return m_type; }
    
    int balance() const
    { return m_balance; }

private:
    QString m_username; ///< 用户名
    int m_type; ///< 用户类型
    int m_balance; ///< 用户余额
};

/// 商品数据结构
class CommodityModel : public QObject
{
Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString intro READ intro CONSTANT)
    Q_PROPERTY(int type READ type CONSTANT)
    Q_PROPERTY(int price READ price CONSTANT)
    Q_PROPERTY(int discount READ discount CONSTANT)
    Q_PROPERTY(int amount READ amount CONSTANT)
    Q_PROPERTY(int order READ order WRITE setOrder NOTIFY orderChanged)

public:
    // 商品详情页
    CommodityModel(int _id, QString _name, QString _intro, int _type, int _price, int _discount, int _amount) :
            m_id(_id), m_name(std::move(_name)), m_intro(std::move(_intro)), m_type(_type), m_price(_price),
            m_discount(_discount), m_amount(_amount)
    {}
    
    CommodityModel(int _id, QString _name, int _type, int _price, int _discount, int _amount) : m_id(_id),
                                                                                                m_name(std::move(
                                                                                                        _name)),
                                                                                                m_type(_type),
                                                                                                m_price(_price),
                                                                                                m_discount(_discount),
                                                                                                m_amount(_amount),
                                                                                                m_order(0)
    {}
    
    // 订单页
    CommodityModel(int _id, QString _name, int _type, int _amount, int _order) : m_id(_id),
                                                                                 m_name(std::move(_name)),
                                                                                 m_type(_type), m_amount(_amount),
                                                                                 m_order(_order)
    {}
    
    int id() const
    { return m_id; }
    
    QString name() const
    { return m_name; }
    
    QString intro() const
    { return m_intro; }
    
    int type() const
    { return m_type; }
    
    int price() const
    { return m_price; }
    
    int discount() const
    { return m_discount; }
    
    int amount() const
    { return m_amount; }
    
    int order() const
    { return m_order; }
    
    void setOrder(int _order)
    {
        m_order = _order;
        emit orderChanged();
    }

Q_SIGNALS:
    
    void orderChanged();

private:
    int m_id; ///< 商品ID
    QString m_name; ///< 商品名
    QString m_intro; ///< 商品介绍
    int m_type; ///< 商品类型
    int m_price; ///< 商品价格
    int m_discount; ///< 商品折扣
    int m_amount; ///< 商品数量
    int m_order; ///< 订单页，表示商品所属订单；购物车页，表示商品是否被选中
};

/// 订单数据结构
class OrderModel : public QObject
{
Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(int mainid READ mainid CONSTANT)
    Q_PROPERTY(int status READ status CONSTANT)
    Q_PROPERTY(int total READ total CONSTANT)
    Q_PROPERTY(QString buyer READ buyer CONSTANT)
    Q_PROPERTY(QString seller READ seller CONSTANT)

public:
    OrderModel(int _id, int _mainid, int _status, int _total, QString _buyer, QString _seller) : m_id(_id),
                                                                                                 m_mainid(_mainid),
                                                                                                 m_status(_status),
                                                                                                 m_total(_total),
                                                                                                 m_buyer(std::move(
                                                                                                         _buyer)),
                                                                                                 m_seller(std::move(
                                                                                                         _seller))
    {}
    
    OrderModel(int _id, int _status, int _total) : m_id(_id), m_status(_status), m_total(_total)
    {}
    
    OrderModel(int _id, int _total) : m_id(_id), m_total(_total)
    {}
    
    int id() const
    { return m_id; }
    
    int mainid() const
    { return m_mainid; }
    
    int status() const
    { return m_status; }
    
    int total() const
    { return m_total; }
    
    QString buyer() const
    { return m_buyer; }
    
    QString seller() const
    { return m_seller; }

private:
    int m_id; ///< 订单ID
    int m_mainid; ///< 主订单ID
    int m_status; ///< 订单状态
    int m_total; ///< 订单金额
    QString m_buyer; ///< 订单买家
    QString m_seller; ///< 订单卖家
};

/// 客户端数据结构
class ClientModel : public QObject
{
Q_OBJECT
    Q_PROPERTY(bool userActive READ userActive NOTIFY userActiveChanged)
    Q_PROPERTY(UserModel * user READ user NOTIFY userChanged)
    Q_PROPERTY(QList<CommodityModel *> commodity READ commodity NOTIFY commodityChanged)
    Q_PROPERTY(int commoditySize READ commoditySize NOTIFY commoditySizeChanged)
    Q_PROPERTY(CommodityModel * singleCommodity READ singleCommodity NOTIFY singleCommodityChanged)
    Q_PROPERTY(QList<OrderModel *> order READ order NOTIFY orderChanged)
    Q_PROPERTY(OrderModel * mainOrder READ mainOrder NOTIFY mainOrderChanged)

public:
    ClientModel(QObject * parent, quint16 _port) : QObject(parent), m_userActive(0), m_user(nullptr),
                                                   m_commoditySize(0), m_singleCommodity(nullptr), m_mainOrder(nullptr),
                                                   client(this, _port)
    {}
    
    virtual ~ClientModel() = default;
    
    bool userActive() const
    { return m_userActive; }
    
    UserModel * user() const
    { return m_user; }
    
    QList<CommodityModel *> commodity() const
    { return m_commodity; }
    
    int commoditySize() const
    { return m_commoditySize; }
    
    CommodityModel * singleCommodity() const
    { return m_singleCommodity; }
    
    QList<OrderModel *> order() const
    { return m_order; }
    
    OrderModel * mainOrder() const
    { return m_mainOrder; }
    
    Q_INVOKABLE QString userLogin(QString username, QString password);
    
    Q_INVOKABLE QString userRegister(QString username, QString password, int type);
    
    Q_INVOKABLE void userLogout();
    
    Q_INVOKABLE QString userGetInfo();
    
    Q_INVOKABLE QString userSetPasswd(QString password);
    
    Q_INVOKABLE QString userAddMoney(QString money);
    
    Q_INVOKABLE QString userSubMoney(QString money);
    
    Q_INVOKABLE QString itemGetAll();
    
    Q_INVOKABLE QString itemGetMine();
    
    Q_INVOKABLE QString itemGetDetail(int id);
    
    Q_INVOKABLE QString itemGetFilter(QString name, int type, QString min_price, QString max_price, int remain);
    
    Q_INVOKABLE QString itemSet(int id, QString name, QString intro, QString price, QString amount);
    
    Q_INVOKABLE QString itemAdd(QString name, QString intro, int type, QString price, QString amount);
    
    Q_INVOKABLE QString itemDelete(int id);
    
    Q_INVOKABLE QString itemDiscount(int type, QString discount);
    
    Q_INVOKABLE QString cartGet();
    
    Q_INVOKABLE QString cartAdd(int id, QString number);
    
    Q_INVOKABLE QString cartSet(int id, QString number);
    
    Q_INVOKABLE QString cartDelete(int id);
    
    Q_INVOKABLE QString orderAdd();
    
    Q_INVOKABLE QString orderGetMine();
    
    Q_INVOKABLE QString orderGetDetail(int id);
    
    Q_INVOKABLE QString orderCancel(int id);
    
    Q_INVOKABLE QString orderPay(int id);
    
    Q_INVOKABLE QString orderRefund(int id);

Q_SIGNALS:
    
    void userChanged();
    
    void userActiveChanged();
    
    void commodityChanged();
    
    void singleCommodityChanged();
    
    void commoditySizeChanged();
    
    void orderChanged();
    
    void mainOrderChanged();

private:
    /// 检查返回json中status字段的状态，如果状态为false，则返回报错信息
    static QString checkStatus(const QJsonObject &ret);
    
    bool m_userActive; ///< 是否已登录
    UserModel * m_user; ///< 用户数据结构
    
    QList<CommodityModel *> m_commodity; ///< 商品列表
    int m_commoditySize; ///< 商品数量
    
    void clearCommodity(); ///< 释放商品列表
    
    CommodityModel * m_singleCommodity; ///< 单个商品，用于商品详情页
    
    QList<OrderModel *> m_order; ///< 订单列表
    
    void clearOrder(); ///< 释放订单列表
    
    OrderModel * m_mainOrder; ///< 主订单
    
    Client client;
};

#endif //PHASE3_CLIENTMODEL_H
