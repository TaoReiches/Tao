/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "MapInfo_table.hpp"
#include "tinyxml.h"
#include <fstream>

std::unique_ptr<MapInfoTableMgr>       MapInfoTableMgr::m_pkMapInfoTableMgr = nullptr;
const std::unique_ptr<MapInfoTableMgr>& MapInfoTableMgr::Get()
{
    if(!m_pkMapInfoTableMgr)
    {
        m_pkMapInfoTableMgr = std::unique_ptr<MapInfoTableMgr>(new MapInfoTableMgr());
    }
    return m_pkMapInfoTableMgr;
}


const std::map<unsigned int, std::shared_ptr<const MapInfoTable>>& MapInfoTableMgr::GetMapInfoTableMap() const
{
    return m_kMapInfoTableMap;
}

const std::shared_ptr<const MapInfoTable>& MapInfoTableMgr::GetMapInfoTable(unsigned int iTypeID) const
{
    auto iter = m_kMapInfoTableMap.find(iTypeID);
    if(iter != m_kMapInfoTableMap.end())
    {
        return iter->second;
    }
    static auto nullValue = std::shared_ptr<const MapInfoTable>();
    return nullValue;
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
        MapInfoTable * row = new MapInfoTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    delete[] fileData;
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
    element->Attribute("Type", &int_value);
    row->uiType = (unsigned int)int_value;
    element->Attribute("Property", &int_value);
    row->uiProperty = (unsigned int)int_value;

    m_kMapInfoTableMap[row->uiMapTypeID] = std::shared_ptr<MapInfoTable>(row);
}
