#include "uil_loginwidget.h"
#include "ui_loginwidget.h"

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
    if (ui->leUserName->text().isEmpty() || ui->leUserGroupNumber->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Name or Group number can not be empty");
        return;
    }

    qDebug() << ui->leUserName->text() << ui->leUserGroupNumber->text();
}



LoginWidget::~LoginWidget()
{
    delete ui;
}
