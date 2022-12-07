#include "uil_chatboxwidget.h"
#include "ui_chatboxwidget.h"

ChatBoxWidget::ChatBoxWidget(QWidget* parent, QString name, qint16 port)
    : QWidget(parent)
    , ui(new Ui::ChatBoxWidget),
      name(name),
      port(port)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("[Chat] %1 on port %2").arg(name).arg(port));
}

ChatBoxWidget::~ChatBoxWidget()
{
    delete ui;
}

