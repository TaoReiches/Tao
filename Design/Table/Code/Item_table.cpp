#include "Item_table.hpp"
#include "tinyxml.h"
#include "EeFileMemory.h"
#include "EeFilePackage.h"

ItemTableMgr* ItemTableMgr::m_pkItemTableMgr = NULL;
ItemTableMgr* ItemTableMgr::Get()
{
    if(m_pkItemTableMgr == NULL)
    {
        m_pkItemTableMgr = new ItemTableMgr();
    }
    return m_pkItemTableMgr;
}


const std::map<unsigned int, ItemTable*>& ItemTableMgr::GetItemTableMap()
{
    return m_kItemTableMap;
}

TableResArray ItemTableMgr::GetItemTableVec()
{
    TableResArray kRecVec;
    for (std::map<unsigned int, ItemTable*>::iterator iMapItr = m_kItemTableMap.begin(); iMapItr != m_kItemTableMap.end(); ++iMapItr)
    {
        kRecVec.pushBack(iMapItr->second);
    }

    return kRecVec;
}

const ItemTable* ItemTableMgr::GetItemTable(unsigned int iTypeID)
{
    std::map<unsigned int, ItemTable*>::iterator iter = m_kItemTableMap.find(iTypeID);
    if(iter != m_kItemTableMap.end())
    {
        return iter->second;
    }
    return NULL;
}

ItemTableMgr::ItemTableMgr()
{
    Load();
}

ItemTableMgr::~ItemTableMgr()
{
    m_kItemTableMap.clear();
}

bool ItemTableMgr::Load()
{
    std::string path = "Data/Table/Item.xml";
    FileMemory kMemory;
    if(!FileLoader::LoadTableFile(path.c_str(),kMemory))
    {
        return false;
    }

    TiXmlDocument doc;
    doc.Parse(kMemory.GetData());
    if (doc.Error())
    {
        std::string err = path + "   " + std::string(doc.ErrorDesc());
        // throw std::exception(err.c_str());
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("root");
    if (root == 0)
    {
        // throw std::exception("root is null!");
        return false;
    }

    TiXmlElement* element = root->FirstChildElement("content");
    while (element != 0)
    {
        ItemTable * row = new ItemTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    return true;
}

void ItemTableMgr::FillData(ItemTable* row, TiXmlElement* element)
{
    int int_value = 0;
    std::string str_value = "";

    element->Attribute("ItemTypeID", &int_value);
    row->uiItemTypeID = (unsigned int)int_value;
    str_value = element->Attribute("Name");
    row->kName = str_value;
    element->Attribute("ItemSkill0", &int_value);
    row->iItemSkill[0] = int_value;
    element->Attribute("ItemSkill1", &int_value);
    row->iItemSkill[1] = int_value;
    element->Attribute("ItemSkill2", &int_value);
    row->iItemSkill[2] = int_value;
    element->Attribute("ItemSkill3", &int_value);
    row->iItemSkill[3] = int_value;
    element->Attribute("ItemSkill4", &int_value);
    row->iItemSkill[4] = int_value;
    element->Attribute("ItemSkill5", &int_value);
    row->iItemSkill[5] = int_value;
    element->Attribute("ItemBuffer0", &int_value);
    row->iItemBuffer[0] = int_value;
    element->Attribute("ItemBuffer1", &int_value);
    row->iItemBuffer[1] = int_value;
    element->Attribute("ItemBuffer2", &int_value);
    row->iItemBuffer[2] = int_value;
    element->Attribute("ItemProperty", &int_value);
    row->uiItemProperty = (unsigned int)int_value;
    element->Attribute("ItemPrice", &int_value);
    row->iItemPrice = int_value;
    element->Attribute("OrgPileCount", &int_value);
    row->iOrgPileCount = int_value;
    element->Attribute("OrgUseCount", &int_value);
    row->iOrgUseCount = int_value;
    element->Attribute("PileCount", &int_value);
    row->iPileCount = int_value;

    m_kItemTableMap[row->uiItemTypeID] = row;
}
