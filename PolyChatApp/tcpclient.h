#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>

namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

private:
    Ui::TcpClient *ui;
};

#endif // TCPCLIENT_H
