#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>


enum E_SKILLTABLE_OPERATETYPE
{
    SKILL_OPERATETYPE_PASSIVE                   =    0      ,   //  Passive
    SKILL_OPERATETYPE_IMMEDIATELY               =    1      ,   //  Immediately
    SKILL_OPERATETYPE_SWITCH                    =    5      ,   //  Switch
    SKILL_OPERATETYPE_SINGLETARGET              =    6      ,   //  SingleTarget
};

enum E_SKILLTABLE_AFTERACTIONTYPE
{
    SKILL_AFTERACTIONTYPE_NONE                  =    1      ,   //  None
    SKILL_AFTERACTIONTYPE_ATTACKTARGET            =    2      ,   //  AttackTarget
    SKILL_AFTERACTIONTYPE_DOLAST                =    3      ,   //  DoLast
    SKILL_AFTERACTIONTYPE_ONLYATTACK            =    4      ,   //  OnlyAttack
};

enum E_SKILLTABLE_ATTACKTYPE
{
    SKILL_ATTACKTYPE_NONE                       =    0      ,   //  None
    SKILL_ATTACKTYPE_MAGIC                      =    2      ,   //  Magic
    SKILL_ATTACKTYPE_PHYSICS                    =    1      ,   //  Physics
    SKILL_ATTACKTYPE_ABSOLUTE                   =    3      ,   //  Absolute
};

enum E_SKILLTABLE_SPELLACTION
{
    SKILL_SPELLACTION_NULL                      =    0      ,   //  null
    SKILL_SPELLACTION_STAND                     =    1      ,   //  stand
    SKILL_SPELLACTION_STAND1                    =    2      ,   //  stand-1
    SKILL_SPELLACTION_WALK                      =    3      ,   //  walk
    SKILL_SPELLACTION_ATTACK                    =    4      ,   //  attack
    SKILL_SPELLACTION_DEATH                     =    5      ,   //  death
    SKILL_SPELLACTION_SPELL                     =    6      ,   //  atk
    SKILL_SPELLACTION_SPELL1                    =    7      ,   //  atk1
    SKILL_SPELLACTION_SPELL2                    =    8      ,   //  atk2
    SKILL_SPELLACTION_SPELL3                    =    9      ,   //  atk3
};

enum E_SKILLTABLE_DATACONTENTA
{
    SKILL_DATACONTENTA_MAXHP                    =    1      ,   //  MaxHP
    SKILL_DATACONTENTA_MAXMP                    =    2      ,   //  MaxMP
    SKILL_DATACONTENTA_ATTACK                   =    3      ,   //  Attack
    SKILL_DATACONTENTA_ARMOR                    =    4      ,   //  Armor
    SKILL_DATACONTENTA_ARMORPERCENT             =    5      ,   //  ArmorPercent
    SKILL_DATACONTENTA_ATTACKPERCEN             =    6      ,   //  AttackPercent
    SKILL_DATACONTENTA_MOVESPEED                =    7      ,   //  MoveSpeed
    SKILL_DATACONTENTA_ATTACKSPEEDPERCENT            =    8      ,   //  AttackSpeedPercent
    SKILL_DATACONTENTA_MOVESPEEDPERCENT            =    9      ,   //  MoveSpeedPercent
    SKILL_DATACONTENTA_MAGIC                    =    10     ,   //  Magic
    SKILL_DATACONTENTA_MAGICRESISTANCE            =    11     ,   //  MagicResistance
    SKILL_DATACONTENTA_LEECH                    =    12     ,   //  Leech
    SKILL_DATACONTENTA_AVOIDANCE                =    13     ,   //  Avoidance
    SKILL_DATACONTENTA_ATTACKSPEED              =    14     ,   //  AttackSpeed
    SKILL_DATACONTENTA_SHUZHI                   =    1000   ,   //  Number
    SKILL_DATACONTENTA_PERCENT                  =    1001   ,   //  Percent
    SKILL_DATACONTENTA_TIME                     =    1002   ,   //  Time
    SKILL_DATACONTENTA_DISTANCE                 =    1003   ,   //  Distance
    SKILL_DATACONTENTA_SCOPE                    =    1004   ,   //  Scope
};


enum M_SKILLTABLE_TARGETTYPE
{
    SKILL_TARGETTYPE_HERO                       =    1 << 0        ,   //  Hero
    SKILL_TARGETTYPE_BUILDING                   =    1 << 1        ,   //  Building
    SKILL_TARGETTYPE_BOSS                       =    1 << 2        ,   //  Boss
    SKILL_TARGETTYPE_ENEMY                      =    1 << 3        ,   //  Enemy
    SKILL_TARGETTYPE_FRIEND                     =    1 << 4        ,   //  Friend
    SKILL_TARGETTYPE_SELF                       =    1 << 5        ,   //  Self
};

