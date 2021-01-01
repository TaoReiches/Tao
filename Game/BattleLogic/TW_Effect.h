#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
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
#include "TW_EffectData.h"
#include "TW_EffectBumpCount.h"

class TwMain;
class TwUnit;
class BeAttackingAttr;
class TwEntityMgr;

class BeEffect : public BeChangeFlagObj, public BeEntity
{
public:
    BeEffect(int iID);
    ~BeEffect(void);

    virtual bool Initialize(int iTypeID) override;
    virtual void Update(int iDeltaTime) override;

    void SetModelFile(int iModelID, BeAttachPos eAttachPos = BAP_ChestRef);
    int  GetModelFile();

    virtual void Link(float fX, float fY, TwEntityMgr* pkMgr) override {};
    virtual void Unlink() override {};

    virtual void OnRemove(void);

    void	SetOwnerID(int iOwnerID);
    void    SetTargetID(int iTargetID);
    void	SetTargetItem(int iItemID);
    void	SetTargetEffect(int iEffectID);

    void	SetOrgScale(float fOrgScale, bool bChangeScale = false);
    void	AddBumCount(int iUnitId);
    bool	bBumCountOk(int iUnitId, int iMaxCount);
    void	SetBirthTime(unsigned int iTime);
    unsigned int	GetBirthTime();
    void	SetSearchTarget(bool bSearch);
    void	SetAttackerPos(TwPos2 pos) { m_posAttacker = pos; }
    void	SetMaxTargetNum(int iNum) { m_iMaxTargetNum = iNum; }

