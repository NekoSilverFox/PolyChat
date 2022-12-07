#ifndef UIL_CHATLIST_H
#define UIL_CHATLIST_H

#include "chat.h"

#include <QWidget>
#include <QUdpSocket>
#include <QToolButton>

namespace Ui {
class ChatList;
}

class ChatList : public QWidget
{
    Q_OBJECT

public:
    explicit ChatList(QWidget *parent, QString localUserName, QString localUserGroupNumber, QHostAddress localIpAddress);
    ~ChatList();

private:
    void receiveMessage();  // 接收 UDP 消息
    void addBtnChat(QString chatName, qint16 chatPort, bool isOpen);  // 更新窗口中的聊天列表

private:
    Ui::ChatList *ui;

    QUdpSocket* udpSocket;
    QVector<QToolButton*> vBtnChat;  // 窗口中的聊天列表
    QVector<bool> vIsOpenChat;
};

#endif // UIL_CHATLIST_H
