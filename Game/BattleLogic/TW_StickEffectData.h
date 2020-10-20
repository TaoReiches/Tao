#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class BeStickEffectData
{
public:
    BeStickEffectData()
    {
        fPosX = 0.0f;
        fPosY = 0.0f;
        fScale = 1.0f;
        dwColor = 0xFFFFFFFF;
        iBirthTime = 0;
        iStandTime = 0;
        iDeathTime = 0;
        fFace = 0;
        uiCreateTime = 0;
        kTextureName = 0;
    }

    int kTextureName;
    float fPosX;
    float fPosY;
    float fScale;
    unsigned int dwColor;
    float fFace;
    int	  iBirthTime;
    int   iStandTime;
    int   iDeathTime;
    unsigned int  uiCreateTime;
};
