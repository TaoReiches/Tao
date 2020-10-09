#include "Skill_table.hpp"
#include "tinyxml.h"
#include "EeFileMemory.h"
#include "EeFilePackage.h"

SkillTableMgr* SkillTableMgr::m_pkSkillTableMgr = NULL;
SkillTableMgr* SkillTableMgr::Get()
{
    if(m_pkSkillTableMgr == NULL)
    {
        m_pkSkillTableMgr = new SkillTableMgr();
    }
    return m_pkSkillTableMgr;
}


const std::map<unsigned int, SkillTable*>& SkillTableMgr::GetSkillTableMap()
{
    return m_kSkillTableMap;
}

TableResArray SkillTableMgr::GetSkillTableVec()
{
    TableResArray kRecVec;
    for (std::map<unsigned int, SkillTable*>::iterator iMapItr = m_kSkillTableMap.begin(); iMapItr != m_kSkillTableMap.end(); ++iMapItr)
    {
        kRecVec.pushBack(iMapItr->second);
    }

    return kRecVec;
}

const SkillTable* SkillTableMgr::GetSkillTable(unsigned int iTypeID)
{
    std::map<unsigned int, SkillTable*>::iterator iter = m_kSkillTableMap.find(iTypeID);
    if(iter != m_kSkillTableMap.end())
    {
        return iter->second;
    }
    return NULL;
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
    std::string path = "Data/Table/Skill.xml";
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
    row->uiTypeID = (unsigned int)int_value;
    str_value = element->Attribute("Name");
    row->kName = str_value;
    element->Attribute("OperateType", &int_value);
    row->uiOperateType = (unsigned int)int_value;
    element->Attribute("OriginTypeID", &int_value);
    row->uiOriginTypeID = (unsigned int)int_value;
    element->Attribute("UIShowPos", &int_value);
    row->iUIShowPos = int_value;
    element->Attribute("AfterActionType", &int_value);
    row->uiAfterActionType = (unsigned int)int_value;
    element->Attribute("TargetType", &int_value);
    row->uiTargetType = (unsigned int)int_value;
    element->Attribute("AttackType", &int_value);
    row->uiAttackType = (unsigned int)int_value;
    element->Attribute("SkillProperty", &int_value);
    row->uiSkillProperty = (unsigned int)int_value;
    element->Attribute("SkillMaxLevel", &int_value);
    row->iSkillMaxLevel = int_value;
    element->Attribute("SpellAction", &int_value);
    row->uiSpellAction = (unsigned int)int_value;
    element->Attribute("LearnLevel0", &int_value);
    row->iLearnLevel[0] = int_value;
    element->Attribute("LearnLevel1", &int_value);
    row->iLearnLevel[1] = int_value;
    element->Attribute("LearnLevel2", &int_value);
    row->iLearnLevel[2] = int_value;
    element->Attribute("LearnLevel3", &int_value);
    row->iLearnLevel[3] = int_value;
    element->Attribute("LearnLevel4", &int_value);
    row->iLearnLevel[4] = int_value;
    element->Attribute("CastTime0", &int_value);
    row->iCastTime[0] = int_value;
    element->Attribute("CastTime1", &int_value);
    row->iCastTime[1] = int_value;
    element->Attribute("CastTime2", &int_value);
    row->iCastTime[2] = int_value;
    element->Attribute("CastTime3", &int_value);
    row->iCastTime[3] = int_value;
    element->Attribute("CastTime4", &int_value);
    row->iCastTime[4] = int_value;
    element->Attribute("ShakesTime0", &int_value);
    row->iShakesTime[0] = int_value;
    element->Attribute("ShakesTime1", &int_value);
    row->iShakesTime[1] = int_value;
    element->Attribute("ShakesTime2", &int_value);
    row->iShakesTime[2] = int_value;
    element->Attribute("ShakesTime3", &int_value);
    row->iShakesTime[3] = int_value;
    element->Attribute("ShakesTime4", &int_value);
    row->iShakesTime[4] = int_value;
    element->Attribute("EffectTime0", &int_value);
    row->iEffectTime[0] = int_value;
    element->Attribute("EffectTime1", &int_value);
    row->iEffectTime[1] = int_value;
    element->Attribute("EffectTime2", &int_value);
    row->iEffectTime[2] = int_value;
    element->Attribute("EffectTime3", &int_value);
    row->iEffectTime[3] = int_value;
    element->Attribute("EffectTime4", &int_value);
    row->iEffectTime[4] = int_value;
    element->Attribute("CoolDown0", &int_value);
    row->iCoolDown[0] = int_value;
    element->Attribute("CoolDown1", &int_value);
    row->iCoolDown[1] = int_value;
    element->Attribute("CoolDown2", &int_value);
    row->iCoolDown[2] = int_value;
    element->Attribute("CoolDown3", &int_value);
    row->iCoolDown[3] = int_value;
    element->Attribute("CoolDown4", &int_value);
    row->iCoolDown[4] = int_value;
    element->Attribute("EffectScope0", &int_value);
    row->iEffectScope[0] = int_value;
    element->Attribute("EffectScope1", &int_value);
    row->iEffectScope[1] = int_value;
    element->Attribute("EffectScope2", &int_value);
    row->iEffectScope[2] = int_value;
    element->Attribute("EffectScope3", &int_value);
    row->iEffectScope[3] = int_value;
    element->Attribute("EffectScope4", &int_value);
    row->iEffectScope[4] = int_value;
    element->Attribute("SkillDistance0", &int_value);
    row->iSkillDistance[0] = int_value;
    element->Attribute("SkillDistance1", &int_value);
    row->iSkillDistance[1] = int_value;
    element->Attribute("SkillDistance2", &int_value);
    row->iSkillDistance[2] = int_value;
    element->Attribute("SkillDistance3", &int_value);
    row->iSkillDistance[3] = int_value;
    element->Attribute("SkillDistance4", &int_value);
    row->iSkillDistance[4] = int_value;
    element->Attribute("SkillAngle0", &int_value);
    row->iSkillAngle[0] = int_value;
    element->Attribute("SkillAngle1", &int_value);
    row->iSkillAngle[1] = int_value;
    element->Attribute("SkillAngle2", &int_value);
    row->iSkillAngle[2] = int_value;
    element->Attribute("SkillAngle3", &int_value);
    row->iSkillAngle[3] = int_value;
    element->Attribute("SkillAngle4", &int_value);
    row->iSkillAngle[4] = int_value;
    element->Attribute("PilePointNums0", &int_value);
    row->iPilePointNums[0] = int_value;
    element->Attribute("PilePointNums1", &int_value);
    row->iPilePointNums[1] = int_value;
    element->Attribute("PilePointNums2", &int_value);
    row->iPilePointNums[2] = int_value;
    element->Attribute("PilePointNums3", &int_value);
    row->iPilePointNums[3] = int_value;
    element->Attribute("PilePointNums4", &int_value);
    row->iPilePointNums[4] = int_value;
    element->Attribute("HpSpend0", &float_value);
    row->fHpSpend[0] = float_value;
    element->Attribute("HpSpend1", &float_value);
    row->fHpSpend[1] = float_value;
    element->Attribute("HpSpend2", &float_value);
    row->fHpSpend[2] = float_value;
    element->Attribute("HpSpend3", &float_value);
    row->fHpSpend[3] = float_value;
    element->Attribute("HpSpend4", &float_value);
    row->fHpSpend[4] = float_value;
    element->Attribute("ManaSpend0", &float_value);
    row->fManaSpend[0] = float_value;
    element->Attribute("ManaSpend1", &float_value);
    row->fManaSpend[1] = float_value;
    element->Attribute("ManaSpend2", &float_value);
    row->fManaSpend[2] = float_value;
    element->Attribute("ManaSpend3", &float_value);
    row->fManaSpend[3] = float_value;
    element->Attribute("ManaSpend4", &float_value);
    row->fManaSpend[4] = float_value;
    element->Attribute("DataContentA", &int_value);
    row->uiDataContentA = (unsigned int)int_value;
    element->Attribute("ValueA0", &float_value);
    row->fValueA[0] = float_value;
    element->Attribute("ValueA1", &float_value);
    row->fValueA[1] = float_value;
    element->Attribute("ValueA2", &float_value);
    row->fValueA[2] = float_value;
    element->Attribute("ValueA3", &float_value);
    row->fValueA[3] = float_value;
    element->Attribute("ValueA4", &float_value);
    row->fValueA[4] = float_value;
    element->Attribute("uiAAdj", &int_value);
    row->uiAAdj = (unsigned int)int_value;
    element->Attribute("fAAdj", &float_value);
    row->fAAdj = (float)float_value;
    element->Attribute("DataContentB", &int_value);
    row->uiDataContentB = (unsigned int)int_value;
    element->Attribute("ValueB0", &float_value);
    row->fValueB[0] = float_value;
    element->Attribute("ValueB1", &float_value);
    row->fValueB[1] = float_value;
    element->Attribute("ValueB2", &float_value);
    row->fValueB[2] = float_value;
    element->Attribute("ValueB3", &float_value);
    row->fValueB[3] = float_value;
    element->Attribute("ValueB4", &float_value);
    row->fValueB[4] = float_value;
    element->Attribute("uiBAdj", &int_value);
    row->uiBAdj = (unsigned int)int_value;
    element->Attribute("fBAdj", &float_value);
    row->fBAdj = (float)float_value;
    element->Attribute("DataContentC", &int_value);
    row->uiDataContentC = (unsigned int)int_value;
    element->Attribute("ValueC0", &float_value);
    row->fValueC[0] = float_value;
    element->Attribute("ValueC1", &float_value);
    row->fValueC[1] = float_value;
    element->Attribute("ValueC2", &float_value);
    row->fValueC[2] = float_value;
    element->Attribute("ValueC3", &float_value);
    row->fValueC[3] = float_value;
    element->Attribute("ValueC4", &float_value);
    row->fValueC[4] = float_value;
    element->Attribute("uiCAdj", &int_value);
    row->uiCAdj = (unsigned int)int_value;
    element->Attribute("fCAdj", &float_value);
    row->fCAdj = (float)float_value;
    element->Attribute("DataContentD", &int_value);
    row->uiDataContentD = (unsigned int)int_value;
    element->Attribute("ValueD0", &float_value);
    row->fValueD[0] = float_value;
    element->Attribute("ValueD1", &float_value);
    row->fValueD[1] = float_value;
    element->Attribute("ValueD2", &float_value);
    row->fValueD[2] = float_value;
    element->Attribute("ValueD3", &float_value);
    row->fValueD[3] = float_value;
    element->Attribute("ValueD4", &float_value);
    row->fValueD[4] = float_value;
    element->Attribute("uiDAdj", &int_value);
    row->uiDAdj = (unsigned int)int_value;
    element->Attribute("fDAdj", &float_value);
    row->fDAdj = (float)float_value;
    element->Attribute("DataContentE", &int_value);
    row->uiDataContentE = (unsigned int)int_value;
    element->Attribute("ValueE0", &float_value);
    row->fValueE[0] = float_value;
    element->Attribute("ValueE1", &float_value);
    row->fValueE[1] = float_value;
    element->Attribute("ValueE2", &float_value);
    row->fValueE[2] = float_value;
    element->Attribute("ValueE3", &float_value);
    row->fValueE[3] = float_value;
    element->Attribute("ValueE4", &float_value);
    row->fValueE[4] = float_value;
    element->Attribute("uiEAdj", &int_value);
    row->uiEAdj = (unsigned int)int_value;
    element->Attribute("fEAdj", &float_value);
    row->fEAdj = (float)float_value;
    element->Attribute("DataContentF", &int_value);
    row->uiDataContentF = (unsigned int)int_value;
    element->Attribute("ValueF0", &float_value);
    row->fValueF[0] = float_value;
    element->Attribute("ValueF1", &float_value);
    row->fValueF[1] = float_value;
    element->Attribute("ValueF2", &float_value);
    row->fValueF[2] = float_value;
    element->Attribute("ValueF3", &float_value);
    row->fValueF[3] = float_value;
    element->Attribute("ValueF4", &float_value);
    row->fValueF[4] = float_value;
    element->Attribute("uiFAdj", &int_value);
    row->uiFAdj = (unsigned int)int_value;
    element->Attribute("fFAdj", &float_value);
    row->fFAdj = (float)float_value;
    element->Attribute("DataContentG", &int_value);
    row->uiDataContentG = (unsigned int)int_value;
    element->Attribute("ValueG0", &float_value);
    row->fValueG[0] = float_value;
    element->Attribute("ValueG1", &float_value);
    row->fValueG[1] = float_value;
    element->Attribute("ValueG2", &float_value);
    row->fValueG[2] = float_value;
    element->Attribute("ValueG3", &float_value);
    row->fValueG[3] = float_value;
    element->Attribute("ValueG4", &float_value);
    row->fValueG[4] = float_value;
    element->Attribute("uiGAdj", &int_value);
    row->uiGAdj = (unsigned int)int_value;
    element->Attribute("fGAdj", &float_value);
    row->fGAdj = (float)float_value;
    element->Attribute("MissileModel", &int_value);
    row->iMissileModel = int_value;
    element->Attribute("MissileSpeed", &float_value);
    row->fMissileSpeed = (float)float_value;
    element->Attribute("MissileArc", &float_value);
    row->fMissileArc = (float)float_value;
    element->Attribute("SrcEffect", &int_value);
    row->iSrcEffect = int_value;
    element->Attribute("DstPosEffect", &int_value);
    row->iDstPosEffect = int_value;
    element->Attribute("DstUnitEffect", &int_value);
    row->iDstUnitEffect = int_value;
    element->Attribute("SkillEffect", &int_value);
    row->iSkillEffect = int_value;

    m_kSkillTableMap[row->uiTypeID] = row;
}
