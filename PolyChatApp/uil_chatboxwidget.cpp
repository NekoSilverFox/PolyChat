#include "uil_chatboxwidget.h"
#include "ui_chatboxwidget.h"
#include "dal_polychat.h"
#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QDesktopServices>

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
    connect(udpSocketOnPortChatBox, &QUdpSocket::readyRead,
            this, &ChatBoxWidget::receiveUDPMessage);

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


    /* 点击退出按钮，关闭窗口 */
    connect(ui->btnExit, &QPushButton::clicked,
            this, [=](){this->close();});

    //////////////////////////////////////////////// 辅助功能 ////////////////////////////////////////////////
    /* 字体 */
    connect(ui->cbxFontType, &QFontComboBox::currentFontChanged,
            [=](const QFont& font){
        ui->msgTextEdit->setCurrentFont(font);
        ui->msgTextEdit->setFocus();
    });

    /* 字号 */
    void(QComboBox::* cbxSingal)(const QString &text) = &QComboBox::currentTextChanged;
    connect(ui->cbxFontSize, cbxSingal,
            [=](const QString &text){
        ui->msgTextEdit->setFontPointSize(text.toDouble());
        ui->msgTextEdit->setFocus();
    });

    /* 加粗 */
    connect(ui->btnBold, &QToolButton::clicked,
            [=](bool isCheck){
        if (isCheck) ui->msgTextEdit->setFontWeight(QFont::Bold);
        else ui->msgTextEdit->setFontWeight(QFont::Normal);
    });

    /* 倾斜 */
    connect(ui->btnItalic, &QToolButton::clicked,
            [=](bool isCheck){
        ui->msgTextEdit->setFontItalic(isCheck);
    });


    /* 下划线 */
    connect(ui->btnUnderLine, &QToolButton::clicked,
            [=](bool isCheck){
        ui->msgTextEdit->setFontUnderline(isCheck);
    });

    /* 更改颜色 */
    connect(ui->btnColor, &QToolButton::clicked,
            [=](){
        QColor color = QColorDialog::getColor(Qt::black);
        ui->msgTextEdit->setTextColor(color);
    });

    /* 清空聊天 */
    connect(ui->btnClean, &QToolButton::clicked,
            this, [=](){

        if (QMessageBox::Ok ==
                QMessageBox::question(this,
                                      "Clean all message",
                                      "Are you sure you want to clear all messages?",
                                      QMessageBox::Ok | QMessageBox::Cancel,
                                      QMessageBox::Cancel))
        {
            ui->msgTextBrowser->clear();
        }});

    /* 保存聊天记录 */
    connect(ui->btnSave, &QToolButton::clicked,
            this, [=](){
        if (ui->msgTextBrowser->document()->isEmpty())
        {
            QMessageBox::warning(this, "Warning", "Can not save!\nMessage box is empty");
            return;
        }

        QString path = QFileDialog::getSaveFileName(this, "Save file", "PolyChat-MsgLog", "(*.txt)");
        if (path.isEmpty())
        {
            QMessageBox::warning(this, "Warning", "Save cancel");
            return;
        }

        QFile file(path);
        file.open(QIODevice::WriteOnly | QIODevice::Text);  // 支持换行
        QTextStream textStream(&file);
        textStream << ui->msgTextBrowser->toPlainText();
        file.close();
    });

    /* 发送文件 */
    connect(ui->btnFileSend, &QToolButton::clicked,
            this, [=](){
        QString path = QFileDialog::getOpenFileName(this, "Send file", ".");

        if (path.isEmpty())
        {
             QMessageBox::warning(this, "Warning", "File sending cancellation");
             return;
        }

        QFileInfo info(path);
        if (info.size() > FILE_SEND_MAX_KB)
        {
            QMessageBox::critical(this, "Error", "File size cannot exceed 1Gb");
            return;
        }

        sendUDPSignal(SignalType::FilePath);
    });

    connect(ui->msgTextBrowser, &QTextBrowser::anchorClicked,
            this, &ChatBoxWidget::openURL);
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
        dataStream << QString("SignalType::ChatExist");
        udpSocketOnPortChatList->writeDatagram(resByteArray,
                                               QHostAddress(QHostAddress::Broadcast),
                                               DAL::getPortChatList());
        break;  // END SignalType::ChatExist

    case SignalType::ChatDestory:
        dataStream <<  QString("SignalType::ChatDestory");
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

    case SignalType::FilePath:
        dataStream << this->lastFilePath;
        udpSocketOnPortChatBox->writeDatagram(resByteArray,
                                              QHostAddress(QHostAddress::Broadcast),
                                              port);
        break;  // END SignalType::FilePath

    case SignalType::UserJoin:
        dataStream <<  QString("SignalType::UserJoin");
        udpSocketOnPortChatBox->writeDatagram(resByteArray,
                                              QHostAddress(QHostAddress::Broadcast),
                                              port);
        break;

    case UserLeft:
        dataStream <<  QString("SignalType::UserLeft");
        udpSocketOnPortChatBox->writeDatagram(resByteArray,
                                              QHostAddress(QHostAddress::Broadcast),
                                              port);
        break;

    default:
        break;
    }
}

/** 接收 UDP 消息
 * @brief receiveMessage
 */
