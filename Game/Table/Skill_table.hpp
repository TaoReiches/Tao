#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>
#include "SeTableResBase.h"


enum E_SKILLTABLE_OPERATETYPE
{
    SKILL_OPERATETYPE_LIJISHIFA                 =    1      ,   //  立即施法
    SKILL_OPERATETYPE_BEIDONG                   =    0      ,   //  被动技能
    SKILL_OPERATETYPE_XIANXING                  =    2      ,   //  线性施法
    SKILL_OPERATETYPE_ANZHU                     =    3      ,   //  按住施法
    SKILL_OPERATETYPE_HUADONG                   =    4      ,   //  滑动施法
    SKILL_OPERATETYPE_KAIGUAN                   =    5      ,   //  开关施法
    SKILL_OPERATETYPE_DANTI                     =    6      ,   //  单体指向
    SKILL_OPERATETYPE_QUYU                      =    7      ,   //  区域指向
    SKILL_OPERATETYPE_SHANXING                  =    8      ,   //  扇形施法
    SKILL_OPERATETYPE_CHONGCI                   =    9      ,   //  冲刺施法
    SKILL_OPERATETYPE_MUBIAO                    =    10     ,   //  目标地点
    SKILL_OPERATETYPE_MOFAQIANG                 =    11     ,   //  魔法墙
    SKILL_OPERATETYPE_GODUSE                    =    12     ,   //  上帝施法
    SKILL_OPERATETYPE_TAKEDRUGS                 =    13     ,   //  下地施法
    SKILL_OPERATETYPE_PANDUAN                   =    14     ,   //  判断选取
};

enum E_SKILLTABLE_AFTERACTIONTYPE
{
    SKILL_AFTERACTIONTYPE_NONE                  =    1      ,   //  无操作
    SKILL_AFTERACTIONTYPE_ATTACKTARGET            =    2      ,   //  攻击目标
    SKILL_AFTERACTIONTYPE_DOLAST                =    3      ,   //  指令持续
    SKILL_AFTERACTIONTYPE_ONLYATTACK            =    4      ,   //  仅攻击
};

enum E_SKILLTABLE_ATTACKTYPE
{
    SKILL_ATTACKTYPE_NONE                       =    0      ,   //  无伤害
    SKILL_ATTACKTYPE_MAGIC                      =    2      ,   //  法术
    SKILL_ATTACKTYPE_PHYSICS                    =    1      ,   //  物理
    SKILL_ATTACKTYPE_ABSOLUTE                   =    3      ,   //  绝对
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
    SKILL_SPELLACTION_SPELL4                    =    10     ,   //  atk4
    SKILL_SPELLACTION_SPELL41                   =    11     ,   //  atk4_1
    SKILL_SPELLACTION_DEMORPH                   =    12     ,   //  demorph
    SKILL_SPELLACTION_SPELL11                   =    13     ,   //  atk1_1
    SKILL_SPELLACTION_SPELL12                   =    14     ,   //  atk1_2
    SKILL_SPELLACTION_SPELL13                   =    15     ,   //  atk1_3
    SKILL_SPELLACTION_SPELL21                   =    16     ,   //  atk2_1
    SKILL_SPELLACTION_SPELL22                   =    17     ,   //  atk2_2
    SKILL_SPELLACTION_SPELL23                   =    18     ,   //  atk2_3
    SKILL_SPELLACTION_SPELL24                   =    19     ,   //  atk2_4
    SKILL_SPELLACTION_SPELL31                   =    20     ,   //  atk3_1
    SKILL_SPELLACTION_SPELL32                   =    21     ,   //  atk3_2
    SKILL_SPELLACTION_SPELL33                   =    22     ,   //  atk3_3
    SKILL_SPELLACTION_SPELL34                   =    23     ,   //  atk3_4
    SKILL_SPELLACTION_SPE_DEATH                 =    27     ,   //  spe_death
    SKILL_SPELLACTION_BACKHOME                  =    29     ,   //  backhome
};

