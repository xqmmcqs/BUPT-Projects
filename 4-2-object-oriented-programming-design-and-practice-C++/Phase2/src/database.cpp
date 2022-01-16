/**
 * @file      database.cpp
 * @brief     数据库
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件的主要内容是数据库类的实现。
*/

#include "../include/database.h"

using namespace std;

void Database::execDebug(const QSqlQuery &sqlQuery)
{
    qDebug() << "执行SQL语句" << sqlQuery.lastQuery();
    QMap<QString, QVariant> sqlIterator(sqlQuery.boundValues());
    for (auto i = sqlIterator.begin(); i != sqlIterator.end(); ++i) // 输出SQL语句中各个变量的值
        qDebug() << i.key().toUtf8().data() << ":" << i.value().toString().toUtf8().data();
}

/**
 * 本函数中检查是否存在user、commodity、orders三个表，如果不存在某个表则创建；同时打开用户名文件，将用户名信息读取到usernameSet中。
 */
Database::Database(const QString &connection, const QString &fileName) : userFile(fileName), usernameSet()
{
    database = QSqlDatabase::addDatabase("QSQLITE", connection);
    database.setDatabaseName("database.sqlite");
    database.open();
    if (!database.tables().contains("user"))
    {
        QSqlQuery sqlQuery(database);
        sqlQuery.prepare("CREATE TABLE user ("
                         "username TEXT PRIMARY KEY NOT NULL,"
                         "passwd TEXT NOT NULL,"
                         "type INT NOT NULL,"
                         "balance INT NOT NULL)");
        execDebug(sqlQuery);
        if (!sqlQuery.exec())
            qCritical() << "user表创建失败" << sqlQuery.lastError();
        else
            qDebug() << "user表创建成功";
    }
    else
        qDebug() << "已存在user表";
    if (!database.tables().contains("commodity"))
    {
        QSqlQuery sqlQuery(database);
        sqlQuery.prepare("CREATE TABLE commodity ("
                         "id INT PRIMARY KEY NOT NULL,"
                         "name TEXT NOT NULL,"
                         "intro TEXT NOT NULL,"
                         "type INT NOT NULL,"
                         "belong TEXT NOT NULL,"
                         "price INT NOT NULL,"
                         "amount INT NOT NULL)");
        execDebug(sqlQuery);
        if (!sqlQuery.exec())
            qCritical() << "commodity表创建失败" << sqlQuery.lastError();
        else
            qDebug() << "commodity表创建成功";
    }
    else
        qDebug() << "已存在commodity表";
    if (!database.tables().contains("orders"))
    {
        QSqlQuery sqlQuery(database);
        sqlQuery.prepare("CREATE TABLE orders ("
                         "id INT PRIMARY KEY NOT NULL,"
                         "mainid INT NOT NULL,"
                         "status INT NOT NULL,"
                         "total INT NOT NULL,"
                         "buyer TEXT NOT NULL,"
                         "seller TEXT NOT NULL,"
                         "content TEXT NOT NULL)");
        execDebug(sqlQuery);
        if (!sqlQuery.exec())
            qCritical() << "orders表创建失败" << sqlQuery.lastError();
        else
            qDebug() << "orders表创建成功";
    }
    else
        qDebug() << "已存在orders表";
    if (!userFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qCritical() << "user文件打开失败";
        return;
    }
    stream.setDevice(&userFile);
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        usernameSet.insert(line);
    }
}

/**
 * user表的主键是username，commodity和orders表的主键是id
 */
inline const QString &Database::tableid(const QString &table)
{
    static QString username("username");
    static QString id("id");
    if (table == "user")return username;
    return id;
}

void Database::modifyData(const QString &table, const QString &id, const QString &key, int value) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("UPDATE " + table + " SET " + key + " = :value WHERE " + tableid(table) + " = :id");
    sqlQuery.bindValue(":id", id);
    sqlQuery.bindValue(":value", value);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "修改失败" << sqlQuery.lastError();
    else
        qDebug() << "修改成功";
}

