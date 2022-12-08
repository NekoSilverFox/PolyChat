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

    virtual void closeEvent(QCloseEvent*);  // 【重写】触发关闭事件

    void userJoin(QString name, QString groupNumber, QHostAddress ip);  // 处理用户加入
    void userLeft(QString name, QString time);  // 处理用户离开

signals:
    void signalClose();  // 【自定义信号】如果关闭窗口则发送关闭信号

private:
    void receiveUDPMessage();          // 接收 UDP 消息

private:
    Ui::ChatBoxWidget* ui;

    QUdpSocket* udpSocketOnPortChatList;
    QUdpSocket* udpSocketOnPortChatBox;
    QString name;
    qint16 port;
};
#endif // CHATBOXWIDGET_H
