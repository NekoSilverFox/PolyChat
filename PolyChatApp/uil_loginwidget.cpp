#include "uil_loginwidget.h"
#include "ui_loginwidget.h"
#include "dal_polychat.h"

#include <QMessageBox>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("PolyChat Login");
    this->setWindowIcon(QIcon(":/icon/icons/user-info.png"));
    ui->leUserName->setFocus();

    /* User checked button `login` */
    connect(ui->btnLogin, &QPushButton::clicked,
            this, &LoginWidget::userLogin);

    connect(ui->btnInfo, &QPushButton::clicked,
            this, [=](){
        QMessageBox::information(this, "About Polychat",
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
        QMessageBox::warning(this, "Warning", "Name or Group number can not be empty");
        return;
    }
    this->close();

    DAL::initAndShowChatList(nullptr);  // If user login, then show ChatList. After this `ChatList` Widget is main windows
}



LoginWidget::~LoginWidget()
{
    delete ui;

}
