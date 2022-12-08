#ifndef UIL_ADDCHAT_H
#define UIL_ADDCHAT_H

#include <QWidget>

namespace Ui {
class AddChat;
}

class AddChat : public QWidget
{
    Q_OBJECT

public:
    explicit AddChat(QWidget *parent = nullptr);
    ~AddChat();

signals:
    void addNewChat(QString name);


private:
    Ui::AddChat *ui;
};

#endif // UIL_ADDCHAT_H
