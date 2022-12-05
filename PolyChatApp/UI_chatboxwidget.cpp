#include "UI_chatboxwidget.h"
#include "ui_chatboxwidget.h"

ChatBoxWidget::ChatBoxWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatBoxWidget)
{
    ui->setupUi(this);
}

ChatBoxWidget::~ChatBoxWidget()
{
    delete ui;
}

