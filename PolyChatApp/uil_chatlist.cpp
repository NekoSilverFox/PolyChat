#include "uil_chatlist.h"
#include "ui_chatlist.h"
#include "uil_addchat.h"

#include "dal_polychat.h"
#include "signaltype.h"


ChatList::ChatList(QWidget* parent, QString localUserName, QString localUserGroupNumber, QHostAddress localIpAddress) :
    QWidget(parent),
    ui(new Ui::ChatList)
{
    ui->setupUi(this);


//    DAL::setLocalUserName(localUserName);
//    DAL::setLocalUserGroupNumber(localUserGroupNumber);
//    DAL::setLocalIpAddress(localIpAddress);
    qDebug() << "IN ChatList::ChatList:" <<  DB::localUserName <<  DB::localUserGroupNumber <<  DB::localIpAddress;

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

    connect(ui->btnNewChat, &QToolButton::clicked,
            this, [=](){
        AddChat* addChat = new AddChat(nullptr);
        addChat->setAttribute(Qt::WA_DeleteOnClose);
        addChat->setWindowModality(Qt::ApplicationModal);
        addChat->show();
    });
}

ChatList::~ChatList()
{
    delete ui;
}

/** 更新窗口中的聊天列表
 * @brief addBtnChat
 */
void ChatList::addBtnChat(QString chatName, qint16 chatPort, bool isOpen)
{
    /* 设置头像 */
    QToolButton* btn = new QToolButton;
    btn->setText(QString("[%1] %2").arg(chatPort).arg(chatName));

    btn->setIcon(QIcon(":/icon/icons/user-group.png"));
    btn->setAutoRaise(true);  // 按钮透明风格
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);  // 设置显示图标和文字
    btn->setFixedSize(220, 50);

    vBtnChat.push_back(btn);
    vIsOpenChat.push_back(isOpen);

    ui->vLayout->addWidget(btn); // 加到垂直布局中
}


/* 接收和解析 UDP 消息 */
void ChatList::receiveMessage()
{
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
    case SignalType::ChatExist:
        if (DAL::isChatExist(chatName_2)) return;

    {
        bool isOpen = false;
        if (localUserName_4 == DAL::getLocalUserName()) isOpen = true;

        DAL::getAndInsertNewChat(chatName_2, chatPort_3, isOpen);
    }
        break;

    case SignalType::ChatDestory: break;

    default:
        break;
    }


}
