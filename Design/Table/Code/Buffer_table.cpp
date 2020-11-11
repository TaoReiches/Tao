#include "Buffer_table.hpp"
#include "tinyxml.h"
#include <fstream>

BufferTableMgr* BufferTableMgr::m_pkBufferTableMgr = nullptr;
BufferTableMgr* BufferTableMgr::Get()
{
    if(m_pkBufferTableMgr == nullptr)
    {
        m_pkBufferTableMgr = new BufferTableMgr();
    }
    return m_pkBufferTableMgr;
}


const std::map<unsigned int, BufferTable*>& BufferTableMgr::GetBufferTableMap()
{
    return m_kBufferTableMap;
}

const BufferTable* BufferTableMgr::GetBufferTable(unsigned int iTypeID)
{
    std::map<unsigned int, BufferTable*>::iterator iter = m_kBufferTableMap.find(iTypeID);
    if(iter != m_kBufferTableMap.end())
    {
        return iter->second;
    }
    return nullptr;
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
        BufferTable * row = new BufferTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    delete[] fileData;
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

    m_kBufferTableMap[row->uiBufferTypeID] = row;
}
