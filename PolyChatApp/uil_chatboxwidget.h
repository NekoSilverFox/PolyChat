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
    ChatBoxWidget(QWidget* parent = nullptr);
    ~ChatBoxWidget();

private:
    Ui::ChatBoxWidget* ui;
};
#endif // CHATBOXWIDGET_H
