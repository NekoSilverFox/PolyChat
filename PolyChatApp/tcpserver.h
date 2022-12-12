#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>

#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>
#include <QTime>

namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent, QString filePath, QHostAddress ip, qint16 port);
    ~TcpServer();

    void appendTextBrowser(Qt::GlobalColor color, QString text);
    virtual void closeEvent(QCloseEvent* event);

private:
    Ui::TcpServer *ui;
    QHostAddress ip;  // 本机 ip（Server）
    qint16 port;  // 本机 port（Server）

    QTcpSocket* tcpSocket;  // 监听套接字
    QTcpServer* tcpServer;  // 通信套接字

    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 bytesAlreadySend;  // 已经发了多少数据

    QTimer* timer;

    QMovie* movie;  // 动图

    void sendData();
};

#endif // TCPSERVER_H
