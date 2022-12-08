#ifndef SIGNALTYPE_H
#define SIGNALTYPE_H


enum SignalType
{
    ChatExist,      // 存在本聊天窗口 (用于广播)
    ChatDestory,    // 当聊天里最后一位用户退出时销毁
    Msg,            // 普通消息
    UserJoin,       // 用户进入聊天
    UserLeft,       // 用户离开

};

static const int    BORDCAST_TIME_STEP      = 3000;             // 广播信息的时间间隔

#endif // SIGNALTYPE_H