enum E_SKILLTABLE_DATACONTENTA
{
    SKILL_DATACONTENTA_MAXHP                    =    8      ,   //  生命上限
    SKILL_DATACONTENTA_MAXMP                    =    9      ,   //  法力上限
    SKILL_DATACONTENTA_ATTACK                   =    16     ,   //  攻击
    SKILL_DATACONTENTA_AMOR                     =    14     ,   //  护甲
    SKILL_DATACONTENTA_PERAMOR                  =    15     ,   //  护甲百分比
    SKILL_DATACONTENTA_PERATTACK                =    18     ,   //  攻击百分比
    SKILL_DATACONTENTA_MOVESPEED                =    21     ,   //  移动速度
    SKILL_DATACONTENTA_PERATTACKSPEED            =    19     ,   //  攻击速度百分比
    SKILL_DATACONTENTA_PERMOVE                  =    22     ,   //  移动速度百分比
    SKILL_DATACONTENTA_FASHANG                  =    28     ,   //  法术加成
    SKILL_DATACONTENTA_FABAO                    =    30     ,   //  法爆比例
    SKILL_DATACONTENTA_FABAORATE                =    29     ,   //  法爆概率
    SKILL_DATACONTENTA_MAGICKANG                =    32     ,   //  魔抗
    SKILL_DATACONTENTA_FASHUCHUANTOU            =    33     ,   //  法术穿透
    SKILL_DATACONTENTA_XIXUE                    =    34     ,   //  吸血
    SKILL_DATACONTENTA_JINQIAN                  =    35     ,   //  初始金钱比例
    SKILL_DATACONTENTA_HAOMOJIANSHAO            =    37     ,   //  减少耗魔
    SKILL_DATACONTENTA_JIANSHAOCD               =    38     ,   //  减少CD
    SKILL_DATACONTENTA_JIANSHAODAMAGE            =    40     ,   //  伤害减免
    SKILL_DATACONTENTA_SHANBI                   =    41     ,   //  闪避
    SKILL_DATACONTENTA_ZHANDOULI                =    51     ,   //  战斗力比例
    SKILL_DATACONTENTA_DIAOLUO                  =    50     ,   //  掉落比例
    SKILL_DATACONTENTA_HUIFUHP                  =    11     ,   //  生命回复百分比
    SKILL_DATACONTENTA_SHUZHI                   =    1000   ,   //  数值
    SKILL_DATACONTENTA_BAIFENBI                 =    1001   ,   //  百分比
    SKILL_DATACONTENTA_SHIJIAN                  =    1002   ,   //  时间
    SKILL_DATACONTENTA_JULI                     =    1003   ,   //  距离
    SKILL_DATACONTENTA_FANWEI                   =    1004   ,   //  范围
    SKILL_DATACONTENTA_RELIVEHP                 =    10     ,   //  生命回复
    SKILL_DATACONTENTA_RELIVEMP                 =    12     ,   //  法力回复
    SKILL_DATACONTENTA_HUIFUMP                  =    13     ,   //  法力回复百分比
    SKILL_DATACONTENTA_ATTACKSPEED              =    58     ,   //  攻击速度
    SKILL_DATACONTENTA_PERCURESKILL             =    45     ,   //  增加技能治疗
    SKILL_DATACONTENTA_WULICHUANTOU             =    60     ,   //  物理穿透
    SKILL_DATACONTENTA_HUSHIHUJIA               =    36     ,   //  忽视护甲
    SKILL_DATACONTENTA_WUBAORATE                =    61     ,   //  暴击概率
    SKILL_DATACONTENTA_HUSHIKANGXING            =    32     ,   //  忽视抗性
    SKILL_DATACONTENTA_TREATMENT                =    45     ,   //  增强治疗效果
    SKILL_DATACONTENTA_PERMAGIC                 =    64     ,   //  法伤百分比
    SKILL_DATACONTENTA_TOUGHNESS                =    65     ,   //  韧性
    SKILL_DATACONTENTA_MAGICLEECH               =    66     ,   //  法术吸血
    SKILL_DATACONTENTA_DECDAMAGE                =    63     ,   //  伤害抵挡
    SKILL_DATACONTENTA_DECPERMAGICARMOR            =    57     ,   //  法术穿透百分比
    SKILL_DATACONTENTA_BAOJIPER                 =    69     ,   //  暴击倍数
    SKILL_DATACONTENTA_BAOJIDEC                 =    70     ,   //  降低暴击倍数
};

enum E_SKILLTABLE_AADJ
{
    SKILL_AADJ_MAXHP                            =    1      ,   //  生命上限
    SKILL_AADJ_CURHP                            =    2      ,   //  当前生命
    SKILL_AADJ_MAXMP                            =    3      ,   //  法力上限
    SKILL_AADJ_CURMP                            =    4      ,   //  当前法力
    SKILL_AADJ_ATTACK                           =    5      ,   //  攻击
    SKILL_AADJ_FASHANG                          =    6      ,   //  法术加成
    SKILL_AADJ_HEROLEVEL                        =    7      ,   //  英雄等级
    SKILL_AADJ_MOVESPEED                        =    8      ,   //  移动速度
    SKILL_AADJ_ADDATTACK                        =    9      ,   //  额外攻击力
    SKILL_AADJ_ADDARMOR                         =    10     ,   //  额外护甲
    SKILL_AADJ_ADDHP                            =    11     ,   //  额外生命值
    SKILL_AADJ_ADDMP                            =    12     ,   //  额外法力值
    SKILL_AADJ_ADDMOVESPEED                     =    13     ,   //  额外移动速度
};


