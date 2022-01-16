/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/5/1
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include <QtCore>
#include <QTextStream>

#include "include/users.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char * file = context.file ? context.file : "";
    const char * function = context.function ? context.function : "";
    switch (type)
    {
        case QtDebugMsg:
            fprintf(stdout, "\x1b[33m[Debug]\x1b[36m(%s:%u)\x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtInfoMsg:
            fprintf(stdout, "\x1b[33m[Info]\x1b[36m(%s:%u)\x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stdout, "\x1b[33m[Warning]\x1b[36m(%s:%u)\x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stdout, "\x1b[33m[Critical]\x1b[36m(%s:%u)\x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtFatalMsg:
            fprintf(stdout, "\x1b[33m[Fatal]\x1b[36m(%s:%u)\x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
    }
}

int main()
{
    qInstallMessageHandler(messageHandler);
    Database database("defaultConnection", "users.txt");
    CommodityManage commodityManage(&database);
    UserManage userManage(&database, &commodityManage);
    QTextStream istream(stdin);
    QTextStream ostream(stdout);
    QString line;
    QJsonValue token(QJsonValue::Null);
    QVector<QString> commodityType{"服饰", "书籍", "电子产品"};
    QVector<QString> userType{"商家", "消费者"};
    
    qInfo() << "初始化完成，欢迎来到电商交易平台";
    while (true)
    {
        ostream << "E-Commerce> " << flush;
        istream.readLineInto(&line);
        auto list = line.split(" ");
        list[0] = list[0].toUpper();
        if (list[0] == "REGISTER")
        {
            if (!token.isNull())
            {
                qInfo() << "已经有用户登录";
                continue;
            }
            bool ok = false;
            if (list.size() != 4 || list[3].toInt(&ok), !ok) goto error;
            auto result = userManage.registerUser(list[1], list[2], list[3].toInt());
            if (result.isEmpty())
                qInfo() << "注册用户成功";
            else
                qInfo() << "注册用户失败 " << result;
        }
        else if (list[0] == "LOGIN")
        {
            if (!token.isNull())
            {
                qInfo() << "已经有用户登录";
                continue;
            }
            if (list.size() != 3) goto error;
            QJsonObject response;
            auto result = userManage.login(list[1], list[2], response);
            if (result.isEmpty())
            {
                token = response;
                qInfo() << "登录成功";
            }
            else
                qInfo() << "登录失败 " << result;
        }
        else if (list[0] == "SETPASSWORD")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            if (list.size() != 2) goto error;
            auto result = userManage.changePasswd(token.toObject(), list[1]);
            if (result.isEmpty())
                qInfo() << "修改密码成功";
            else
                qInfo() << "修改密码失败 " << result;
        }
        else if (list[0] == "LOGOUT")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            if (list.size() != 1) goto error;
            auto result = userManage.logout(token.toObject());
            if (result.isEmpty())
                qInfo() << "登出成功";
            else
                qInfo() << "登出失败 " << result;
            token = QJsonValue(QJsonValue::Null);
        }
        else if (list[0] == "MINE")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            if (list.size() != 1) goto error;
            QJsonObject response;
            auto result = userManage.getInfo(token.toObject(), response);
            if (result.isEmpty())
            {
                qInfo() << "查询成功";
                qInfo() << "用户名" << response["username"].toString() << "类型" << userType[response["type"].toInt()]
                        << "余额" << response["balance"].toInt() / 100.0;
            }
            else
                qInfo() << "查询失败 " << result;
        }
        else if (list[0] == "ADDMONEY")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            bool ok = false;
            if (list.size() != 2 || list[1].toDouble(&ok), !ok) goto error;
            auto result = userManage.changeBalance(token.toObject(), list[1].toDouble() * 100);
            if (result.isEmpty())
                qInfo() << "余额充值成功";
            else
                qInfo() << "余额充值失败 " << result;
        }
        else if (list[0] == "SUBMONEY")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            bool ok = false;
            if (list.size() != 2 || list[1].toDouble(&ok), !ok) goto error;
            auto result = userManage.changeBalance(token.toObject(), -list[1].toDouble() * 100);
            if (result.isEmpty())
                qInfo() << "余额消费成功";
            else
                qInfo() << "余额消费失败" << result;
        }
        else if (list[0] == "ADDITEM")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            bool ok = false;
            if (list.size() != 6 || list[4].toDouble(&ok), !ok || list[5].toInt(&ok), !ok) goto error;
            QJsonObject info;
            info.insert("name", list[1]);
            info.insert("intro", list[2]);
            info.insert("type", list[3]);
            info.insert("price", static_cast<int>(list[4].toDouble() * 100));
            info.insert("amount", list[5].toInt());
            auto result = userManage.addCommodity(token.toObject(), info);
            if (result.isEmpty())
                qInfo() << "添加物品成功";
            else
                qInfo() << "添加物品失败" << result;
        }
        else if (list[0] == "DISPITEM")
        {
            if (list.size() != 1) goto error;
            QJsonObject query;
            query.insert("TYPE", 0);
            QJsonArray response;
            auto result = userManage.queryCommodity(query, response);
            if (result.isEmpty())
            {
                auto total = response.size();
                qInfo() << "查询到" << total << "件商品";
                for (int i = 0; i < total; ++i)
                {
                    QJsonObject item = response[i].toObject();
                    qInfo() << "商品ID" << item["id"].toInt() << "名称"
                            << item["name"].toString() << "类型" << commodityType[item["type"].toInt()] << "价格"
                            << item["price"].toInt() / 100.0 << "折扣" << item["discount"].toInt() / 100.0 << "数量"
                            << item["amount"].toInt();
                }
            }
            else
                qInfo() << "查询失败" << result;
        }
        else if (list[0] == "DISPMYITEM")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            bool ok = false;
            if (list.size() != 1) goto error;
            QJsonObject query;
            query.insert("TYPE", 3);
            query.insert("token", token);
            QJsonArray response;
            auto result = userManage.queryCommodity(query, response);
            if (result.isEmpty())
            {
                auto total = response.size();
                qInfo() << "查询到" << total << "件商品";
                for (int i = 0; i < total; ++i)
                {
                    QJsonObject item = response[i].toObject();
                    qInfo() << "商品ID" << item["id"].toInt() << "名称" << item["name"].toString() << "类型"
                            << commodityType[item["type"].toInt()] << "价格" << item["price"].toInt() / 100.0 << "折扣"
                            << item["discount"].toInt() / 100.0 << "数量" << item["amount"].toInt();
                }
            }
            else
                qInfo() << "查询失败" << result;
        }
        else if (list[0] == "DISPITEMID")
        {
            bool ok = false;
            if (list.size() != 2 || list[1].toInt(&ok), !ok) goto error;
            QJsonObject query;
            query.insert("TYPE", 1);
            query.insert("id", list[1].toInt());
            QJsonArray response;
            auto result = userManage.queryCommodity(query, response);
            if (result.isEmpty())
            {
                auto total = response.size();
                qInfo() << "查询到" << total << "件商品";
                for (int i = 0; i < total; ++i)
                {
                    QJsonObject item = response[i].toObject();
                    qInfo() << "商品ID" << item["id"].toInt() << "名称" << item["name"].toString() << "介绍"
                            << item["intro"].toString() << "类型" << commodityType[item["type"].toInt()] << "价格"
                            << item["price"].toInt() / 100.0 << "折扣" << item["discount"].toInt() / 100.0 << "数量"
                            << item["amount"].toInt();
                }
            }
            else
                qInfo() << "查询失败" << result;
        }
        else if (list[0] == "SEARCH")
        {
            bool ok = false;
            if (list.size() < 1) goto error;
            QJsonObject query;
            query.insert("TYPE", 2);
            for (int i = 1; i < list.size(); i += 2)
            {
                if (list[i].toUpper() == "NAME")
                {
                    if (i + 1 == list.size()) goto error;
                    query.insert("name", list[i + 1]);
                }
                else if (list[i].toUpper() == "TYPE")
                {
                    if (i + 1 == list.size()) goto error;
                    query.insert("type", list[i + 1]);
                }
                else if (list[i].toUpper() == "MINPRICE")
                {
                    if (i + 1 == list.size() || list[i + 1].toDouble(&ok), !ok) goto error;
                    query.insert("min_price", static_cast<int>(list[i + 1].toDouble() * 100));
                }
                else if (list[i].toUpper() == "MAXPRICE")
                {
                    if (i + 1 == list.size() || list[i + 1].toDouble(&ok), !ok) goto error;
                    query.insert("max_price", static_cast<int>(list[i + 1].toDouble() * 100));
                }
                else if (list[i].toUpper() == "REMAIN")
                {
                    if (i + 1 == list.size() || list[i + 1].toInt(&ok), !ok) goto error;
                    query.insert("remain", list[i + 1].toInt());
                }
                else goto error;
            }
            QJsonArray response;
            auto result = userManage.queryCommodity(query, response);
            if (result.isEmpty())
            {
                auto total = response.size();
                qInfo() << "查询到" << total << "件商品";
                for (int i = 0; i < total; ++i)
                {
                    QJsonObject item = response[i].toObject();
                    qInfo() << "商品ID" << item["id"].toInt() << "名称" << item["name"].toString() << "类型"
                            << commodityType[item["type"].toInt()] << "价格" << item["price"].toInt() / 100.0 << "折扣"
                            << item["discount"].toInt() / 100.0 << "数量" << item["amount"].toInt();
                }
            }
            else
                qInfo() << "查询失败" << result;
        }
        else if (list[0] == "SETITEM")
        {
            if (token.isNull())
            {
                qInfo() << "未登录";
                continue;
            }
            bool ok = false;
            if (list.size() < 2 || list[1].toInt(&ok), !ok) goto error; // 检查id
            QJsonObject query;
            query.insert("id", list[1].toInt());
            for (int i = 2; i < list.size(); i += 2)
            {
                if (list[i].toUpper() == "NAME")
                {
                    if (i + 1 == list.size()) goto error;
                    query.insert("name", list[i + 1]);
                }
                else if (list[i].toUpper() == "INTRO")
                {
                    if (i + 1 == list.size()) goto error;
                    query.insert("intro", list[i + 1]);
                }
                else if (list[i].toUpper() == "PRICE")
                {
                    if (i + 1 == list.size() || list[i + 1].toDouble(&ok), !ok) goto error;
                    query.insert("price", static_cast<int>(list[i + 1].toDouble() * 100));
                }
                else if (list[i].toUpper() == "AMOUNT")
                {
                    if (i + 1 == list.size() || list[i + 1].toInt(&ok), !ok) goto error;
                    query.insert("amount", list[i + 1].toInt());
                }
                else goto error;
            }
            auto result = userManage.changeCommodity(token.toObject(), query);
            if (result.isEmpty())
                qInfo() << "修改成功";
            else
                qInfo() << "修改失败" << result;
        }
        else if (list[0] == "DISCOUNT")
        {
            bool ok = false;
            if (list.size() != 3 || list[1].toInt(&ok), !ok || list[2].toInt(&ok), !ok) goto error;
            auto result = userManage.discount(list[1].toInt(), list[2].toInt());
            if (result.isEmpty())
                qInfo() << "打折成功";
            else
                qInfo() << "打折失败" << result;
        }
        else if (list[0] == "DELITEM")
        {
            bool ok = false;
            if (list.size() != 2 || list[1].toInt(&ok), !ok) goto error;
            auto result = userManage.deleteCommodity(token.toObject(), list[1].toInt());
            if (result.isEmpty())
                qInfo() << "删除成功";
            else
                qInfo() << "删除失败" << result;
        }
        else if (list[0] == "EXIT")
            return 0;
        else if (list[0] == "HELP")
        {
            qInfo() << "帮助：";
            if (list.size() == 1 || list[1].toUpper() == "REGISTER")
            {
                qInfo() << "REGISTER <用户名> <密码> 0/1";
                qInfo() << "    注册用户，0表示商家，1表示消费者";
            }
            if (list.size() == 1 || list[1].toUpper() == "LOGIN")
            {
                qInfo() << "LOGIN <用户名> <密码>";
                qInfo() << "    登录";
            }
            if (list.size() == 1 || list[1].toUpper() == "SETPASSWORD")
            {
                qInfo() << "SETPASSWORD <密码>";
                qInfo() << "    修改密码";
            }
            if (list.size() == 1 || list[1].toUpper() == "LOGOUT")
            {
                qInfo() << "LOGOUT";
                qInfo() << "    登出";
            }
            if (list.size() == 1 || list[1].toUpper() == "MINE")
            {
                qInfo() << "MINE";
                qInfo() << "    查询自己的用户信息";
            }
            if (list.size() == 1 || list[1].toUpper() == "ADDMONEY")
            {
                qInfo() << "ADDMONEY <金额>";
                qInfo() << "    给自己增加余额";
            }
            if (list.size() == 1 || list[1].toUpper() == "SUBMONEY")
            {
                qInfo() << "SUBMONEY <金额>";
                qInfo() << "    给自己减少余额";
            }
            if (list.size() == 1 || list[1].toUpper() == "ADDITEM")
            {
                qInfo() << "ADDITEM <名称> <介绍> 0/1/2 <价格> <数量>";
                qInfo() << "    添加一个物品，0表示服饰，1表示书籍，2表示电子产品";
            }
            if (list.size() == 1 || list[1].toUpper() == "DISPITEM")
            {
                qInfo() << "DISPITEM";
                qInfo() << "    查看所有商品";
            }
            if (list.size() == 1 || list[1].toUpper() == "DISPMYITEM")
            {
                qInfo() << "DISPMYITEM";
                qInfo() << "    查看自己的商品";
            }
            if (list.size() == 1 || list[1].toUpper() == "DISPITEMID")
            {
                qInfo() << "DISPITEMID <商品ID>";
                qInfo() << "    根据商品ID查看商品";
            }
            if (list.size() == 1 || list[1].toUpper() == "SEARCH")
            {
                qInfo() << "SEARCH NAME <商品名的子串> TYPE 0/1/2 MINPRICE <最小价格> MAXPRICE <最大价格> REMAIN 0/1";
                qInfo() << "    根据筛选条件查看商品，筛选条件均可选，并且顺序可以变化";
            }
            if (list.size() == 1 || list[1].toUpper() == "SETITEM")
            {
                qInfo() << "SETITEM <商品ID> NAME <名称> INTRO <介绍> PRICE <价格> AMOUNT <数量>";
                qInfo() << "    根据商品ID修改商品内容，除了ID外的参数均可选，并且顺序可以变化";
            }
            if (list.size() == 1 || list[1].toUpper() == "DISCOUNT")
            {
                qInfo() << "DISCOUNT 0/1/2 <折扣>";
                qInfo() << "    为某种类型的商品打折，折扣为0-100之间的整数";
            }
            if (list.size() == 1 || list[1].toUpper() == "DELITEM")
            {
                qInfo() << "DELITEM <商品ID>";
                qInfo() << "    删除某个商品";
            }
            if (list.size() == 1 || list[1].toUpper() == "HELP")
            {
                qInfo() << "HELP";
                qInfo() << "    打印本帮助";
            }
            if (list.size() == 1 || list[1].toUpper() == "EXIT")
            {
                qInfo() << "EXIT";
                qInfo() << "    退出系统";
            }
        }
        else
error:
            qInfo() << "输入指令有误";
    }
}

#pragma clang diagnostic pop