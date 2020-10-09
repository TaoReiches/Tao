#include "Unit_table.hpp"
#include "tinyxml.h"
#include "EeFileMemory.h"
#include "EeFilePackage.h"

UnitTableMgr* UnitTableMgr::m_pkUnitTableMgr = nullptr;
UnitTableMgr* UnitTableMgr::Get()
{
    if(m_pkUnitTableMgr == nullptr)
    {
        m_pkUnitTableMgr = new UnitTableMgr();
    }
    return m_pkUnitTableMgr;
}


UnitTable* UnitTableMgr::GetUnitTable(unsigned int iTypeID)
{
    std::map<unsigned int, UnitTable*>::iterator iter = m_kUnitTableMap.find(iTypeID);
    if(iter != m_kUnitTableMap.end())
    {
        return iter->second;
    }
    return nullptr;
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
    std::string path = "./Data/Table/Unit.xml";
    FileMemory kMemory;
    if(!FileLoader::LoadFile(path.c_str(),kMemory))
    {
        return false;
    }

    TiXmlDocument doc;
    doc.Parse(kMemory.GetData());
    if (doc.Error())
    {
        std::string err = path + "   " + std::string(doc.ErrorDesc());
        throw std::exception(err.c_str());
    }

    TiXmlElement* root = doc.FirstChildElement("root");
    if (root == 0)
    {
        throw std::exception("root is null!");
    }

    TiXmlElement* element = root->FirstChildElement("content");
    while (element != 0)
    {
        UnitTable * row = new UnitTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    return true;
}

void UnitTableMgr::FillData(UnitTable* row, TiXmlElement* element)
{
    int int_value = 0;
    std::string str_value = "";
    double float_value = 0.0f;

    element->Attribute("UnitTypeID", &int_value);
    row->UnitTypeID = (unsigned int)int_value;
    str_value = element->Attribute("UnitName");
    row->UnitName = str_value;
    element->Attribute("ClassType", &int_value);
    row->ClassType = (unsigned int)int_value;
    element->Attribute("Property", &int_value);
    row->Property = (unsigned int)int_value;
    element->Attribute("Primary", &int_value);
    row->Primary = (unsigned int)int_value;
    element->Attribute("OrgSTR", &float_value);
    row->OrgSTR = (float)float_value;
    element->Attribute("PlusSTR", &float_value);
    row->PlusSTR = (float)float_value;
    element->Attribute("OrgAGI", &float_value);
    row->OrgAGI = (float)float_value;
    element->Attribute("PlusAGI", &float_value);
    row->PlusAGI = (float)float_value;
    element->Attribute("OrgINT", &float_value);
    row->OrgINT = (float)float_value;
    element->Attribute("PlusINT", &float_value);
    row->PlusINT = (float)float_value;
    element->Attribute("OrgMaxHP", &float_value);
    row->OrgMaxHP = (float)float_value;
    element->Attribute("OrgDayRegenHP", &float_value);
    row->OrgDayRegenHP = (float)float_value;
    element->Attribute("OrgMaxMP", &float_value);
    row->OrgMaxMP = (float)float_value;
    element->Attribute("OrgRegenMP", &float_value);
    row->OrgRegenMP = (float)float_value;
    element->Attribute("DayVision", &int_value);
    row->DayVision = int_value;
    element->Attribute("AttackType", &int_value);
    row->AttackType = (unsigned int)int_value;
    element->Attribute("OrgMinDamage", &float_value);
    row->OrgMinDamage = (float)float_value;
    element->Attribute("OrgMaxDamage", &float_value);
    row->OrgMaxDamage = (float)float_value;
    element->Attribute("AttackRange", &float_value);
    row->AttackRange = (float)float_value;
    element->Attribute("OrgAttackCD", &int_value);
    row->OrgAttackCD = int_value;
    element->Attribute("DefenseType", &int_value);
    row->DefenseType = (unsigned int)int_value;
    element->Attribute("OrgArmor", &float_value);
    row->OrgArmor = (float)float_value;
    element->Attribute("DeathMoney", &int_value);
    row->DeathMoney = int_value;
    element->Attribute("DeathExp", &int_value);
    row->DeathExp = int_value;
    element->Attribute("MissileSpeed", &float_value);
    row->MissileSpeed = (float)float_value;
    element->Attribute("MissileArc", &float_value);
    row->MissileArc = (float)float_value;
    element->Attribute("AttackDamagePt", &float_value);
    row->AttackDamagePt = (float)float_value;
    element->Attribute("AttackBackPt", &float_value);
    row->AttackBackPt = (float)float_value;
    element->Attribute("SpellDamagePt", &float_value);
    row->SpellDamagePt = (float)float_value;
    element->Attribute("SpellBackPt", &float_value);
    row->SpellBackPt = (float)float_value;
    str_value = element->Attribute("Icon");
    row->Icon = str_value;
    str_value = element->Attribute("SkillList");
    row->SkillList = str_value;
    element->Attribute("OrgMoveSpeed", &float_value);
    row->OrgMoveSpeed = (float)float_value;
    element->Attribute("TurnSpeed", &float_value);
    row->TurnSpeed = (float)float_value;
    element->Attribute("FlyHeight", &int_value);
    row->FlyHeight = int_value;
    element->Attribute("Collision", &float_value);
    row->Collision = (float)float_value;
    str_value = element->Attribute("ModelFile");
    row->ModelFile = str_value;
    element->Attribute("ModelScale", &float_value);
    row->ModelScale = (float)float_value;
    str_value = element->Attribute("MissileModel");
    row->MissileModel = str_value;
    element->Attribute("SelectScale", &float_value);
    row->SelectScale = (float)float_value;
    str_value = element->Attribute("SelectSound");
    row->SelectSound = str_value;
    str_value = element->Attribute("AttackVoice");
    row->AttackVoice = str_value;
    str_value = element->Attribute("WalkSound");
    row->WalkSound = str_value;
    str_value = element->Attribute("AttackSound");
    row->AttackSound = str_value;
    str_value = element->Attribute("AttackedSound");
    row->AttackedSound = str_value;
    str_value = element->Attribute("DeathSound");
    row->DeathSound = str_value;
    str_value = element->Attribute("ExclusiveItemTypeID");
    row->ExclusiveItemTypeID = str_value;

    m_kUnitTableMap[row->UnitTypeID] = row;
}
