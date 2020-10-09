#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>


enum E_UNITTABLE_CLASSTYPE
{
    UNIT_CLASSTYPE_HERO                         =    0      ,   //  英雄
    UNIT_CLASSTYPE_SOLIDER                      =    1      ,   //  士兵
    UNIT_CLASSTYPE_TOWER                        =    2      ,   //  投石车
    UNIT_CLASSTYPE_BUILDING                     =    3      ,   //  建筑
    UNIT_CLASSTYPE_MONST                        =    4      ,   //  野怪
    UNIT_CLASSTYPE_BOSS                         =    5      ,   //  Boss
    UNIT_CLASSTYPE_SHOP                         =    6      ,   //  商店
};

enum E_UNITTABLE_PRIMARY
{
    UNIT_PRIMARY_NONE                           =    0      ,   //  无
    UNIT_PRIMARY_LILIANG                        =    1      ,   //  力量
    UNIT_PRIMARY_MINJIE                         =    2      ,   //  敏捷
    UNIT_PRIMARY_ZHILI                          =    3      ,   //  智力
};

enum E_UNITTABLE_ATTACKTYPE
{
    UNIT_ATTACKTYPE_NONE                        =    0      ,   //  无
    UNIT_ATTACKTYPE_PUTONG                      =    1      ,   //  普通
    UNIT_ATTACKTYPE_CHUANCI                     =    2      ,   //  穿刺
    UNIT_ATTACKTYPE_MOFA                        =    3      ,   //  魔法
    UNIT_ATTACKTYPE_HERO                        =    4      ,   //  英雄
    UNIT_ATTACKTYPE_GONGCHENG                   =    5      ,   //  攻城
    UNIT_ATTACKTYPE_HUNLUAN                     =    6      ,   //  混乱
    UNIT_ATTACKTYPE_SKILL                       =    7      ,   //  技能
    UNIT_ATTACKTYPE_JUEDUI                      =    8      ,   //  绝对
};

enum E_UNITTABLE_DEFENSETYPE
{
    UNIT_DEFENSETYPE_QINGJIA                    =    0      ,   //  轻甲
    UNIT_DEFENSETYPE_ZHONGJIA                   =    1      ,   //  中甲
    UNIT_DEFENSETYPE_MAXJIA                     =    2      ,   //  重甲
    UNIT_DEFENSETYPE_HERO                       =    3      ,   //  英雄
    UNIT_DEFENSETYPE_ADDJIA                     =    4      ,   //  加强
};


enum M_UNITTABLE_PROPERTY
{
    UNIT_PROPERTY_NONE                          =    1 << 0        ,   //  无特性
    UNIT_PROPERTY_SHENGWU                       =    1 << 1        ,   //  生物
    UNIT_PROPERTY_JIXIE                         =    1 << 2        ,   //  机械
    UNIT_PROPERTY_YUANGU                        =    1 << 3        ,   //  远古
    UNIT_PROPERTY_YINCANG                       =    1 << 4        ,   //  小地图隐藏
    UNIT_PROPERTY_POSUN                         =    1 << 5        ,   //  破损动作
};

//  自动生成表结构
struct UnitTable
{
    unsigned int               UnitTypeID               ;   
    std::string                UnitName                 ;   
    int                        ClassType                ;   
    unsigned int               Property                 ;   
    int                        Primary                  ;   
    float                      OrgSTR                   ;   
    float                      PlusSTR                  ;   
    float                      OrgAGI                   ;   
    float                      PlusAGI                  ;   
    float                      OrgINT                   ;   
    float                      PlusINT                  ;   
    float                      OrgMaxHP                 ;   
    float                      OrgDayRegenHP            ;   
    float                      OrgMaxMP                 ;   
    float                      OrgRegenMP               ;   
    int                        DayVision                ;   
    int                        AttackType               ;   
    float                      OrgMinDamage             ;   
    float                      OrgMaxDamage             ;   
    float                      AttackRange              ;   
    int                        OrgAttackCD              ;   
    int                        DefenseType              ;   
    float                      OrgArmor                 ;   
    int                        DeathMoney               ;   
    int                        DeathExp                 ;   
    float                      MissileSpeed             ;   
    float                      MissileArc               ;   
    float                      AttackDamagePt           ;   
    float                      AttackBackPt             ;   
    float                      SpellDamagePt            ;   
    float                      SpellBackPt              ;   
    std::string                Icon                     ;   
    std::string                SkillList                ;   
    float                      OrgMoveSpeed             ;   
    float                      TurnSpeed                ;   
    int                        FlyHeight                ;   
    float                      Collision                ;   
    std::string                ModelFile                ;   
    float                      ModelScale               ;   
    std::string                MissileModel             ;   
    float                      SelectScale              ;   
    std::string                SelectSound              ;   
    std::string                AttackVoice              ;   
    std::string                WalkSound                ;   
    std::string                AttackSound              ;   
    std::string                AttackedSound            ;   
    std::string                DeathSound               ;   
    std::string                ExclusiveItemTypeID      ;   
};

class TiXmlElement;
class UnitTableMgr
{
public:
    static UnitTableMgr* Get();
    UnitTableMgr();
    ~UnitTableMgr();

    UnitTable* GetUnitTable(unsigned int iTypeID);

private:
    bool    Load();
    void    FillData(UnitTable* row, TiXmlElement* element);

    std::map<unsigned int,UnitTable*>      m_kUnitTableMap;
    static UnitTableMgr* m_pkUnitTableMgr;
};
