#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>

enum class BeAwardNoticeType
{
    BANT_NONE = 0,
    BANT_GOT,
    BANT_QUEST,
};

enum class BeQuestStatus
{
    BQS_NULL = 0,
    BQS_ACCEPT,
    BQS_CANNT_FINISH,
    BQS_CAN_FINISH,
    BQS_PAUSE,
    BQS_FINISH,
    BQS_FAILED,
};

enum class BeQuestType
{
    BQT_NULL = 0,
    BQT_ITEM,
    BQT_MONSTER,
    BQT_ALIVE,
    BQT_TIME_OPTION,
    BQT_TIME_LIMIT,
};

enum class BeQuestAwardType
{
    BQAT_ITEM = 0,
    BQAT_GOLD,
    BQAT_EXP,
};

class BeQuestInfo
{
public:
    BeQuestInfo()
    {
        iMapID = 0;
        iStatus = 0;
        iNPC = 0;
        iType = 0;
        iNum = 0;
        iNeedNum = 0;
        iTime = 0;
        iPauseTime = 0;
        iMissionRate = 0;
        iMissionMaxPoint = 0;
        iMissionPoint = 0;
    }
    int iMapID;
    int iStatus;
    int iNPC;
    int iType;
    int iNum;
    int iNeedNum;
    int iTime;
    int iPauseTime;
    int iMissionRate;
    int iMissionMaxPoint;
    int iMissionPoint;

    std::string acMissionName;
    std::string acName;
    std::string acContent;
    std::string acKeyWord;
};
