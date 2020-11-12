#pragma once
#pragma execution_character_set("utf-8")

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <string>
#include <map>
#include <memory>


enum E_UNITTABLE_CLASSTYPE
{
    UNIT_CLASSTYPE_HERO                         =    0      ,   //  Hero
    UNIT_CLASSTYPE_SOLIDER                      =    1      ,   //  Solider
    UNIT_CLASSTYPE_BUILDING                     =    3      ,   //  Building
};

enum E_UNITTABLE_WEAPONTYPE
{
    UNIT_WEAPONTYPE_ONEHAND                     =    1      ,   //  OneHand
    UNIT_WEAPONTYPE_TWOHAND                     =    2      ,   //  TwoHand
    UNIT_WEAPONTYPE_LONGHAND                    =    3      ,   //  LongHand
    UNIT_WEAPONTYPE_ARCHER                      =    4      ,   //  Archer
};

enum E_UNITTABLE_ATTACKTYPE
{
    UNIT_ATTACKTYPE_NONE                        =    0      ,   //  None
    UNIT_ATTACKTYPE_NORMAL                      =    1      ,   //  Normal
    UNIT_ATTACKTYPE_MAGIC                       =    3      ,   //  Magic
    UNIT_ATTACKTYPE_HERO                        =    4      ,   //  Hero
    UNIT_ATTACKTYPE_SKILL                       =    7      ,   //  Skill
    UNIT_ATTACKTYPE_ABSOLUTE                    =    8      ,   //  Absolute
};


enum M_UNITTABLE_PROPERTY
{
    UNIT_PROPERTY_NONE                          =    1 << 0        ,   //  None
    UNIT_PROPERTY_HIDEBAR                       =    1 << 1        ,   //  HideHUD
    UNIT_PROPERTY_HIDEEVER                      =    1 << 2        ,   //  HideEver
};

class UnitTable
{
public:
    unsigned int               uiUnitTypeID                        ;   //  Type ID
    std::string                kUnitName                           ;   //  Name
    int                        uiClassType                         ;   //  Type
    unsigned int               uiProperty                          ;   //  Attr
    int                        uiWeaponType                        ;   //  weapon Type
    float                      fOrgMaxHP                           ;   //  Original Max HP
    float                      fOrgDayRegenHP                      ;   //  Original Regen HP
    float                      fOrgMaxMP                           ;   //  Original Max MP
    float                      fOrgRegenMP                         ;   //  Original Regen MP
    int                        uiAttackType                        ;   //  Attack Type
    float                      fOrgDamage                          ;   //  Attack Number
    float                      fOrgMagic                           ;   //  Original Magic
    float                      fAttackRange                        ;   //  Attack Range
    float                      fOrgArmor                           ;   //  Original Armor
    float                      fOrgAntiMagic                       ;   //  AntiMagic
    unsigned int               uiAttackDamagePt                    ;   //  Attack Damage pt
    float                      fAttackBackPt                       ;   //  Attack CD Time
    int                        iSkillList[5]                          ;   //  Skills
    float                      fOrgMoveSpeed                       ;   //  Move Speed
    float                      fTurnSpeed                          ;   //  Turn Speed
    unsigned int               uiCollision                         ;   //  Collision
    float                      fTouchRadius                        ;   //  Touch Radius
    float                      fMissileSpeed                       ;   //  Missile Speed
    unsigned int               uiMissileModel                      ;   //  Missile Model
    float                      fHpLevelAdd                         ;   //  HP level add
    float                      fMpLevelAdd                         ;   //  MP level add
    float                      fOrgAddUpDamage                     ;   //  Attack level add
    float                      fAttackSpeedAddUp                   ;   //  Attack speed level add
    float                      fMagicAddUp                         ;   //  Magic level add
    float                      fAmorAddUp                          ;   //  Armor level add
    float                      fMagicAmorAddUp                     ;   //  AntiMagic level add
    float                      fHpRegenAddUp                       ;   //  HP regen level add
    float                      fMpRegenAddUp                       ;   //  MP regen level add
};

class TiXmlElement;
class UnitTableMgr
{
public:
    static const std::unique_ptr<UnitTableMgr>& Get();
    UnitTableMgr();
    ~UnitTableMgr();

    const std::shared_ptr<const UnitTable>& GetUnitTable(unsigned int iTypeID) const;
    const std::map<unsigned int, std::shared_ptr<const UnitTable>>& GetUnitTableMap() const;

private:
    bool    Load();
    void    FillData(UnitTable* row, TiXmlElement* element);

    std::map<unsigned int,std::shared_ptr<const UnitTable>>      m_kUnitTableMap;
    static std::unique_ptr<UnitTableMgr>        m_pkUnitTableMgr;
};
