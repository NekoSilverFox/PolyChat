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

    // 优先寻找以192开头的IPv4地址
    for (const QHostAddress &address : ipAddressesList)
    {
        if (address != QHostAddress::LocalHost && address.toIPv4Address())
        {
            QString ipString = address.toString();
            if (ipString.startsWith("192"))
            {
                ipAddress = address;
                break;
            }
        }
    }

    // 如果没有找到以192开头的地址，再考虑其他IPv4地址
    if (ipAddress.isNull())
    {
        for (const QHostAddress &address : ipAddressesList) {
            if (address != QHostAddress::LocalHost && address.toIPv4Address())
            {
                ipAddress = address;
                break;
            }
        }
    }

    // 如果仍然没有找到，返回 localhost IPv4 地址
    if (ipAddress.isNull()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost);
    }

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
    ::localUserName           = name;
    ::localUserGroupNumber    = groupNumber;
    ::localIpAddress          = getIPAddress();

    return true;
}

static QString getLocalUserName()
{
    return ::localUserName;
}
static void setLocalUserName(QString localUserName)
{
    ::localUserName = localUserName;
}

static QString getLocalUserGroupNumber()
{
    return ::localUserGroupNumber;
}
static void setLocalUserGroupNumber(QString localUserGroupNumber)
{
    ::localUserGroupNumber = localUserGroupNumber;
}


static QHostAddress getLocalIpAddress()
{
    return ::localIpAddress;
}
static void setLocalIpAddress(QHostAddress localIpAddress)
{
    ::localIpAddress = localIpAddress;
}

static qint16 getPortChatList()
{
    return ::PORT_CHAT_LIST;
}

/** Show ChatList Widget
 * @brief initAndShowChatList
 */
static void initAndShowChatList(QWidget* parent)
{
    ::chatList = new ChatList(parent, ::localUserName, ::localUserGroupNumber, ::localIpAddress);
    ::chatList->show();
}

static void loadTranslator()
{
    ::translator_EN = new QTranslator;
    ::translator_EN->load(":/language/language_EN.qm");

    ::translator_zn_CN = new QTranslator;
    ::translator_zn_CN->load(":/language/language_zh_CN.qm");

    ::translator_RU = new QTranslator;
    ::translator_RU->load(":/language/language_RU.qm");
}

}

#endif // BLL_POLYCHAT_H
