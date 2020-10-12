#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "TW_ChangeFlagObject.h"
#include "TW_MemoryPool.h"
#include "TW_Pos2.h"
#include "TW_ShareUnitData.h"
#include "TW_Functions.h"
#include "TW_EffectDefine.h"
#include "TW_FlagObject.h"
#include "TW_UserDataDefine.h"
#include "TW_Entity.h"
#include <TW_PointerType.h>

enum BeNewFontEffectType
{
    BNFET_FABAO = 0,
    BNFET_WULIBAOJI,
    BNFET_XIXUE,
    BNFET_SHANBIGEDANG,
    BNFET_NORMALDAMAGE,
    BNFET_SKILLDAMAGE,
    BNFET_GOLD,
    BNFET_SLOWDOWN,
    BNFET_DIZZY,
    BNFET_NARMAL,
};

enum BeFontEffectType
{
    BFET_PICFONT = 0,
    BFET_PROGRAMFONT_DOWNTOUP,
    BFET_PROGRAMFONT_UPTODOWN,
    BFET_PROGRAMFONT_MIDDLE,
    BFET_PROGRAMFONT_TIMERECIPROCAL,
};

enum BeProgramFontFlag
{
    BPFF_SONGTI = 0,
    BPFF_YAHEI = 1,
    BPFF_HUAKANG = 2,

    BPFF_FONT_MASK = 0x07,

    BPFF_SIZE_MASK = 0xF8,

    BPFF_BOLD = (1 << 8),
    BPFF_SHADOW = (1 << 9),
    BPFF_OUTLINE = (1 << 10),
    BPFF_OUTLINE_EX = (1 << 11),
    BPFF_UNDERLINE = (1 << 12),
    BPFF_ITALIC = (1 << 13),
    BPFF_VERTICAL = (1 << 14),
    BPFF_ROTATE_L90 = (1 << 15),
    BPFF_ROTATE_R90 = (1 << 16),
    BPFF_SMOOTH = (1 << 17),

    BPFF_EFFECT_MASK = 0xFF00,

    BPFF_SEFFECT_MASK = 0xF0000,

    BPFF_SPECIAL = (1 << 31),

    BPFF_SYSTEM_FLAG = (BPFF_FONT_MASK | BPFF_SIZE_MASK | BPFF_EFFECT_MASK | BPFF_SPECIAL),
    BPFF_CACHE_FLAG = (BPFF_SYSTEM_FLAG | BPFF_SEFFECT_MASK),

};
#define MAKE_FONTB(font,size,flag)   ( (font&BPFF_FONT_MASK)|((((size /2-1)<<3)&BPFF_SIZE_MASK ))|(flag&~(BPFF_FONT_MASK|BPFF_SIZE_MASK)) )

#define BPFF_DEFAULT	 MAKE_FONTB(BPFF_YAHEI,14,BPFF_BOLD)
#define BPFF_UNIT_WND	 MAKE_FONTB(BPFF_YAHEI,12,BPFF_BOLD)
#define BPFF_CHAT_WORD	  MAKE_FONTB(BPFF_YAHEI,14,BPFF_BOLD)
#define BPFF_TITLE_WORD	 MAKE_FONTB(BPFF_YAHEI,16,BPFF_BOLD)

enum BeFontEffectColor
{
    BFEC_GREEN = 0,
    BFEC_RED,
    BFEC_SILVER,
    BFEC_PURPLE,
    BFEC_BLUE,
    BFEC_GOLD,
    BFEC_BLOOD_GREEN,
    BFEC_BLOOD_BLUE,
};

enum BeFontEffectSize
{
    BFES_SMALL = 0,
    BFES_BIG,
    BFES_CHARACTER,
    BFES_BIG_NEW,
};

enum BeEffectCurveStyle
{
    BECS_LEFTBIG = 0,
    BECS_RIGHTBIG,
    BECS_MIDBIG,
    BECS_WAVE,
};

enum BeEffectCurveType
{
    BECT_CURVELEFT = 0,
    BECT_CURVERIGHT,
    BECT_CURVERMID,
    BECT_CURVESLOWLEFT,
    BECT_CURVESLOWRIGHT,
};

enum BeChainTargetType
{
    BCTF_UNIT,
    BCTF_EFFECT,
    BCTF_DOODAD,
};

