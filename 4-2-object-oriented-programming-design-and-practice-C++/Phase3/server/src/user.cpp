/**
 * @file      user.cpp
 * @brief     用户和用户管理
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是用户类和用户管理类的实现。
*/

#include "../include/user.h"

void Customer::addToCart(int id, int number)
{
    qDebug() << "添加" << number << "件商品" << id;
    cart[id] += number;
}

void Customer::removeFromCart(int id, int number)
{
    qDebug() << "移除" << number << "件商品" << id;
    cart[id] -= number;
}

void Customer::eraseFromCart(int id)
{
    qDebug() << "移除商品" << id;
//    cart[id] = 0;
    cart.remove(id);
}

int Customer::getItemNum(int id) const
{
    qDebug() << "查找商品" << id << "在购物车内的件数";
    return cart.value(id, 0);
}

QString UserManage::registerUser(const QString &username, const QString &passwd, bool type) const
{
    if (username.size() > 30)
        return "用户名最长30个字节";
    if (database->queryUserByName(username))
        return "用户名已被占用";
    qDebug() << "注册用户";
    database->insertUserData(username, passwd, type, 0);
    return {};
}

QString UserManage::login(const QString &username, const QString &passwd, QJsonObject &ret)
{
    QString formPasswd; ///< 数据库中保存的密码
    bool type;
    int balance;
    if (!database->queryUserByName(username, formPasswd, type, balance))
        return "未查找到用户";
    if (formPasswd != passwd)
        return "密码错误";
    qDebug() << "用户" << username << "登录";
    if (type)
    {
        if (!map[username])
            map[username] = QSharedPointer<Customer>::create(balance); // 用户登录
    }
    else
    {
        if (!map[username])
            map[username] = QSharedPointer<Merchant>::create(balance);
    }
    ret.insert("iss", "Ziheng Mao");
    ret.insert("username", username);
    return {};
}

QString UserManage::verify(const QJsonObject &token) const
{
    if (!token.contains("username") || !map.value(token["username"].toString(), nullptr) || !token.contains("iss") ||
        token["iss"].toString() != "Ziheng Mao")
    {
        qWarning() << "验证失败";
        return {};
    }
    qDebug() << "验证" << token["username"].toString() << "成功";
    return token["username"].toString();
}

QString UserManage::changePasswd(const QJsonObject &token, const QString &passwd) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    qDebug() << "用户修改密码";
    database->modifyUserPasswd(username, passwd);
    return {};
}

QString UserManage::getInfo(const QJsonObject &token, QJsonObject &ret) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    qDebug() << "获取用户信息";
    ret.insert("username", username);
    ret.insert("type", map[username]->getUserType());
    ret.insert("balance", map[username]->getBalance());
    return {};
}

QString UserManage::changeBalance(const QJsonObject &token, int balance) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getBalance() + balance < 0)
        return "余额不能为负";
    if (static_cast<long long>(map[username]->getBalance()) + balance >= 2000000000)
        return "余额最多为20000000";
    qDebug() << "修改用户余额";
    database->modifyUserBalance(username, map[username]->getBalance() + balance);
    map[username]->addBalance(balance);
    return {};
}

bool UserManage::changeBalance(const QJsonObject &token, int balance, const QString &anotherUser) const
{
    if (!database->queryUserByName(anotherUser))return false;
    int curNum; // 另一个用户现有余额
    database->queryUserByName(anotherUser, curNum);
    if (static_cast<long long>(curNum) - balance >= 2000000000)
        return "余额最多为20000000";
    if (!changeBalance(token, balance).isEmpty())return false;
    qDebug() << "修改另一个用户的余额";
    database->modifyUserBalance(anotherUser, curNum - balance);
    return true;
}

QString UserManage::logout(const QJsonObject &token)
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    qDebug() << "用户登出";
    map.remove(username);
    return {};
}

QString UserManage::addCommodity(const QJsonObject &token, const QJsonObject &info) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::MERCHANT)
        return "用户不为商家";
    if (!info.contains("name") || !info.contains("intro") || !info.contains("type") || !info.contains("price") ||
        !info.contains("amount"))
        return "商品信息不全/错误";
    if (info["name"].toString().size() > 30)
        return "商品名最长30个字节";
    commodityManage->insertData(info["name"].toString(), info["intro"].toString(),
                                info["type"].toInt(), username,
                                info["price"].toInt(), info["amount"].toInt());
    return {};
}

