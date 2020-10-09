#include "Buffer_table.hpp"
#include "tinyxml.h"
#include "EeFileMemory.h"
#include "EeFilePackage.h"

BufferTableMgr* BufferTableMgr::m_pkBufferTableMgr = NULL;
BufferTableMgr* BufferTableMgr::Get()
{
    if(m_pkBufferTableMgr == NULL)
    {
        m_pkBufferTableMgr = new BufferTableMgr();
    }
    return m_pkBufferTableMgr;
}


const std::map<unsigned int, BufferTable*>& BufferTableMgr::GetBufferTableMap()
{
    return m_kBufferTableMap;
}

TableResArray BufferTableMgr::GetBufferTableVec()
{
    TableResArray kRecVec;
    for (std::map<unsigned int, BufferTable*>::iterator iMapItr = m_kBufferTableMap.begin(); iMapItr != m_kBufferTableMap.end(); ++iMapItr)
    {
        kRecVec.pushBack(iMapItr->second);
    }

    return kRecVec;
}

const BufferTable* BufferTableMgr::GetBufferTable(unsigned int iTypeID)
{
    std::map<unsigned int, BufferTable*>::iterator iter = m_kBufferTableMap.find(iTypeID);
    if(iter != m_kBufferTableMap.end())
    {
        return iter->second;
    }
    return NULL;
}

BufferTableMgr::BufferTableMgr()
{
    Load();
}

BufferTableMgr::~BufferTableMgr()
{
    m_kBufferTableMap.clear();
}

bool BufferTableMgr::Load()
{
    std::string path = "Data/Table/Buffer.xml";
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
        BufferTable * row = new BufferTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    return true;
}

void BufferTableMgr::FillData(BufferTable* row, TiXmlElement* element)
{
    int int_value = 0;
    std::string str_value = "";

    element->Attribute("BufferTypeID", &int_value);
    row->uiBufferTypeID = (unsigned int)int_value;
    str_value = element->Attribute("Name");
    row->kName = str_value;
    element->Attribute("Property", &int_value);
    row->uiProperty = (unsigned int)int_value;
    element->Attribute("SingleSpliceNum", &int_value);
    row->iSingleSpliceNum = int_value;
    element->Attribute("MultiSpliceNum", &int_value);
    row->iMultiSpliceNum = int_value;

    m_kBufferTableMap[row->uiBufferTypeID] = row;
}
