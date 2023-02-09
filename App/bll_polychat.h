#ifndef BLL_POLYCHAT_H
#define BLL_POLYCHAT_H

#include "db_localdata.h"

#include <QNetworkInterface>
#include <QRandomGenerator>

namespace BLL
{

/** Get IP address local PC, fomat like `192.168.XX.XX`
 * @brief getIPAddress
 * @return [QHostAddress] IP Address local PC
 */
static QHostAddress getIPAddress()
{
    QHostAddress ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost
            && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i);
            break;
        }
    }

    if (ipAddress.toString().isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost);

    return ipAddress;
}


/**
 * @brief initLocalUser
 * @param name
 * @param groupNumber
 * @return true
 */
static bool initLocalUser(QString name, QString groupNumber)
{
    localUserName           = name;
    localUserGroupNumber    = groupNumber;
    localIpAddress          = getIPAddress();

    return true;
}

static QString getLocalUserName()
{
    return localUserName;
}
static void setLocalUserName(QString localUserName)
{
    localUserName = localUserName;
}

static QString getLocalUserGroupNumber()
{
    return localUserGroupNumber;
}
static void setLocalUserGroupNumber(QString localUserGroupNumber)
{
    localUserGroupNumber = localUserGroupNumber;
}


static QHostAddress getLocalIpAddress()
{
    return localIpAddress;
}
static void setLocalIpAddress(QHostAddress localIpAddress)
{
    localIpAddress = localIpAddress;
}

static qint16 getPortChatList()
{
    return PORT_CHAT_LIST;
}

/** Show ChatList Widget
 * @brief initAndShowChatList
 */
static void initAndShowChatList(QWidget* parent)
{
    chatList = new ChatList(parent, localUserName, localUserGroupNumber, localIpAddress);
    chatList->show();
}

}

#endif // BLL_POLYCHAT_H