struct BeStickEffectData
{
    std::string kTextureName;
    float fPosX;
    float fPosY;
    float fScale;
    unsigned int dwColor;
    float fFace;
    int	  iBirthTime;
    int   iStandTime;
    int   iDeathTime;
    unsigned int  uiCreateTime;

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
    }
};

enum BeChainEffectType
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

enum BeEffeAttachType
{
    BATTAT_NORMAL = 0,
    BATTAT_WING,
    BATTAT_L_WEAPON,
    BATTAT_R_WEAPON,
    BATTAT_RIDER,
};

struct BeChainEffectData
{
    BeChainEffectData()
    {
        eChainType = BCET_LIGHTNING;
        eTargetType = BCTF_UNIT;
        iSourceID = 0;
        iTargetID = 0;
        iAllLiveTime = 1000;
        kPicture = "";
        dwColor = 0xFFFFFFFF;
        iWidth = -1;
        iConnectBufferID = 0;
        iUnitID = 0;
        iShowPlayer = -1;
        iData = 0;
        kSourceAttach = "";
        uiCreateTime = 0;
    }
    BeChainEffectType eChainType;
    BeChainTargetType eTargetType;
    int iSourceID;
    int iTargetID;
    int iAllLiveTime;
    std::string kPicture;
    unsigned int dwColor;
    int iWidth;
    int iConnectBufferID;
    int iUnitID;
    int iShowPlayer;
    int iData;
    std::string kSourceAttach;
    unsigned int uiCreateTime;
};

struct  BumpCount
{
    BumpCount()
    {
        iUnitID = 0;
        iBumpCount = 0;
    }
    int iUnitID;
    int iBumpCount;
};

typedef std::vector<int> UnitGroupID;
class BeMain;
class BeUnit;
class BeAttackingAttr;
class BeEntityMgr;
typedef BeUnit* (*SearchFunc)(UnitGroupID& kGroup, BeMain* pkAttachMain, int iMaxNum, std::string strFlag);

class BeEffect : public BeChangeFlagObj, public BeEntity
{
    DECLARE_POOL1(BeEffect);

protected:
    BeEffect(int iID);

public:
    ~BeEffect(void);

    int GetRace(void)
    {
        return m_iRace;
    }
    void SetRace(int iValue)
    {
        if (m_iRace != iValue)
        {
            SetChangeFlag(BECF_RACE);
        }
        m_iRace = iValue;
    }

    bool IsIgnoreDirection(void)
    {
        return (m_kEffectData.iProperty & BEP_IGNOREDIRECTION) != 0;
    }

    bool OnCycleRound(void)
    {
        return (m_kEffectData.iProperty & BEP_ONCYCLEROUND) != 0;
    }

    void SetProperty(int iValue)
    {
        SetChangeFlag(BECF_PROPERTY);
        m_kEffectData.iProperty |= iValue;
    }

    inline int GetOwnerID(void)
    {
        return m_kEffectData.iOwnerID;
    }

    inline int GetPlayer(void)
    {
        return m_kEffectData.iPlayer;
    }

    inline void SetEffectRemoveTime(unsigned int uiTime, bool bNeedRecordChange = true)
    {
        m_kEffectData.uiRemoveTime = uiTime;
    }

    inline unsigned int GetEffectRemoveTime(void)	const
    {
        return m_kEffectData.uiRemoveTime;
    }

    inline int GetSourceID(void)
    {
        return m_kEffectData.iSourceID;
    }

    inline void SetSourceID(int iSourceID)
    {
        m_kEffectData.iSourceID = iSourceID;
    }

    inline int GetTargetID(void)
    {
        return m_kEffectData.iTargetID;
    }

    inline float GetMoveSpeed(void)
    {
        return m_kEffectData.fMoveSpeed;
    }

    inline void SetMoveSpeed(float fMoveSpeed)
    {
        if (m_kEffectData.fMoveSpeed != fMoveSpeed)
        {
            SetChangeFlag(BECF_MOVESPEED);
        }

        m_kEffectData.fMoveSpeed = fMoveSpeed;
    }

    inline float GetMissileArc(void)
    {
        return m_kEffectData.fMissileArc;
    }

    void SetMissileArc(float fMissileArc)
    {
        if (m_kEffectData.fMissileArc != fMissileArc)
        {
            SetChangeFlag(BECF_MISSILEARC);
        }

        m_kEffectData.fMissileArc = fMissileArc;
    }

    inline float GetPosX(void)
    {
        return m_kEffectData.fPosX;
    }

