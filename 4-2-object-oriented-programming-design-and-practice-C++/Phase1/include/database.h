/**
 * @file      database.h
 * @brief     数据库
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件定义了数据库类及其接口。
 *
 * 数据库类是对一个SQLite数据库和一个用户名文件的的封装。
 *
 * 对于用户部分，定义了插入用户、查询用户、修改用户密码、修改用户余额的接口。
 *
 * 对于商品部分，定义了插入商品、查询商品、修改商品信息、删除商品的接口。
 *
 * 除此之外，定义了几个工具函数的接口，例如打印调试信息，以及将商品的查询结果转化为商品类。
*/

#ifndef PHASE1_DATABASE_H
#define PHASE1_DATABASE_H

#include <QFile>
#include <QtSql>

#include "commodity.h"

class Commodity;

class Database
{
private:
    /// SQLite数据库
    QSqlDatabase database;
    
    /// 用户名文件
    QFile userFile;
    
    /// 用于读写用户名的流
    QTextStream stream;
    
    /// 用户名的集合
    QSet<QString> usernameSet;
    
    /**
     * @brief 根据表名返回该表的主键
     * @param table 数据表名
     * @return 该表的主键
     */
    static const QString &tableid(const QString &table);
    
    /**
     * @brief 执行SQL语句并且打印调试信息
     * @param sqlQuery 待执行的SQL语句
     */
    static void execDebug(const QSqlQuery &sqlQuery);
    
    /**
     * @brief 将商品查询结果转化为商品类
     * @param sqlQuery 商品的查询结果
     * @return 指向一个新创建的商品类的指针
     */
    static QSharedPointer<Commodity> commodityResult2Object(const QSqlQuery &sqlQuery);
    
    /**
     * @brief 修改数据库中某个记录的值
     * @param table 表名
     * @param id 需要修改的记录的主键
     * @param key 需要修改的键
     * @param value 修改的值
     */
    void modifyData(const QString &table, const QString &id, const QString &key, int value) const;
    
    /**
     * @brief 修改数据库中某个记录的值
     * @param table 表名
     * @param id 需要修改的记录的主键
     * @param key 需要修改的键
     * @param value 修改的值
     */
    void modifyData(const QString &table, const QString &id, const QString &key, const QString &value) const;

public:
    Database(const QString &connection, const QString &fileName);
    
    void insertUserData(const QString &username, const QString &passwd, bool type, int balance);
    
    /**
     * @brief 根据用户名查询用户
     * @param username 用户名
     * @return 如果查询到用户，返回true
     */
    bool queryUserByName(const QString &username) const;
    
    /**
     * @brief 根据用户名查询用户余额
     * @param username 用户名
     * @param balance 查询结果
     * @return 如果查询到用户，返回true
     */
    bool queryUserByName(const QString &username, int &balance) const;
    
    /**
     * @brief 根据用户名查询用户
     * @param username 用户名
     * @param passwd 查询结果，用户的密码
     * @param type 查询结果，用户的类型
     * @param balance 查询结果，用户的余额
     * @return 如果查询到用户，返回true
     */
    bool queryUserByName(const QString &username, QString &passwd, bool &type, int &balance) const;
    
    void modifyUserPasswd(const QString &username, const QString &passwd) const;
    
    void modifyUserBalance(const QString &username, int balance) const;
    
    /**
     * @brief 查询表中主键的最大值
     * @param table 表名
     * @return 最大主键值
     */
    int queryMaxId(const QString &table) const;
    
    void
    insertCommodityData(int id, const QString &name, const QString &intro, int type, const QString &belong, int price,
                        int amount) const;
    
    bool queryCommodityById(int id, int &result) const;
    
    bool queryCommodityById(int id, QString &result) const;
    
    bool queryCommodityById(int id, QSharedPointer<Commodity> &result) const;
    
    int
    queryCommodityByFilter(const QString &name, int type, int min_price, int max_price, bool remain,
                           QList<QSharedPointer<Commodity>> &result) const;
    
    int queryCommodityByOwner(const QString &belong, QList<QSharedPointer<Commodity>> &result) const;
    
    void modifyCommodityName(int id, const QString &name) const;
    
    void modifyCommodityIntro(int id, const QString &intro) const;
    
    void modifyCommodityPrice(int id, int price) const;
    
    void modifyCommodityAmount(int id, int amount) const;
    
    void deleteCommodity(const QString &id) const;
};

#endif //PHASE1_DATABASE_H
