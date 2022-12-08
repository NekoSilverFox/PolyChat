#include "uil_chatlist.h"
#include "ui_chatlist.h"
#include "uil_addchat.h"

#include "dal_polychat.h"
#include "signaltype.h"

#include "uil_chatboxwidget.h"

#include <QMessageBox>

ChatList::ChatList(QWidget* parent, QString localUserName, QString localUserGroupNumber, QHostAddress localIpAddress) :
    QWidget(parent),
    ui(new Ui::ChatList)
{
    ui->setupUi(this);


    qDebug() << "IN ChatList::ChatList:" <<  ::localUserName <<  ::localUserGroupNumber <<  ::localIpAddress;

    /* Init data on UI */
    ui->lbName->setText(localUserName);
    ui->lbGroupNumber->setText(localUserGroupNumber);
    ui->lbIP->setText(localIpAddress.toString());

    /* 接收（监听）消息 */
    this->udpSocket = new QUdpSocket(this);
    this->udpSocket->bind(DAL::getPortChatList(),  // 绑定端口号
                          QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);  // 共享地址 + 断线重连
    connect(udpSocket, &QUdpSocket::readyRead,
            this, &ChatList::receiveMessage);


    /* 点击添加群聊按钮，会返回一个要创建群聊名称的信号 */
    connect(ui->btnNewChat, &QToolButton::clicked,
            this, [=](){
        AddChat* addChat = new AddChat(nullptr);
        addChat->setAttribute(Qt::WA_DeleteOnClose);
        addChat->setWindowModality(Qt::ApplicationModal);
        addChat->show();

        connect(addChat, &AddChat::addNewChat,
                this, [=](QString name){

                if (isChatExist(name))
                {
                    QMessageBox::warning(this, "Warning", "Chat with the same name already exists");
                    return;
                }

                /* 条件满足，创建群聊，并且把他放入到本地记录 */
                qint16 port = getRandomPort();
                QToolButton* btn = getNewBtn(name, port, true);
                Chat* chat = new Chat(name, port, true);
                this->vPair_OChat_BtnChat.push_back(QPair<Chat*, QToolButton*>(chat, btn));
                addBtnChatInLayout(btn);

                /* 条件满足，添加新的聊天窗口 */
                ChatBoxWidget* chatBoxWidget = new ChatBoxWidget(nullptr, name, port);
                chatBoxWidget->setAttribute(Qt::WA_DeleteOnClose);
                chatBoxWidget->show();

        });
    });

}

ChatList::~ChatList()
{
    delete ui;
}

/** 更新窗口中的聊天列表
 * @brief addBtnChat
 */
void ChatList::addBtnChatInLayout(QToolButton* btn)
{
    if (nullptr == btn) return;

    ui->vLayout->addWidget(btn); // 加到垂直布局中
}


/* 接收和解析 UDP 消息 */
void ChatList::receiveMessage()
{
    qDebug() << "ChatList::receiveMessage : Msg get!!!!";
    /* 拿到数据报文 */
    qint64 size = udpSocket->pendingDatagramSize();
    QByteArray byteArrayGetUDP = QByteArray(size, 0);
    udpSocket->readDatagram(byteArrayGetUDP.data(), size);

    /** 解析数据
    *   第1段：数据类型
    *   第2段：发送本信号的群组名称
    *   第3段：发送本信号的群组端口号
    *   第4段：当前（本地）用户名 localUserName
    *   第5段：当前（本地）用户组号 localUserGroupNumber
    *   第6段：当前（本地）用户ip localIpAddress
    *   第7段：具体内容
    */
    SignalType      signalType_1          ;
    QString         chatName_2            ;
    qint16          chatPort_3            ;
    QString         localUserName_4       ;
    QString         localUserGroupNumber_5;
    QHostAddress    localIpAddress_6      ;
    QString         msg_7;

    QDataStream dataStream(&byteArrayGetUDP, QIODevice::ReadOnly);

    dataStream >> signalType_1          ;
    dataStream >> chatName_2            ;
    dataStream >> chatPort_3            ;
    dataStream >> localUserName_4       ;
    dataStream >> localUserGroupNumber_5;
    dataStream >> localIpAddress_6      ;
    dataStream >> msg_7                 ;

    switch (signalType_1)
    {
    /* 接受到 ChatBox 存在的信号 */
    case SignalType::ChatExist:
    {
        /* 如果不存在于本地记录，说明是在局域网上的其他群聊，在本窗口里肯定也没按钮 */
        if (!isChatExist(chatName_2))
        {
            Chat* chat = new Chat(chatName_2, chatPort_3, false);
            QToolButton* btn = getNewBtn(chatName_2, chatPort_3, false);
            this->vPair_OChat_BtnChat.push_back(QPair<Chat*, QToolButton*>(chat, btn));
            addBtnChatInLayout(btn);
        }
    }
        break;

    case SignalType::ChatDestory: break;

    default:
        break;
    }

    qDebug() << "ChaList receiveMessage signal: SignalType::" << signalType_1
             << chatName_2 << chatPort_3
             << localUserName_4
             << localUserGroupNumber_5
             << localIpAddress_6;
}

