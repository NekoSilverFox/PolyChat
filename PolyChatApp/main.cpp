#include "uil_chatboxwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatBoxWidget w;
    w.show();
    return a.exec();
}
