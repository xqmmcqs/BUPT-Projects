/**
 * @file      commodity.h
 * @brief     商品和商品管理
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
 *
 * 本文件中定义了商品类和商品管理类。
 *
 * 商品类包括一个商品基类，以及服饰、书籍、电子产品三个派生类。
 *
 * 商品类中保存商品ID、商品名、商品介绍、商品所属商家、商品价格、商品数量。
 *
 * 每个派生类还有一个静态成员，用于某一类商品的打折。商品价格和折扣都用整数表示，打折后的价格也会四舍五入到整数。
 *
 * 商品管理类中定义了添加商品、查询所有商品、根据ID查询商品、根据筛选条件查询商品、根据所属商家查询商品、修改商品、商品打折、删除商品的接口。
*/

#ifndef PHASE1_COMMODITY_H
#define PHASE1_COMMODITY_H

#include <QString>
#include <QSharedPointer>

class Database;

class Commodity
{
protected:
    int id;
    QString name;
    QString intro;
    QString belong;
    int price; ///< 商品价格，单位为分
    int amount;

public:
    enum Type
    {
        CLOTHES, BOOKS, ELECTRONIC
    };
    
    Commodity(int _id, QString _name, QString _intro, QString _belong, int _price, int _amount) :
            id(_id), name(std::move(_name)), intro(std::move(_intro)), belong(std::move(_belong)), price(_price),
            amount(_amount)
    {}
    
    virtual int getCommodityType() const = 0;
    
    virtual int getDiscount() const = 0;
    
    int getId() const
    { return id; }
    
    const QString &getName() const
    { return name; }
    
    const QString &getIntro() const
    { return intro; }
    
    const QString &getBelong() const
    { return belong; }
    
    int getPrice() const
    { return price; }
    
    int getAmount() const
    { return amount; }
    
    virtual ~Commodity() = default;
};

class Clothes : public Commodity
{
private:
    static int discount;

public:
    Clothes(int _id, QString _name, QString _intro, QString _belong, int _price, int _amount) :
            Commodity(_id, std::move(_name), std::move(_intro), std::move(_belong), _price, _amount)
    {}
    
    static void setDiscount(int _discount)
    { discount = _discount; }
    
    int getDiscount() const override
    { return discount; }
    
    int getCommodityType() const override
    { return CLOTHES; }
};

class Books : public Commodity
{
private:
    static int discount;

public:
    Books(int _id, QString _name, QString _intro, QString _belong, int _price, int _amount) :
            Commodity(_id, std::move(_name), std::move(_intro), std::move(_belong), _price, _amount)
    {}
    
    static void setDiscount(int _discount)
    { discount = _discount; }
    
    int getDiscount() const override
    { return discount; }
    
    int getCommodityType() const override
    { return BOOKS; }
};

class Electronic : public Commodity
{
private:
    static int discount;

public:
    Electronic(int _id, QString _name, QString _intro, QString _belong, int _price, int _amount) :
            Commodity(_id, std::move(_name), std::move(_intro), std::move(_belong), _price, _amount)
    {}
    
    static void setDiscount(int _discount)
    { discount = _discount; }
    
    int getDiscount() const override
    { return discount; }
    
    int getCommodityType() const override
    { return ELECTRONIC; }
};

class CommodityManage
{
private:
    Database * database;
    int total; ///< 已有的最大商品ID

public:
    CommodityManage(Database * _database);
    
    void insertData(const QString &name, const QString &intro, int type, const QString &belong, int price, int amount);
    
    int queryAll(QList<QSharedPointer<Commodity>> &result) const;
    
    /**
     * @brief 根据商品ID查询商品剩余量
     * @param id 商品ID
     * @param result 查询结果
     * @return 如果查询到商品，返回true
     */
    bool queryById(int id, int &result) const;
    
    /**
     * @brief 根据商品ID查询商品名称
     * @param id 商品ID
     * @param result 查询结果
     * @return 如果查询到商品，返回true
     */
    bool queryById(int id, QString &result) const;
    
    /**
     * @brief 根据商品ID查询商品
     * @param id 商品ID
     * @param result 查询结果
     * @return 如果查询到商品，返回true
     */
    bool queryById(int id, QSharedPointer<Commodity> &result) const;
    
    /**
     * @brief 根据条件筛选查询商品
     * @param name 商品名，任何包含该子串的商品都会被选中，默认为空
     * @param type 商品类型，默认为-1
     * @param min_price 商品最小价格，默认为-1
     * @param max_price 商品最大价格，默认为-1
     * @param remain 是否筛选有剩余的商品，默认为false
     * @param result 查询结果
     * @return 查询到商品的数量
     */
    int queryByFilter(const QString &name, int type, int min_price, int max_price, bool remain,
                      QList<QSharedPointer<Commodity>> &result) const;
    
    int queryByOwner(const QString &belong, QList<QSharedPointer<Commodity>> &result) const;
    
    void changeName(int id, const QString &name) const;
    
    void changeIntro(int id, const QString &intro) const;
    
    void changePrice(int id, int price) const;
    
    void changeAmount(int id, int amount) const;
    
    static void discount(int type, int discount);
    
    void deleteData(int id) const;
};

#endif //PHASE1_COMMODITY_H
