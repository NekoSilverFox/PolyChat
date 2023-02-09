#include "tcpclient.h"
#include "ui_tcpclient.h"
#include "dal_polychat.h"

#include <QMovie>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

TcpClient::TcpClient(QWidget *parent, QString fileName, qint64 fileSizeBytes, QHostAddress serverIp, qint16 serverPort) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    this->setWindowTitle("File receipter");
    this->setAttribute(Qt::WA_DeleteOnClose);
    appendTextBrowser(Qt::blue, "[INFO] TCP client start");


    ///////////////////////////////// ui 图片 /////////////////////////////////
    movie = new QMovie(":/gif/icons/eating.gif");
    movie->start();
    ui->labelPic->setMovie(movie);


    ///////////////////////////////// 初始化变量 /////////////////////////////////
    appendTextBrowser(Qt::blue, "[INFO] Initializing the TCP client...");

    ui->lbClientIP->setText(DAL::getLocalIpAddress().toString());
    ui->lbClientPort->setText(QString::number(PORT_TCP_FILE));


    this->tcpSocket = new QTcpSocket(this);
    /* 如果与主机建立连接 */
    connect(this->tcpSocket, &QTcpSocket::connected, [=](){
        ui->btnSave->setEnabled(false);
        appendTextBrowser(Qt::green, "[INFO] Successfully establish a link with the host");
    });

    /* 如果与主机断开连接 */
    connect(this->tcpSocket, &QTcpSocket::disconnected, [=](){
        ui->btnSave->setEnabled(true);
        appendTextBrowser(Qt::darkYellow, "[WARNING] Disconnected with the host");
    });

    /* 从通信套接字里面取内容 */
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::receiveTcpDataAndSave);

    this->fileName = fileName;
    ui->lbFileName->setText(this->fileName);

    this->fileSizeBytes = fileSizeBytes;
    ui->lbFileSize->setText(QString("%1Kb").arg(QString::number(this->fileSizeBytes / 1024)));

    this->serverIp = serverIp;
    ui->lbServerIP->setText(this->serverIp.toString());

    this->serverPort = serverPort;
    ui->lbServerPort->setText(QString::number(this->serverPort));

    /* 点击 Save 按钮 */
    connect(ui->btnSave, &QPushButton::clicked, this, &TcpClient::connectTcpServerAndOpenFile);

    /* 点击 Cancel 按钮 */
    connect(ui->btnCancel, &QPushButton::clicked, [=](){ this->close(); });

    appendTextBrowser(Qt::blue, "[INFO] Initializing the TCP client done");
    appendTextBrowser(Qt::red, "[INFO] Click `Save` to receive the file");
}

/** 点击 Save 文件按钮，初始化数据，并且保存文件
 * @brief TcpClient::saveFile
 */
void TcpClient::connectTcpServerAndOpenFile()
{
    /* 获取保存路径 */
    ui->lbFileSavePath->setText("[EMPTY] Click `Save` to receive the file");
    QString savePath = QFileDialog::getSaveFileName(this, "Save file to", fileName);
    if (savePath.isEmpty())
    {
        appendTextBrowser(Qt::red, "[ERROR] Save cancel, because the file save path is empty. Click on the `Save` button to receive the file again");
        QMessageBox::critical(this, "ERROR", "File path is empty, because the file save path is empty (You did not specify a save directory).\n"
                                             "You can click on the `Save` button to receive the file again");
        return;
    }

    this->file.setFileName(savePath);
    bool isFileOpen = file.open(QIODevice::WriteOnly);
    if (!isFileOpen)
    {
        appendTextBrowser(Qt::red, QString("[ERROR] Can not write file %1 to file: %2").arg(fileName).arg(savePath));
        return;
    }

    ui->lbFileSavePath->setText(savePath);
    appendTextBrowser(Qt::blue, QString("[INFO] Open file successfully, the file will save to path: %1").arg(file.fileName()));

    /* 初始化/重置变量 */
    this->isHeaderReceived = false;
    this->bytesReceived = 0;
    this->tcpSocket->connectToHost(QHostAddress(serverIp), serverPort);  // 服务器建立连接
    appendTextBrowser(Qt::blue, "[INFO] Try to connect to server...");
}


