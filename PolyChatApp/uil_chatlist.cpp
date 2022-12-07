#include "uil_chatlist.h"
#include "ui_chatlist.h"
#include "uil_addchat.h"

ChatList::ChatList(QWidget* parent, QString localUserName, QString localUserGroupNumber, QString localIpAddress) :
    QWidget(parent),
    ui(new Ui::ChatList)
{
    ui->setupUi(this);

    /* Init data on UI */
    ui->lbName->setText(localUserName);
    ui->lbGroupNumber->setText(localUserGroupNumber);
    ui->lbIP->setText(localIpAddress);

    connect(ui->btnNewChat, &QToolButton::clicked,
            this, [=](){
        AddChat* addChat = new AddChat(nullptr);
        addChat->setAttribute(Qt::WA_DeleteOnClose);
        addChat->setWindowModality(Qt::ApplicationModal);
        addChat->show();

    });
}

ChatList::~ChatList()
{
    delete ui;
}
