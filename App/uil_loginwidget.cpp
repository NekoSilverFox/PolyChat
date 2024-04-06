#include "uil_loginwidget.h"
#include "ui_loginwidget.h"
#include "dal_polychat.h"

#include <QMessageBox>
#include <QPalette>
#include <QMainWindow>
#include <QMenuBar>


LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("PolyChat Login"));
    this->setWindowIcon(QIcon(":/icon/icons/nekosilverfox.png"));
    ui->leUserName->setFocus();
//    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setAutoFillBackground(true);
    QPalette background = this->palette();
    background.setBrush(QPalette::Window,
                        QBrush(QPixmap(":/bk/background/bg-login.png").scaled(// 缩放背景图.
                            this->size(),
                            Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    this->setPalette(background);

    /* User checked button `login` */
    connect(ui->btnLogin, &QPushButton::clicked,
            this, &LoginWidget::userLogin);

    connect(ui->btnInfo, &QPushButton::clicked,
            this, [=](){
        QMessageBox::about(this, "About Polychat",
                                 "Project SPbSTU<br><br>"
                                 "Source Code:<br>"
                                 "<a href=\"https://github.com/NekoSilverFox/PolyChat\">[Github] PolyChat</a>"
                                 "<br><br>"
                                 "License: Apache License 2.0"
                                 "<br><br>"
                                 "Made on Qt 6.2");
    });
}


/** [SLOT] User Login
 * @brief LoginWidget::userLogin
 */
void LoginWidget::userLogin()
{
    /* Check user info in LineEdit, if OK then init data in DB */
    bool isSuccInitLocalUser = DAL::initLocalUser(ui->leUserName->text(), ui->leUserGroupNumber->text());
    if (!isSuccInitLocalUser)
    {
        QMessageBox::warning(this, ("Warning"), tr("Name or Group number can not be empty"));
        return;
    }
    this->close();

    DAL::initAndShowChatList(nullptr);  // If user login, then show ChatList. After this `ChatList` Widget is main windows
}



LoginWidget::~LoginWidget()
{
    delete ui;

}