QString UserManage::changeCommodity(const QJsonObject &token, const QJsonObject &info) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::MERCHANT)
        return "用户不为商家";
    int id;
    QSharedPointer<Commodity> temp;
    if (!info.contains("id") || !commodityManage->queryById(id = info["id"].toInt(), temp))
        return "缺少字段或商品不存在";
    if (temp->getBelong() != username)
        return "不能修改别人的商品";
    if (info["name"].toString().size() > 30)
        return "商品名最长30个字节";
    if (info.contains("name"))commodityManage->changeName(id, info["name"].toString());
    if (info.contains("intro"))commodityManage->changeIntro(id, info["intro"].toString());
    if (info.contains("price"))commodityManage->changePrice(id, info["price"].toInt());
    if (info.contains("amount"))commodityManage->changeAmount(id, info["amount"].toInt());
    return {};
}

void UserManage::constructCommodityJson(const QSharedPointer<Commodity> &commodity, QJsonObject &object)
{
    object.insert("id", commodity->getId());
    object.insert("name", commodity->getName());
    object.insert("intro", commodity->getIntro());
    object.insert("type", commodity->getCommodityType());
    object.insert("belong", commodity->getBelong());
    object.insert("price", commodity->getPrice());
    object.insert("discount", commodity->getDiscount());
    object.insert("amount", commodity->getAmount());
}

QString UserManage::queryCommodity(const QJsonObject &info, QJsonArray &ret) const
{
    if (!info.contains("TYPE"))
        return "缺少字段";
    int total;
    QList<QSharedPointer<Commodity>> result;
    if (info["TYPE"].toInt() == 0)
    {
        total = commodityManage->queryAll(result);
    }
    else if (info["TYPE"].toInt() == 1)
    {
        if (!info.contains("id"))
            return "缺少字段";
        QSharedPointer<Commodity> temp;
        commodityManage->queryById(info["id"].toInt(), temp);
        result.append(temp);
        total = 1;
    }
    else if (info["TYPE"].toInt() == 2)
    {
        QString name;
        if (info.contains("name"))
            name = info["name"].toString();
        int type = -1;
        if (info.contains("type"))
            type = info["type"].toInt();
        int min_price = -1;
        if (info.contains("min_price"))
            min_price = info["min_price"].toInt();
        int max_price = -1;
        if (info.contains("max_price"))
            max_price = info["max_price"].toInt();
        bool remain = false;
        if (info.contains("remain"))
            remain = true;
        total = commodityManage->queryByFilter(name, type, min_price, max_price, remain, result);
    }
    else if (info["TYPE"].toInt() == 3)
    {
        QString username = verify(info["token"].toObject());
        if (username.isEmpty())
            return "验证失败";
        if (map[username]->getUserType() != User::MERCHANT)
            return "用户不为商家";
        total = commodityManage->queryByOwner(username, result);
    }
    else
        return "TYPE有误";
    for (const auto &i:result)
    {
        QJsonObject temp;
        constructCommodityJson(i, temp);
        ret.append(temp);
    }
    return {};
}

QString UserManage::discount(int type, int discount)
{
    if (type >= 3)
        return "只有3种商品类型";
    CommodityManage::discount(type, discount);
    return {};
}

QString UserManage::deleteCommodity(const QJsonObject &token, int id) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::MERCHANT)
        return "用户不为商家";
    QSharedPointer<Commodity> temp;
    if (!commodityManage->queryById(id, temp))
        return "商品不存在";
    if (temp->getBelong() != username)
        return "不能删除别人的商品";
    commodityManage->deleteData(id);
    return {};
}

QString UserManage::viewCart(const QJsonObject &token, QJsonArray &ret) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::CUSTOMER)
        return "用户不为消费者";
    qDebug() << "获取购物车内容";
    QSharedPointer<Customer> customer = map[username].dynamicCast<Customer>();
    auto cart = customer->getCart();
    for (const auto &i:cart.keys())
    {
        QSharedPointer<Commodity> commodity;
        if (!commodityManage->queryById(i, commodity) || commodity->getAmount() == 0)
        {
            qDebug() << "商品不存在";
            customer->eraseFromCart(i);
            continue;
        }
        if (cart[i] > commodity->getAmount())
        {
            qDebug() << "商品比存量多";
            customer->removeFromCart(i, cart[i] - commodity->getAmount());
        }
        QJsonObject item;
        item.insert("id", i);
        item.insert("name", commodity->getName());
        item.insert("price", commodity->getPrice());
        item.insert("discount", commodity->getDiscount());
        item.insert("type", commodity->getCommodityType());
        item.insert("amount", customer->getItemNum(i));
        ret.append(item);
    }
    return {};
}

