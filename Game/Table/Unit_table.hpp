#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>


enum E_UNITTABLE_CLASSTYPE
{
    UNIT_CLASSTYPE_HERO                         =    0      ,   //  英雄
    UNIT_CLASSTYPE_SOLIDER                      =    1      ,   //  士兵
    UNIT_CLASSTYPE_MACHINE                      =    2      ,   //  机械
    UNIT_CLASSTYPE_BUILDING                     =    3      ,   //  建筑
    UNIT_CLASSTYPE_SMALLMONST                   =    4      ,   //  小型野怪
    UNIT_CLASSTYPE_BIGMONST                     =    5      ,   //  大型野怪
    UNIT_CLASSTYPE_BOSS                         =    6      ,   //  Boss
    UNIT_CLASSTYPE_SHOP                         =    7      ,   //  商店
    UNIT_CLASSTYPE_EYE                          =    8      ,   //  眼
    UNIT_CLASSTYPE_TOTEM                        =    9      ,   //  图腾
    UNIT_CLASSTYPE_BRAND                        =    10     ,   //  印记
    UNIT_CLASSTYPE_PET                          =    11     ,   //  宠物
    UNIT_CLASSTYPE_MIJINGMONSTER                =    12     ,   //  秘境小怪
    UNIT_CLASSTYPE_MIJINGBOSS                   =    13     ,   //  秘境大怪
};

enum E_UNITTABLE_WEAPONTYPE
{
    UNIT_WEAPONTYPE_ONEHAND                     =    1      ,   //  单手
    UNIT_WEAPONTYPE_TWOHAND                     =    2      ,   //  双持
    UNIT_WEAPONTYPE_LONGHAND                    =    3      ,   //  长柄
    UNIT_WEAPONTYPE_ARCHER                      =    4      ,   //  弓箭
};

enum E_UNITTABLE_ATTACKTYPE
{
    UNIT_ATTACKTYPE_NONE                        =    0      ,   //  不攻击
    UNIT_ATTACKTYPE_PUTONG                      =    1      ,   //  普通
    UNIT_ATTACKTYPE_CHUANCI                     =    2      ,   //  穿刺
    UNIT_ATTACKTYPE_MOFA                        =    3      ,   //  魔法
    UNIT_ATTACKTYPE_HERO                        =    4      ,   //  英雄
    UNIT_ATTACKTYPE_GONGCHENG                   =    5      ,   //  攻城
    UNIT_ATTACKTYPE_HUNLUAN                     =    6      ,   //  混乱
    UNIT_ATTACKTYPE_SKILL                       =    7      ,   //  技能
    UNIT_ATTACKTYPE_JUEDUI                      =    8      ,   //  绝对
};


enum M_UNITTABLE_PROPERTY
{
    UNIT_PROPERTY_NONE                          =    1 << 0        ,   //  无特性
    UNIT_PROPERTY_YUANGU                        =    1 << 3        ,   //  远古
    UNIT_PROPERTY_YINCANG                       =    1 << 4        ,   //  小地图隐藏
    UNIT_PROPERTY_POSUN                         =    1 << 5        ,   //  破损动作
    UNIT_PROPERTY_TURNBUILD                     =    1 << 6        ,   //  转身建筑
    UNIT_PROPERTY_HIDEBAR                       =    1 << 7        ,   //  血条隐藏
    UNIT_PROPERTY_DECPOWDAMAGE                  =    1 << 8        ,   //  受到塔伤害降低
    UNIT_PROPERTY_HIDEEVER                      =    1 << 9        ,   //  永久隐藏
    UNIT_PROPERTY_NOATTACKACTION                =    1 << 10       ,   //  没有攻击动作
    UNIT_PROPERTY_CRITATTACKACTION              =    1 << 11       ,   //  暴击动作
    UNIT_PROPERTY_POWERBAR                      =    1 << 12       ,   //  能量条
    UNIT_PROPERTY_HUDUNBAR                      =    1 << 13       ,   //  护盾条
    UNIT_PROPERTY_NUQIBAR                       =    1 << 14       ,   //  怒气条
    UNIT_PROPERTY_NOBAR                         =    1 << 15       ,   //  没有条
    UNIT_PROPERTY_ZIDANBAR                      =    1 << 16       ,   //  子弹条
    UNIT_PROPERTY_NOTADDATTACKCD                =    1 << 17       ,   //  不享受攻速
    UNIT_PROPERTY_NOTADDBAOJI                   =    1 << 18       ,   //  不享受暴击
};

