QT -= gui
QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/users.cpp \
        src/commodity.cpp \
        src/database.cpp

TRANSLATIONS += \
    Phase1_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/users.h \
    include/commodity.h \
    include/database.h

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
