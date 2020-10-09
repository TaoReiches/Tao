#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>


enum E_SKILLTABLE_OPERATETYPE
{
    SKILL_OPERATETYPE_LIJISHIFA                 =    0      ,   //  立即施法
    SKILL_OPERATETYPE_BEIDONG                   =    1      ,   //  被动技能
    SKILL_OPERATETYPE_XIANXING                  =    2      ,   //  线性施法
    SKILL_OPERATETYPE_ANZHU                     =    3      ,   //  按住施法
    SKILL_OPERATETYPE_HUADONG                   =    4      ,   //  滑动施法
    SKILL_OPERATETYPE_KAIGUAN                   =    5      ,   //  开关施法
    SKILL_OPERATETYPE_DANTI                     =    6      ,   //  单体指向
    SKILL_OPERATETYPE_QUYU                      =    7      ,   //  区域指向
};

enum E_SKILLTABLE_ATTACKTYPE
{
    SKILL_ATTACKTYPE_NONE                       =    0      ,   //  无
    SKILL_ATTACKTYPE_PUTONG                     =    1      ,   //  普通
    SKILL_ATTACKTYPE_CHUANCI                    =    2      ,   //  穿刺
    SKILL_ATTACKTYPE_MOFA                       =    3      ,   //  魔法
    SKILL_ATTACKTYPE_HERO                       =    4      ,   //  英雄
    SKILL_ATTACKTYPE_GONGCHENG                  =    5      ,   //  攻城
    SKILL_ATTACKTYPE_HUNLUAN                    =    6      ,   //  混乱
    SKILL_ATTACKTYPE_SKILL                      =    7      ,   //  技能
    SKILL_ATTACKTYPE_JUEDUI                     =    8      ,   //  绝对
};

enum E_SKILLTABLE_DATACONTENTA
{
    SKILL_DATACONTENTA_ALLATTR                  =    1      ,   //  全属性
    SKILL_DATACONTENTA_MAINATTR                 =    2      ,   //  主属性
    SKILL_DATACONTENTA_LILIANG                  =    3      ,   //  力量
    SKILL_DATACONTENTA_MINJIE                   =    4      ,   //  敏捷
    SKILL_DATACONTENTA_ZHILI                    =    5      ,   //  智力
    SKILL_DATACONTENTA_MAXHP                    =    6      ,   //  最大生命值
    SKILL_DATACONTENTA_MAXMP                    =    7      ,   //  最大法力值
    SKILL_DATACONTENTA_ATTACK                   =    8      ,   //  攻击
    SKILL_DATACONTENTA_AMOR                     =    9      ,   //  护甲
    SKILL_DATACONTENTA_PERATTACK                =    10     ,   //  攻击比例
    SKILL_DATACONTENTA_MOVESPEED                =    11     ,   //  移动速度
    SKILL_DATACONTENTA_ATTACKSPEED              =    12     ,   //  攻击速度比例
    SKILL_DATACONTENTA_PERMOVE                  =    13     ,   //  移动速度比例
    SKILL_DATACONTENTA_FASHANG                  =    14     ,   //  法伤比例
    SKILL_DATACONTENTA_FABAO                    =    15     ,   //  法爆比例
    SKILL_DATACONTENTA_FABAORATE                =    16     ,   //  法爆概率
    SKILL_DATACONTENTA_MAGICKANG                =    17     ,   //  法术抗性
    SKILL_DATACONTENTA_FASHUCHUANTOU            =    18     ,   //  法术穿透
    SKILL_DATACONTENTA_XIXUE                    =    19     ,   //  吸血
    SKILL_DATACONTENTA_JINQIAN                  =    20     ,   //  初始金钱比例
    SKILL_DATACONTENTA_HAOMOJIANSHAO            =    21     ,   //  减少耗魔
    SKILL_DATACONTENTA_JIANSHAOCD               =    22     ,   //  减少冷却
    SKILL_DATACONTENTA_JIANSHAODAMAGE            =    23     ,   //  伤害减免
    SKILL_DATACONTENTA_SHANBI                   =    24     ,   //  闪避
    SKILL_DATACONTENTA_ZHANDOULI                =    25     ,   //  战斗力比例
    SKILL_DATACONTENTA_DIAOLUO                  =    26     ,   //  掉落比例
    SKILL_DATACONTENTA_HUIFUHP                  =    27     ,   //  生命回复百分比
    SKILL_DATACONTENTA_SHUZHI                   =    28     ,   //  数值
    SKILL_DATACONTENTA_BAIFENBI                 =    29     ,   //  百分比
    SKILL_DATACONTENTA_SHIJIAN                  =    30     ,   //  时间
    SKILL_DATACONTENTA_JULI                     =    31     ,   //  距离
    SKILL_DATACONTENTA_FANWEI                   =    32     ,   //  范围
};