//  自动生成表结构
struct UnitTable
{
    unsigned int               uiUnitTypeID                        ;   //  ID   
    std::string                kUnitName                           ;   //  单位名称   
    int                        uiClassType                         ;   //  单位种类   
    unsigned int               uiProperty                          ;   //  特性   
    int                        uiWeaponType                        ;   //  武器类型   
    float                      fOrgMaxHP                           ;   //  基础生命   
    float                      fOrgDayRegenHP                      ;   //  生命回复   
    float                      fOrgMaxMP                           ;   //  基础法力   
    float                      fOrgRegenMP                         ;   //  法力回复   
    int                        uiAttackType                        ;   //  攻击类型   
    float                      fOrgDamage                          ;   //  攻击值   
    float                      fOrgMagic                           ;   //  法术强度   
    float                      fAttackRange                        ;   //  攻击范围   
    float                      fOrgArmor                           ;   //  基础护甲   
    float                      fOrgAntiMagic                       ;   //  法抗值   
    int                        iDeathMoney                         ;   //  金钱奖励   
    int                        iDeathExp                           ;   //  经验奖励   
    float                      fAttackDamagePt                     ;   //  攻击结算点   
    float                      fAttackBackPt                       ;   //  攻击CD总时间   
    int                        iSkillList[5]                          ;   //  单位技能   
    float                      fOrgMoveSpeed                       ;   //  移动速度   
    float                      fTurnSpeed                          ;   //  转身速度   
    float                      fCollision                          ;   //  碰撞体积   
    float                      fTouchRadius                        ;   //  接触半径   
    float                      fMissileSpeed                       ;   //  投射速度   
    float                      fMissileArc                         ;   //  投射弧度   
    int                        iMissileModel                       ;   //  投射模型   
    std::string                kExclusiveItemTypeID                ;   //  专属物品   
    float                      fHpLevelAdd                         ;   //  生命成长   
    float                      fMpLevelAdd                         ;   //  法力成长   
    float                      fOrgAddUpDamage                     ;   //  攻击成长   
    float                      fAttackSpeedAddUp                   ;   //  攻速成长   
    float                      fMagicAddUp                         ;   //  法术成长   
    float                      fAmorAddUp                          ;   //  护甲成长   
    float                      fMagicAmorAddUp                     ;   //  法术抗性成长   
    float                      fHpRegenAddUp                       ;   //  生命回复成长   
    float                      fMpRegenAddUp                       ;   //  法力回复成长   
    float                      fModelScale                         ;   //  模型比例   
    int                        iTargetDamage                       ;   //  目标的被击特效   
    int                        iBasisEquip[2]                          ;   //  英雄基础装备   
    int                        iRecEquip[7]                          ;   //  英雄推荐装备   
};

class TiXmlElement;
class UnitTableMgr
{
public:
    static UnitTableMgr* Get();
    UnitTableMgr();
    ~UnitTableMgr();

    const UnitTable* GetUnitTable(unsigned int iTypeID);
    const std::map<unsigned int, UnitTable*>& GetUnitTableMap();

private:
    bool    Load();
    void    FillData(UnitTable* row, TiXmlElement* element);

    std::map<unsigned int,UnitTable*>      m_kUnitTableMap;
    static UnitTableMgr* m_pkUnitTableMgr;
};
