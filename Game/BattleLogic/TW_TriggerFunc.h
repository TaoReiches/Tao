#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>

class BeCarry;
class BeEffect;
class TwUnit;
class BeSkill;
class BeBuffer;
class BeItem;
class BeMapItem;
class BeAttackingAttr;
class TwPtTriggerMgr;
class SkillTable;
enum class TwPtCondResult;

class TwTriggerFunc
{
public:
    TwTriggerFunc();
    virtual ~TwTriggerFunc();

    BeCarry* GetTrgCarry(void);
    int			GetTrgSkillTypeID(void);
    int			GetTrgSkillOrgTypeID(void);
    int			GetTrgSkillLevel(void);
    int			GetTrgSpellTargetID(void);
    int* GetTrgPtrSpellTargetID(void);
    int			GetTrgSpellUsePlayer(void);
    float       GetTrgSpellOrgPosX(void);
    float		GetTrgSpellOrgPosY(void);
    float		GetTrgSpellTargetPosX(void);
    float		GetTrgSpellTargetPosY(void);
    float		GetTrgSpellDirectPosX(void);
    float		GetTrgSpellDirectPosY(void);
    std::shared_ptr<TwUnit> GetTrgUnit(bool bIncSuspend = false);
    std::shared_ptr<TwUnit> GetTrgAttacker(void);
    float		GetTrgAttackedDamge(void);
    int			GetTrgAttackerPlayer(void);
    std::shared_ptr<TwUnit> GetTrgTarget(void);
    int			GetTrgAttackedType(void);
    int			GetTrgAttackedIsFirst(void);
    int			GetTrgAttackedFlag(void);
    int* GetTrgAttackedFlagPtr(void);
    int			GetTrgBufferTypeID(void);
    int			GetTrgBufferLevel(void);
    int			GetTrgBufferInnerID(void);
    int			GetTrgTargetID();

    int			GetTrgIntData(void);
    float		GetTrgFloatData(void);
    bool		GetTrgIsLearnSkill(void);

    std::shared_ptr<TwUnit> TrgGetPtr_Unit(void);
    std::shared_ptr<TwUnit> TrgGetPtr_Attacker(void);
    std::shared_ptr<TwUnit> TrgGetPtr_Target(void);
    BeSkill* TrgGetPtr_Skill(void);
    BeBuffer* TrgGetPtr_Buffer(void);
    BeEffect* TrgGetPtr_Effect(void);
    BeItem* TrgGetPtr_Item(void);
    BeMapItem* TrgGetPtr_MapItem(void);
    int			TrgGetItemPos(void);
    int			TrgGetItemID(void);
    int			TrgGetItemTypeID(void);
    int			TrgGetTriggerItemID(void);
    float* TrgGetPtr_Damage(void);
    float		GetTrgAttackedRawDamge(void);
    float		TrgGetDamageFace(void);

    BeAttackingAttr* TrgGetPtr_AttackAttr(void);
    int* TrgGetPtr_Persist(void);
    int* TrgGetPtr_PersistDelta(void);
    int TrgGetTargetType(void);

    TwPtCondResult DefaultBuffer_Condition(int iOriginTypeID);
    bool DefaultSkill_ActionParam(std::shared_ptr<TwUnit>& pkTrgUnit, unsigned int& uiSkillTypeID, int& iSkillLevel, const SkillTable* pkSkillRes);
    void InitSkillEventTrigger(int iTypeID);
    void InitItemEventTrigger(int iTypeID);

    TwPtTriggerMgr* m_pkTriggerMgr;
};
