#ifndef CHATBOXWIDGET_H
#define CHATBOXWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatBoxWidget; }
QT_END_NAMESPACE

class ChatBoxWidget : public QWidget
{
    Q_OBJECT

public:
    ChatBoxWidget(QWidget* parent, QString name, qint16 port);
    ~ChatBoxWidget();

private:
    Ui::ChatBoxWidget* ui;

public:
    QString name;
    qint16 port;
};
#endif // CHATBOXWIDGET_H