/** 查找一个名称的群聊是否已经存在
 * @brief isChatExist
 * @param name
 * @return 存在返回 true
 */
bool ChatList::isChatExist(const QString name)
{
    for (auto i : this->vPair_OChat_BtnChat)
    {
        if (name == i.first->name) return true;
    }

    return false;
}

/** 聊天已经记录（存在）的情况下，按钮不存在于 this 的 vPair_OChat_BtnChat 中
 * @brief ChatList::isBtnChatInVector
 * @param name
 * @return
 */
bool ChatList::isChatExist_But_BtnNotExist(QString name)
{
    if (!isChatExist(name)) return false;

    for (auto i: this->vPair_OChat_BtnChat)
    {
        if ((name == i.first->name) && (nullptr == i.second)) return true;
    }
    return false;
}

/** 查找一个端口号是否被占用
 * @brief isPortExist
 * @param port
 * @return
 */
bool ChatList::isPortExist(const qint16 port)
{
    if (vPair_OChat_BtnChat.isEmpty()) return false;
    for (auto i : vPair_OChat_BtnChat)
    {
        if ((port == i.first->port) || (port == PORT_CHAT_LIST)) return true;
    }

    return false;
}

/** 获取一个不重复的随机端口号
 * @brief getRandomPort
 * @return
 */
qint16 ChatList::getRandomPort()
{
    while (true)
    {
        qint16 port = QRandomGenerator::global()->bounded(PORT_MIN, PORT_MAX);
        if (!isPortExist(port)) return port;
    }
}

/** 创建并插入新的聊天窗口信息
 * @brief getAndInsertNewChat
 * @param name
 * @param port
 * @param isOpen
 * @return
 */
Chat* ChatList::getAndInsertNewChat(QString name, qint16 port, bool isOpen, QToolButton* btnChat)
{
    Chat* chat = new Chat(name, port, isOpen);
    vPair_OChat_BtnChat.push_back(QPair<Chat*, QToolButton*>(chat, btnChat));
    return chat;
}


bool ChatList::isChatOpen(QString name)
{
    for (auto i : vPair_OChat_BtnChat)
    {
        if (name == i.first->name) return i.first->isOpen;
    }
    return true;  // FIX!
}

/** 创建并返回一个 按钮对象
 * @brief ChatList::getNewBtn
 * @param btn_text
 * @param port
 * @param isOpen
 * @return
 */
QToolButton* ChatList::getNewBtn(QString btn_text, qint16 port, bool isOpen)
{
    /* 设置头像 */
    QToolButton* btn = new QToolButton;
//    btn->setText(QString("[%1] %2").arg(chatPort).arg(chatName));
    btn->setText(btn_text);

    btn->setIcon(QIcon(":/icon/icons/user-group.png"));
    btn->setAutoRaise(true);  // 按钮透明风格
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);  // 设置显示图标和文字
    btn->setFixedSize(220, 50);

    return btn;
}


bool ChatList::updateBtnInvPair(QString name, QToolButton* btn)
{
    for (auto i : this->vPair_OChat_BtnChat)
    {
        if (name == i.first->name)
        {
            i.second = btn;
            return true;
        }
    }

    qDebug() << "[ERROR] File to update btn, ChatBox named" << name << "do not exits in local vPair_OChat_BtnChat";
    return false;
}
