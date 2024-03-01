#ifndef DB_LOCALDATA_H
#define DB_LOCALDATA_H
#include <QString>
#include <QHostAddress>
#include <QVector>

#include "uil_chatlist.h"
#include "chat.h"


/**
 * 在此修改配置
 */
static const qint16 PORT_TCP_FILE           = 8848;  // TCP 文件发送-接收端口
static const qint16 PORT_CHAT_LIST          = 2903;
static const unsigned int TCP_DELAY_MS      = 20;    // TCP 文件发送间隔，防止黏包

/**
 * 群组聊天的端口号范围
 */
static const qint16 PORT_MIN                = 1;
static const qint16 PORT_MAX                = 32767;

/**
 * TCP 能够传输的最大文件大小，单位为 Byte
 */
static const int    FILE_SEND_MAX_BYTES     = 1 * 1024 * 1024 * 1024;  // bytes


/**
 * 本机信息
 */
extern QString      localUserName;
extern QString      localUserGroupNumber;
extern QHostAddress localIpAddress      ;
extern ChatList*    chatList            ;


#endif // DB_LOCALDATA_H
