#ifndef DB_LOCALDATA_H
#define DB_LOCALDATA_H
#include <QString>
#include <QHostAddress>

#include "uil_chatlist.h"

namespace DB
{
static QString      localUserName           = "";               // User Name (get in user login)
static QString      localUserGroupNumber    = "";               // Group number (get in user login)
static QHostAddress localIpAddress          = QHostAddress();

       ChatList*    chatList                = nullptr;          // Widget ChatList (Only one)


}

#endif // DB_LOCALDATA_H
