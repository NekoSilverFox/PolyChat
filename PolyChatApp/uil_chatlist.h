#ifndef UIL_CHATLIST_H
#define UIL_CHATLIST_H

#include <QWidget>

namespace Ui {
class ChatList;
}

class ChatList : public QWidget
{
    Q_OBJECT

public:
    explicit ChatList(QWidget *parent = nullptr);
    ~ChatList();

private:
    Ui::ChatList *ui;
};

#endif // UIL_CHATLIST_H