    void	SetNoDelayDelete(bool bDelay);
    bool	GetNoDelayDelete() { return m_bNoDelayDelete; }
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
    int GetRace(void);
    void SetRace(int iValue);
    bool IsIgnoreDirection(void);
    bool OnCycleRound(void);
    void SetProperty(int iValue);
    int GetOwnerID(void);
    int GetPlayer(void);
    void SetEffectRemoveTime(unsigned int uiTime, bool bNeedRecordChange = true);
    unsigned int GetEffectRemoveTime(void)	const;
    int GetSourceID(void);
    void SetSourceID(int iSourceID);
    int GetTargetID(void);
    float GetMoveSpeed(void);
    void SetMoveSpeed(float fMoveSpeed);
    float GetMissileArc(void);
    void SetMissileArc(float fMissileArc);
    float GetPosX(void);
    float GetPosY(void);
    float GetPosZ(void);
    float GetTarPosX(void);
    float GetTarPosY(void);
    float GetEffectFace(void);
    void SetFace(float fFace, bool bNeedChangeFlag = true);
    void SetAttachSkill(int iSkillTypeID, int iSkillLevel);
    int GetAttachSkill(void);
    int GetAttachSkillLevel(void);
    float GetDamageScope(void);
    void SetDamageScope(float fScope);
    void SetTargetFlag(int iFlag);
    float GetCollisionScope(void);
    void SetCollisionScope(float fScope);
    void SetPlayer(int iPlayer);
    void SetCurveStyle(BeEffectCurveStyle eStyle);
    void SetData(int iData);
    int GetData(void);
    void SetFloatData(float fData);
    float GetFloatData(void);
    bool HasBumped(int iUnitID) const;
    void AddBumped(int iUnitID);
    int GetBumpedNum();
    void SetSkillBumpCount(int iCount);
    int GetSkillBumpCount(void) const;
    void SetTargetType(BeEffectTargetType eType);
    BeEffectTargetType GetTargetType();
    float GetTarPosZ(void);
    const BeEffectData* GetEffectData(void) const;
    void SetPureShow(bool bPure);
    void SetMustCreate(bool bMustCreate);
    bool GetMustCreate(void);
    float GetOrgPosX(void);
    float GetOrgPosY(void);
    float GetOrgPosZ(void);
    int GetAttrBumpCount(void);
    bool GetChangeScale(void);
    float GetOrgScale(void);
    bool GetRotate(void);
    bool GetTurnning(void);
    void SetPreX(float fPrePosX);
    float GetPreX(void);
    void SetPreY(float fPrePosY);
    float GetPreY(void);
    void SetPreZ(float fPrePosZ);
    float GetPreZ(void);
    void SetAllDistance(float fAllDistance);
    float GetAllDistance(void);
    void SetCurveType(BeEffectCurveType eCurveType);
    BeEffectCurveType GetCurveType(void);
    void SetHasCulCurve(bool bHasCulCurve);
    bool GetHasCulCurve(void);
    void SetOwnerFace(float fOwnerFace);
    float GetOwnerFace(void);
    BeEffectCurveStyle GetCurveStyle(void);
    void SetCurveDividNum(int iCurveDividNum);
    int GetCurveDividNum(void);
    TwPos2 GetPointMul2Pos(void);
    TwPos2 GetPointMul3Pos(void);
    void SetOrgPosX(float fOrgPosX);
    void SetOrgPosY(float fOrgPosY);
    void SetOrgPosZ(float fOrgPosZ);
    void SetPosX(float fPosX);
    void SetPosY(float fPosY);
    void SetPosZ(float fPosZ);
    void SetTargetPosX(float fPosX);
    void SetTargetPosY(float fPosY);
    void SetTargetPosZ(float fPosZ);
    bool GetPureShow();
    void SetFlag(int iFlag, bool bNeedRecordChange = true);
    void ClrFlag(int iFlag);
    void SetCreateTime(unsigned int dwTime);
    unsigned int GetCreateTime();
    bool HasUserData(UserDataKey kKey) const;
    void ClearUserData(UserDataKey rkKey);
    void SetUD_Int(UserDataKey rkKey, int i);
    void SetUD_Float(UserDataKey rkKey, float f);
    void SetUD_Void(UserDataKey rkKey, void* pkVoid);
    int GetUD_Int(UserDataKey rkKey, int iDefault) const;
    float GetUD_Float(UserDataKey rkKey, float fDefault) const;
    void* GetUD_Void(UserDataKey rkKey) const;
    BeAttachPos	GetAttachNode();
    int GetProperty() const;
    void    SetNewEffect(bool bNew);
    bool    IsNewEffect();

protected:
    void	UpdateAttach(int iDeltaTime);
    void    UpdateLineFollowEarth(int iDeltaTime);
    void 	UpdateLineTrace(int iDeltaTime);
    //void 	UpdateCurveTraceFrank(int iDeltaTime);
    void	UpdateLineTraceCollision(int iDeltaTime);
    void	UpdateCastTrace(int iDeltaTime);
    void	UpdateCastTraceCollision(int iDeltaTime);
    void    UpdateCurveTrace(int iDeltaTime);
    void	UpdateCurveTraceCollision(int iDeltaTime);
    void    ParseAttack(void);
    void*   UpdateTargetPos(void);
    void    GetCurvePos(float fOrgX, float fOrgY, float fTarX, float fTarY, float fFace, std::vector<TwPos2>& akCurvePos, int iPointNum = 100, BeEffectCurveType eCurveType = BECT_CURVELEFT, BeEffectCurveStyle eCurveStyle = BECS_LEFTBIG);
    void    ParseCurveTrace(void);
    float   GetEffecttrueFace(int iDY, int iDX, int iDeltaTime = 0);

protected:
    int						m_iModelID;
    BeAttachPos				m_eAttachNode;
    BeEffectCurveType   	m_eCurveType;
    float					m_fAllDistance;
    std::vector<TwPos2> 	m_akCurvePosW;
    std::vector<TwPos2> 	m_akCurvePosH;
    float					m_fOwnerFace;
    float					m_fPrePosX;
    float					m_fPrePosY;
    float					m_fPrePosZ;
    TwPos2					m_kCurvePointMul2;
    TwPos2					m_kCurvePointMul3;
    float					m_fCycleAngle;
    bool					m_bTurning;
    bool					m_bRotate;
    bool					m_bHasCulCurve;
    int						m_iCurveDividNum;
    bool					m_bNewEffect;
    unsigned int			m_birthTime;
    bool					m_bSearchTargetSelf;
    int						m_iMaxTargetNum;
    TwPos2					m_posAttacker;
    std::unordered_map<UserDataKey, TwPointerType>	m_kUserData;
    bool					m_bNoDelayDelete;
    BeEffectData		    m_kEffectData;
    std::unique_ptr<BeAttackingAttr>        m_pkAttr;
    std::vector<int>	    m_akBumpAttackedID;
    std::vector<BumpCount>  m_BumpCount;
    BeEffectCurveStyle	    m_eCurveStyle;
    float				    m_fNeedMoveDistance;
    float				    m_fCanMoveDistance;
    bool				    m_bPureShow;
    float				    m_fOrgScale;
    bool				    m_bChangeScale;
    int					    m_iRace;
};

