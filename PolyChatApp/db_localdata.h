#ifndef DB_LOCALDATA_H
#define DB_LOCALDATA_H
#include <QString>
#include <QHostAddress>
#include <QVector>

#include "uil_chatlist.h"
#include "chat.h"

namespace DB
{
static const qint16 PORT_CHAT_LIST          = 8888;
static const qint16 PORT_MIN                = 1;
static const qint16 PORT_MAX                = 32767;

static QString      localUserName           = "";               // User Name (get in user login)
static QString      localUserGroupNumber    = "";               // Group number (get in user login)
static QHostAddress localIpAddress          = QHostAddress();

static ChatList*    chatList                = nullptr;          // Widget ChatList (Only one)
static QVector<Chat*> vChat;
}

#endif // DB_LOCALDATA_H
