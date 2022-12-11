#ifndef DB_LOCALDATA_H
#define DB_LOCALDATA_H
#include <QString>
#include <QHostAddress>
#include <QVector>

#include "uil_chatlist.h"
#include "chat.h"

static const qint16 PORT_TCP_FILE           = 8848;
static const qint16 PORT_CHAT_LIST          = 8888;
static const qint16 PORT_MIN                = 1;
static const qint16 PORT_MAX                = 32767;

static const int    FILE_SEND_MAX_KB        = 1 * 1024 * 1024;  // Kb


extern QString      localUserName;
extern QString      localUserGroupNumber;
extern QHostAddress localIpAddress      ;
extern ChatList*    chatList            ;


#endif // DB_LOCALDATA_H