QString UserManage::addToCart(const QJsonObject &token, int id, int number) const
{
    if (number <= 0)
        return "添加的数目应为正整数";
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::CUSTOMER)
        return "用户不为消费者";
    QSharedPointer<Customer> customer = map[username].dynamicCast<Customer>();
    if (customer->getItemNum(id))
        return "购物车中已经存在该商品";
    int curNum; ///< 商品的剩余量
    if (!commodityManage->queryById(id, curNum))
        return "不存在该商品";
    if (curNum == 0)
        return "商品没有剩余";
    if (number > curNum) number = curNum;
    customer->addToCart(id, number);
    return {};
}

QString UserManage::modifyCart(const QJsonObject &token, int id, int number) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::CUSTOMER)
        return "用户不为消费者";
    QSharedPointer<Customer> customer = map[username].dynamicCast<Customer>();
    int curNum = customer->getItemNum(id); ///< 购物车种商品的数量
    if (!curNum)
        return "购物车中不存在该商品";
    if (number <= 0)
    {
        customer->eraseFromCart(id);
        return {};
    }
    int temp; ///< 商品的剩余量
    if (!commodityManage->queryById(id, temp))
    {
        customer->eraseFromCart(id);
        return "不存在该商品";
    }
    if (temp == 0)
    {
        customer->eraseFromCart(id);
        return "商品没有剩余";
    }
    if (number > temp) number = temp;
    if (number > curNum)customer->addToCart(id, number - curNum);
    else if (number < curNum)customer->removeFromCart(id, curNum - number);
    return "";
}

QString UserManage::removeFromCart(const QJsonObject &token, int id) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::CUSTOMER)
        return "用户不为消费者";
    QSharedPointer<Customer> customer = map[username].dynamicCast<Customer>();
    int curNum = customer->getItemNum(id);
    if (!curNum)
        return "购物车中不存在该商品";
    customer->eraseFromCart(id);
    return "";
}

QString UserManage::generateOrder(const QJsonObject &token, const QJsonArray &info) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::CUSTOMER)
        return "用户不为消费者";
    QSharedPointer<Customer> customer = map[username].dynamicCast<Customer>();
    QList<QPair<QSharedPointer<Commodity>, int>> list; ///< 商品实例、数量二元组的列表
    for (const auto &i:info)
    {
        int id = i.toInt();
        int curNum = customer->getItemNum(id);
        if (!curNum)continue; // 商品不存在
        QSharedPointer<Commodity> commodity;
        if (commodityManage->queryById(id, commodity) && commodity->getAmount())
        {
            list.append(qMakePair(commodity, std::min(curNum, commodity->getAmount())));
            commodityManage->changeAmount(id, std::max(commodity->getAmount() - curNum, 0));
        }
        customer->eraseFromCart(id);
    }
    orderManage->addOrder(username, list);
    return {};
}

QString UserManage::viewOrderByName(const QJsonObject &token, QJsonArray &ret) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    QList<QPair<int, int>> result;
    int total;
    if (map[username]->getUserType() == User::CUSTOMER)
        total = orderManage->queryByBuyerName(username, result);
    else
        total = orderManage->queryBySellerName(username, result);
    for (const auto &i:result)
    {
        QJsonObject object;
        object.insert("id", i.first);
        object.insert("total", i.second);
        ret.append(object);
    }
    return {};
}

QString UserManage::viewOrderDetails(const QJsonObject &token, int id, QJsonObject &ret) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    QList<QSharedPointer<Order>> result;
    int total;
    if (map[username]->getUserType() == User::CUSTOMER)
        total = orderManage->queryByMainId(id, result);
    else
    {
        result.append(nullptr);
        total = orderManage->queryById(id, result[0]);
    }
    if (!total)
        return "查找不到订单";
    if (result[0]->getBuyer() != username && result[0]->getSeller() != username)
        return "只能查找和自己有关的订单";
    QJsonArray orderList, commodityList;
    total = 0;
    for (const auto &i:result)
    {
        QJsonObject order;
        order.insert("id", i->getId());
        order.insert("mainid", i->getMainId());
        order.insert("status", i->getStatus());
        order.insert("total", i->getTotal());
        total += i->getTotal();
        order.insert("buyer", i->getBuyer());
        order.insert("seller", i->getSeller());
        for (const auto &j:i->getContent())
        {
            QJsonObject commodity;
            commodity.insert("id", j.first);
            QSharedPointer<Commodity> ptr;
            commodityManage->queryById(j.first, ptr);
            commodity.insert("name", ptr->getName());
            commodity.insert("type", ptr->getCommodityType());
            commodity.insert("amount", j.second);
            commodity.insert("order", i->getId());
            commodityList.append(commodity);
        }
        orderList.append(order);
    }
    ret.insert("order", orderList);
    ret.insert("commodity", commodityList);
    ret.insert("id", result[0]->getMainId());
    ret.insert("total", total);
    ret.insert("status", result[0]->getStatus() >= 3 ? 2 : result[0]->getStatus()); // 主订单只能有未付款、已付款、已取消三种状态
    return {};
}

