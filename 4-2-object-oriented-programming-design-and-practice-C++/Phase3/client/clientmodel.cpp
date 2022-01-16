/**
 * @file      clientmodel.cpp
 * @brief     客户端数据结构
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是对客户端数据结构各接口的实现。
*/

#include "clientmodel.h"

#include <QJsonObject>
#include <QJsonArray>

QString ClientModel::checkStatus(const QJsonObject &ret)
{
    if (ret["status"].toBool() == true)
        return {};
    else
        return ret["payload"].toString();
}

QString ClientModel::userLogin(QString username, QString password)
{
    if (username.isEmpty() || password.isEmpty())
        return "用户名和密码不能为空";
    QJsonObject msg;
    msg.insert("username", username);
    msg.insert("password", password);
    auto ret = client.login(msg);
    if (ret["status"].toBool() == true)
    {
        m_userActive = true;
        emit userActiveChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::userRegister(QString username, QString password, int type)
{
    if (username.isEmpty() || password.isEmpty())
        return "用户名和密码不能为空";
    QJsonObject msg;
    msg.insert("username", username);
    msg.insert("password", password);
    msg.insert("type", type);
    auto ret = client.request(Client::USER_REGISTER, msg);
    return checkStatus(ret);
}

void ClientModel::userLogout()
{
    client.logout();
    m_userActive = false;
    emit userActiveChanged();
}

QString ClientModel::userGetInfo()
{
    QJsonObject msg;
    auto ret = client.request(Client::USER_GET_INFO, msg, true);
    if (ret["status"].toBool() == true)
    {
        auto payload = ret["payload"].toObject();
        if (m_user != nullptr)
            delete m_user;
        m_user = new UserModel(payload["username"].toString(), payload["type"].toInt(),
                               payload["balance"].toInt());
        emit userChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::userSetPasswd(QString password)
{
    if (password.isEmpty())
        return "密码不能为空";
    QJsonObject msg;
    msg.insert("password", password);
    auto ret = client.request(Client::USER_SET_PASSWD, msg, true);
    return checkStatus(ret);
}

QString ClientModel::userAddMoney(QString money)
{
    if (money.isEmpty())
        return "金额不能为空";
    bool ok = false;
    if (money.toDouble(&ok), !ok)
        return "请输入一个小数";
    if (money.toDouble() < 0)
        return "请输入一个非负数";
    QJsonObject msg;
    msg.insert("money", static_cast<int>(money.toDouble() * 100));
    auto ret = client.request(Client::USER_ADD_MONEY, msg, true);
    return checkStatus(ret);
}

QString ClientModel::userSubMoney(QString money)
{
    if (money.isEmpty())
        return "金额不能为空";
    bool ok = false;
    if (money.toDouble(&ok), !ok)
        return "请输入一个小数";
    if (money.toDouble() < 0)
        return "请输入一个非负数";
    QJsonObject msg;
    msg.insert("money", static_cast<int>(money.toDouble() * 100));
    auto ret = client.request(Client::USER_SUB_MONEY, msg, true);
    return checkStatus(ret);
}

void ClientModel::clearCommodity()
{
    for (auto i = m_commodity.begin(); i != m_commodity.end(); ++i)
        if (*i != nullptr) delete *i;
    m_commodity.clear();
}

QString ClientModel::itemGetAll()
{
    QJsonObject msg;
    auto ret = client.request(Client::ITEM_GET_ALL, msg);
    if (ret["status"].toBool() == true)
    {
        clearCommodity();
        QJsonArray arr(ret["payload"].toArray());
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            m_commodity.append(new CommodityModel(obj["id"].toInt(), obj["name"].toString(), obj["type"].toInt(),
                                                  obj["price"].toInt(), obj["discount"].toInt(),
                                                  obj["amount"].toInt()));
        }
        emit commodityChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::itemGetMine()
{
    QJsonObject msg;
    auto ret = client.request(Client::ITEM_GET_MINE, msg, true);
    if (ret["status"].toBool() == true)
    {
        clearCommodity();
        QJsonArray arr(ret["payload"].toArray());
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            m_commodity.append(new CommodityModel(obj["id"].toInt(), obj["name"].toString(), obj["type"].toInt(),
                                                  obj["price"].toInt(), obj["discount"].toInt(),
                                                  obj["amount"].toInt()));
        }
        emit commodityChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::itemGetDetail(int id)
{
    QJsonObject msg;
    msg.insert("id", id);
    auto ret = client.request(Client::ITEM_GET_DETAIL, msg);
    if (ret["status"].toBool() == true)
    {
        QJsonArray arr(ret["payload"].toArray());
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            if (m_singleCommodity)
                delete m_singleCommodity;
            m_singleCommodity = new CommodityModel(obj["id"].toInt(), obj["name"].toString(), obj["intro"].toString(),
                                                   obj["type"].toInt(), obj["price"].toInt(), obj["discount"].toInt(),
                                                   obj["amount"].toInt());
        }
        emit singleCommodityChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::itemGetFilter(QString name, int type, QString min_price, QString max_price, int remain)
{
    QJsonObject msg;
    msg.insert("name", name);
    if (type)
        msg.insert("type", type - 1);
    bool ok = false;
    if (!min_price.isEmpty())
    {
        if (min_price.toDouble(&ok), !ok)
            return "请在最小价格输入一个非负小数";
        else if (min_price.toDouble() > -0)
            msg.insert("min_price", static_cast<int>(min_price.toDouble() * 100));
    }
    if (!max_price.isEmpty())
    {
        if (max_price.toDouble(&ok), !ok)
            return "请在最大价格输入一个非负小数";
        else if (max_price.toDouble() >= 0)
            msg.insert("max_price", static_cast<int>(max_price.toDouble() * 100));
    }
    if (remain)
        msg.insert("remain", remain);
    auto ret = client.request(Client::ITEM_GET_FILTER, msg);
    if (ret["status"].toBool() == true)
    {
        clearCommodity();
        QJsonArray arr(ret["payload"].toArray());
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            m_commodity.append(new CommodityModel(obj["id"].toInt(), obj["name"].toString(),
                                                  obj["type"].toInt(), obj["price"].toInt(), obj["discount"].toInt(),
                                                  obj["amount"].toInt()));
        }
        emit commodityChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::itemSet(int id, QString name, QString intro, QString price, QString amount)
{
    QJsonObject info, msg;
    info.insert("id", id);
    if (!name.isEmpty())info.insert("name", name);
    if (!intro.isEmpty())info.insert("intro", intro);
    bool ok = false;
    if (!price.isEmpty())
    {
        if (price.toDouble(&ok), !ok)
            return "请在价格输入一个非负小数";
        else if (price.toDouble() >= 0)
            info.insert("price", static_cast<int>(price.toDouble() * 100));
    }
    if (!amount.isEmpty())
    {
        if (amount.toInt(&ok), !ok)
            return "请在数量输入一个非负整数";
        else if (amount.toInt() >= 0)
            info.insert("amount", amount.toInt());
    }
    msg.insert("info", info);
    auto ret = client.request(Client::ITEM_SET, msg, true);
    return checkStatus(ret);
}

QString ClientModel::itemAdd(QString name, QString intro, int type, QString price, QString amount)
{
    if (name.isEmpty() || intro.isEmpty() || price.isEmpty() || amount.isEmpty())
        return "字段不能为空";
    QJsonObject info, msg;
    info.insert("name", name);
    info.insert("intro", intro);
    info.insert("type", type);
    bool ok = false;
    if (price.toDouble(&ok), !ok)
        return "请在价格输入一个小数";
    if (price.toDouble() < 0)
        return "请在价格输入一个非负数";
    info.insert("price", static_cast<int>(price.toDouble() * 100));
    if (amount.toInt(&ok), !ok)
        return "请在数量输入一个整数";
    if (amount.toInt() < 0)
        return "请在数量输入一个非负数";
    info.insert("amount", amount.toInt());
    msg.insert("info", info);
    auto ret = client.request(Client::ITEM_ADD, msg, true);
    return checkStatus(ret);
}

QString ClientModel::itemDelete(int id)
{
    QJsonObject msg;
    msg.insert("id", id);
    auto ret = client.request(Client::ITEM_DELETE, msg, true);
    return checkStatus(ret);
}

QString ClientModel::itemDiscount(int type, QString discount)
{
    if (discount.isEmpty())
        return "字段不能为空";
    bool ok = false;
    if (discount.toInt(&ok), !ok)
        return "请在折扣输入一个整数";
    if (discount.toInt() < 0 || discount.toInt() > 100)
        return "请在折扣输入一个0-100之间的数";
    QJsonObject msg;
    msg.insert("type", type);
    msg.insert("discount", discount.toInt());
    auto ret = client.request(Client::ITEM_DISCOUNT, msg);
    return checkStatus(ret);
}

QString ClientModel::cartGet()
{
    QJsonObject msg;
    auto ret = client.request(Client::CART_GET, msg, true);
    if (ret["status"].toBool() == true)
    {
        clearCommodity();
        QJsonArray arr(ret["payload"].toArray());
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            m_commodity.append(new CommodityModel(obj["id"].toInt(), obj["name"].toString(), obj["type"].toInt(),
                                                  obj["price"].toInt(), obj["discount"].toInt(),
                                                  obj["amount"].toInt()));
        }
        m_commoditySize = arr.size();
        emit commodityChanged();
        emit commoditySizeChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::cartAdd(int id, QString number)
{
    if (number.isEmpty())
        return "字段不能为空";
    bool ok = false;
    if (number.toInt(&ok), !ok)
        return "请输入一个整数";
    if (number.toInt() <= 0)
        return "请输入一个正数";
    QJsonObject msg;
    msg.insert("id", id);
    msg.insert("number", number.toInt());
    auto ret = client.request(Client::CART_ADD, msg, true);
    return checkStatus(ret);
}

QString ClientModel::cartSet(int id, QString number)
{
    if (number.isEmpty())
        return "字段不能为空";
    bool ok = false;
    if (number.toInt(&ok), !ok)
        return "请输入一个整数";
    if (number.toInt() <= 0)
        return "请输入一个正数";
    QJsonObject msg;
    msg.insert("id", id);
    msg.insert("number", number.toInt());
    auto ret = client.request(Client::CART_SET, msg, true);
    return checkStatus(ret);
}

QString ClientModel::cartDelete(int id)
{
    QJsonObject msg;
    msg.insert("id", id);
    auto ret = client.request(Client::CART_DELETE, msg, true);
    return checkStatus(ret);
}

void ClientModel::clearOrder()
{
    for (auto i = m_order.begin(); i != m_order.end(); ++i)
        if (*i != nullptr) delete *i;
    m_order.clear();
}

QString ClientModel::orderAdd()
{
    QJsonObject msg;
    QJsonArray arr;
    for (auto i = m_commodity.begin(); i != m_commodity.end(); ++i)
        if ((*i)->order())arr.append((*i)->id());
    msg.insert("info", arr);
    auto ret = client.request(Client::ORDER_ADD, msg, true);
    return checkStatus(ret);
}

QString ClientModel::orderGetMine()
{
    QJsonObject msg;
    auto ret = client.request(Client::ORDER_GET_MINE, msg, true);
    if (ret["status"].toBool() == true)
    {
        clearOrder();
        QJsonArray arr(ret["payload"].toArray());
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            m_order.append(new OrderModel(obj["id"].toInt(), obj["total"].toInt()));
        }
        emit orderChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::orderGetDetail(int id)
{
    QJsonObject msg;
    msg.insert("id", id);
    auto ret = client.request(Client::ORDER_GET_DETAIL, msg, true);
    if (ret["status"].toBool() == true)
    {
        auto payload = ret["payload"].toObject();
        if (m_mainOrder)
            delete m_mainOrder;
        m_mainOrder = new OrderModel(payload["id"].toInt(), payload["status"].toInt(), payload["total"].toInt());
        emit mainOrderChanged();
        clearOrder();
        QJsonArray arr(payload["order"].toArray());
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            m_order.append(new OrderModel(obj["id"].toInt(), obj["mainid"].toInt(), obj["status"].toInt(),
                                          obj["total"].toInt(), obj["buyer"].toString(), obj["seller"].toString()));
        }
        emit orderChanged();
        clearCommodity();
        arr = payload["commodity"].toArray();
        for (auto i = arr.begin(); i != arr.end(); ++i)
        {
            QJsonObject obj = i->toObject();
            m_commodity.append(new CommodityModel(obj["id"].toInt(), obj["name"].toString(), obj["type"].toInt(),
                                                  obj["amount"].toInt(), obj["order"].toInt()));
        }
        emit commodityChanged();
        return {};
    }
    else
        return ret["payload"].toString();
}

QString ClientModel::orderCancel(int id)
{
    QJsonObject msg;
    msg.insert("id", id);
    auto ret = client.request(Client::ORDER_CANCEL, msg, true);
    return checkStatus(ret);
}

QString ClientModel::orderPay(int id)
{
    QJsonObject msg;
    msg.insert("id", id);
    auto ret = client.request(Client::ORDER_PAY, msg, true);
    return checkStatus(ret);
}

QString ClientModel::orderRefund(int id)
{
    QJsonObject msg;
    msg.insert("id", id);
    auto ret = client.request(Client::ORDER_REFUND, msg, true);
    return checkStatus(ret);
}
