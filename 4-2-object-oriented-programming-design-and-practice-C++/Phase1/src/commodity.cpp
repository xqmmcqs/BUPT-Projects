/**
 * @file      commodity.cpp
 * @brief     商品和商品管理
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是商品类和商品管理类的实现。
*/

#include "../include/commodity.h"

#include "../include/database.h"

using namespace std;

int Clothes::discount = 100, Books::discount = 100, Electronic::discount = 100; // 默认折扣为1

CommodityManage::CommodityManage(Database * _database) : database(_database)
{ total = database->queryMaxId("commodity"); }

void CommodityManage::insertData(const QString &name, const QString &intro, int type, const QString &belong, int price,
                                 int amount)
{
    qDebug() << "添加商品";
    database->insertCommodityData(++total, name, intro, type, belong, price, amount);
}

int CommodityManage::queryAll(QList<QSharedPointer<Commodity>> &result) const
{
    qDebug() << "查找所有商品";
    return database->queryCommodityByFilter(QString(), -1, -1, -1, false, result);
}

bool CommodityManage::queryById(int id, int &result) const
{
    qDebug() << "按id查找商品";
    return database->queryCommodityById(id, result);
}

bool CommodityManage::queryById(int id, QString &result) const
{
    qDebug() << "按id查找商品";
    return database->queryCommodityById(id, result);
}

bool CommodityManage::queryById(int id, QSharedPointer<Commodity> &result) const
{
    qDebug() << "按id查找商品";
    return database->queryCommodityById(id, result);
}

int CommodityManage::queryByFilter(const QString &name, int type, int min_price, int max_price, bool remain,
                                   QList<QSharedPointer<Commodity>> &result) const
{
    qDebug() << "按筛选查找商品";
    return database->queryCommodityByFilter(name, type, min_price, max_price, remain, result);
}

int CommodityManage::queryByOwner(const QString &belong, QList<QSharedPointer<Commodity>> &result) const
{
    qDebug() << "按所有者查找商品";
    return database->queryCommodityByOwner(belong, result);
}

void CommodityManage::changeName(int id, const QString &name) const
{
    qDebug() << "修改商品名称";
    database->modifyCommodityName(id, name);
}

void CommodityManage::changeIntro(int id, const QString &intro) const
{
    qDebug() << "修改商品介绍";
    database->modifyCommodityIntro(id, intro);
}

void CommodityManage::changePrice(int id, int price) const
{
    qDebug() << "修改商品价格";
    database->modifyCommodityPrice(id, price);
}

void CommodityManage::changeAmount(int id, int amount) const
{
    qDebug() << "修改商品剩余量";
    database->modifyCommodityAmount(id, amount);
}

void CommodityManage::discount(int type, int discount)
{
    qDebug() << "开始打折";
    if (type == 0)Clothes::setDiscount(discount);
    if (type == 1)Books::setDiscount(discount);
    if (type == 2)Electronic::setDiscount(discount);
}

void CommodityManage::deleteData(int id) const
{
    qDebug() << "删除商品";
    database->deleteCommodity(QString::number(id));
}
