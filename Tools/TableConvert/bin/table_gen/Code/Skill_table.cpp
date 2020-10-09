#include "Skill_table.hpp"
#include "tinyxml.h"
#include "EeFileMemory.h"
#include "EeFilePackage.h"

SkillTableMgr* SkillTableMgr::m_pkSkillTableMgr = nullptr;
SkillTableMgr* SkillTableMgr::Get()
{
    if(m_pkSkillTableMgr == nullptr)
    {
        m_pkSkillTableMgr = new SkillTableMgr();
    }
    return m_pkSkillTableMgr;
}


SkillTable* SkillTableMgr::GetSkillTable(unsigned int iTypeID)
{
    std::map<unsigned int, SkillTable*>::iterator iter = m_kSkillTableMap.find(iTypeID);
    if(iter != m_kSkillTableMap.end())
    {
        return iter->second;
    }
    return nullptr;
}

SkillTableMgr::SkillTableMgr()
{
    Load();
}

SkillTableMgr::~SkillTableMgr()
{
    m_kSkillTableMap.clear();
}

bool SkillTableMgr::Load()
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
        SkillTable * row = new SkillTable();
        FillData(row, element);
        element = element->NextSiblingElement();
    }

    return true;
}

void SkillTableMgr::FillData(SkillTable* row, TiXmlElement* element)
{
    int int_value = 0;
    std::string str_value = "";
    double float_value = 0.0f;

    element->Attribute("TypeID", &int_value);
    row->TypeID = (unsigned int)int_value;
    str_value = element->Attribute("Name");
    row->Name = str_value;
    str_value = element->Attribute("Desc");
    row->Desc = str_value;
    element->Attribute("OperateType", &int_value);
    row->OperateType = (unsigned int)int_value;
    element->Attribute("OriginTypeID", &int_value);
    row->OriginTypeID = (unsigned int)int_value;
    element->Attribute("UIShowPos", &int_value);
    row->UIShowPos = int_value;
    element->Attribute("TargetType", &int_value);
    row->TargetType = (unsigned int)int_value;
    element->Attribute("AttackType", &int_value);
    row->AttackType = (unsigned int)int_value;
    element->Attribute("SkillProperty", &int_value);
    row->SkillProperty = (unsigned int)int_value;
    element->Attribute("BufferID", &int_value);
    row->BufferID = (unsigned int)int_value;
    element->Attribute("SkillMaxLevel", &int_value);
    row->SkillMaxLevel = int_value;
    str_value = element->Attribute("SpellAction");
    row->SpellAction = str_value;
    element->Attribute("LearnLevel", &int_value);
    row->LearnLevel = int_value;
    element->Attribute("CastTime", &int_value);
    row->CastTime = int_value;
    element->Attribute("EffectTime", &int_value);
    row->EffectTime = int_value;
    element->Attribute("CoolDown", &int_value);
    row->CoolDown = int_value;
    element->Attribute("EffectScope", &int_value);
    row->EffectScope = int_value;
    element->Attribute("SkillDistance", &int_value);
    row->SkillDistance = int_value;
    element->Attribute("PilePointNums", &int_value);
    row->PilePointNums = int_value;
    element->Attribute("ManaSpend", &int_value);
    row->ManaSpend = int_value;
    element->Attribute("DataContentA", &int_value);
    row->DataContentA = (unsigned int)int_value;
    element->Attribute("ValueA", &float_value);
    row->ValueA = (float)float_value;
    element->Attribute("AAdj", &float_value);
    row->AAdj = (float)float_value;
    element->Attribute("DataContentB", &int_value);
    row->DataContentB = (unsigned int)int_value;
    element->Attribute("ValueB", &float_value);
    row->ValueB = (float)float_value;
    element->Attribute("BAdj", &float_value);
    row->BAdj = (float)float_value;
    element->Attribute("DataContentC", &int_value);
    row->DataContentC = (unsigned int)int_value;
    element->Attribute("ValueC", &float_value);
    row->ValueC = (float)float_value;
    element->Attribute("CAdj", &float_value);
    row->CAdj = (float)float_value;
    element->Attribute("DataContentD", &int_value);
    row->DataContentD = (unsigned int)int_value;
    element->Attribute("ValueD", &float_value);
    row->ValueD = (float)float_value;
    element->Attribute("DAdj", &float_value);
    row->DAdj = (float)float_value;
    element->Attribute("DataContentE", &int_value);
    row->DataContentE = (unsigned int)int_value;
    element->Attribute("ValueE", &float_value);
    row->ValueE = (float)float_value;
    element->Attribute("EAdj", &float_value);
    row->EAdj = (float)float_value;
    element->Attribute("DataContentF", &int_value);
    row->DataContentF = (unsigned int)int_value;
    element->Attribute("ValueF", &float_value);
    row->ValueF = (float)float_value;
    element->Attribute("FAdj", &float_value);
    row->FAdj = (float)float_value;
    str_value = element->Attribute("MissileModel");
    row->MissileModel = str_value;
    element->Attribute("MissileSpeed", &float_value);
    row->MissileSpeed = (float)float_value;
    element->Attribute("MissileArc", &float_value);
    row->MissileArc = (float)float_value;
    str_value = element->Attribute("SrcEffect");
    row->SrcEffect = str_value;
    str_value = element->Attribute("DstPosEffect");
    row->DstPosEffect = str_value;
    str_value = element->Attribute("DstUnitEffect");
    row->DstUnitEffect = str_value;
    str_value = element->Attribute("SkillEffect");
    row->SkillEffect = str_value;
    str_value = element->Attribute("LearnIcon");
    row->LearnIcon = str_value;
    str_value = element->Attribute("PrepareSound");
    row->PrepareSound = str_value;
    str_value = element->Attribute("EffectSound");
    row->EffectSound = str_value;
    str_value = element->Attribute("SpecialSound");
    row->SpecialSound = str_value;
    element->Attribute("FightScore", &float_value);
    row->FightScore = (float)float_value;
    str_value = element->Attribute("SimpleDesc");
    row->SimpleDesc = str_value;

    m_kSkillTableMap[row->TypeID] = row;
}