    inline float GetPosY(void)
    {
        return m_kEffectData.fPosY;
    }

    inline float GetPosZ(void)
    {
        return m_kEffectData.fPosZ;
    }

    inline float GetTarPosX(void)
    {
        return m_kEffectData.fTarPosX;
    }

    inline float GetTarPosY(void)
    {
        return m_kEffectData.fTarPosY;
    }

    float GetEffectFace(void)
    {
        return m_kEffectData.fFace;
    }

    void SetFace(float fFace, bool bNeedChangeFlag = true)
    {
        AdjustRadian(fFace);

        if (bNeedChangeFlag && m_kEffectData.fFace != fFace)
        {
            SetChangeFlag(BECF_FACE);
        }
        m_kEffectData.fFace = fFace;
    }

    inline void SetAttachSkill(int iSkillTypeID, int iSkillLevel) {

        if (m_kEffectData.iSkillTypeID != iSkillTypeID)
        {
            SetChangeFlag(BECF_SKILLTYPEID);
        }

        m_kEffectData.iSkillTypeID = iSkillTypeID;
        m_kEffectData.iSkillLevel = iSkillLevel;
    }

    inline int GetAttachSkill(void)
    {
        return m_kEffectData.iSkillTypeID;
    }

    inline int GetAttachSkillLevel(void)
    {
        return m_kEffectData.iSkillLevel;
    }


    inline float GetDamageScope(void)
    {
        return m_kEffectData.fDamageScope;
    }

    inline void SetDamageScope(float fScope)
    {
        m_kEffectData.fDamageScope = fScope;
    }

    inline void SetTargetFlag(int iFlag)
    {
        m_kEffectData.iTargetFlag = iFlag;
    }

    inline float GetCollisionScope(void)
    {
        return m_kEffectData.fCollisionScope;
    }

    inline void SetCollisionScope(float fScope)
    {
        m_kEffectData.fCollisionScope = fScope;
    }

    inline void SetPlayer(int iPlayer)
    {
        m_kEffectData.iPlayer = iPlayer;
    }

    inline void SetCurveStyle(BeEffectCurveStyle eStyle)
    {
        m_eCurveStyle = eStyle;
    }

    inline void SetData(int iData)
    {
        m_kEffectData.iData = iData;
    }

    inline int GetData(void)
    {
        return m_kEffectData.iData;
    }

    inline void SetFloatData(float fData)
    {
        m_kEffectData.fData = fData;
    }

    inline float GetFloatData(void)
    {
        return m_kEffectData.fData;
    }

    inline bool HasBumped(int iUnitID) const
    {
        for (int j = 0; j < (int)m_akBumpAttackedID.size(); ++j)
        {
            if (iUnitID == m_akBumpAttackedID[j])
            {
                return true;
            }
        }
        return false;
    }

    inline void AddBumped(int iUnitID)
    {
        AddBumCount(iUnitID);
        if (!HasBumped(iUnitID))
        {
            m_akBumpAttackedID.push_back(iUnitID);
        }
    }

    inline	int GetBumpedNum()
    {
        return (int)m_akBumpAttackedID.size();
    }

    inline void SetSkillBumpCount(int iCount)
    {
        m_kEffectData.iBumpCount = iCount;
    }

    inline int GetSkillBumpCount(void) const
    {
        return m_kEffectData.iBumpCount;
    }

    inline void SetTargetType(BeEffectTargetType eType)
    {
        m_kEffectData.eTargetType = eType;
    }

    inline BeEffectTargetType GetTargetType()
    {
        return m_kEffectData.eTargetType;
    }

    inline float GetTarPosZ(void)
    {
        return m_kEffectData.fTarPosZ;
    }

    inline const BeEffectData* GetEffectData(void) const
    {
        return &m_kEffectData;
    }

    inline void SetPureShow(bool bPure)
    {
        m_bPureShow = bPure;
    }

    inline void SetShowPlayer(int iShowPlayer)
    {
        m_kEffectData.iShowPlayer = iShowPlayer;
    }

    inline void SetShowGroup(int iShowGroup)
    {
        m_kEffectData.iShowGroup = iShowGroup;
    }

    inline void SetMustCreate(bool bMustCreate)
    {
        m_kEffectData.bMustCreate = bMustCreate;
    }

    inline int GetShowPlayer(void)
    {
        return m_kEffectData.iShowPlayer;
    }

    inline int GetShowGroup(void)
    {
        return m_kEffectData.iShowGroup;
    }

