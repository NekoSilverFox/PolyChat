#ifndef CHATBOXWIDGET_H
#define CHATBOXWIDGET_H
#include "signaltype.h"

#include <QWidget>
#include <QTimer>
#include <QUdpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class ChatBoxWidget; }
QT_END_NAMESPACE

class ChatBoxWidget : public QWidget
{
    Q_OBJECT

public:
    ChatBoxWidget(QWidget* parent, QString name, qint16 port);
    ~ChatBoxWidget();

    void sendUDPSignal(const SignalType type);  // 广播 UDP 消息
    QString getAndCleanMsg();

private:
    Ui::ChatBoxWidget* ui;

    QUdpSocket* udpSocketOnPortChatList;
    QUdpSocket* udpSocketOnPortChatBox;
    QString name;
    qint16 port;
};
#endif // CHATBOXWIDGET_H