enum M_SKILLTABLE_TARGETTYPE
{
    SKILL_TARGETTYPE_HERO                       =    1 << 0        ,   //  英雄
    SKILL_TARGETTYPE_SOLIDER                    =    1 << 1        ,   //  士兵
    SKILL_TARGETTYPE_TOUSHI                     =    1 << 2        ,   //  投石车
    SKILL_TARGETTYPE_BUILDING                   =    1 << 3        ,   //  建筑
    SKILL_TARGETTYPE_YEGUAI                     =    1 << 4        ,   //  野怪
    SKILL_TARGETTYPE_YUANGU                     =    1 << 5        ,   //  大型野怪
    SKILL_TARGETTYPE_BOSS                       =    1 << 6        ,   //  BOSS
    SKILL_TARGETTYPE_DIJUN                      =    1 << 7        ,   //  敌军
    SKILL_TARGETTYPE_YOUJUN                     =    1 << 8        ,   //  友军
    SKILL_TARGETTYPE_SELF                       =    1 << 9        ,   //  自己
    SKILL_TARGETTYPE_MOMIAN                     =    1 << 10       ,   //  魔免
    SKILL_TARGETTYPE_WUDI                       =    1 << 11       ,   //  无敌
    SKILL_TARGETTYPE_ZHONGLI                    =    1 << 12       ,   //  中立
    SKILL_TARGETTYPE_YAN                        =    1 << 13       ,   //  眼
    SKILL_TARGETTYPE_TOTEM                      =    1 << 14       ,   //  图腾
    SKILL_TARGETTYPE_BRAND                      =    1 << 15       ,   //  印记
    SKILL_TARGETTYPE_PET                        =    1 << 16       ,   //  宠物
    SKILL_TARGETTYPE_MJ                         =    1 << 17       ,   //  秘境怪物
};

enum M_SKILLTABLE_SKILLPROPERTY
{
    SKILL_SKILLPROPERTY_NOMOMIAN                =    1 << 1        ,   //  无视魔免
    SKILL_SKILLPROPERTY_GUANGHUAN               =    1 << 2        ,   //  光环技能
    SKILL_SKILLPROPERTY_ZIDONGCHIXU             =    1 << 3        ,   //  自动持续
    SKILL_SKILLPROPERTY_WEIYI                   =    1 << 4        ,   //  技能唯一
    SKILL_SKILLPROPERTY_KEDADUAN                =    1 << 5        ,   //  可打断前摇
    SKILL_SKILLPROPERTY_CHIXUSHIFA              =    1 << 6        ,   //  持续施法
    SKILL_SKILLPROPERTY_BEIJICHUFA              =    1 << 7        ,   //  被击触发
    SKILL_SKILLPROPERTY_GONGJICHUFA             =    1 << 8        ,   //  攻击触发
    SKILL_SKILLPROPERTY_SHANGHAICHUFA            =    1 << 9        ,   //  伤害触发
    SKILL_SKILLPROPERTY_KILLCHUFA               =    1 << 10       ,   //  击杀触发
    SKILL_SKILLPROPERTY_DEADCHUFA               =    1 << 11       ,   //  死亡触发
    SKILL_SKILLPROPERTY_CDCHUFA                 =    1 << 12       ,   //  CD触发
    SKILL_SKILLPROPERTY_YANSHENGJINENG            =    1 << 13       ,   //  多段施法
    SKILL_SKILLPROPERTY_XULISHIFA               =    1 << 14       ,   //  蓄力施法
    SKILL_SKILLPROPERTY_ONSPELLFIRE             =    1 << 15       ,   //  施法触发
    SKILL_SKILLPROPERTY_BIANJIQI                =    1 << 16       ,   //  编辑器技能
    SKILL_SKILLPROPERTY_CASTCMD                 =    1 << 17       ,   //  不打断当前指令
    SKILL_SKILLPROPERTY_SENDBAR                 =    1 << 18       ,   //  发送读条
    SKILL_SKILLPROPERTY_DEATHRELIVE             =    1 << 19       ,   //  死亡复活
    SKILL_SKILLPROPERTY_NOTSHOWCD               =    1 << 20       ,   //  不显示CD
    SKILL_SKILLPROPERTY_NOTDECCD                =    1 << 21       ,   //  不减CD
    SKILL_SKILLPROPERTY_CHARGE                  =    1 << 22       ,   //  充能
    SKILL_SKILLPROPERTY_DISPLACE                =    1 << 23       ,   //  位移
    SKILL_SKILLPROPERTY_USEMAXHP                =    1 << 24       ,   //  消耗最大生命
    SKILL_SKILLPROPERTY_USECURRENTHP            =    1 << 25       ,   //  消耗当前生命
    SKILL_SKILLPROPERTY_USEHP                   =    1 << 26       ,   //  消耗固定生命
    SKILL_SKILLPROPERTY_XIADI                   =    1 << 27       ,   //  下地施法
    SKILL_SKILLPROPERTY_NOJULI                  =    1 << 28       ,   //  无限距
    SKILL_SKILLPROPERTY_SHOUSUO                 =    1 << 29       ,   //  线性缩放
    SKILL_SKILLPROPERTY_DAMAGEDVISION            =    1 << 30       ,   //  被击视野
    SKILL_SKILLPROPERTY_QUANTU                  =    1 << 31       ,   //  全图指示
    SKILL_SKILLPROPERTY_XIAODITU                =    1 << 32       ,   //  小地图指示
    SKILL_SKILLPROPERTY_XULI                    =    1 << 32       ,   //  蓄力
};