    inline bool GetMustCreate(void)
    {
        return m_kEffectData.bMustCreate;
    }

    inline float GetOrgPosX(void)
    {
        return m_kEffectData.fOrgPosX;
    }

    inline float GetOrgPosY(void)
    {
        return m_kEffectData.fOrgPosY;
    }

    inline float GetOrgPosZ(void)
    {
        return m_kEffectData.fOrgPosZ;
    }

    inline int GetAttrBumpCount(void)
    {
        if (!m_pkAttr)
        {
            return 0;
        }
        //return m_pkAttr->iBumpCount;
    }

    inline bool GetChangeScale(void)
    {
        return m_bChangeScale;
    }

    inline float GetPreEarthHeight(void)
    {
        return m_fPreEarthHeight;
    }

    inline void SetPreEarthHeight(float fPreEarthHeight)
    {
        m_fPreEarthHeight = fPreEarthHeight;
    }

    inline float GetAllEarthHeight(void)
    {
        return m_fAllEarthHeight;
    }

    inline void SetAllEarthHeight(float fAllEarthHeight)
    {
        m_fAllEarthHeight = fAllEarthHeight;
    }

    inline float GetOrgScale(void)
    {
        return m_fOrgScale;
    }

    inline bool GetRotate(void)
    {
        return m_bRotate;
    }

    inline bool GetDeasil(void)
    {
        return m_bDeasil;
    }

    inline float GetRad(void)
    {
        return m_fRad;
    }

    inline bool GetTurnning(void)
    {
        return m_bTurning;
    }

    inline void SetPreX(float fPrePosX)
    {
        m_fPrePosX = fPrePosX;
    }

    inline float GetPreX(void)
    {
        return m_fPrePosX;
    }

    inline void SetPreY(float fPrePosY)
    {
        m_fPrePosY = fPrePosY;
    }

    inline float GetPreY(void)
    {
        return m_fPrePosY;
    }

    inline void SetPreZ(float fPrePosZ)
    {
        m_fPrePosZ = fPrePosZ;
    }

    inline float GetPreZ(void)
    {
        return m_fPrePosZ;
    }

    inline void SetAllDistance(float fAllDistance)
    {
        m_fAllDistance = fAllDistance;
    }

    inline float GetAllDistance(void)
    {
        return m_fAllDistance;
    }

    inline void SetCurveType(BeEffectCurveType eCurveType)
    {
        m_eCurveType = eCurveType;
    }

    inline BeEffectCurveType GetCurveType(void)
    {
        return m_eCurveType;
    }

    inline void SetHasCulCurve(bool bHasCulCurve)
    {
        m_bHasCulCurve = bHasCulCurve;
    }

    inline bool GetHasCulCurve(void)
    {
        return m_bHasCulCurve;
    }

    inline void SetOwnerFace(float fOwnerFace)
    {
        m_fOwnerFace = fOwnerFace;
    }

    inline float GetOwnerFace(void)
    {
        return m_fOwnerFace;
    }

    inline BeEffectCurveStyle GetCurveStyle(void)
    {
        return m_eCurveStyle;
    }

    inline void SetCurveDividNum(int iCurveDividNum)
    {
        m_iCurveDividNum = iCurveDividNum;
    }

    inline int GetCurveDividNum(void)
    {
        return m_iCurveDividNum;
    }

    inline TePos2 GetPointMul2Pos(void)
    {
        return m_kCurvePointMul2;
    }

    inline TePos2 GetPointMul3Pos(void)
    {
        return m_kCurvePointMul3;
    }

    inline void SetOrgPosX(float fOrgPosX)
    {
        m_kEffectData.fOrgPosX = fOrgPosX;
    }

    inline void SetOrgPosY(float fOrgPosY)
    {
        m_kEffectData.fOrgPosY = fOrgPosY;
    }

    inline void SetOrgPosZ(float fOrgPosZ)
    {
        m_kEffectData.fOrgPosZ = fOrgPosZ;
    }

    inline void SetPosX(float fPosX)
    {
        m_kEffectData.fPosX = fPosX;
    }

    inline void SetPosY(float fPosY)
    {
        m_kEffectData.fPosY = fPosY;
    }

    inline void SetPosZ(float fPosZ)
    {
        m_kEffectData.fPosZ = fPosZ;
    }

    inline void SetTargetPosX(float fPosX)
    {
        m_kEffectData.fTarPosX = fPosX;
    }