enum M_SKILLTABLE_TARGETTYPE
{
    SKILL_TARGETTYPE_HERO                       =    1 << 0        ,   //  英雄
    SKILL_TARGETTYPE_SOLIDER                    =    1 << 1        ,   //  士兵
    SKILL_TARGETTYPE_TOUSHI                     =    1 << 2        ,   //  投石车
    SKILL_TARGETTYPE_BUILDING                   =    1 << 3        ,   //  建筑
    SKILL_TARGETTYPE_YEGUAI                     =    1 << 4        ,   //  野怪
    SKILL_TARGETTYPE_BOSS                       =    1 << 5        ,   //  BOSS
    SKILL_TARGETTYPE_YUANGU                     =    1 << 6        ,   //  远古
    SKILL_TARGETTYPE_DIJUN                      =    1 << 7        ,   //  敌军
    SKILL_TARGETTYPE_YOUJUN                     =    1 << 8        ,   //  友军
    SKILL_TARGETTYPE_SELF                       =    1 << 9        ,   //  自己
    SKILL_TARGETTYPE_MOMIAN                     =    1 << 10       ,   //  魔免
    SKILL_TARGETTYPE_WUDI                       =    1 << 11       ,   //  无敌
};

enum M_SKILLTABLE_SKILLPROPERTY
{
    SKILL_SKILLPROPERTY_NOMOMIAN                =    1 << 1        ,   //  无视魔免
    SKILL_SKILLPROPERTY_GUANGHUAN               =    1 << 2        ,   //  光环技能
    SKILL_SKILLPROPERTY_ZIDONGCHIXU             =    1 << 3        ,   //  自动持续
    SKILL_SKILLPROPERTY_WEIYI                   =    1 << 4        ,   //  技能唯一
    SKILL_SKILLPROPERTY_BUKEDADUAN              =    1 << 5        ,   //  不可打断
    SKILL_SKILLPROPERTY_CHIXUSHIFA              =    1 << 6        ,   //  持续施法
    SKILL_SKILLPROPERTY_BEIJICHUFA              =    1 << 7        ,   //  被击触发
    SKILL_SKILLPROPERTY_GONGJICHUFA             =    1 << 8        ,   //  攻击触发
    SKILL_SKILLPROPERTY_SHANGHAICHUFA            =    1 << 9        ,   //  伤害触发
    SKILL_SKILLPROPERTY_KILLCHUFA               =    1 << 10       ,   //  击杀触发
    SKILL_SKILLPROPERTY_DEADCHUFA               =    1 << 11       ,   //  死亡触发
    SKILL_SKILLPROPERTY_CDCHUFA                 =    1 << 12       ,   //  CD触发
    SKILL_SKILLPROPERTY_YANSHENGJINENG            =    1 << 13       ,   //  衍生技能
};

//  自动生成表结构
struct SkillTable
{
    unsigned int               TypeID                   ;   
    std::string                Name                     ;   
    std::string                Desc                     ;   
    int                        OperateType              ;   
    unsigned int               OriginTypeID             ;   
    int                        UIShowPos                ;   
    unsigned int               TargetType               ;   
    int                        AttackType               ;   
    unsigned int               SkillProperty            ;   
    unsigned int               BufferID                 ;   
    int                        SkillMaxLevel            ;   
    std::string                SpellAction              ;   
    int                        LearnLevel               ;   
    int                        CastTime                 ;   
    int                        EffectTime               ;   
    int                        CoolDown                 ;   
    int                        EffectScope              ;   
    int                        SkillDistance            ;   
    int                        PilePointNums            ;   
    int                        ManaSpend                ;   
    int                        DataContentA             ;   
    float                      ValueA                   ;   
    float                      AAdj                     ;   
    int                        DataContentB             ;   
    float                      ValueB                   ;   
    float                      BAdj                     ;   
    int                        DataContentC             ;   
    float                      ValueC                   ;   
    float                      CAdj                     ;   
    int                        DataContentD             ;   
    float                      ValueD                   ;   
    float                      DAdj                     ;   
    int                        DataContentE             ;   
    float                      ValueE                   ;   
    float                      EAdj                     ;   
    int                        DataContentF             ;   
    float                      ValueF                   ;   
    float                      FAdj                     ;   
    std::string                MissileModel             ;   
    float                      MissileSpeed             ;   
    float                      MissileArc               ;   
    std::string                SrcEffect                ;   
    std::string                DstPosEffect             ;   
    std::string                DstUnitEffect            ;   
    std::string                SkillEffect              ;   
    std::string                LearnIcon                ;   
    std::string                PrepareSound             ;   
    std::string                EffectSound              ;   
    std::string                SpecialSound             ;   
    float                      FightScore               ;   
    std::string                SimpleDesc               ;   
};

class TiXmlElement;
class SkillTableMgr
{
public:
    static SkillTableMgr* Get();
    SkillTableMgr();
    ~SkillTableMgr();

    SkillTable* GetSkillTable(unsigned int iTypeID);

private:
    bool    Load();
    void    FillData(SkillTable* row, TiXmlElement* element);

    std::map<unsigned int,SkillTable*>      m_kSkillTableMap;
    static SkillTableMgr* m_pkSkillTableMgr;
};