QString UserManage::cancelOrder(const QJsonObject &token, int id) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::CUSTOMER)
        return "用户不为消费者";
    QList<QSharedPointer<Order>> result;
    int total;
    total = orderManage->queryByMainId(id, result);
    if (!total)
        return "查找不到订单";
    if (result[0]->getBuyer() != username)
        return "只能取消自己的订单";
    if (result[0]->getStatus() != Order::UNPAID)
        return "订单必须为未支付状态";
    for (const auto &i:result)
        for (const auto &j:i->getContent())
        {
            int amount;
            if (!commodityManage->queryById(j.first, amount))
                return "商品不存在";
        }
    QSharedPointer<Customer> customer = map[username].dynamicCast<Customer>();
    for (const auto &order:result)
    {
        qDebug() << "取消订单，id为" << order->getId();
        for (const auto &i:order->getContent())
        {
            int curNum;
            commodityManage->queryById(i.first, curNum);
            commodityManage->changeAmount(i.first, curNum + i.second);
            customer->addToCart(i.first, i.second);
        }
        orderManage->changeOrderStatus(order->getId(), Order::CANCELLED);
    }
    return {};
}

QString UserManage::payOrder(const QJsonObject &token, int id) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() != User::CUSTOMER)
        return "用户不为消费者";
    QList<QSharedPointer<Order>> result;
    int total;
    total = orderManage->queryByMainId(id, result);
    if (!total)
        return "查找不到订单";
    if (result[0]->getBuyer() != username)
        return "只能支付自己的订单";
    if (result[0]->getStatus() != Order::UNPAID)
        return "订单必须为未支付状态";
    QSharedPointer<Customer> customer = map[username].dynamicCast<Customer>();
    int price = 0;
    for (const auto &i:result)
        price += i->getTotal();
    if (price > customer->getBalance())
        return "余额不足";
    qDebug() << "支付订单";
    for (const auto &i:result)
    {
        changeBalance(token, -i->getTotal(), i->getSeller());
        orderManage->changeOrderStatus(i->getId(), Order::PAID);
    }
    return {};
}

QString UserManage::refundOrder(const QJsonObject &token, int id) const
{
    QString username = verify(token);
    if (username.isEmpty())
        return "验证失败";
    if (map[username]->getUserType() == User::CUSTOMER)
    {
        QSharedPointer<Order> result;
        bool total;
        total = orderManage->queryById(id, result);
        if (!total)
            return "查找不到订单";
        if (result->getBuyer() != username)
            return "只能退款自己的订单";
        if (result->getStatus() != Order::PAID)
            return "订单必须为已支付状态";
        qDebug() << "发起订单退款";
        orderManage->changeOrderStatus(id, Order::REFUNDING);
        return {};
    }
    else
    {
        QSharedPointer<Order> result;
        bool total;
        total = orderManage->queryById(id, result);
        if (!total)
            return "查找不到订单";
        if (result->getSeller() != username)
            return "只能确认退款自己的订单";
        if (result->getStatus() != Order::REFUNDING)
            return "订单必须为正在退款状态";
        QSharedPointer<Merchant> merchant = map[username].dynamicCast<Merchant>();
        if (result->getTotal() > merchant->getBalance())
            return "余额不足";
        for (const auto &i:result->getContent())
        {
            int amount;
            if (!commodityManage->queryById(i.first, amount))
                return "商品不存在";
        }
        qDebug() << "确认订单退款";
        for (const auto &i:result->getContent())
        {
            int curNum;
            commodityManage->queryById(i.first, curNum);
            commodityManage->changeAmount(i.first, curNum + i.second);
        }
        changeBalance(token, -result->getTotal(), result->getBuyer());
        orderManage->changeOrderStatus(id, Order::REFUNDED);
        return {};
    }
}