    inline void SetTargetPosY(float fPosY)
    {
        m_kEffectData.fTarPosY = fPosY;
    }

    inline void SetTargetPosZ(float fPosZ)
    {
        m_kEffectData.fTarPosZ = fPosZ;
    }

    inline bool GetPureShow()
    {
        return m_bPureShow;
    }

#define ClientEffectUsedFlag	(BEF_ATTRMISSLE|BEF_NODEATHACTION|BEF_REMOVE_WHENUNITDEAD|BEF_REMOVE_WHENUNITDEAD|BEF_IN_MAP_SHOW_CYCLE|BEF_CANT_FORBID|BEF_MUST_CREATE|BEF_NOT_SETPOS|BEF_VISION|BEF_SHOW_IN_WARFOG|BEF_CANT_FORBID|BEF_IN_MAP_SHOW_MOVE|BEF_IN_MAP_SHOW_CYCLE)

    inline void SetFlag(int iFlag, bool bNeedRecordChange = true)
    {
        if (bNeedRecordChange)
        {
            int iTempFlag = iFlag;
            iTempFlag &= ClientEffectUsedFlag;
        }
    }

    inline void ClrFlag(int iFlag)
    {
        int iTempFlag = iFlag;
        iTempFlag &= ClientEffectUsedFlag;
    }

    inline void SetCreateTime(unsigned int dwTime)
    {
        m_kEffectData.dwCreateTime = dwTime;
    }

    inline unsigned int GetCreateTime()
    {
        return m_kEffectData.dwCreateTime;
    }

    inline bool HasUserData(UserDataKey kKey) const
    {
        if (m_kUserData.find(kKey) != m_kUserData.end())
        {
            return true;
        }
        return false;
    }

    inline void ClearUserData(UserDataKey rkKey)
    {
        m_kUserData.erase(rkKey);
    }

    inline void SetUD_Int(UserDataKey rkKey, int i)
    {
        m_kUserData[rkKey] = TePointerType(i);
    }

    inline void SetUD_Float(UserDataKey rkKey, float f)
    {
        m_kUserData[rkKey] = TePointerType(f);
    }

    inline void SetUD_Void(UserDataKey rkKey, void* pkVoid)
    {
        m_kUserData[rkKey] = TePointerType(pkVoid);
    }

    inline int GetUD_Int(UserDataKey rkKey, int iDefault) const
    {
        std::map<UserDataKey, TePointerType>::const_iterator itr = m_kUserData.find(rkKey);
        if (itr != m_kUserData.end())
        {
            return itr->second.v.iValue;
        }
        return iDefault;
    }

    inline float GetUD_Float(UserDataKey rkKey, float fDefault) const
    {
        std::map<UserDataKey, TePointerType>::const_iterator itr = m_kUserData.find(rkKey);
        if (itr != m_kUserData.end())
        {
            return itr->second.v.fValue;
        }
        return fDefault;
    }

    inline void* GetUD_Void(UserDataKey rkKey) const
    {
        std::map<UserDataKey, TePointerType>::const_iterator itr = m_kUserData.find(rkKey);
        if (itr != m_kUserData.end())
        {
            return itr->second.v.pVoid;
        }
        return NULL;
    }

    BeAttachPos	GetAttachNode()
    {
        return m_eAttachNode;
    }

    inline int GetProperty() const
    {
        return m_kEffectData.iProperty;
    }

    virtual bool Initialize(int iTypeID);
    virtual void Update(int iDeltaTime);

    void SetModelFile(int iModelID, BeAttachPos eAttachPos = BAP_ChestRef);
    int GetModelFile();

    virtual void Link(float fX, float fY, BeEntityMgr* pkMgr) {};
    virtual void Unlink() {};

    virtual void OnRemove(void);

    void	SetOwnerID(int iOwnerID);
    void    SetTargetID(int iTargetID);
    void	SetTargetItem(int iItemID);
    void	SetTargetEffect(int iEffectID);

    void	SetOrgScale(float fOrgScale, bool bChangeScale = false);

    // BeAttackingAttr& GetAttackingAttr(void);
    //	int	GetGroup(void);

    void	SetHuanZhuangUnit(int iUnit);
    int		GetHuanZhuangUnit(void);

    void	AddBumCount(int iUnitId);
    bool	bBumCountOk(int iUnitId, int iMaxCount);

    void	SetBirthTime(unsigned int iTime);
    unsigned int	GetBirthTime();

