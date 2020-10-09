#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>
#include "SeTableResBase.h"



enum M_ITEMTABLE_ITEMPROPERTY
{
    ITEM_ITEMPROPERTY_CANUSE                    =    1 << 1        ,   //  可使用
    ITEM_ITEMPROPERTY_USEDEL                    =    1 << 2        ,   //  用完消失
    ITEM_ITEMPROPERTY_PICKUSE                   =    1 << 3        ,   //  拾取生效
    ITEM_ITEMPROPERTY_ONLYONE                   =    1 << 4        ,   //  装备唯一
};

//  自动生成表结构
struct ItemTable : SeTableResBase
{
    unsigned int               uiItemTypeID                        ;   //  ID   
    std::string                kName                               ;   //  物品名字   
    int                        iItemSkill[6]                          ;   //  物品技能   
    int                        iItemBuffer[3]                          ;   //  物品出售时删除buffer   
    unsigned int               uiItemProperty                      ;   //  物品特性   
    int                        iItemPrice                          ;   //  物品价格   
    int                        iOrgPileCount                       ;   //  初始叠加   
    int                        iOrgUseCount                        ;   //  使用次数   
    int                        iPileCount                          ;   //  可叠加次数   
};

class TiXmlElement;
class ItemTableMgr
{
public:
    static ItemTableMgr* Get();
    ItemTableMgr();
    ~ItemTableMgr();

    const ItemTable* GetItemTable(unsigned int iTypeID);
    const std::map<unsigned int, ItemTable*>& GetItemTableMap();
    TableResArray GetItemTableVec();

private:
    bool    Load();
    void    FillData(ItemTable* row, TiXmlElement* element);

    std::map<unsigned int,ItemTable*>      m_kItemTableMap;
    static ItemTableMgr* m_pkItemTableMgr;
};