void ChatBoxWidget::receiveUDPMessage()
{
    /* 拿到数据报文 */
    qint64 msgSize = udpSocketOnPortChatBox->pendingDatagramSize();
    QByteArray resByteArray = QByteArray(msgSize, 0);
    udpSocketOnPortChatBox->readDatagram(resByteArray.data(), msgSize);

    /** 解析数据 - 消息分为 5 类 (SignalType)，所以要对数据做分段处理
    *   第1段：数据类型
    *   第2段：发送本信号的群组名称
    *   第3段：发送本信号的群组端口号
    *   第4段：当前（本地）用户名 localUserName
    *   第5段：当前（本地）用户组号 localUserGroupNumber
    *   第6段：当前（本地）用户ip localIpAddress
    *   第7段：具体内容 QString
    */
    SignalType      signalType_1            ;
    QString         chatName_2              ;
    qint16          chatPort_3              ;
    QString         localUserName_4         ;
    QString         localUserGroupNumber_5  ;
    QHostAddress    localIpAddress_6        ;
    QString         msg_7;

    QDataStream dataStream(&resByteArray, QIODevice::ReadOnly);
    QString time = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");

    dataStream >> signalType_1;             // 第1段：数据类型
    dataStream >> chatName_2;               // 第2段：发送信号（本）信号的群组名称
    dataStream >> chatPort_3;               // 第3段：发送信号（本）群组端口号
    dataStream >> localUserName_4;          // 第4段：发送信号用户名
    dataStream >> localUserGroupNumber_5;   // 第5段：发送信号用户组号
    dataStream >> localIpAddress_6;         // 第6段：发送信号用户ip
    dataStream >> msg_7;                    // 第7段：具体内容 QString

    qDebug() <<  "收到消息：" << SignalType::Msg
             <<  signalType_1
             <<  chatName_2
             <<  chatPort_3
             <<  localUserName_4
             <<  localUserGroupNumber_5
             <<  localIpAddress_6
             <<  msg_7;

    switch (signalType_1) {
    case SignalType::Msg:
        // 追加聊天记录
        ui->msgTextBrowser->setTextColor(Qt::blue);
        ui->msgTextBrowser->append("[" + localUserName_4 + "] " + time);
        ui->msgTextBrowser->append(msg_7);
        break;

    case SignalType::FilePath:
        // 追加聊天记录
//        ui->msgTextBrowser->setTextColor(Qt::red);
//        ui->msgTextBrowser->append(">>> [FILE] " + time);
//        ui->msgTextBrowser->append(QString("<div><p><a href=\"%1\" target=\"_blank\">[Name]: %2    [Size]: %3Kb</a></p></div>").arg(msg_7).arg(QFileInfo(msg_7).fileName()).arg(QFileInfo(msg_7).size()));
        break;

    case SignalType::UserJoin:
        userJoin(localUserName_4, localUserGroupNumber_5, localIpAddress_6);
        break;

    case SignalType::UserLeft:
        userLeft(localUserName_4, time);
        break;

    default:
        break;
    }
}

/* 处理用户加入 */
void ChatBoxWidget::userJoin(QString name, QString groupNumber, QHostAddress ip)
{
    if (ui->tbUser->findItems(name, Qt::MatchExactly).isEmpty())
    {
        /* 更新用户列表 */
        ui->tbUser->insertRow(0);
        ui->tbUser->setItem(0, 0, new QTableWidgetItem(name));
        ui->tbUser->setItem(0, 1, new QTableWidgetItem(groupNumber));
        ui->tbUser->setItem(0, 2, new QTableWidgetItem(ip.toString()));

        /* 追加聊天记录 */
        ui->msgTextBrowser->setTextColor(Qt::gray);
        ui->msgTextBrowser->append(QString("%1 online！").arg(name));

        /* 在线用户更新 */
        ui->lbNumberOnlineUse->setText(QString("Number of online user: %1").arg(ui->tbUser->rowCount()));

        /* 把自身信息广播 */
        sendUDPSignal(SignalType::UserJoin);
    }
}


/* 处理用户离开 */
void ChatBoxWidget::userLeft(QString name, QString time)
{
    if (!ui->tbUser->findItems(name, Qt::MatchExactly).isEmpty())
    {
        /* 更新用户列表 */
        ui->tbUser->removeRow(ui->tbUser->findItems(name, Qt::MatchExactly).first()->row());

        /* 追加聊天记录 */
        ui->msgTextBrowser->setTextColor(Qt::gray);
        ui->msgTextBrowser->append(QString("%1 left %2 on ").arg(name).arg(time));

        /* 在线用户更新 */
        ui->lbNumberOnlineUse->setText(QString("Number of online user：%1").arg(ui->tbUser->rowCount()));
    }
}

QString ChatBoxWidget::getAndCleanMsg()
{
    QString str = ui->msgTextEdit->toHtml();

    // 顺便清空对话框
    ui->msgTextEdit->clear();
    ui->msgTextEdit->setFocus();

    return str;
}


/* 触发关闭事件 */
void ChatBoxWidget::closeEvent(QCloseEvent* event)
{
    emit this->signalClose();

    if (1 == ui->tbUser->rowCount())
    {
        sendUDPSignal(SignalType::ChatDestory);
    }
    else
    {
        sendUDPSignal(SignalType::UserLeft);
    }

    udpSocketOnPortChatList->close();  // 关闭套接字
    udpSocketOnPortChatList->destroyed();

    udpSocketOnPortChatBox->close();
    udpSocketOnPortChatBox->destroyed();

    QWidget::closeEvent(event);
}

void ChatBoxWidget::openURL(const QUrl &url)
{
    QString strUrl = url.toString();
    qDebug() << strUrl;
    QDesktopServices::openUrl(url);
}