enum M_SKILLTABLE_SKILLPROPERTY
{
    SKILL_SKILLPROPERTY_BEATTACKTRIGGER            =    1 << 1        ,   //  BeAttackTrigger
    SKILL_SKILLPROPERTY_ATTACKTRIGGER            =    1 << 2        ,   //  AttackTrigger
    SKILL_SKILLPROPERTY_DAMAGETRIGGER            =    1 << 3        ,   //  DamageTrigger
    SKILL_SKILLPROPERTY_KILLTRIGGER             =    1 << 4        ,   //  KillTrigger
    SKILL_SKILLPROPERTY_DEADTRIGGER             =    1 << 5        ,   //  DeadTrigger
    SKILL_SKILLPROPERTY_CDTRIGGER               =    1 << 6        ,   //  CDTrigger
    SKILL_SKILLPROPERTY_SKILLTRIGGER            =    1 << 7        ,   //  SkillTrigger
    SKILL_SKILLPROPERTY_CASTCMD                 =    1 << 8        ,   //  CastCmd
    SKILL_SKILLPROPERTY_DEATHRELIVE             =    1 << 9        ,   //  Relive
};

struct SkillTable
{
    unsigned int               uiTypeID                            ;   //  TypeID
    std::string                kName                               ;   //  SkillName
    int                        uiOperateType                       ;   //  SkillType
    unsigned int               uiOriginTypeID                      ;   //  Orignal Type ID
    int                        iUIShowPos                          ;   //  UI position
    int                        uiAfterActionType                   ;   //  Action after skill
    unsigned int               uiTargetType                        ;   //  Target Type
    int                        uiAttackType                        ;   //  Damage Type
    unsigned int               uiSkillProperty                     ;   //  Attr
    int                        iSkillMaxLevel                      ;   //  maximum Level
    int                        uiSpellAction                       ;   //  Action
    int                        iLearnLevel[5]                          ;   //  Learn Level
    int                        iCastTime[5]                          ;   //  Cast Time
    int                        iShakesTime[5]                          ;   //  Shakes Time
    int                        iEffectTime[5]                          ;   //  Effect Time
    int                        iCoolDown[5]                          ;   //  CoolDown Time
    int                        iEffectScope[5]                          ;   //  Effect Scope
    int                        iSkillDistance[5]                          ;   //  Skill Distance
    int                        iSkillAngle[5]                          ;   //  Skill Angle
    int                        iPilePointNums[5]                          ;   //  Pile Point Count
    int                        iHpSpend[5]                          ;   //  HP Spend
    int                        iManaSpend[5]                          ;   //  Mana Spend
    int                        uiDataContentA                      ;   //  TypeA
    float                      fValueA[5]                          ;   //  Number A
    int                        uiDataContentB                      ;   //  Type B
    float                      fValueB[5]                          ;   //  Number B
    int                        uiDataContentC                      ;   //  Type C
    float                      fValueC[5]                          ;   //  Number C
    int                        uiDataContentD                      ;   //  Type D
    float                      fValueD[5]                          ;   //  Number D
    int                        uiDataContentE                      ;   //  Type E
    float                      fValueE[5]                          ;   //  Number E
    int                        uiDataContentF                      ;   //  Type F
    float                      fValueF[5]                          ;   //  Number F
    int                        uiDataContentG                      ;   //  Type G
    float                      fValueG[5]                          ;   //  Number G
    unsigned int               uiMissileModel                      ;   //  Missile Model
    float                      fMissileSpeed                       ;   //  Missile Speed
    unsigned int               uiSrcEffect                         ;   //  Src Effect
    unsigned int               uiDstPosEffect                      ;   //  Dst Point Effect
    unsigned int               uiDstUnitEffect                     ;   //  Target Effect
    unsigned int               uiSkillEffect                       ;   //  Skill Effect
};

class TiXmlElement;
class SkillTableMgr
{
public:
    static SkillTableMgr* Get();
    SkillTableMgr();
    ~SkillTableMgr();

    const SkillTable* GetSkillTable(unsigned int iTypeID);
    const std::map<unsigned int, SkillTable*>& GetSkillTableMap();

private:
    bool    Load();
    void    FillData(SkillTable* row, TiXmlElement* element);

    std::map<unsigned int,SkillTable*>      m_kSkillTableMap;
    static SkillTableMgr* m_pkSkillTableMgr;
};
