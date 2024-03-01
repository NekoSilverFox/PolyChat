#ifndef UIL_CHATLIST_H
#define UIL_CHATLIST_H

#include "chat.h"

#include <QWidget>
#include <QUdpSocket>
#include <QToolButton>
#include <QRegularExpression>

namespace Ui {
class ChatList;
}

class ChatList : public QWidget
{
    Q_OBJECT

public:
    explicit ChatList(QWidget *parent, QString localUserName, QString localUserGroupNumber, QHostAddress localIpAddress);
    ~ChatList();

#if !QT_NO_DEBUG  // 用于开放 public 权限给单元测试
public:
#else
private:
#endif
    void receiveMessage();  // 接收 UDP 消息

    bool isPortExist(const qint16 port);  // 查找一个端口号是否被占用
    bool isChatExist_But_BtnNotExist(const QString &name); // UNUSED
    bool isChatExist(const QString &name);  // 查找一个名称的群聊是否已经存在
    bool isChatOpen(const QString &name);

    void addBtnChatInLayout(QToolButton* btn);  // 添加按钮对象到 Layout

    QToolButton* getNewBtn(QString btn_text, qint16 port, bool isOpen);
    qint16 getRandomPort();  //获取一个不重复的随机端口号

    bool setChatState(const QString &name, bool state);  // 设置聊天窗口为打开或者关闭
    bool updateBtnInvPair(const QString &name, QToolButton* btn); // UNUSED

    bool isNeedHideBtn(QString textOnBtn);  // 根据正则表达式，判断是否需要隐藏按钮


private:
    Ui::ChatList* ui;

    QUdpSocket* udpSocket;


#if !QT_NO_DEBUG  // 用于开放 public 权限给单元测试
public:
#else
private:
#endif
    QVector<QPair<Chat*, QToolButton*>> vPair_OChat_BtnChat;  // 第一个是 Chat 对象，第二个是 按钮对象
};

#endif // UIL_CHATLIST_H
