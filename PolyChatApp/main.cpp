#include "uil_chatboxwidget.h"

#include <QApplication>
#include <uil_loginwidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWidget login;
    login.show();

    ChatBoxWidget w;
    w.show();
    return a.exec();
}
