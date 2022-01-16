/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include <QCoreApplication>

#include "include/server.h"

/// 调试信息格式化输出
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char * file = context.file ? context.file : "";
    switch (type)
    {
        case QtDebugMsg:
            fprintf(stdout, "\x1b[33m[Debug]\x1b[36m %s:%u \x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtInfoMsg:
            fprintf(stdout, "\x1b[33m[Info]\x1b[36m %s:%u \x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stdout, "\x1b[33m[Warning]\x1b[36m %s:%u \x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stdout, "\x1b[33m[Critical]\x1b[36m %s:%u \x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
        case QtFatalMsg:
            fprintf(stdout, "\x1b[33m[Fatal]\x1b[36m %s:%u \x1b[0m %s\n", file, context.line, localMsg.constData());
            break;
    }
}

int main(int argc, char * argv[])
{
    qInstallMessageHandler(messageHandler);
    QCoreApplication a(argc, argv);
    
    Database database("defaultConnection", "../data/users.txt");
    CommodityManage commodityManage(&database);
    OrderManage orderManage(&database);
    UserManage userManage(&database, &commodityManage, &orderManage);
    Server server(&a, 8946, &userManage);
    
    return a.exec();
}
