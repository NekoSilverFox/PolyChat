#include "uil_chatlist.h"
#include "ui_chatlist.h"

ChatList::ChatList(QWidget* parent, QString localUserName, QString localUserGroupNumber, QString localIpAddress) :
    QWidget(parent),
    ui(new Ui::ChatList)
{
    ui->setupUi(this);

    /* Init data on UI */
    ui->lbName->setText(localUserName);
    ui->lbGroupNumber->setText(localUserGroupNumber);
    ui->lbIP->setText(localIpAddress);
}

ChatList::~ChatList()
{
    delete ui;
}
