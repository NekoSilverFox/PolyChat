QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat.cpp \
    main.cpp \
    tcpclient.cpp \
    tcpserver.cpp \
    uil_addchat.cpp \
    uil_chatboxwidget.cpp \
    uil_chatlist.cpp \
    uil_loginwidget.cpp

HEADERS += \
    bll_polychat.h \
    chat.h \
    dal_polychat.h \
    db_localdata.h \
    signaltype.h \
    tcpclient.h \
    tcpserver.h \
    uil_addchat.h \
    uil_chatboxwidget.h \
    uil_chatlist.h \
    uil_loginwidget.h

FORMS += \
    addchat.ui \
    chatboxwidget.ui \
    chatlist.ui \
    loginwidget.ui \
    tcpclient.ui \
    tcpserver.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = PolyChat

RESOURCES += \
    resource.qrc

TRANSLATIONS += language_zh_CN.ts \
                language_EN.ts \
                language_RU.ts

RC_ICONS = logo_fox.ico
