/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "Buffer_table.hpp"
#include "tinyxml.h"
#include <fstream>

std::unique_ptr<BufferTableMgr>       BufferTableMgr::m_pkBufferTableMgr(nullptr);
const std::unique_ptr<BufferTableMgr>& BufferTableMgr::Get()
{
    if(!m_pkBufferTableMgr)
    {
        m_pkBufferTableMgr = std::unique_ptr<BufferTableMgr>(new BufferTableMgr());
    }
    return m_pkBufferTableMgr;
}


const std::map<unsigned int, std::shared_ptr<const BufferTable>>& BufferTableMgr::GetBufferTableMap() const
{
    return m_kBufferTableMap;
}

const std::shared_ptr<const BufferTable>& BufferTableMgr::GetBufferTable(unsigned int iTypeID) const
{
    auto iter = m_kBufferTableMap.find(iTypeID);
    if(iter != m_kBufferTableMap.end())
    {
        return iter->second;
    }
    static auto nullValue = std::shared_ptr<const BufferTable>();
    return nullValue;
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

    m_kBufferTableMap[row->uiBufferTypeID] = std::shared_ptr<BufferTable>(row);
}
