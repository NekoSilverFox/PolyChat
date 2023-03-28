QT += core testlib gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TARGET = PolyChatTester
TEMPLATE = app

CONFIG += console
CONFIG += c++17

SOURCES += tst_PolyChatTester.cpp \
    ../App/chat.cpp \
    ../App/tcpclient.cpp \
    ../App/tcpserver.cpp \
    ../App/uil_addchat.cpp \
    ../App/uil_chatboxwidget.cpp \
    ../App/uil_chatlist.cpp \
    ../App/uil_loginwidget.cpp \

HEADERS += \
    ../App/bll_polychat.h \
    ../App/chat.h \
    ../App/dal_polychat.h \
    ../App/db_localdata.h \
    ../App/signaltype.h \
    ../App/tcpclient.h \
    ../App/tcpserver.h \
    ../App/uil_addchat.h \
    ../App/uil_chatboxwidget.h \
    ../App/uil_chatlist.h \
    ../App/uil_loginwidget.h

FORMS += \
    ../App/addchat.ui \
    ../App/chatboxwidget.ui \
    ../App/chatlist.ui \
    ../App/loginwidget.ui \
    ../App/tcpclient.ui \
    ../App/tcpserver.ui

QMAKE_CXXFLAGS += -w
