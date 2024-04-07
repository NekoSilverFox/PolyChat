#ifndef DAL_POLYCHAT_H
#define DAL_POLYCHAT_H

#include "bll_polychat.h"


namespace DAL
{


/** Check info and init
 * @brief initLocalUser
 * @param name
 * @param groupNumber
 * @return false in name or group have error,
 *          if not init local user and return true
 */
static bool initLocalUser(const QString &name, const QString &groupNumber)
{
    if (name.isEmpty() || groupNumber.isEmpty())
    {
        return false;
    }

    BLL::initLocalUser(name, groupNumber);
    return true;
}


static QString getLocalUserName()
{
    return BLL::getLocalUserName();
}
static void setLocalUserName(const QString &localUserName)
{
    BLL::setLocalUserName(localUserName);
}


static QString getLocalUserGroupNumber()
{
    return BLL::getLocalUserGroupNumber();
}
static void setLocalUserGroupNumber(const QString &localUserGroupNumber)
{
    BLL::setLocalUserGroupNumber(localUserGroupNumber);
}


static QHostAddress getLocalIpAddress()
{
    return BLL::getLocalIpAddress();
}
static void setLocalIpAddress(const QHostAddress &localIpAddress)
{
    BLL::setLocalIpAddress(localIpAddress);
}


static qint16 getPortChatList()
{
    return BLL::getPortChatList();
}


/** Show ChatList Widget
 * @brief initAndShowChatList
 */
static void initAndShowChatList(QWidget* parent)
{
    BLL::initAndShowChatList(parent);
}

static void loadTranslator()
{
    BLL::loadTranslator();
}

}

#endif // DAL_POLYCHAT_H
