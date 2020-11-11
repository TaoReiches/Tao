#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>



enum M_ITEMTABLE_ITEMPROPERTY
{
    ITEM_ITEMPROPERTY_CANUSE                    =    1 << 1        ,   //  CanUse
    ITEM_ITEMPROPERTY_USEDEL                    =    1 << 2        ,   //  AutoDel
    ITEM_ITEMPROPERTY_ONLYONE                   =    1 << 3        ,   //  OnlyOne
};

struct ItemTable
{
    unsigned int               uiItemTypeID                        ;   //  TypeID
    std::string                kName                               ;   //  ItemName
    int                        iItemSkill[6]                          ;   //  ItemSkills
    unsigned int               uiItemProperty                      ;   //  Attr
    unsigned int               uiItemPrice                         ;   //  Price
    unsigned int               uiOrgPileCount                      ;   //  Original pile count
    unsigned int               uiOrgUseCount                       ;   //  Use count
    unsigned int               uiPileCount                         ;   //  Pile count
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

private:
    bool    Load();
    void    FillData(ItemTable* row, TiXmlElement* element);

    std::map<unsigned int,ItemTable*>      m_kItemTableMap;
    static ItemTableMgr* m_pkItemTableMgr;
};
