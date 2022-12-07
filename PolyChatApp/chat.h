#ifndef CHAT_H
#define CHAT_H

#include <QString>

class Chat
{
public:
    Chat(QString name, qint16 port, bool isOpen = false);

public:
    QString name;
    qint16 port;
    bool isOpen;
};

#endif // CHAT_H