void Database::modifyData(const QString &table, const QString &id, const QString &key, const QString &value) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("UPDATE " + table + " SET " + key + " = :value WHERE " + tableid(table) + " = :id");
    sqlQuery.bindValue(":value", value);
    sqlQuery.bindValue(":id", id);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "修改失败" << sqlQuery.lastError();
    else
        qDebug() << "修改成功";
}

void Database::insertUserData(const QString &username, const QString &passwd, bool type, int balance)
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("INSERT INTO user VALUES(:username, :passwd, :type, :balance)");
    sqlQuery.bindValue(":username", username);
    sqlQuery.bindValue(":passwd", passwd);
    sqlQuery.bindValue(":type", type);
    sqlQuery.bindValue(":balance", balance);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "插入user项失败" << sqlQuery.lastError();
    else
        qDebug() << "插入user项成功";
    usernameSet.insert(username);
    stream << username << endl; // 向文件中写入用户名
}

bool Database::queryUserByName(const QString &username) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT * FROM user WHERE username = :username");
    sqlQuery.bindValue(":username", username);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "查找成功";
        if (sqlQuery.next())
        {
            if (usernameSet.find(username) != usernameSet.constEnd())
                qDebug() << "查找到用户" << username;
            else
                qFatal("数据库和文件不同步");
            return true;
        }
        return false;
    }
}

bool Database::queryUserByName(const QString &username, int &balance) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT balance FROM user WHERE username = :username");
    sqlQuery.bindValue(":username", username);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "查找成功";
        if (sqlQuery.next())
        {
            balance = sqlQuery.value(0).toInt();
            return true;
        }
        return false;
    }
}

bool Database::queryUserByName(const QString &username, QString &passwd, bool &type, int &balance) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT passwd, type, balance FROM user WHERE username = :username");
    sqlQuery.bindValue(":username", username);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "查找成功";
        if (sqlQuery.next())
        {
            passwd = sqlQuery.value(0).toString();
            type = sqlQuery.value(1).toBool();
            balance = sqlQuery.value(2).toInt();
            return true;
        }
        return false;
    }
}

void Database::modifyUserPasswd(const QString &username, const QString &passwd) const
{
    modifyData("user", username, "passwd", passwd);
}

void Database::modifyUserBalance(const QString &username, int balance) const
{
    modifyData("user", username, "balance", balance);
}

int Database::queryMaxId(const QString &table) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT MAX(id) FROM " + table);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "获取最大id失败" << sqlQuery.lastError();
        return 0;
    }
    else
    {
        qDebug() << "获取最大id成功";
        if (sqlQuery.next())return sqlQuery.value(0).toInt();
        return 0;
    }
}

void
Database::insertCommodityData(int id, const QString &name, const QString &intro, int type, const QString &belong,
                              int price, int amount) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("INSERT INTO commodity VALUES(:id, :name, :intro, :type, :belong, :price, :amount)");
    sqlQuery.bindValue(":id", id);
    sqlQuery.bindValue(":name", name);
    sqlQuery.bindValue(":intro", intro);
    sqlQuery.bindValue(":type", type);
    sqlQuery.bindValue(":belong", belong);
    sqlQuery.bindValue(":price", price);
    sqlQuery.bindValue(":amount", amount);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "插入commodity项失败" << sqlQuery.lastError();
    else
        qDebug() << "插入commodity项成功";
}

