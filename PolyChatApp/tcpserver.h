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

private:
    Ui::TcpServer *ui;
    QHostAddress ip;
    qint16 port;

    QTcpSocket* tcpSocket;  // 监听套接字
    QTcpServer* tcpServer;  // 通信套接字

    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 bytesAlreadySend;  // 已经发了多少数据

    QTimer* timer;

    void sendData();
};

#endif // TCPSERVER_H
