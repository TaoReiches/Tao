#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <map>
#include <memory>



enum M_ITEMTABLE_ITEMPROPERTY
{
    ITEM_ITEMPROPERTY_CANUSE                    =    1 << 1        ,   //  CanUse
    ITEM_ITEMPROPERTY_USEDEL                    =    1 << 2        ,   //  AutoDel
    ITEM_ITEMPROPERTY_ONLYONE                   =    1 << 3        ,   //  OnlyOne
};

class ItemTable
{
public:
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
    static const std::unique_ptr<ItemTableMgr>& Get();
    ItemTableMgr();
    ~ItemTableMgr();

    const std::shared_ptr<const ItemTable>& GetItemTable(unsigned int iTypeID) const;
    const std::map<unsigned int, std::shared_ptr<const ItemTable>>& GetItemTableMap() const;

private:
    bool    Load();
    void    FillData(ItemTable* row, TiXmlElement* element);

    std::map<unsigned int,std::shared_ptr<const ItemTable>>      m_kItemTableMap;
    static std::unique_ptr<ItemTableMgr>        m_pkItemTableMgr;
};
