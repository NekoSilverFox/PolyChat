#include <QApplication>

#include "uil_chatboxwidget.h"
#include "uil_loginwidget.h"
#include "uil_chatlist.h"
#include "tcpserver.h"

#include "db_localdata.h"
QString      localUserName           = "";               // User Name (get in user login)
QString      localUserGroupNumber    = "";               // Group number (get in user login)
QHostAddress localIpAddress          = QHostAddress();
ChatList*    chatList                = nullptr;          // Widget ChatList (Only one)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/icons/logo_fox.png"));              

    LoginWidget login;
    login.show();

    return a.exec();
}