//  自动生成表结构
struct SkillTable : SeTableResBase
{
    unsigned int               uiTypeID                            ;   //  技能ID   
    std::string                kName                               ;   //  技能名称   
    int                        uiOperateType                       ;   //  施法类型   
    unsigned int               uiOriginTypeID                      ;   //  原始技能ID   
    int                        iUIShowPos                          ;   //  界面位置   
    int                        uiAfterActionType                   ;   //  技能施放后的动作   
    unsigned int               uiTargetType                        ;   //  目标类型   
    int                        uiAttackType                        ;   //  伤害类型   
    unsigned int               uiSkillProperty                     ;   //  技能特性   
    int                        iSkillMaxLevel                      ;   //  等级上限    
    int                        uiSpellAction                       ;   //  施法动作   
    int                        iLearnLevel[5]                          ;   //  学习等级   
    int                        iCastTime[5]                          ;   //  前置时间   
    int                        iShakesTime[5]                          ;   //  后摇时间   
    int                        iEffectTime[5]                          ;   //  施法时间   
    int                        iCoolDown[5]                          ;   //  冷却时间   
    int                        iEffectScope[5]                          ;   //  作用范围   
    int                        iSkillDistance[5]                          ;   //  施法距离   
    int                        iSkillAngle[5]                          ;   //  扇形角度   
    int                        iPilePointNums[5]                          ;   //  累积次数   
    float                      fHpSpend[5]                          ;   //  生命消耗   
    float                      fManaSpend[5]                          ;   //  法术消耗   
    int                        uiDataContentA                      ;   //  类型A   
    float                      fValueA[5]                          ;   //  数值A   
    unsigned int               uiAAdj                              ;   //  伤害加成A   
    float                      fAAdj                               ;   //  伤害加成A   
    int                        uiDataContentB                      ;   //  类型B   
    float                      fValueB[5]                          ;   //  数值B   
    unsigned int               uiBAdj                              ;   //  伤害加成B   
    float                      fBAdj                               ;   //  伤害加成B   
    int                        uiDataContentC                      ;   //  类型C   
    float                      fValueC[5]                          ;   //  数值C   
    unsigned int               uiCAdj                              ;   //  伤害加成C   
    float                      fCAdj                               ;   //  伤害加成C   
    int                        uiDataContentD                      ;   //  类型D   
    float                      fValueD[5]                          ;   //  数值D   
    unsigned int               uiDAdj                              ;   //  伤害加成D   
    float                      fDAdj                               ;   //  伤害加成D   
    int                        uiDataContentE                      ;   //  类型E   
    float                      fValueE[5]                          ;   //  数值E   
    unsigned int               uiEAdj                              ;   //  伤害加成E   
    float                      fEAdj                               ;   //  伤害加成E   
    int                        uiDataContentF                      ;   //  类型F   
    float                      fValueF[5]                          ;   //  数值F   
    unsigned int               uiFAdj                              ;   //  伤害加成F   
    float                      fFAdj                               ;   //  伤害加成F   
    int                        uiDataContentG                      ;   //  类型G   
    float                      fValueG[5]                          ;   //  数值G   
    unsigned int               uiGAdj                              ;   //  伤害加成G   
    float                      fGAdj                               ;   //  伤害加成G   
    int                        iMissileModel                       ;   //  投射模型   
    float                      fMissileSpeed                       ;   //  投射速度   
    float                      fMissileArc                         ;   //  投射弧度   
    int                        iSrcEffect                          ;   //  自身特效   
    int                        iDstPosEffect                       ;   //  指定点特效   
    int                        iDstUnitEffect                      ;   //  目标特效   
    int                        iSkillEffect                        ;   //  技能特效   
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
    TableResArray GetSkillTableVec();

private:
    bool    Load();
    void    FillData(SkillTable* row, TiXmlElement* element);

    std::map<unsigned int,SkillTable*>      m_kSkillTableMap;
    static SkillTableMgr* m_pkSkillTableMgr;
};
