#include "uil_loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    ui->leUserName->setFocus();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}