inline int BeEffect::GetRace(void)
{
    return m_iRace;
}
inline void BeEffect::SetRace(int iValue)
{
    if (m_iRace != iValue)
    {
        SetChangeFlag(BECF_RACE);
    }
    m_iRace = iValue;
}
inline bool BeEffect::IsIgnoreDirection(void)
{
    return (m_kEffectData.iProperty & BEP_IGNOREDIRECTION) != 0;
}
inline bool BeEffect::OnCycleRound(void)
{
    return (m_kEffectData.iProperty & BEP_ONCYCLEROUND) != 0;
}
inline void BeEffect::SetProperty(int iValue)
{
    SetChangeFlag(BECF_PROPERTY);
    m_kEffectData.iProperty |= iValue;
}
inline int BeEffect::GetOwnerID(void)
{
    return m_kEffectData.iOwnerID;
}
inline int BeEffect::GetPlayer(void)
{
    return m_kEffectData.iPlayer;
}
inline void BeEffect::SetEffectRemoveTime(unsigned int uiTime, bool bNeedRecordChange)
{
    m_kEffectData.uiRemoveTime = uiTime;
}
inline unsigned int BeEffect::GetEffectRemoveTime(void)	const
{
    return m_kEffectData.uiRemoveTime;
}
inline int BeEffect::GetSourceID(void)
{
    return m_kEffectData.iSourceID;
}
inline void BeEffect::SetSourceID(int iSourceID)
{
    m_kEffectData.iSourceID = iSourceID;
}
inline int BeEffect::GetTargetID(void)
{
    return m_kEffectData.iTargetID;
}
inline float BeEffect::GetMoveSpeed(void)
{
    return m_kEffectData.fMoveSpeed;
}
inline void BeEffect::SetMoveSpeed(float fMoveSpeed)
{
    if (m_kEffectData.fMoveSpeed != fMoveSpeed)
    {
        SetChangeFlag(BECF_MOVESPEED);
    }
    m_kEffectData.fMoveSpeed = fMoveSpeed;
}
inline float BeEffect::GetMissileArc(void)
{
    return m_kEffectData.fMissileArc;
}
inline void BeEffect::SetMissileArc(float fMissileArc)
{
    if (m_kEffectData.fMissileArc != fMissileArc)
    {
        SetChangeFlag(BECF_MISSILEARC);
    }

    m_kEffectData.fMissileArc = fMissileArc;
}
inline float BeEffect::GetPosX(void)
{
    return m_kEffectData.fPosX;
}
inline float BeEffect::GetPosY(void)
{
    return m_kEffectData.fPosY;
}
inline float BeEffect::GetPosZ(void)
{
    return m_kEffectData.fPosZ;
}
inline float BeEffect::GetTarPosX(void)
{
    return m_kEffectData.fTarPosX;
}
inline float BeEffect::GetTarPosY(void)
{
    return m_kEffectData.fTarPosY;
}
inline float BeEffect::GetEffectFace(void)
{
    return m_kEffectData.fFace;
}
inline void BeEffect::SetFace(float fFace, bool bNeedChangeFlag)
{
    AdjustRadian(fFace);

    if (bNeedChangeFlag && m_kEffectData.fFace != fFace)
    {
        SetChangeFlag(BECF_FACE);
    }
    m_kEffectData.fFace = fFace;
}
inline void BeEffect::SetAttachSkill(int iSkillTypeID, int iSkillLevel) {

    if (m_kEffectData.iSkillTypeID != iSkillTypeID)
    {
        SetChangeFlag(BECF_SKILLTYPEID);
    }

    m_kEffectData.iSkillTypeID = iSkillTypeID;
    m_kEffectData.iSkillLevel = iSkillLevel;
}
inline int BeEffect::GetAttachSkill(void)
{
    return m_kEffectData.iSkillTypeID;
}
inline int BeEffect::GetAttachSkillLevel(void)
{
    return m_kEffectData.iSkillLevel;
}
inline float BeEffect::GetDamageScope(void)
{
    return m_kEffectData.fDamageScope;
}
inline void BeEffect::SetDamageScope(float fScope)
{
    m_kEffectData.fDamageScope = fScope;
}
inline void BeEffect::SetTargetFlag(int iFlag)
{
    m_kEffectData.iTargetFlag = iFlag;
}
inline float BeEffect::GetCollisionScope(void)
{
    return m_kEffectData.fCollisionScope;
}
inline void BeEffect::SetCollisionScope(float fScope)
{
    m_kEffectData.fCollisionScope = fScope;
}
inline void BeEffect::SetPlayer(int iPlayer)
{
    m_kEffectData.iPlayer = iPlayer;
}
inline void BeEffect::SetCurveStyle(BeEffectCurveStyle eStyle)
{
    m_eCurveStyle = eStyle;
}
inline void BeEffect::SetData(int iData)
{
    m_kEffectData.iData = iData;
}
inline int BeEffect::GetData(void)
{
    return m_kEffectData.iData;
}
inline void BeEffect::SetFloatData(float fData)
{
    m_kEffectData.fData = fData;
}
inline float BeEffect::GetFloatData(void)
{
    return m_kEffectData.fData;
}
inline bool BeEffect::HasBumped(int iUnitID) const
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
inline void BeEffect::AddBumped(int iUnitID)
{
    AddBumCount(iUnitID);
    if (!HasBumped(iUnitID))
    {
        m_akBumpAttackedID.push_back(iUnitID);
    }
}
inline	int BeEffect::GetBumpedNum()
{
    return (int)m_akBumpAttackedID.size();
}
inline void BeEffect::SetSkillBumpCount(int iCount)
{
    m_kEffectData.iBumpCount = iCount;
}
inline int BeEffect::GetSkillBumpCount(void) const
{
    return m_kEffectData.iBumpCount;
}
inline void BeEffect::SetTargetType(BeEffectTargetType eType)
{
    m_kEffectData.eTargetType = eType;
}
inline BeEffectTargetType BeEffect::GetTargetType()
{
    return m_kEffectData.eTargetType;
}
inline float BeEffect::GetTarPosZ(void)
{
    return m_kEffectData.fTarPosZ;
}
inline const BeEffectData* BeEffect::GetEffectData(void) const
{
    return &m_kEffectData;
}
inline void BeEffect::SetPureShow(bool bPure)
{
    m_bPureShow = bPure;
}
inline void BeEffect::SetMustCreate(bool bMustCreate)
{
    m_kEffectData.bMustCreate = bMustCreate;
}
inline bool BeEffect::GetMustCreate(void)
{
    return m_kEffectData.bMustCreate;
}
inline float BeEffect::GetOrgPosX(void)
{
    return m_kEffectData.fOrgPosX;
}
inline float BeEffect::GetOrgPosY(void)
{
    return m_kEffectData.fOrgPosY;
}
inline float BeEffect::GetOrgPosZ(void)
{
    return m_kEffectData.fOrgPosZ;
}
inline int BeEffect::GetAttrBumpCount(void)
{
    if (!m_pkAttr)
    {
        return 0;
    }
    //return m_pkAttr->iBumpCount;
}
inline bool BeEffect::GetChangeScale(void)
{
    return m_bChangeScale;
}
inline float BeEffect::GetOrgScale(void)
{
    return m_fOrgScale;
}
inline bool BeEffect::GetRotate(void)
{
    return m_bRotate;
}
inline bool BeEffect::GetTurnning(void)
{
    return m_bTurning;
}
inline void BeEffect::SetPreX(float fPrePosX)
{
    m_fPrePosX = fPrePosX;
}
inline float BeEffect::GetPreX(void)
{
    return m_fPrePosX;
}
inline void BeEffect::SetPreY(float fPrePosY)
{
    m_fPrePosY = fPrePosY;
}
inline float BeEffect::GetPreY(void)
{
    return m_fPrePosY;
}
inline void BeEffect::SetPreZ(float fPrePosZ)
{
    m_fPrePosZ = fPrePosZ;
}
inline float BeEffect::GetPreZ(void)
{
    return m_fPrePosZ;
}
inline void BeEffect::SetAllDistance(float fAllDistance)
{
    m_fAllDistance = fAllDistance;
}
inline float BeEffect::GetAllDistance(void)
{
    return m_fAllDistance;
}
inline void BeEffect::SetCurveType(BeEffectCurveType eCurveType)
{
    m_eCurveType = eCurveType;
}
inline BeEffectCurveType BeEffect::GetCurveType(void)
{
    return m_eCurveType;
}
inline void BeEffect::SetHasCulCurve(bool bHasCulCurve)
{
    m_bHasCulCurve = bHasCulCurve;
}
inline bool BeEffect::GetHasCulCurve(void)
{
    return m_bHasCulCurve;
}
inline void BeEffect::SetOwnerFace(float fOwnerFace)
{
    m_fOwnerFace = fOwnerFace;
}
inline float BeEffect::GetOwnerFace(void)
{
    return m_fOwnerFace;
}
inline BeEffectCurveStyle BeEffect::GetCurveStyle(void)
{
    return m_eCurveStyle;
}
inline void BeEffect::SetCurveDividNum(int iCurveDividNum)
{
    m_iCurveDividNum = iCurveDividNum;
}
inline int BeEffect::GetCurveDividNum(void)
{
    return m_iCurveDividNum;
}
inline TwPos2 BeEffect::GetPointMul2Pos(void)
{
    return m_kCurvePointMul2;
}
inline TwPos2 BeEffect::GetPointMul3Pos(void)
{
    return m_kCurvePointMul3;
}
inline void BeEffect::SetOrgPosX(float fOrgPosX)
{
    m_kEffectData.fOrgPosX = fOrgPosX;
}
inline void BeEffect::SetOrgPosY(float fOrgPosY)
{
    m_kEffectData.fOrgPosY = fOrgPosY;
}
inline void BeEffect::SetOrgPosZ(float fOrgPosZ)
{
    m_kEffectData.fOrgPosZ = fOrgPosZ;
}
inline void BeEffect::SetPosX(float fPosX)
{
    m_kEffectData.fPosX = fPosX;
}
inline void BeEffect::SetPosY(float fPosY)
{
    m_kEffectData.fPosY = fPosY;
}
inline void BeEffect::SetPosZ(float fPosZ)
{
    m_kEffectData.fPosZ = fPosZ;
}
inline void BeEffect::SetTargetPosX(float fPosX)
{
    m_kEffectData.fTarPosX = fPosX;
}
inline void BeEffect::SetTargetPosY(float fPosY)
{
    m_kEffectData.fTarPosY = fPosY;
}
inline void BeEffect::SetTargetPosZ(float fPosZ)
{
    m_kEffectData.fTarPosZ = fPosZ;
}
inline bool BeEffect::GetPureShow()
{
    return m_bPureShow;
}
#define ClientEffectUsedFlag	(BEF_ATTRMISSLE|BEF_NODEATHACTION|BEF_REMOVE_WHENUNITDEAD|BEF_REMOVE_WHENUNITDEAD|BEF_IN_MAP_SHOW_CYCLE|BEF_CANT_FORBID|BEF_MUST_CREATE|BEF_NOT_SETPOS|BEF_VISION|BEF_SHOW_IN_WARFOG|BEF_CANT_FORBID|BEF_IN_MAP_SHOW_MOVE|BEF_IN_MAP_SHOW_CYCLE)
inline void BeEffect::SetFlag(int iFlag, bool bNeedRecordChange)
{
    if (bNeedRecordChange)
    {
        int iTempFlag = iFlag;
        iTempFlag &= ClientEffectUsedFlag;
    }
}
inline void BeEffect::ClrFlag(int iFlag)
{
    int iTempFlag = iFlag;
    iTempFlag &= ClientEffectUsedFlag;
}
inline void BeEffect::SetCreateTime(unsigned int dwTime)
{
    m_kEffectData.dwCreateTime = dwTime;
}
inline unsigned int BeEffect::GetCreateTime()
{
    return m_kEffectData.dwCreateTime;
}
inline bool BeEffect::HasUserData(UserDataKey kKey) const
{
    if (m_kUserData.find(kKey) != m_kUserData.end())
    {
        return true;
    }
    return false;
}
inline void BeEffect::ClearUserData(UserDataKey rkKey)
{
    m_kUserData.erase(rkKey);
}
inline void BeEffect::SetUD_Int(UserDataKey rkKey, int i)
{
    m_kUserData[rkKey] = TwPointerType(i);
}
inline void BeEffect::SetUD_Float(UserDataKey rkKey, float f)
{
    m_kUserData[rkKey] = TwPointerType(f);
}
inline void BeEffect::SetUD_Void(UserDataKey rkKey, void* pkVoid)
{
    m_kUserData[rkKey] = TwPointerType(pkVoid);
}
inline int BeEffect::GetUD_Int(UserDataKey rkKey, int iDefault) const
{
    auto itr = m_kUserData.find(rkKey);
    if (itr != m_kUserData.end())
    {
        return itr->second.v.iValue;
    }
    return iDefault;
}
inline float BeEffect::GetUD_Float(UserDataKey rkKey, float fDefault) const
{
    auto itr = m_kUserData.find(rkKey);
    if (itr != m_kUserData.end())
    {
        return itr->second.v.fValue;
    }
    return fDefault;
}
inline void* BeEffect::GetUD_Void(UserDataKey rkKey) const
{
    auto itr = m_kUserData.find(rkKey);
    if (itr != m_kUserData.end())
    {
        return itr->second.v.pVoid;
    }
    return nullptr;
}
inline BeAttachPos	BeEffect::GetAttachNode()
{
    return m_eAttachNode;
}
inline int BeEffect::GetProperty() const
{
    return m_kEffectData.iProperty;
}
inline void BeEffect::SetBirthTime(unsigned int iTime)
{
    m_birthTime = iTime;
}
inline unsigned int BeEffect::GetBirthTime()
{
    return m_birthTime;
}
inline void BeEffect::SetSearchTarget(bool bSearch)
{
    m_bSearchTargetSelf = bSearch;
}
inline void BeEffect::SetNoDelayDelete(bool bDelay)
{
    m_bNoDelayDelete = bDelay;
}
inline int BeEffect::GetModelFile()
{
    return m_iModelID;
}
