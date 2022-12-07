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
static bool initLocalUser(const QString name, const QString groupNumber)
{
    if (name.isEmpty() || groupNumber.isEmpty())
    {
        return false;
    }

    BLL::initLocalUser(name, groupNumber);
    return true;
}

/** Show ChatList Widget
 * @brief initAndShowChatList
 */
static void initAndShowChatList(QWidget* parent)
{
    BLL::initAndShowChatList(parent);
}


/** 查找一个名称的群聊是否已经存在
 * @brief isChatExist
 * @param name
 * @return
 */
static bool isChatExist(const QString name)
{
    return BLL::isChatExist(name);
}

/** 获取一个不重复的随机端口号
 * @brief getRandomPort
 * @return
 */
static qint16 getRandomPort()
{
    return BLL::getRandomPort();
}

/** 创建并插入新的聊天窗口信息
 * @brief getAndInsertNewChat
 * @param name
 * @param port
 * @param isOpen
 * @return
 */
static Chat* getAndInsertNewChat(QString name, qint16 port, bool isOpen = true)
{
    return BLL::getAndInsertNewChat(name, port, isOpen);
}

}

#endif // DAL_POLYCHAT_H
