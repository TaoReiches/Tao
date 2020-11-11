#include "Unit_table.hpp"
#include "tinyxml.h"


UnitTableMgr* UnitTableMgr::m_pkUnitTableMgr = NULL;
UnitTableMgr* UnitTableMgr::Get()
{
    if(m_pkUnitTableMgr == NULL)
    {
        m_pkUnitTableMgr = new UnitTableMgr();
    }
    return m_pkUnitTableMgr;
}


const std::map<unsigned int, UnitTable*>& UnitTableMgr::GetUnitTableMap()
{
    return m_kUnitTableMap;
}


const UnitTable* UnitTableMgr::GetUnitTable(unsigned int iTypeID)
{
    std::map<unsigned int, UnitTable*>::iterator iter = m_kUnitTableMap.find(iTypeID);
    if(iter != m_kUnitTableMap.end())
    {
        return iter->second;
    }
    return NULL;
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
    //std::string path = "Data/Table/Unit.xml";
    //FileMemory kMemory;
    //if(!FileLoader::LoadTableFile(path.c_str(),kMemory))
    //{
    //    return false;
    //}

    //TiXmlDocument doc;
    //doc.Parse(kMemory.GetData());
    //if (doc.Error())
    //{
    //    std::string err = path + "   " + std::string(doc.ErrorDesc());
    //    // throw std::exception(err.c_str());
    //    return false;
    //}

    //TiXmlElement* root = doc.FirstChildElement("root");
    //if (root == 0)
    //{
    //    // throw std::exception("root is null!");
    //    return false;
    //}

    //TiXmlElement* element = root->FirstChildElement("content");
    //while (element != 0)
    //{
    //    UnitTable * row = new UnitTable();
    //    FillData(row, element);
    //    element = element->NextSiblingElement();
    //}

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
    element->Attribute("DeathMoney", &int_value);
    row->iDeathMoney = int_value;
    element->Attribute("DeathExp", &int_value);
    row->iDeathExp = int_value;
    element->Attribute("AttackDamagePt", &float_value);
    row->fAttackDamagePt = (float)float_value;
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
    element->Attribute("Collision", &float_value);
    row->fCollision = (float)float_value;
    element->Attribute("TouchRadius", &float_value);
    row->fTouchRadius = (float)float_value;
    element->Attribute("MissileSpeed", &float_value);
    row->fMissileSpeed = (float)float_value;
    element->Attribute("MissileArc", &float_value);
    row->fMissileArc = (float)float_value;
    element->Attribute("MissileModel", &int_value);
    row->iMissileModel = int_value;
    str_value = element->Attribute("ExclusiveItemTypeID");
    row->kExclusiveItemTypeID = str_value;
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
    element->Attribute("ModelScale", &float_value);
    row->fModelScale = (float)float_value;
    element->Attribute("TargetDamage", &int_value);
    row->iTargetDamage = int_value;
    element->Attribute("BasisEquip0", &int_value);
    row->iBasisEquip[0] = int_value;
    element->Attribute("BasisEquip1", &int_value);
    row->iBasisEquip[1] = int_value;
    element->Attribute("RecEquip0", &int_value);
    row->iRecEquip[0] = int_value;
    element->Attribute("RecEquip1", &int_value);
    row->iRecEquip[1] = int_value;
    element->Attribute("RecEquip2", &int_value);
    row->iRecEquip[2] = int_value;
    element->Attribute("RecEquip3", &int_value);
    row->iRecEquip[3] = int_value;
    element->Attribute("RecEquip4", &int_value);
    row->iRecEquip[4] = int_value;
    element->Attribute("RecEquip5", &int_value);
    row->iRecEquip[5] = int_value;
    element->Attribute("RecEquip6", &int_value);
    row->iRecEquip[6] = int_value;

    m_kUnitTableMap[row->uiUnitTypeID] = row;
}
