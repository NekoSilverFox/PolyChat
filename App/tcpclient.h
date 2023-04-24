#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QTime>

namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget* parent, QString fileName, qint64 fileSizeBytes, QHostAddress serverIp, qint16 serverPort);
    ~TcpClient();

    void connectTcpServerAndOpenFile();
    void receiveTcpDataAndSave();

    virtual void closeEvent(QCloseEvent* event);

private:
    void appendTextBrowser(Qt::GlobalColor color, QString text);

private:
    Ui::TcpClient *ui;
    QMovie* movie;  // 动图
    QTimer* timer_progressBar; // 更新进度条用的定时器

    /* 参数传入 */
    QString fileName;
    qint64 fileSizeBytes;
    QHostAddress serverIp;
    qint16 serverPort;

    QTcpSocket* tcpSocket;  // 监听套接字

    QFile file;  //文件对象
    bool isHeaderReceived;  // 判断是否是接收到文件头
    qint64 bytesReceived;   // 已经接收了多少数据
};

#endif // TCPCLIENT_H
