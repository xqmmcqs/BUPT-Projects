/**
 * @file      order.h
 * @brief     订单和订单管理
 * @author    Ziheng Mao
 * @date      2021/5/3
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中定义了订单类和订单管理类。
 *
 * 系统中实现的订单包含一个ID和主订单ID，当消费者创建订单时，会根据订单中商品归属的商家，分成多个子订单，一个子订单中的商品都属于同一个商家，这些
 * 子订单的共同构成一个主订单。主订单的ID并不连续，由子订单的最小ID决定。
 *
 * 除此之外，订单类还保存订单状态、金额、买家用户名、卖家用户名、订单内容（商品ID和数量）。
 *
 * 订单管理类中定义了添加订单、根据ID和主订单ID查询订单、根据买家用户名和卖家用户名查询订单、修改订单状态的接口。
*/

#ifndef PHASE2_ORDER_H
#define PHASE2_ORDER_H

#include <QList>
#include <QString>
#include <QSharedPointer>

#include "commodity.h"

class Database;

class Order
{
public:
    /// 订单状态
    enum Status
    {
        UNPAID, CANCELLED, PAID, REFUNDING, REFUNDED
    };

private:
    int id;
    int mainId;
    Status status;
    int total; ///< 订单金额，单位为分
    QString buyer;
    QString seller;
    QList<QPair<int, int>> content; ///< 订单中商品ID、商品数量二元组的列表

public:
    Order(int _id, int _mainId, int _status, int _total, QString _buyer, QString _seller, const QString &_content);
    
    int getId() const
    { return id; }
    
    int getMainId() const
    { return mainId; }
    
    int getStatus() const
    { return status; }
    
    int getTotal() const
    { return total; }
    
    const QString &getBuyer() const
    { return buyer; }
    
    const QString &getSeller() const
    { return seller; }
    
    const QList<QPair<int, int>> &getContent() const
    { return content; }
};

class OrderManage
{
private:
    Database * database;
    int total; ///< 已有的最大订单ID

public:
    OrderManage(Database * _database);
    
    void addOrder(const QString &buyer, const QList<QPair<QSharedPointer<Commodity>, int>> &commodityList);
    
    /**
     * @brief 根据买家用户名查询（主）订单
     * @param name 买家用户名
     * @param result 查询结果，订单ID、订单金额二元组的列表
     * @return 查询到订单的数量
     */
    int queryByBuyerName(const QString &name, QList<QPair<int, int>> &result) const;
    
    /**
     * @brief 根据卖家用户名查询（子）订单
     * @param name 卖家用户名
     * @param result 查询结果，订单ID、订单金额二元组的列表
     * @return 查询到订单的数量
     */
    int queryBySellerName(const QString &name, QList<QPair<int, int>> &result) const;
    
    int queryByMainId(int mainId, QList<QSharedPointer<Order>> &result) const;
    
    bool queryById(int id, QSharedPointer<Order> &result) const;
    
    void changeOrderStatus(int id, Order::Status status) const;
};

#endif //PHASE2_ORDER_H
