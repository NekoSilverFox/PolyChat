#include "uil_chatboxwidget.h"

#include <QApplication>
#include <uil_loginwidget.h>
#include "uil_chatlist.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatList chatList;
    chatList.show();

#if 0
    LoginWidget login;
    login.show();

    ChatBoxWidget w;
    w.show();

#endif;

    return a.exec();
}
