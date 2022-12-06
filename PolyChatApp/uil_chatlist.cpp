#include "uil_chatlist.h"
#include "ui_chatlist.h"

ChatList::ChatList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatList)
{
    ui->setupUi(this);
}

ChatList::~ChatList()
{
    delete ui;
}
