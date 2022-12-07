#include "uil_addchat.h"
#include "ui_addchat.h"

#include "uil_chatboxwidget.h"

#include "dal_polychat.h"

#include <QMessageBox>

AddChat::AddChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddChat)
{
    ui->setupUi(this);

    connect(ui->btnCancelAddChat, &QPushButton::clicked,
            this, [=](){this->close();});

    connect(ui->btnAddChat, &QPushButton::clicked,
            this, [=](){
        if (ui->leNameNewChat->text().isEmpty())
        {
            QMessageBox::warning(this, "Warning", "Name can not be empty");
            return;
        }

        if (DAL::isChatExist(ui->leNameNewChat->text()))
        {
            QMessageBox::warning(this, "Warning", "Chat with the same name already exists");
            return;
        }

        /* 条件满足，添加新的聊天窗口 */
        Chat* chat = DAL::getAndInsertNewChat(ui->leNameNewChat->text(), DAL::getRandomPort(), true);  // 添加到本地记录
        ChatBoxWidget* chatBoxWidget = new ChatBoxWidget(nullptr, chat->name, chat->port);
        chatBoxWidget->setAttribute(Qt::WA_DeleteOnClose);
        chatBoxWidget->show();

        this->close();
    });
}

AddChat::~AddChat()
{
    delete ui;
}