    void	SetSearchTarget(bool bSearch);
    void	SetAttackerPos(TePos2 pos) { m_posAttacker = pos; }
    void	SetMaxTargetNum(int iNum) { m_iMaxTargetNum = iNum; }

    void	SetNoDelayDelete(bool bDelay);
    bool	GetNoDelayDelete() { return m_bNoDelayDelete; }
protected:
    void	UpdateAttach(int iDeltaTime);
    void    UpdateLineFollowEarth(int iDeltaTime);
    void 	UpdateLineTrace(int iDeltaTime);
    void 	UpdateCurveTraceFrank(int iDeltaTime);
    void	UpdateLineTraceCollision(int iDeltaTime);
    void	UpdateCastTrace(int iDeltaTime);
    void	UpdateCastTraceCollision(int iDeltaTime);
    void    UpdateCurveTrace(int iDeltaTime);
    void	UpdateCurveTraceCollision(int iDeltaTime);

    void ParseAttack(void);
    void* UpdateTargetPos(void);

protected:
    //const BeEffectRes*  m_pkRes;
    BeEffectData		m_kEffectData;
    BeAttackingAttr* m_pkAttr;
    std::vector<int>	m_akBumpAttackedID;
    std::vector<BumpCount> m_BumpCount;

    BeEffectCurveStyle	m_eCurveStyle;
    float				m_fNeedMoveDistance;
    float				m_fCanMoveDistance;
    bool				m_bPureShow;

    float				m_fOrgScale;
    bool				m_bChangeScale;

    int					m_iRace;

public:
    void SetEffectPosition(int iTargetID = 0, float fOrgPosX = 0.0f, float fOrgPosY = 0.0f,
        float fTarPosX = 0.0f, float fTarPosY = 0.0f, float fMissileArc = 0.0f,
        float fOrgPosZ = 0.0f, float fTarPosZ = 0.0f, BeEffectCurveType eCurveType = BECT_CURVELEFT);
    void SetCurvePointMul2(float fPointMulX, float fPointMulY);
    void SetCurvePointMul3(float fPointMulX, float fPointMulY);
    void SetCycleAngle(float fAngle);
    float GetCycleAngle(void);
    void SetTurning(bool bTurn);
    void SetRotateOption(float fRad, bool bDeasil = true);
    void CopyBumpAttackeID(const BeEffect* pEffect);

protected:
    void GetCurvePos(float fOrgX, float fOrgY, float fTarX, float fTarY, float fFace, std::vector<TePos2>& akCurvePos, int iPointNum = 100, BeEffectCurveType eCurveType = BECT_CURVELEFT, BeEffectCurveStyle eCurveStyle = BECS_LEFTBIG);
    void ParseCurveTrace(void);
    float GetEffecttrueFace(int iDY, int iDX, int iDeltaTime = 0);

public:
    void SetNewEffect(bool bNew);
    bool IsNewEffect();

    inline			void		SetVisionRadius(float fValue);
    inline			float		GetVisionRadius();

protected:
    int						m_iModelID;
    BeAttachPos				m_eAttachNode;
    BeEffectCurveType   	m_eCurveType;
    float					m_fAllDistance;
    std::vector<TePos2> 	m_akCurvePosW;
    std::vector<TePos2> 	m_akCurvePosH;
    float					m_fOwnerFace;
    float					m_fPrePosX;
    float					m_fPrePosY;
    float					m_fPrePosZ;
    TePos2					m_kCurvePointMul2;
    TePos2					m_kCurvePointMul3;
    float					m_fCycleAngle;
    bool					m_bTurning;
    bool					m_bRotate;
    bool					m_bDeasil;
    float					m_fRad;
    float					m_fPreEarthHeight;
    float					m_fAllEarthHeight;
    bool					m_bHasCulCurve;
    int						m_iCurveDividNum;
    bool					m_bNewEffect;
    unsigned int					m_birthTime;
    bool					m_bSearchTargetSelf;
    int						m_iMaxTargetNum;
    TePos2					m_posAttacker;
    std::map<UserDataKey, TePointerType>	m_kUserData;

    bool					m_bNoDelayDelete;
};

inline			void		BeEffect::SetVisionRadius(float fValue)
{
    SetChangeFlag(BECF_VISION);
    m_kEffectData.fVisionRadius = fValue;
}
inline			float		BeEffect::GetVisionRadius()
{
    return m_kEffectData.fVisionRadius;
}
