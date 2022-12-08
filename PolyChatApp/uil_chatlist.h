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
    bool isBtnChatInVector(QString name);

    bool isPortExist(const qint16 port);  // 查找一个端口号是否被占用
    qint16 getRandomPort(); //获取一个不重复的随机端口号
    bool isChatExist(const QString name);  // 查找一个名称的群聊是否已经存在
    Chat* getAndInsertNewChat(QString name, qint16 port, bool isOpen = true);  // 创建并插入新的聊天窗口信息

private:
    Ui::ChatList *ui;

    QUdpSocket* udpSocket;
    QVector<QPair<Chat*, QToolButton*>> vPair_OChat_BtnChat;  // 第一个是 Chat 对象，第二个是 按钮对象
//    QVector<Chat*> vChat;
//    QVector<QToolButton*> vBtnChat;  // 窗口中的聊天列表
//    QVector<bool> vIsOpenChat;
};

#endif // UIL_CHATLIST_H
