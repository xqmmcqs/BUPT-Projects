/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/5/6
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include <QFont>
#include <QHostAddress>

#include "clientmodel.h"

int main(int argc, char * argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    
    QGuiApplication app(argc, argv);
    
    QFont font;
    QFontDatabase::addApplicationFont(":/Fonts/SourceHanSerif");
    font.setPointSize(15);
    font.setFamily("Source Han Serif CN");
    app.setFont(font);
    
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject * obj, const QUrl &objUrl)
                     {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    
    ClientModel model(&app, 8946);
    engine.rootContext()->setContextProperty("client_model", &model);
    
    engine.load(url);
    
    return app.exec();
}
