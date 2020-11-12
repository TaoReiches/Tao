/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "Unit_table.hpp"
#include "tinyxml.h"
#include <fstream>

std::unique_ptr<UnitTableMgr>       UnitTableMgr::m_pkUnitTableMgr(nullptr);
const std::unique_ptr<UnitTableMgr>& UnitTableMgr::Get()
{
    if(!m_pkUnitTableMgr)
    {
        m_pkUnitTableMgr = std::unique_ptr<UnitTableMgr>(new UnitTableMgr());
    }
    return m_pkUnitTableMgr;
}


const std::map<unsigned int, std::shared_ptr<const UnitTable>>& UnitTableMgr::GetUnitTableMap() const
{
    return m_kUnitTableMap;
}

const std::shared_ptr<const UnitTable>& UnitTableMgr::GetUnitTable(unsigned int iTypeID) const
{
    auto iter = m_kUnitTableMap.find(iTypeID);
    if(iter != m_kUnitTableMap.end())
    {
        return iter->second;
    }
    static auto nullValue = std::shared_ptr<const UnitTable>();
    return nullValue;
}

UnitTableMgr::UnitTableMgr()
{
    Load();
}

UnitTableMgr::~UnitTableMgr()
{
    m_kUnitTableMap.clear();
}

bool UnitTableMgr::Load()
{
    std::string path = "Data/Table/Unit.xml";
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
        UnitTable * row = new UnitTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    delete[] fileData;
    return true;
}

void UnitTableMgr::FillData(UnitTable* row, TiXmlElement* element)
{
    int int_value = 0;
    std::string str_value = "";
    double float_value = 0.0f;

    element->Attribute("UnitTypeID", &int_value);
    row->uiUnitTypeID = (unsigned int)int_value;
    str_value = element->Attribute("UnitName");
    row->kUnitName = str_value;
    element->Attribute("ClassType", &int_value);
    row->uiClassType = (unsigned int)int_value;
    element->Attribute("Property", &int_value);
    row->uiProperty = (unsigned int)int_value;
    element->Attribute("WeaponType", &int_value);
    row->uiWeaponType = (unsigned int)int_value;
    element->Attribute("OrgMaxHP", &float_value);
    row->fOrgMaxHP = (float)float_value;
    element->Attribute("OrgDayRegenHP", &float_value);
    row->fOrgDayRegenHP = (float)float_value;
    element->Attribute("OrgMaxMP", &float_value);
    row->fOrgMaxMP = (float)float_value;
    element->Attribute("OrgRegenMP", &float_value);
    row->fOrgRegenMP = (float)float_value;
    element->Attribute("AttackType", &int_value);
    row->uiAttackType = (unsigned int)int_value;
    element->Attribute("OrgDamage", &float_value);
    row->fOrgDamage = (float)float_value;
    element->Attribute("OrgMagic", &float_value);
    row->fOrgMagic = (float)float_value;
    element->Attribute("AttackRange", &float_value);
    row->fAttackRange = (float)float_value;
    element->Attribute("OrgArmor", &float_value);
    row->fOrgArmor = (float)float_value;
    element->Attribute("OrgAntiMagic", &float_value);
    row->fOrgAntiMagic = (float)float_value;
    element->Attribute("AttackDamagePt", &int_value);
    row->uiAttackDamagePt = (unsigned int)int_value;
    element->Attribute("AttackBackPt", &float_value);
    row->fAttackBackPt = (float)float_value;
    element->Attribute("SkillList0", &int_value);
    row->iSkillList[0] = int_value;
    element->Attribute("SkillList1", &int_value);
    row->iSkillList[1] = int_value;
    element->Attribute("SkillList2", &int_value);
    row->iSkillList[2] = int_value;
    element->Attribute("SkillList3", &int_value);
    row->iSkillList[3] = int_value;
    element->Attribute("SkillList4", &int_value);
    row->iSkillList[4] = int_value;
    element->Attribute("OrgMoveSpeed", &float_value);
    row->fOrgMoveSpeed = (float)float_value;
    element->Attribute("TurnSpeed", &float_value);
    row->fTurnSpeed = (float)float_value;
    element->Attribute("Collision", &int_value);
    row->uiCollision = (unsigned int)int_value;
    element->Attribute("TouchRadius", &float_value);
    row->fTouchRadius = (float)float_value;
    element->Attribute("MissileSpeed", &float_value);
    row->fMissileSpeed = (float)float_value;
    element->Attribute("MissileModel", &int_value);
    row->uiMissileModel = (unsigned int)int_value;
    element->Attribute("HpLevelAdd", &float_value);
    row->fHpLevelAdd = (float)float_value;
    element->Attribute("MpLevelAdd", &float_value);
    row->fMpLevelAdd = (float)float_value;
    element->Attribute("OrgAddUpDamage", &float_value);
    row->fOrgAddUpDamage = (float)float_value;
    element->Attribute("AttackSpeedAddUp", &float_value);
    row->fAttackSpeedAddUp = (float)float_value;
    element->Attribute("MagicAddUp", &float_value);
    row->fMagicAddUp = (float)float_value;
    element->Attribute("AmorAddUp", &float_value);
    row->fAmorAddUp = (float)float_value;
    element->Attribute("MagicAmorAddUp", &float_value);
    row->fMagicAmorAddUp = (float)float_value;
    element->Attribute("HpRegenAddUp", &float_value);
    row->fHpRegenAddUp = (float)float_value;
    element->Attribute("MpRegenAddUp", &float_value);
    row->fMpRegenAddUp = (float)float_value;

    m_kUnitTableMap[row->uiUnitTypeID] = std::shared_ptr<UnitTable>(row);
}
