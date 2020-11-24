#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class BeCarry;
class BeEffect;
class BeUnit;
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
    BeUnit* GetTrgUnit(bool bIncSuspend = false);
    BeUnit* GetTrgAttacker(void);
    float		GetTrgAttackedDamge(void);
    int			GetTrgAttackerPlayer(void);
    BeUnit* GetTrgTarget(void);
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

    BeUnit* TrgGetPtr_Unit(void);
    BeUnit* TrgGetPtr_Attacker(void);
    BeUnit* TrgGetPtr_Target(void);
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
    bool DefaultSkill_ActionParam(BeUnit*& pkTrgUnit, unsigned int& uiSkillTypeID, int& iSkillLevel, const SkillTable* pkSkillRes);
    void InitSkillEventTrigger(int iTypeID);
    void InitItemEventTrigger(int iTypeID);

    TwPtTriggerMgr* m_pkTriggerMgr;
};
