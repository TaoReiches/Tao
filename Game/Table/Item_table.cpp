#include "Item_table.hpp"
#include "tinyxml.h"
#include <fstream>

std::unique_ptr<ItemTableMgr>       ItemTableMgr::m_pkItemTableMgr = nullptr;
const std::unique_ptr<ItemTableMgr>& ItemTableMgr::Get()
{
    if(!m_pkItemTableMgr)
    {
        m_pkItemTableMgr = std::unique_ptr<ItemTableMgr>(new ItemTableMgr());
    }
    return m_pkItemTableMgr;
}


const std::map<unsigned int, std::shared_ptr<const ItemTable>>& ItemTableMgr::GetItemTableMap() const
{
    return m_kItemTableMap;
}

const std::shared_ptr<const ItemTable>& ItemTableMgr::GetItemTable(unsigned int iTypeID) const
{
    auto iter = m_kItemTableMap.find(iTypeID);
    if(iter != m_kItemTableMap.end())
    {
        return iter->second;
    }
    static auto nullValue = std::shared_ptr<const ItemTable>();
    return nullValue;
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
    char* fileData = nullptr;
    std::ifstream file (path, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        std::streampos size = file.tellg();
        fileData = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(fileData, size);
        file.close();
    }
    if (fileData == nullptr)
    {
        return false;
    }
    TiXmlDocument doc;
    doc.Parse(fileData);
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

    delete[] fileData;
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
    element->Attribute("ItemProperty", &int_value);
    row->uiItemProperty = (unsigned int)int_value;
    element->Attribute("ItemPrice", &int_value);
    row->uiItemPrice = (unsigned int)int_value;
    element->Attribute("OrgPileCount", &int_value);
    row->uiOrgPileCount = (unsigned int)int_value;
    element->Attribute("OrgUseCount", &int_value);
    row->uiOrgUseCount = (unsigned int)int_value;
    element->Attribute("PileCount", &int_value);
    row->uiPileCount = (unsigned int)int_value;

    m_kItemTableMap[row->uiItemTypeID] = std::shared_ptr<ItemTable>(row);
}
