#include "MapInfo_table.hpp"
#include "tinyxml.h"
#include "EeFileMemory.h"
#include "EeFilePackage.h"

MapInfoTableMgr* MapInfoTableMgr::m_pkMapInfoTableMgr = NULL;
MapInfoTableMgr* MapInfoTableMgr::Get()
{
    if(m_pkMapInfoTableMgr == NULL)
    {
        m_pkMapInfoTableMgr = new MapInfoTableMgr();
    }
    return m_pkMapInfoTableMgr;
}


const std::map<unsigned int, MapInfoTable*>& MapInfoTableMgr::GetMapInfoTableMap()
{
    return m_kMapInfoTableMap;
}

TableResArray MapInfoTableMgr::GetMapInfoTableVec()
{
    TableResArray kRecVec;
    for (std::map<unsigned int, MapInfoTable*>::iterator iMapItr = m_kMapInfoTableMap.begin(); iMapItr != m_kMapInfoTableMap.end(); ++iMapItr)
    {
        kRecVec.pushBack(iMapItr->second);
    }

    return kRecVec;
}

const MapInfoTable* MapInfoTableMgr::GetMapInfoTable(unsigned int iTypeID)
{
    std::map<unsigned int, MapInfoTable*>::iterator iter = m_kMapInfoTableMap.find(iTypeID);
    if(iter != m_kMapInfoTableMap.end())
    {
        return iter->second;
    }
    return NULL;
}

MapInfoTableMgr::MapInfoTableMgr()
{
    Load();
}

MapInfoTableMgr::~MapInfoTableMgr()
{
    m_kMapInfoTableMap.clear();
}

bool MapInfoTableMgr::Load()
{
    std::string path = "Data/Table/MapInfo.xml";
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
        MapInfoTable * row = new MapInfoTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    return true;
}

void MapInfoTableMgr::FillData(MapInfoTable* row, TiXmlElement* element)
{
    int int_value = 0;
    std::string str_value = "";

    element->Attribute("MapTypeID", &int_value);
    row->uiMapTypeID = (unsigned int)int_value;
    str_value = element->Attribute("Name");
    row->kName = str_value;
    str_value = element->Attribute("resId");
    row->kresId = str_value;
    element->Attribute("Type", &int_value);
    row->uiType = (unsigned int)int_value;
    element->Attribute("Property", &int_value);
    row->uiProperty = (unsigned int)int_value;

    m_kMapInfoTableMap[row->uiMapTypeID] = row;
}
