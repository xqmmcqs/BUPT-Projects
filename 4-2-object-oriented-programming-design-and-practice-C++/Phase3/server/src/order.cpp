/**
 * @file      order.cpp
 * @brief     订单和订单管理
 * @author    Ziheng Mao
 * @date      2021/5/3
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是订单类和订单管理类的实现。
*/

#include "../include/order.h"

#include "../include/database.h"

Order::Order(int _id, int _mainId, int _status, int _total, QString _buyer, QString _seller, const QString &_content) :
        id(_id), mainId(_mainId), status(Order::Status(_status)), total(_total), buyer(std::move(_buyer)),
        seller(std::move(_seller))
{
    // 数据库中保存的订单内容的格式如下
    // 商品1ID,商品1数量;商品2ID,商品2数量;...
    auto list = _content.split(';', QString::SkipEmptyParts);
    for (auto &i:list)
    {
        auto j = i.split(',');
        content.append(qMakePair(j[0].toInt(), j[1].toInt()));
    }
}

OrderManage::OrderManage(Database * _database) : database(_database)
{ total = database->queryMaxId("orders"); }

void OrderManage::addOrder(const QString &buyer, const QList<QPair<QSharedPointer<Commodity>, int>> &commodityList)
{
    qDebug() << "添加订单";
    struct TempCommodity
    {
        int id;
        int amount;
        int price;
        int discount;
    };
    QMap<QString, QList<TempCommodity>> map; // 商家到商品列表的映射
    // 将商品按商家分类
    for (const auto &i:commodityList)
        map[i.first->getBelong()].append({i.first->getId(), i.second, i.first->getPrice(), i.first->getDiscount()});
    
    int mainId = total + 1;
    for (auto i = map.begin(); i != map.end(); ++i)
    {
        int price = 0;
        QString content;
        for (const auto &j:i.value())
        {
            content += QString::number(j.id) + ',' + QString::number(j.amount) + ';';
            price += j.amount * j.price * j.discount;
        }
        database->insertOrderData(++total, mainId, static_cast<int>(price / 100.0 + 0.5), buyer, i.key(), content);
    }
}

int OrderManage::queryByBuyerName(const QString &name, QList<QPair<int, int>> &result) const
{
    qDebug() << "通过买方username查询订单";
    return database->queryOrderByBuyerName(name, result);
}

int OrderManage::queryBySellerName(const QString &name, QList<QPair<int, int>> &result) const
{
    qDebug() << "通过卖方username查询订单";
    return database->queryOrderBySellerName(name, result);
}

int OrderManage::queryByMainId(int mainId, QList<QSharedPointer<Order>> &result) const
{
    qDebug() << "通过主订单id查询订单";
    return database->queryOrderByMainId(mainId, result);
}

bool OrderManage::queryById(int id, QSharedPointer<Order> &result) const
{
    qDebug() << "通过订单id查询订单";
    return database->queryOrderById(id, result);
}

void OrderManage::changeOrderStatus(int id, Order::Status status) const
{
    qDebug() << "修改订单状态";
    database->modifyOrderStatus(id, status);
}
