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
    ui->leUserName->setFocus();

    /* User checked button `login` */
    connect(ui->btnLogin, &QPushButton::clicked,
            this, &LoginWidget::userLogin);

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

    qDebug() << "IN LoginWidget::userLogin:" <<  localUserName <<  localUserGroupNumber <<  localIpAddress;
    DAL::initAndShowChatList(nullptr);  // If user login, then show ChatList. After this `ChatList` Widget is main windows
    qDebug() << "IN LoginWidget::userLogin222:" <<  localUserName <<  localUserGroupNumber <<  localIpAddress;
}



LoginWidget::~LoginWidget()
{
    delete ui;

}
