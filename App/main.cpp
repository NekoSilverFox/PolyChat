#include <QApplication>
#include <QTranslator>
#include <QHostInfo>

#include "uil_chatboxwidget.h"
#include "uil_loginwidget.h"
#include "uil_chatlist.h"
#include "tcpserver.h"

#include "db_localdata.h"

QString      localUserName           = "";               // User Name (will get in user login)
QString      localUserGroupNumber    = "";               // Group number (will get in user login)
QHostAddress localIpAddress          = QHostAddress();
QString      localHostName           = QHostInfo::localHostName();

ChatList*    chatList                = nullptr;          // Widget ChatList (Only one)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/icons/logo_fox.png"));

#if !QT_NO_DEBUG
    qDebug() << "[Debug] appDirPath: " << QCoreApplication::applicationDirPath();   //程序所在路径
    qDebug() << "[Debug] localHostName: "<<localHostName;
    qDebug() << "[Debug] QHostAddress::LocalHost: " << QHostAddress::LocalHost;
#endif

    LoginWidget login;
    login.show();

    return a.exec();
}
