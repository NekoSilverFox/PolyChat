#include "uil_chatboxwidget.h"
#include "ui_chatboxwidget.h"
#include "dal_polychat.h"
#include <QDataStream>

ChatBoxWidget::ChatBoxWidget(QWidget* parent, QString name, qint16 port)
    : QWidget(parent)
    , ui(new Ui::ChatBoxWidget),
      name(name),
      port(port)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("[Chat] %1 on port %2").arg(name).arg(port));

    /* 对所有窗口的同样地址广播 8888 (告诉 ChatList 本窗口存在) */
    this->udpSocketOnPortChatList = new QUdpSocket(this);
    this->udpSocketOnPortChatList->bind(DAL::getPortChatList(),
                                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);  // 共享地址 + 断线重连


    /* 对所有本窗口的地址广播 自己的随机端口 */
    this->udpSocketOnPortChatBox = new QUdpSocket(this);
    this->udpSocketOnPortChatBox->bind(this->port,
                                       QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);  // 共享地址 + 断线重连

    /* 每隔 BORDCAST_TIME_STEP 毫秒就发送一下当前窗口打开的信息 */
    QTimer* timer = new QTimer(this);
    timer->start(BORDCAST_TIME_STEP);
    connect(timer, &QTimer::timeout,
            this, [=](){ sendUDPSignal(SignalType::ChatExist); });

    // 创建新窗口就相当于新用户进入
    sendUDPSignal(SignalType::UserJoin);

    /* 点击发送按钮发送消息 */
    connect(ui->btnSend, &QPushButton::clicked,
            [=](){sendUDPSignal(SignalType::Msg);});
}

ChatBoxWidget::~ChatBoxWidget()
{
    delete ui;
}

/** 广播 UDP 消息
 * @brief sendMsg
 * @param type
 */
void ChatBoxWidget::sendUDPSignal(const SignalType type)
{
    /** 消息分为 5 类 (SignalType)，所以要对数据做分段处理
    *   第1段：数据类型
    *   第2段：发送本信号的群组名称
    *   第3段：发送本信号的群组端口号
    *   第4段：当前（本地）用户名 localUserName
    *   第5段：当前（本地）用户组号 localUserGroupNumber
    *   第6段：当前（本地）用户ip localIpAddress
    *   第7段：具体内容
    */
    SignalType      signalType_1            = type;
    QString         chatName_2              = this->name;
    qint16          chatPort_3              = this->port;
    QString         localUserName_4         = DAL::getLocalUserName();
    QString         localUserGroupNumber_5  = DAL::getLocalUserGroupNumber();
    QHostAddress    localIpAddress_6        = DAL::getLocalIpAddress();


    QByteArray      resByteArray;   // 最终的消息内容
    QDataStream     dataStream(&resByteArray, QIODevice::WriteOnly);

    dataStream << signalType_1;             // 第1段：数据类型
    dataStream << chatName_2;               // 第2段：发送本信号的群组名称
    dataStream << chatPort_3;               // 第3段：发送本信号的群组端口号
    dataStream << localUserName_4;          // 第4段：当前（本地）用户名 localUserName
    dataStream << localUserGroupNumber_5;   // 第5段：当前（本地）用户组号 localUserGroupNumber
    dataStream << localIpAddress_6;         // 第6段：当前（本地）用户ip localIpAddress

    /* 添加消息块 7 的内容 */
    switch (type) {

    case SignalType::ChatExist:
        dataStream << "SignalType::ChatExist";
        udpSocketOnPortChatList->writeDatagram(resByteArray,
                                               QHostAddress(QHostAddress::Broadcast),
                                               DAL::getPortChatList());
        break;  // END SignalType::ChatExist

    case SignalType::ChatDestory:
        dataStream << "SignalType::ChatDestory";
        udpSocketOnPortChatList->writeDatagram(resByteArray,
                                               QHostAddress(QHostAddress::Broadcast),
                                               DAL::getPortChatList());
        break;  // END SignalType::ChatDestory

    case SignalType::Msg:
        if (ui->msgTextEdit->toPlainText().isEmpty()) return;

        dataStream << getAndCleanMsg();
        udpSocketOnPortChatBox->writeDatagram(resByteArray,
                                              QHostAddress(QHostAddress::Broadcast),
                                              port);
        break;  // END SignalType::Msg

    case UserJoin:
        udpSocketOnPortChatBox->writeDatagram(resByteArray,
                                              QHostAddress(QHostAddress::Broadcast),
                                              port);
        break;

    case UserLeft:
//        udpSocketOnPortChatBox->writeDatagram(resByteArray,
//                                              QHostAddress(QHostAddress::Broadcast),
//                                              port);  还要添加 closeEvent！！！！ TODO
        break;

    default:
        break;
    }

    qDebug() << "ChatBoxWidget send signal on port:" << udpSocketOnPortChatList
             << "SignalType::" << type
             << chatName_2 << chatPort_3
             << localUserName_4
             << localUserGroupNumber_5
             << localIpAddress_6;
}


QString ChatBoxWidget::getAndCleanMsg()
{
    QString str = ui->msgTextEdit->toHtml();

    // 顺便清空对话框
    ui->msgTextEdit->clear();
    ui->msgTextEdit->setFocus();

    return str;
}