QSharedPointer<Commodity> Database::commodityResult2Object(const QSqlQuery &sqlQuery)
{
    if (sqlQuery.value(3).toInt() == 0)
        return QSharedPointer<Clothes>::create(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(),
                                               sqlQuery.value(2).toString(), sqlQuery.value(4).toString(),
                                               sqlQuery.value(5).toInt(), sqlQuery.value(6).toInt());
    else if (sqlQuery.value(3).toInt() == 1)
        return QSharedPointer<Books>::create(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(),
                                             sqlQuery.value(2).toString(), sqlQuery.value(4).toString(),
                                             sqlQuery.value(5).toInt(), sqlQuery.value(6).toInt());
    else if (sqlQuery.value(3).toInt() == 2)
        return QSharedPointer<Electronic>::create(sqlQuery.value(0).toInt(), sqlQuery.value(1).toString(),
                                                  sqlQuery.value(2).toString(), sqlQuery.value(4).toString(),
                                                  sqlQuery.value(5).toInt(), sqlQuery.value(6).toInt());
    return nullptr;
}

bool Database::queryCommodityById(int id, int &result) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT amount FROM commodity WHERE id = :id");
    sqlQuery.bindValue(":id", id);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "查找成功";
        if (sqlQuery.next())
        {
            result = sqlQuery.value(0).toInt();
            return true;
        }
        return false;
    }
}

bool Database::queryCommodityById(int id, QString &result) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT name FROM commodity WHERE id = :id");
    sqlQuery.bindValue(":id", id);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "查找成功";
        if (sqlQuery.next())
        {
            result = sqlQuery.value(0).toString();
            return true;
        }
        return false;
    }
}

bool Database::queryCommodityById(int id, QSharedPointer<Commodity> &result) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT * FROM commodity WHERE id = :id");
    sqlQuery.bindValue(":id", id);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "查找成功";
        if (sqlQuery.next())
        {
            result = commodityResult2Object(sqlQuery);
            return true;
        }
        return false;
    }
}

int Database::queryCommodityByFilter(const QString &name, int type, int min_price, int max_price, bool remain,
                                     QList<QSharedPointer<Commodity>> &result) const
{
    QString queryString("SELECT * FROM commodity");
    QSqlQuery sqlQuery(database);
    int cnt = 0; // 筛选条件的数量，第一个筛选条件前用WHERE，之后用AND
    if (!name.isEmpty())
    {
        queryString += " WHERE name LIKE \'%" + name + "%\'";
        ++cnt;
    }
    if (~type)
    {
        queryString += QString(cnt ? " AND " : " WHERE ") + "type = :type";
        ++cnt;
    }
    if (~min_price)
    {
        queryString += QString(cnt ? " AND " : " WHERE ") + "price >= :min_price";
        ++cnt;
    }
    if (~max_price)
    {
        queryString += QString(cnt ? " AND " : " WHERE ") + "price <= :max_price";
        ++cnt;
    }
    if (remain)
    {
        queryString += QString(cnt ? " AND " : " WHERE ") + "amount > 0";
        ++cnt;
    }
    sqlQuery.prepare(queryString);
    if (~type)sqlQuery.bindValue(":type", type);
    if (min_price != -1)sqlQuery.bindValue(":min_price", min_price);
    if (max_price != -1)sqlQuery.bindValue(":max_price", max_price);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return 0;
    }
    else
    {
        qDebug() << "查找成功";
        cnt = 0;
        while (sqlQuery.next())
        {
            result.append(commodityResult2Object(sqlQuery));
            ++cnt;
        }
        return cnt;
    }
}

int Database::queryCommodityByOwner(const QString &belong, QList<QSharedPointer<Commodity>> &result) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT * FROM commodity WHERE belong = :belong");
    sqlQuery.bindValue(":belong", belong);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return 0;
    }
    else
    {
        qDebug() << "查找成功";
        int cnt = 0;
        while (sqlQuery.next())
        {
            result.append(commodityResult2Object(sqlQuery));
            ++cnt;
        }
        return cnt;
    }
}

void Database::modifyCommodityName(int id, const QString &name) const
{
    modifyData("commodity", QString::number(id), "name", name);
}

void Database::modifyCommodityIntro(int id, const QString &intro) const
{
    modifyData("commodity", QString::number(id), "intro", intro);
}

void Database::modifyCommodityPrice(int id, int price) const
{
    modifyData("commodity", QString::number(id), "price", price);
}

