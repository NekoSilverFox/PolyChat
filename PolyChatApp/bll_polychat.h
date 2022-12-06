#ifndef BLL_POLYCHAT_H
#define BLL_POLYCHAT_H

#include "db_localdata.h"

#include <QNetworkInterface>

namespace BLL
{
/** Get IP address local PC, fomat like `192.168.XX.XX`
 * @brief getIPAddress
 * @return [QHostAddress] IP Address local PC
 */
QHostAddress getIPAddress()
{
    QHostAddress ipAddress;
    QList ipAddressesList = QNetworkInterface::allAddresses();
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
bool initLocalUser(QString name, QString groupNumber)
{
    DB::localUserName = name;
    DB::localUserGroupNumber = groupNumber;
    DB::localIpAddress = getIPAddress();

    return true;
}



/** Show ChatList Widget
 * @brief initAndShowChatList
 */
void initAndShowChatList(QWidget* parent)
{
    DB::chatList = new ChatList(parent, DB::localUserName, DB::localUserGroupNumber, DB::localIpAddress.toString());
    DB::chatList->show();
}

}

#endif // BLL_POLYCHAT_H
