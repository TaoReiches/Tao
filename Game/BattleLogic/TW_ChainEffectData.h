#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum class BeChainEffectType
{
    BCET_LIGHTNING = 0,
    BCET_LIGHTNING_RED,
    BCET_LIGHTNING_FORKED,
    BCET_DRAIN_LIFE,
    BCET_DRAIN_MANA,
    BCET_BURN_MANA,
    BCET_LINK_SPIRIT,
    BCET_LARIAT_CAUGHT,
    BCET_HEAL_BEAM,
    BCET_LASER_LIGHT,
    BCET_FLY_HOOK,
    BCET_FLY_HOOK2,
    BCET_FLY_HOOK3,
    BCET_FLY_HOOK4,
};

enum class BeChainTargetType
{
    BCTF_UNIT,
    BCTF_EFFECT,
    BCTF_DOODAD,
};

class BeChainEffectData
{
public:
    BeChainEffectData()
    {
        eChainType = BeChainEffectType::BCET_LIGHTNING;
        eTargetType = BeChainTargetType::BCTF_UNIT;
        iSourceID = 0;
        iTargetID = 0;
        iAllLiveTime = 1000;
        kPicture = 0;
        dwColor = 0xFFFFFFFF;
        iWidth = -1;
        iConnectBufferID = 0;
        iUnitID = 0;
        iShowPlayer = -1;
        iData = 0;
        kSourceAttach = 0;
        uiCreateTime = 0;
    }
    BeChainEffectType eChainType;
    BeChainTargetType eTargetType;
    int iSourceID;
    int iTargetID;
    int iAllLiveTime;
    int kPicture;
    unsigned int dwColor;
    int iWidth;
    int iConnectBufferID;
    int iUnitID;
    int iShowPlayer;
    int iData;
    int kSourceAttach;
    unsigned int uiCreateTime;
};
