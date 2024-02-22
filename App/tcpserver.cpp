#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "db_localdata.h"

#include <QMovie>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

TcpServer::TcpServer(QWidget *parent, QString filePath, QHostAddress ip, qint16 port) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    this->setWindowTitle("File sender");
    this->setAttribute(Qt::WA_DeleteOnClose);
    appendTextBrowser(Qt::blue, "[INFO] TCP server start");

    ///////////////////////////////// ui 图片 /////////////////////////////////
    movie = new QMovie(":/gif/icons/loading.gif");
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


    /* 初始化 ui */
    ui->lbServerIP->setText(this->ip.toString());
    ui->lbServerPort->setText(QString::number(this->port));
    ui->lbFilePath->setText(filePath);
    ui->lbFileSize->setText(QString("%1Kb").arg(this->fileSize / 1024));
    appendTextBrowser(Qt::blue, "[INFO] Initializing the TCP server done");

    ///////////////////////////////// 监听套接字: 如果有连接就开始发送文件 /////////////////////////////////
    this->tcpSocket = nullptr;
    this->tcpServer = new QTcpServer(this);
    this->tcpServer->listen(QHostAddress::Any, this->port);
    connect(tcpServer, &QTcpServer::newConnection, this, [=](){

        this->tcpSocket = tcpServer->nextPendingConnection();  // 将下一个挂起的连接作为已连接的QTcpSocket对象返回

        /* 记录 Client 信息 */
        QString clientIp = tcpSocket->peerAddress().toString();
        qint16 clientPort = tcpSocket->peerPort();
        ui->lbClientIP->setText(clientIp);
        ui->lbClientPort->setText(QString::number(clientPort));
        ui->progressBar->setValue(0);

        /* 如果断开连接 */
        connect(tcpSocket, &QTcpSocket::disconnected, [=](){
            ui->lbClientIP->setText("[NONE] NO CONNECT");
            ui->lbClientPort->setText("[NONE] NO CONNECT");
            appendTextBrowser(Qt::darkYellow, QString("[WARRING] Disconnect with client %1:%2 ").arg(clientIp).arg(clientPort));
        });

        appendTextBrowser(Qt::green, QString("[INFO] Success connected to %1:%2 ").arg(clientIp).arg(clientPort));

        /* 开始处理（读取）文件 */
        this->bytesAlreadySend = 0;
        this->file.setFileName(filePath);
        bool isOpenSucc = this->file.open(QIODevice::ReadOnly);
        if (isOpenSucc)
        {
            appendTextBrowser(Qt::blue,
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
            appendTextBrowser(Qt::blue, "[INFO] The file header is sent successfully");
            timer->start(TCP_DELAY_MS);  // TCP 文件发送间隔，防止黏包
            return;
        }
        else
        {
            appendTextBrowser(Qt::red, "[ERROR] File header sending failed");
            file.close();
            return;
        }
    });

    /* 发送完文件头之后，每隔固定时间发送一个数据包 */
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        timer->stop();
        sendData(); });


    /* 点击取消按钮 */
    connect(ui->btnCancel, &QPushButton::clicked, [=](){ this->close(); });
}


void TcpServer::sendData()
{
    appendTextBrowser(Qt::blue, "[INFO] Data is started to be sent......");
    movie->start();

    qint64 lenPackage = 0;  // 记录[当前]数据包大小
    this->bytesAlreadySend = 0;

    // 使用定时器更新进度条
    QTimer* timer_progressBar = new QTimer(this);
    connect(timer_progressBar, &QTimer::timeout, this,
            [=](){
        ui->progressBar->setValue(this->bytesAlreadySend / this->fileSize * 100);
        ui->progressBar->update();

#if !QT_NO_DEBUG
        appendTextBrowser(Qt::darkCyan, QString("[DEBUG] TCP-Server this->bytesAlreadySend:%1").arg(this->bytesAlreadySend));
#endif
    });

    timer_progressBar->start(100);
    do {
        char buf[4 * 1024] = {0};  // 【数据包】每次发送的数据 4Kb
        lenPackage = file.read(buf, sizeof(buf));
        lenPackage = tcpSocket->write(buf, lenPackage);  // 发送数据，读多少，发多少

        this->bytesAlreadySend += lenPackage;  // 发送的数据需要累积


    } while (lenPackage > 0);

    if (this->bytesAlreadySend == this->fileSize)
    {
        timer_progressBar->stop();
        ui->progressBar->setValue(this->bytesAlreadySend / this->fileSize * 100);
        ui->progressBar->update();

        appendTextBrowser(Qt::green, "[INFO] Success to send file\n#############################\n\n");
        this->file.close();
        this->tcpSocket->disconnectFromHost();
        this->tcpSocket->close();
        this->tcpSocket = nullptr;
//        movie->stop();
    }

    return;
}

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


void  TcpServer::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton btnPush = QMessageBox::warning(this, "Cancel send",
                         "The current file transfer will be cancelled if it is not completed and all connections will be disconnected.\n"
                         "Are you sure you want to cancel sending files?",
                         QMessageBox::No | QMessageBox::Yes,
                         QMessageBox::No);
    if (btnPush == QMessageBox::No)
    {
        event->ignore();
    }

    /* 【注意】这里可以防止在未发送文件（tcpSocket 未初始化）的情况下关闭窗口出现的崩溃 */
    if (nullptr != this->tcpSocket)
    {
        tcpSocket->disconnectFromHost(); //断开连接
        tcpSocket->close(); //关闭套接字
    }
    event->accept();
    QWidget::closeEvent(event);
}


TcpServer::~TcpServer()
{
    delete ui;
}