/** 文件对象打开并且连接到 TCP Server 后，开始读取数据到文件
 * @brief TcpClient::receiveTcpDataAndSave
 */
void TcpClient::receiveTcpDataAndSave()
{
    if (!this->file.isOpen())
    {
        appendTextBrowser(Qt::red, "[ERROR] The file is not open and receiving data is interrupted. Please click the `Save` button to retry");
        tcpSocket->disconnectFromHost(); //断开连接
        tcpSocket->close(); //关闭套接字
        return;
    }

    QByteArray byteArray = tcpSocket->readAll();
    /* 如果是文件头 */
    if (!isHeaderReceived)
    {
        appendTextBrowser(Qt::blue, "[INFO] Start parsing the data header");
        this->isHeaderReceived = true;
        this->bytesReceived = 0;
        ui->progressBar->setValue(0);

        /** 解析 header
        *   header第1段：文件名（QString）
        *   header第2段：文件大小（qint64）
        */
        QDataStream dataStream(&byteArray, QIODevice::ReadOnly);
        dataStream >> this->fileName;             // header第1段：文件名（QString）
        dataStream >> this->fileSizeBytes;        // header第2段：文件大小（qint64）
        appendTextBrowser(Qt::blue, QString("[INFO] The result of the file header parsing is:\n"
                                            "Name: %1\n"
                                            "Size: %2Kb").arg(fileName).arg(fileSizeBytes / 1024));
        appendTextBrowser(Qt::blue, "[INFO] Start to save file, receive data from TCP Server.\nPLEASE WAIT, DO NOT CLOSE THIS WINDOW...");
    }
    /* 文件头接收，接下来是文件的数据内容 */
    else
    {
        qint64 lenWrite = file.write(byteArray);
        if (lenWrite > 0)
        {
            this->bytesReceived += lenWrite;
        }
        ui->progressBar->setValue(bytesReceived / fileSizeBytes * 100);  // 更新进度条

#if !QT_NO_DEBUG
        appendTextBrowser(Qt::darkCyan, QString("[DEBUG] TCP-Client bytesReceived:%1").arg(bytesReceived));
#endif
        if (fileSizeBytes == bytesReceived)
        {
            ui->textBrowser->setTextColor(Qt::green);
            ui->textBrowser->append(QString("[INFO] File %1 received successfully, already saved to the path: %2").arg(fileName).arg(file.fileName()));

            this->isHeaderReceived = false;
            this->bytesReceived = 0;

            tcpSocket->disconnectFromHost(); //断开连接
            tcpSocket->close(); //关闭套接字
            file.close(); //关闭文件
        }
    }
}


void TcpClient::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton btnPush = QMessageBox::warning(this, "Cancel receive",
                         "If the file is not received, it will be stopped and disconnected.\n"
                         "Are you sure you want to cancel receiving files?",
                         QMessageBox::No | QMessageBox::Yes,
                         QMessageBox::No);
    if (btnPush == QMessageBox::No)
    {
        event->ignore();
    }
    else
    {
        tcpSocket->disconnectFromHost(); //断开连接
        tcpSocket->close(); //关闭套接字
        event->accept();
        QWidget::closeEvent(event);
    }
}


/** 给 ui 的 TextBrowser 追加内容
 * @brief appendTextBrowser
 * @param color
 * @param text
 */
void TcpClient::appendTextBrowser(Qt::GlobalColor color, QString text)
{
    ui->textBrowser->setTextColor(color);
    ui->textBrowser->append(text);
}


TcpClient::~TcpClient()
{
    delete ui;
}