void Database::modifyCommodityAmount(int id, int amount) const
{
    modifyData("commodity", QString::number(id), "amount", amount);
}

void Database::deleteCommodity(const QString &id) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("DELETE FROM commodity WHERE id = :id");
    sqlQuery.bindValue(":id", id);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "删除失败";
    else
        qDebug() << "删除成功";
}

void Database::insertOrderData(int id, int mainId, int total, const QString &buyer, const QString &seller,
                               const QString &content) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("INSERT INTO orders VALUES(:id, :mainid, 0, :total, :buyer, :seller, :content)");
    sqlQuery.bindValue(":id", id);
    sqlQuery.bindValue(":mainid", mainId);
    sqlQuery.bindValue(":total", total);
    sqlQuery.bindValue(":buyer", buyer);
    sqlQuery.bindValue(":seller", seller);
    sqlQuery.bindValue(":content", content);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
        qCritical() << "插入order项失败" << sqlQuery.lastError();
    else
        qDebug() << "插入order项成功";
}

QSharedPointer<Order> Database::orderResult2Object(const QSqlQuery &sqlQuery)
{
    return QSharedPointer<Order>::create(sqlQuery.value(0).toInt(), sqlQuery.value(1).toInt(),
                                         sqlQuery.value(2).toInt(), sqlQuery.value(3).toInt(),
                                         sqlQuery.value(4).toString(), sqlQuery.value(5).toString(),
                                         sqlQuery.value(6).toString());
}

bool Database::queryOrderById(int id, QSharedPointer<Order> &result) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT * FROM orders WHERE id = :id");
    sqlQuery.bindValue(":id", id);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return false;
    }
    else
    {
        qDebug() << "查找成功";
        if (sqlQuery.next()) // 根据ID查询订单至多只有一个查询结果
        {
            result = orderResult2Object(sqlQuery);
            return true;
        }
        return false;
    }
}

int Database::queryOrderByMainId(int mainId, QList<QSharedPointer<Order>> &result) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT * FROM orders WHERE mainid = :mainid");
    sqlQuery.bindValue(":mainid", mainId);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return 0;
    }
    else
    {
        qDebug() << "查找成功";
        int cnt = 0;
        while (sqlQuery.next())
        {
            result.append(orderResult2Object(sqlQuery));
            ++cnt;
        }
        return cnt;
    }
}

int Database::queryOrderByBuyerName(const QString &name, QList<QPair<int, int>> &result) const
{
    QSqlQuery sqlQuery(database);
    // 根据买家查询主订单，订单号取子订单号的最小值，金额取子订单的金额之和
    sqlQuery.prepare("SELECT MIN(id), SUM(total) FROM orders WHERE buyer = :name GROUP BY mainid");
    sqlQuery.bindValue(":name", name);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return 0;
    }
    else
    {
        qDebug() << "查找成功";
        int cnt = 0;
        while (sqlQuery.next())
        {
            result.append(qMakePair(sqlQuery.value(0).toInt(), sqlQuery.value(1).toInt()));
            ++cnt;
        }
        return cnt;
    }
}

int Database::queryOrderBySellerName(const QString &name, QList<QPair<int, int>> &result) const
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare("SELECT id, total FROM orders WHERE seller = :name");
    sqlQuery.bindValue(":name", name);
    execDebug(sqlQuery);
    if (!sqlQuery.exec())
    {
        qCritical() << "查找失败" << sqlQuery.lastError();
        return 0;
    }
    else
    {
        qDebug() << "查找成功";
        int cnt = 0;
        while (sqlQuery.next())
        {
            result.append(qMakePair(sqlQuery.value(0).toInt(), sqlQuery.value(1).toInt()));
            ++cnt;
        }
        return cnt;
    }
}

void Database::modifyOrderStatus(int id, int status) const
{
    modifyData("orders", QString::number(id), "status", status);
}
