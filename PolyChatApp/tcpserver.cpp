#include "tcpserver.h"
#include "ui_tcpserver.h"

#include <QMovie>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

TcpServer::TcpServer(QWidget *parent, QString filePath, QHostAddress ip, qint16 port) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    appendTextBrowser(Qt::blue, "[INFO] TCP server start");

    ///////////////////////////////// ui 图片 /////////////////////////////////
    QMovie* movie = new QMovie(":/gif/icons/loading.gif");
    movie->start();
    ui->labelPic->setMovie(movie);

    ///////////////////////////////// 初始化变量 /////////////////////////////////
    appendTextBrowser(Qt::blue, "[INFO] Initializing the TCP server...");
    if (filePath.isEmpty())
    {
        appendTextBrowser(Qt::red, "[ERROR] Send Cancel");
        QMessageBox::critical(this, "ERROR", "File path is empty");
        return;
    }

    this->ip = ip;
    this->port = port;

    QFileInfo info(filePath);
    this->fileName = info.fileName();
    this->fileSize = info.size();  // bytes
    this->bytesAlreadySend = 0;
    this->file.setFileName(filePath);
    bool isOpenSucc = this->file.open(QIODevice::ReadOnly);
    if (isOpenSucc)
    {
        appendTextBrowser(Qt::green,
                          QString("[INFO] File opened successfully\n"
                                  "Name: %1\n"
                                  "Path: %2\n"
                                  "Size: %3Kb").arg(this->fileName).arg(filePath).arg(fileSize / 1024));
    }
    else
    {
        appendTextBrowser(Qt::red, "[ERROR] File opening failed");
        return;
    }

    /* 初始化 ui */
    ui->lbServerIP->setText(this->ip.toString());
    ui->lbServerPort->setText(QString(this->port));
    ui->lbFilePath->setText(filePath);
    ui->lbFileSize->setText(QString("%1Kb").arg(this->fileSize / 1024));
    appendTextBrowser(Qt::blue, "[INFO] Initializing the TCP server done");

    ///////////////////////////////// 监听套接字 /////////////////////////////////
    this->tcpServer = new QTcpServer(this);
    this->tcpServer->listen(QHostAddress::Any, this->port);
    connect(tcpServer, &QTcpServer::newConnection, this, [=](){

        this->tcpSocket = tcpServer->nextPendingConnection();  // 将下一个挂起的连接作为已连接的QTcpSocket对象返回

        /* 记录 Client 信息 */
        QString clientIp = tcpSocket->peerAddress().toString();
        qint16 clientPort = tcpSocket->peerPort();
        ui->lbClientIP->setText(clientIp);
        ui->lbClientPort->setText(QString(clientPort));

        /* 如果断开连接 */
        connect(tcpSocket, &QTcpSocket::disconnected, [=](){
            appendTextBrowser(Qt::darkYellow, QString("[WARRING] Disconnect with client %1:%2 ").arg(clientIp).arg(clientPort));
        });

        appendTextBrowser(Qt::green, QString("[INFO] Success connected to %1:%2 ").arg(clientIp).arg(clientPort));

        /* 建立连接，开始发送数据，先发送 header 再发送数据  */
        appendTextBrowser(Qt::blue, "[INFO] Start sending file header information");
        QByteArray      headerByteArray;   // 先发送 header
        QDataStream     dataStream(&headerByteArray, QIODevice::WriteOnly);

        dataStream << this->fileName;             // header第1段：文件名（QString）
        dataStream << this->fileSize;             // header第2段：文件大小（qint64）

        qint64 len = tcpSocket->write(headerByteArray);

        /* 如果header发送成功了，再发送数据。 并且为了防止TCP黏包，需要通过定时器延时 20 ms*/
        if (len > 0)
        {
            appendTextBrowser(Qt::blue, "[INFO] The file is sent successfully and the data is started to be sent...");
            timer->start(1000);
            return;
        }
        else
        {
            file.close();
            appendTextBrowser(Qt::red, "[ERROR] File header sending failed");
            return;
        }
    });

    /* 发送完文件头之后，每隔固定时间发送一个数据包 */
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            [=](){
        timer->stop();
        sendData();
    });



}


void sendData();

/** 给 ui 的 TextBrowser 追加内容
 * @brief appendTextBrowser
 * @param color
 * @param text
 */
void TcpServer::appendTextBrowser(Qt::GlobalColor color, QString text)
{
    ui->textBrowser->setTextColor(color);
    ui->textBrowser->append(text);
}


TcpServer::~TcpServer()
{
    delete ui;
}
