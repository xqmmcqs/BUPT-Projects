/**
 * @file      users.cpp
 * @brief     用户和用户管理
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的内容是用户类和用户管理类的实现。
*/

#include "../include/users.h"

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
    int curNum; ///< 另一个用户现有余额
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
    if (!total)
        return "查找不到商品";
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
    if (discount < 0 || discount > 100)
        return "折扣值应当在0-100之间";
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
