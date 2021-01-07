/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <algorithm>
#include "TW_UnitMgr.h"
#include "TW_Unit.h"
#include "TW_Main.h"
#include "TW_TriggerEvent.h"
#include "Skill_table.hpp"
#include "Item_table.hpp"
#include "TW_MemoryObject.h"
#include <TW_TriggerMgr.h>

TwUnitMgr::TwUnitMgr(void)
{
}

TwUnitMgr::~TwUnitMgr(void)
{
	Clear();
}

bool TwUnitMgr::Initialize(void)
{
	Clear();

	//return TwEntityMgr::Initialize();
    return true;
}

std::shared_ptr<TwUnit> TwUnitMgr::GetUnitByTypeID(int iTypeID)
{
	for (std::unordered_map<int, std::shared_ptr<TwUnit>>::const_iterator itr = m_kID2Unit.begin(); itr != m_kID2Unit.end(); ++itr)
	{
		std::shared_ptr<TwUnit> pkUnit = itr->second;
		if (pkUnit && !pkUnit->IsDead())
		{
			if (iTypeID == pkUnit->GetTypeID())
			{
				return pkUnit;
			}
		}
	}
	return nullptr;
}

void TwUnitMgr::Update(int iDeltaTime)
{
	//BeEntityMgr::Update(iDeltaTime);

	if (!m_kID2SuspendUnit.empty())
	{
		for (std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2SuspendUnit.begin(); itr != m_kID2SuspendUnit.end();)
		{
			std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr1 = itr;
			std::shared_ptr<TwUnit> pkUnit = itr->second;
			++itr;
			if (pkUnit)
			{
				int iReliveTime = pkUnit->GetUnitReliveTime();
				int iCurTime = gTime;
				if (iReliveTime <= iCurTime)
				{
					m_kID2SuspendUnit.erase(itr1);
					m_kID2Unit[pkUnit->GetID()] = pkUnit;
					pkUnit->OnResume();

					pkUnit->SetHP(pkUnit->GetMaxHP());
					pkUnit->SetMP(pkUnit->GetMaxMP());
					int iPlayer = pkUnit->GetPlayer();

					TwPos2 kPos;
					float fFace = 0.0f;
					//gMain->GetHeroOrgPos(iPlayer, kPos, fFace);
					pkUnit->SetAbsFace(fFace);
					//float fPosX = pkUnit->GetUD_Float(UDK_UnitRelivePosX, pkUnit->GetPosX());
					//float fPosY = pkUnit->GetUD_Float(UDK_UnitRelivePosY, pkUnit->GetPosY());
					//pkUnit->PopUD_Float(UDK_UnitRelivePosX);
					//pkUnit->PopUD_Float(UDK_UnitRelivePosY);

					//gMap.SetUnitPosition(pkUnit, fPosX, fPosY, 0.0f, 600.0f, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);

					TwPtParam kParamRelive;
					kParamRelive.SetParam(TwTrgParamID::BTP_pkTrgUnit, pkUnit.get());

					gTrgMgr.FireTrigger(TwTriggerEvent::BTE_HERO_RELIVE, kParamRelive);

					//float fHP = pkUnit->GetUD_Float(UDK_UnitReliveMaxHp, pkUnit->GetMaxHP());
					//float fMP = pkUnit->GetUD_Float(UDK_UnitReliveMaxMp, pkUnit->GetMaxMP());
					//pkUnit->PopUD_Float(UDK_UnitReliveMaxHp);
					//pkUnit->PopUD_Float(UDK_UnitReliveMaxMp);
	
					//pkUnit->SetHP(fHP);
					//pkUnit->SetMP(fMP);


					//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
					//{
					//	BeItem* pkOrgItem = pkUnit->GetItemByPos(i);
					//	if (pkOrgItem)
					//	{
					//		pkUnit->OnItemUpDate(pkOrgItem->GetID());
					//	}
					//}
					for (int i = 0; i < iMaxHeroSkillNum; i++)
					{
						BeSkill* pkSkill = pkUnit->GetSkillByPos(i);
						if (pkSkill)
						{
							TwPtParam kParam;
							kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, pkUnit.get());
							kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill);

							gTrgMgr.FireTrigger(TwTriggerEvent::BTE_LEARN_SPELL, kParam);
						}
					}

					//if (pkUnit->IsHero() && pkUnit->GetPlayer() < MAX_ACTIVEPLAYERS && pkUnit->GetPlayer() >= 0)
					//{
					//	gMain->SetHeroFocusPlayerID(pkUnit->GetPlayer(), -1);
					//}

					pkUnit->SetOutputFlag(TwUnitOutputFlag::BSUDCF_ACTIONNAME);
				}

				//for (int iPos = 0; iPos < UNIT_MAX_ITEM; ++iPos)
				//{
				//	BeItem* pkItem = pkUnit->GetItemByPos(iPos);
				//	if (pkItem && pkItem->GetBuyInDead())
				//	{
				//		pkItem->SetBuyTime(gTime);
				//	}
				//}
			}
			else
			{
				m_kID2SuspendUnit.erase(itr1);
			}
		}
	}
	m_aiVecRandomHeroID.clear();
	for (std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2Unit.begin(); itr != m_kID2Unit.end();)
	{
		std::shared_ptr<TwUnit> pkUnit = itr->second;
		std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr1 = itr;
		++itr;
		if (pkUnit->HasFlag(TwUnitFlag::BUF_REMOVE))
		{
			if (pkUnit->HasFlag(TwUnitFlag::BUF_RELIVE))
			{
					int iPlayer = pkUnit->GetPlayer();
					m_kID2Unit.erase(itr1);
					pkUnit->OnSuspend();
					m_kID2SuspendUnit[pkUnit->GetID()] = pkUnit;
			}
			else
			{
					OnUnitDead(pkUnit);
					SafeDeleteUnit(pkUnit);
					mpUnit.free(pkUnit.get());
					pkUnit = nullptr;
					m_kID2Unit.erase(itr1);
			}
		}
		else if (!pkUnit->IsDead() && pkUnit->GetUnitAllLiveTime() != 0 && pkUnit->GetUnitCurLiveTime() < 0)
		{
				pkUnit->OperateUnitDead(BAT_ABS, 0.0f, nullptr, -1, 0, true);
		}
		else
		{
			pkUnit->Update(iDeltaTime);
		}
	}
}

void TwUnitMgr::Finialize(void)
{
	Clear();

	//TwEntityMgr::Finialize();
}

void TwUnitMgr::Clear()
{
	if (!m_kID2Unit.empty())
	{
		for (auto itr = m_kID2Unit.begin(); itr != m_kID2Unit.end();)
		{
			SafeDeleteUnit(itr->second);
			mpUnit.free(itr->second.get());
			itr->second = nullptr;
			itr = m_kID2Unit.erase(itr);
		}
		m_kID2Unit.clear();
	}

	if (!m_kID2SuspendUnit.empty())
	{
		for (std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2SuspendUnit.begin(); itr != m_kID2SuspendUnit.end();)
		{
			SafeDeleteUnit(itr->second);
			mpUnit.free(itr->second.get());
			itr->second = nullptr;
			itr = m_kID2SuspendUnit.erase(itr);
		}
		m_kID2SuspendUnit.clear();
	}

	m_kVPureNeedUpdateID.clear();
	m_kPureDelID.clear();
	m_kPureDelHeroBossBuildID.clear();
	m_kPureNewBossID.clear();
	m_kLoseVisionUnitGroup.clear();
}

std::shared_ptr<TwUnit> TwUnitMgr::NewUnit(int iID)
{
	std::shared_ptr<TwUnit> pkUnit = std::shared_ptr<TwUnit>(mpUnit.alloc(iID),[](TwUnit* p) {mpUnit.free(p);});
	pkUnit->AttachMain(pkAttachMain);
	return pkUnit;
}

std::shared_ptr<TwUnit> TwUnitMgr::AddUnit(int iTypeID, int iSkillLevel, std::uint64_t iPlayer, int iUnitID)
{
	int iID = iUnitID;
	if (iID == -1)
	{
		iID = gMain->GenerateID(BeGenIDType::GIT_ENTITY);
	}
	else
	{
		gMain->SetGenerateID(BeGenIDType::GIT_ENTITY, iID);
	}

	auto pkUnit = NewUnit(iID);
    pkUnit->AttachUnit(pkUnit);
	//gMain->AddEntityPointer(GIT_ENTITY, iID, pkUnit);

	m_kID2Unit[iID] = pkUnit;
	if (iPlayer != -1)
	{
		pkUnit->SetPlayer(iPlayer);
	}
	if (pkUnit->Initialize(iTypeID))
	{
		pkUnit->UpdateAttribute();
		pkUnit->AppyUnitSkill();

		if (iPlayer != -1)
		{
			pkUnit->SetPlayer(iPlayer);
		}
			if (iPlayer != -1)
			{
				m_kVHeroPtr.push_back(pkUnit);
				//				pkUnit->SetClientFlag(BUCF_PLAYER_HERO);
			}

			m_kVAllHeroID.push_back(pkUnit->GetID());

		TwPtParam kParam;
		kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, pkUnit.get());

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ADDUNIT, kParam);

		return pkUnit;
	}
	else
	{
		auto it = m_kID2Unit.find(iID);
		if (it != m_kID2Unit.end())
		{
			m_kID2Unit.erase(it);
		}

		SafeDeleteUnit(pkUnit);
		return nullptr;
	}
}

void TwUnitMgr::OnDelUnit(std::shared_ptr<TwUnit> pkUnit)
{
}

void TwUnitMgr::OnUnitDead(std::shared_ptr<TwUnit> pkUnit)
{
}

void TwUnitMgr::DelUnit(int iID)
{
	std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2Unit.find(iID);
	if (itr != m_kID2Unit.end())
	{
		std::shared_ptr<TwUnit> pkUnit = itr->second;
		if (pkUnit)
		{
			TwPtParam kParamDel;
			kParamDel.SetParam(TwTrgParamID::BTP_pkTrgUnit, pkUnit.get());

			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_DELUNIT, kParamDel);

			OnDelUnit(pkUnit);
			pkUnit->SetFlag(TwUnitFlag::BUF_REMOVE);
		}
		else
		{
			m_kID2Unit.erase(itr);
		}
	}
	else
	{
		itr = m_kID2SuspendUnit.find(iID);
		if (itr != m_kID2SuspendUnit.end())
		{
			std::shared_ptr<TwUnit> pkUnit = itr->second;
			if (pkUnit)
			{
				OnDelUnit(pkUnit);
				pkUnit->SetFlag(TwUnitFlag::BUF_REMOVE);
			}
			else
			{
				m_kID2SuspendUnit.erase(itr);
			}
		}
	}
}

void TwUnitMgr::SafeDeleteUnit(std::shared_ptr<TwUnit>& pkUnit)
{
	if (!pkUnit)
	{
		return;
	}

	int iID = pkUnit->GetID();

	pkUnit->OnDelete();
}

std::shared_ptr<TwUnit> TwUnitMgr::GetUnitByID(int iID, bool bSuspend, bool bSoul)
{
	if (iID == 0)
	{
		return nullptr;
	}

	//return (std::shared_ptr<BeUnit>)(gMain->GetEntityPointer(GIT_ENTITY, iID));

	std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2Unit.find(iID);
	if (itr == m_kID2Unit.end())
	{
		if (bSuspend)
		{
			std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr2 = m_kID2SuspendUnit.find(iID);
			if (itr2 == m_kID2SuspendUnit.end())
			{
				return nullptr;
			}
			return itr2->second;
		}
		return nullptr;
	}

	if (bSoul)
	{
		return itr->second;
	}
	else
	{
		std::shared_ptr<TwUnit> pkUnit = itr->second;
		return pkUnit;
	}
}

void TwUnitMgr::GetBlockAreaGroup(UnitGroup& kGroup, float fX1, float fY1, float fX2, float fY2, int iPlayerIdx, int iFlag)
{
	kGroup.clear();

	int iPlayerGroup = -1;

	float fX = (fX1 + fX2) / 2.0f;
	float fY = (fY1 + fY2) / 2.0f;
	float fDX = fabs(fX - fX1);
	float fDY = fabs(fY - fY1);
	float fRadius = std::max(fDX, fDY);
	int iBX, iBY, iEX, iEY;
	//GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	//for (int y = iBY; y <= iEY; y++)
	//{
	//	for (int x = iBX; x <= iEX; x++)
	//	{
	//		auto pkHead = &m_akBlock[m_iBlocksW * y + x];
 //           auto temp = pkHead->get()->pkNext;
	//		std::shared_ptr<BeUnit> pkUnit = std::shared_ptr<BeUnit>((BeUnit*)temp.get());
	//		if (!pkUnit)
	//		{
	//			continue;
	//		}
	//		while (pkUnit.get() != pkHead->get())
	//		{
	//			while (true)
	//			{
	//				if (fabs(pkUnit->GetPosX() - fX) > fDX || fabs(pkUnit->GetPosY() - fY) > fDY)
	//				{
	//					break;
	//				}

	//				if (!IsPassUnit(pkUnit, iPlayerGroup, iFlag))
	//				{
	//					break;
	//				}

	//				kGroup.push_back(pkUnit);
	//				break;
	//			}
	//			pkUnit = std::make_shared<BeUnit>(pkUnit->pkNext.get());
	//		}
	//	}
	//}
}

void TwUnitMgr::GetFanAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fB, float fE, int iPlayerIdx, int iFlag)
{
	kGroup.clear();

	if (fE > 2.0f * D3DX_PI)
	{
		fB -= 2.0f * D3DX_PI;
		fE -= 2.0f * D3DX_PI;
	}

	int iPlayerGroup = -1;

	int iBX, iBY, iEX, iEY;
	//GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	fRadius *= fRadius;
	//for (int y = iBY; y <= iEY; y++)
	//{
	//	for (int x = iBX; x <= iEX; x++)
	//	{
 //           auto pkHead = &m_akBlock[m_iBlocksW * y + x];
 //           auto temp = pkHead->get()->pkNext;
 //           std::shared_ptr<BeUnit> pkUnit = std::make_shared<BeUnit>(temp.get());
	//		if (!pkUnit)
	//		{
	//			continue;
	//		}
	//		while (pkUnit.get() != pkHead->get())
	//		{
	//			while (true)
	//			{
	//				if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius)
	//				{
	//					if (!IsPassUnit(pkUnit, iPlayerGroup, iFlag))
	//					{
	//						break;
	//					}

	//					float fAngle = atan2f(pkUnit->GetPosY() - fY, pkUnit->GetPosX() - fX);
	//					if (fAngle >= fB && fAngle <= fE)
	//					{
	//						kGroup.push_back(pkUnit);
	//						break;
	//					}

	//					fAngle -= 2 * D3DX_PI;
	//					if (fAngle >= fB && fAngle <= fE)
	//					{
	//						kGroup.push_back(pkUnit);
	//						break;
	//					}
	//				}
	//				break;
	//			}

	//			pkUnit = std::make_shared<BeUnit>(pkUnit->pkNext.get());
	//		}
	//	}
	//}
}

int	TwUnitMgr::GetUnitNums()
{
	return (int)m_kID2Unit.size();
}

void TwUnitMgr::GetAllMapGroup(UnitGroup& kGroup, int iPlayerIdx, int iFlag)
{
	kGroup.clear();

	int iPlayerGroup = -1;

	std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2Unit.begin();
	for (; itr != m_kID2Unit.end(); ++itr)
	{
		std::shared_ptr<TwUnit> pkUnit = itr->second;
		if (!pkUnit || !IsPassUnit(pkUnit, iPlayerGroup, iFlag))
		{
			continue;
		}

		kGroup.push_back(pkUnit);
	}
}

void TwUnitMgr::GetAllMapGroup(UnitGroup& kGroup, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData)
{
	if (!pkSrcUnit)
	{
		return;
	}
	kGroup.clear();

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();

	std::shared_ptr<const SkillTable> pkRes = nullptr;
	if (eCommand == TwCommandType::BCT_SPELL)
	{
		pkRes = SkillTableMgr::Get()->GetSkillTable(iData);
	}
	else if (eCommand == TwCommandType::BCT_USE_ITEM)
	{
	}
	std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2Unit.begin();
	//for (; itr != m_kID2Unit.end(); ++itr)
	//{
	//	std::shared_ptr<BeUnit> pkUnit = itr->second;
	//	if (!pkUnit)
	//	{
	//		continue;
	//	}
	//	bool bPass = false;
	//	switch (eCommand)
	//	{
	//	case TwCommandType::BCT_STOP:
	//	case TwCommandType::BCT_HOLD:
	//	case TwCommandType::BCT_PATROL:
	//	case TwCommandType::BCT_ATTACK:

	//			bPass = pkSrcUnit->UnitCanAttack(pkUnit, true);
	//		break;
	//	case TwCommandType::BCT_SPELL:
	//		bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
	//		break;
	//	case TwCommandType::BCT_USE_ITEM:
	//		bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
	//		break;
	//	default:
	//		break;
	//	}
	//	if (bPass)
	//	{
	//		kGroup.push_back(pkUnit);
	//	}
	//}
}

const UnitGroup& TwUnitMgr::GetAllHeroPtr()
{
	return m_kVHeroPtr;
}

const UnitGroupID& TwUnitMgr::GetAllHeroID()
{
	return m_kVAllHeroID;
}

void TwUnitMgr::GetUnitGroupByID(UnitGroup& kGroup, int iID)
{
	kGroup.clear();

	std::shared_ptr<TwUnit> pkUnit = nullptr;
	std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2Unit.begin();
	for (; itr != m_kID2Unit.end(); ++itr)
	{
		pkUnit = itr->second;
		if (!pkUnit)
		{
			continue;
		}
		if (pkUnit->IsDead())
		{
			continue;
		}

		if (iID != pkUnit->GetTypeID())
		{
			continue;
		}
		kGroup.push_back(pkUnit);
	}
}

bool TwUnitMgr::IsPassUnit(std::shared_ptr<TwUnit> pkUnit, int iPlayerGroup, int iFlag, bool bDead) const
{
	if (!pkUnit || (pkUnit->IsDead() && !bDead))
	{
		return false;
	}

	int iUnitFlag = 0;

	if ((iUnitFlag & iFlag) == iUnitFlag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TwUnitMgr::GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius) const
{
    int BX = 0, BY = 0, EX = 0, EY = 0;
    GetBlockArea(fX, fY, fRadius, BX, BY, EX, EY);

    for (int y = BY; y <= EY; y++)
    {
        for (int x = BX; x <= EX; x++)
        {
            //auto index = m_iBlocksW * y + x;
            //auto pkHead = &m_akBlock[index];
            //auto temp = pkHead->pkNext;
            //std::shared_ptr<TwUnit> pkUnit = temp->pkBlock;
            //if (!pkUnit)
            //{
            //    continue;
            //}
            //while (pkUnit.get() != pkHead->pkBlock.get())
            //{
            //    float fRadius2 = (fRadius) * (fRadius);
            //    if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
            //    {
            //        kGroup.push_back(pkUnit);
            //    }
            //    pkUnit = std::shared_ptr<TwUnit>(static_cast<TwUnit*>(pkUnit->pkNext.get()));
            //}
        }
    }
}

void TwUnitMgr::GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, int iPlayerIdx, int iFlag, bool bDead) const
{
	kGroup.clear();

	int iPlayerGroup = -1;

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	//for (int y = iBY; y <= iEY; y++)
	//{
	//	for (int x = iBX; x <= iEX; x++)
	//	{
 //           auto pkHead = &m_akBlock[m_iBlocksW * y + x];
 //           auto temp = pkHead->get()->pkNext;
 //           std::shared_ptr<BeUnit> pkUnit = std::make_shared<BeUnit>(temp.get());
	//		if (!pkUnit)
	//		{
	//			continue;
	//		}
	//		while (pkUnit.get() != pkHead->get())
	//		{
	//			float fRadius2 = (fRadius) * (fRadius);
	//			if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
	//			{
	//				if (IsPassUnit(pkUnit, iPlayerGroup, iFlag, bDead))
	//				{
	//					kGroup.push_back(pkUnit);
	//				}
	//			}
	//			pkUnit = std::make_shared<BeUnit>(pkUnit->pkNext.get());
	//		}
	//	}
	//}
}

void TwUnitMgr::GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, int iDynamicProperty, int iStaticProperty) const
{
	kGroup.clear();

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	//for (int y = iBY; y <= iEY; y++)
	//{
	//	for (int x = iBX; x <= iEX; x++)
	//	{
 //           auto pkHead = &m_akBlock[m_iBlocksW * y + x];
 //           auto temp = pkHead->get()->pkNext;
 //           std::shared_ptr<BeUnit> pkUnit = std::make_shared<BeUnit>(temp.get());
	//		if (!pkUnit)
	//		{
	//			continue;
	//		}
	//		while (pkUnit.get() != pkHead->get())
	//		{
	//			float fRadius2 = (fRadius) * (fRadius);
	//			if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
	//			{
	//				bool bPass = pkSrcUnit->IsTargetUnit(pkUnit, iStaticProperty, iDynamicProperty);
	//				if (bPass)
	//				{
	//					kGroup.push_back(pkUnit);
	//				}
	//			}
	//			pkUnit = std::make_shared<BeUnit>(pkUnit->pkNext.get());
	//		}
	//	}
	//}
}

void TwUnitMgr::GetAreaGroupByAttackedType(UnitGroup& kGroup, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, int iAttackedType) const
{
	kGroup.clear();

	int iSkillTargetFlag = iAttackedType;
	int iStaticProcFlag = 0;
	int iDynaPropFlag = 0;
	pkSrcUnit->TranslateSkillTargetType(iSkillTargetFlag, iStaticProcFlag, iDynaPropFlag);

	GetAreaGroup(kGroup, fX, fY, fRadius, pkSrcUnit, iDynaPropFlag, iStaticProcFlag);
}

void TwUnitMgr::GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, int iDynamicProperty, int iStaticProperty) const
{
	rkGroupID.clear();

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	//for (int y = iBY; y <= iEY; y++)
	//{
	//	for (int x = iBX; x <= iEX; x++)
	//	{
 //           auto pkHead = &m_akBlock[m_iBlocksW * y + x];
 //           auto temp = pkHead->get()->pkNext;
 //           std::shared_ptr<BeUnit> pkUnit = std::make_shared<BeUnit>(temp.get());
	//		if (!pkUnit)
	//		{
	//			continue;
	//		}
	//		while (pkUnit.get() != pkHead->get())
	//		{
	//			float fRadius2 = (fRadius) * (fRadius);
	//			if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
	//			{
	//				bool bPass = pkSrcUnit->IsTargetUnit(pkUnit, iStaticProperty, iDynamicProperty);
	//				if (bPass)
	//				{
	//					rkGroupID.push_back(pkUnit->GetID());
	//				}
	//			}
	//			pkUnit = std::make_shared<BeUnit>(pkUnit->pkNext.get());
	//		}
	//	}
	//}
}

void TwUnitMgr::GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData) const
{
	kGroup.clear();
	if (!pkSrcUnit)
	{
		return;
	}

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();
	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	std::shared_ptr<const SkillTable> pkRes;

	if (eCommand == TwCommandType::BCT_SPELL)
	{
		pkRes = SkillTableMgr::Get()->GetSkillTable(iData);
	}
	else if (eCommand == TwCommandType::BCT_USE_ITEM)
	{
		auto& pkItemRes = ItemTableMgr::Get()->GetItemTable(iData);
		if (!pkItemRes)
		{
			return;
		}

		int iSkillTypeID = 0;
	}

	//for (int y = iBY; y <= iEY; y++)
	//{
	//	for (int x = iBX; x <= iEX; x++)
	//	{
 //           auto pkHead = &m_akBlock[m_iBlocksW * y + x];
 //           auto temp = pkHead->get()->pkNext;
 //           std::shared_ptr<BeUnit> pkUnit = std::make_shared<BeUnit>(temp.get());
	//		if (!pkUnit)
	//		{
	//			continue;
	//		}
	//		while (pkUnit.get() != pkHead->get())
	//		{
	//			float fRadius2 = (fRadius + pkUnit->GetCollision()) * (fRadius + pkUnit->GetCollision());
	//			float fDis2 = GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY);
	//			if (fDis2 <= fRadius2)
	//			{
	//				bool bPass = false;
	//				switch (eCommand)
	//				{
	//				case TwCommandType::BCT_STOP:
	//				case TwCommandType::BCT_HOLD:
	//					// case BCT_MOVE:
	//				case TwCommandType::BCT_PATROL:
	//				case TwCommandType::BCT_ATTACK:
	//						bPass = pkSrcUnit->UnitCanAttack(pkUnit, true);
	//					break;
	//				case TwCommandType::BCT_SPELL:
	//					bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
	//					break;
	//				case TwCommandType::BCT_USE_ITEM:
	//					bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
	//					break;
	//				default:
	//					break;
	//				}
	//				if (bPass)
	//				{
	//					kGroup.push_back(pkUnit);
	//				}
	//			}
	//			pkUnit = std::make_shared<BeUnit>(pkUnit->pkNext.get());
	//		}
	//	}
	//}
}

void	TwUnitMgr::GetRectangleAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fDis, float fFace, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData) const
{
	kGroup.clear();
	UnitGroup kFirstGroup;
	GetAreaGroup(kFirstGroup, fX, fY, fDis, pkSrcUnit, eCommand, iData);

	float	fHalfPI = D3DX_PI / 2.0f;
	float	fDeltaX = fRadius * cosf(fHalfPI - fFace);
	float	fDeltaY = fRadius * sinf(fHalfPI - fFace);
	float	fX0 = fX - fDeltaX;
	float	fY0 = fY + fDeltaY;
	float	fX1 = fX0 + fDis * cosf(fFace);
	float	fY1 = fY0 + fDis * sinf(fFace);
	float	fX3 = fX + fDeltaX;
	float	fY3 = fY - fDeltaY;
	float	fX2 = fX3 + fDis * cosf(fFace);
	float	fY2 = fY3 + fDis * sinf(fFace);

	for (int i = 0; i < kFirstGroup.size(); i++)
	{
		std::shared_ptr<TwUnit> pkTarget = kFirstGroup[i];
		if (pkTarget)
		{
			if (IsInQuadrangle(fX0, fY0, fX1, fY1, fX2, fY2, fX3, fY3, pkTarget->GetPosX(), pkTarget->GetPosY()))
			{
				kGroup.push_back(pkTarget);
			}
		}
	}
}

void TwUnitMgr::GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const std::shared_ptr<TwUnit> pkSrcUnit, TwCommandType eCommand, int iData) const
{
	rkGroupID.clear();
	if (!pkSrcUnit)
	{
		return;
	}

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	std::shared_ptr<const SkillTable> pkRes;
	if (eCommand == TwCommandType::BCT_SPELL)
	{
		pkRes = SkillTableMgr::Get()->GetSkillTable(iData);
	}
	else if (eCommand == TwCommandType::BCT_USE_ITEM)
	{
		auto& pkItemRes = ItemTableMgr::Get()->GetItemTable(iData);
		if (!pkItemRes)
		{
			return;
		}

		int iSkillTypeID = 0;
	}

	//for (int y = iBY; y <= iEY; y++)
	//{
	//	for (int x = iBX; x <= iEX; x++)
	//	{
 //           auto pkHead = &m_akBlock[m_iBlocksW * y + x];
 //           auto temp = pkHead->get()->pkNext;
 //           std::shared_ptr<BeUnit> pkUnit = std::make_shared<BeUnit>(temp.get());
	//		if (!pkUnit)
	//		{
	//			continue;
	//		}
	//		while (pkUnit.get() != pkHead->get())
	//		{
	//			float fRadius2 = (fRadius) * (fRadius);
	//			if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
	//			{
	//				bool bPass = false;
	//				switch (eCommand)
	//				{
	//				case TwCommandType::BCT_STOP:
	//				case TwCommandType::BCT_HOLD:
	//					// case BCT_MOVE:
	//				case TwCommandType::BCT_PATROL:
	//				case TwCommandType::BCT_ATTACK:
	//					bPass = pkSrcUnit->UnitCanAttack(pkUnit, true);
	//					break;
	//				case TwCommandType::BCT_SPELL:
	//					bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
	//					break;
	//				case TwCommandType::BCT_USE_ITEM:
	//					bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
	//					break;
	//				default:
	//					break;
	//				}
	//				if (bPass)
	//				{
	//					rkGroupID.push_back(pkUnit->GetID());
	//				}
	//			}
	//			pkUnit = std::make_shared<BeUnit>(pkUnit->pkNext.get());
	//		}
	//	}
	//}
}

std::shared_ptr<TwUnit> TwUnitMgr::GetGroupFarestUnit(const UnitGroup& kGroup, float fPosX, float fPosY) const
{
	std::shared_ptr<TwUnit> pkFarest = nullptr;
	float fFarestDis = 0.0f;
	if (!kGroup.empty())
	{
		for (int i = 0; i < (int)kGroup.size(); ++i)
		{
			std::shared_ptr<TwUnit> pkNearUnit = kGroup[i];
			if (pkNearUnit && !pkNearUnit->IsDead())
			{
				float fDis = GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fPosX, fPosY);
				if (fFarestDis == 0.0f)
				{
					fFarestDis = fDis;
					pkFarest = pkNearUnit;
				}
				else
				{
					if (fFarestDis < fDis)
					{
						fFarestDis = fDis;
						pkFarest = pkNearUnit;
					}
				}
			}
		}
		return pkFarest;
	}
	else
	{
		return nullptr;
	}
}

std::shared_ptr<TwUnit> TwUnitMgr::GetGroupNearestUnit(const UnitGroup& kGroup, float fPosX, float fPosY, float fDistance, float fRefX, float fRefY) const
{
	std::shared_ptr<TwUnit> pkNearestUnit = nullptr;
	float fNearestDis = 0.0f;
	if (!kGroup.empty())
	{
		for (int i = 0; i < (int)kGroup.size(); ++i)
		{
			std::shared_ptr<TwUnit> pkNearUnit = kGroup[i];
			if (pkNearUnit && !pkNearUnit->IsDead())
			{
				if (fDistance > 0 && GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fRefX, fRefY) > fDistance * fDistance)
				{
					continue;
				}

				float fDis = GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fPosX, fPosY);
				if (fNearestDis == 0.0f)
				{
					fNearestDis = fDis;
					pkNearestUnit = pkNearUnit;
				}
				else
				{
					if (fNearestDis > fDis)
					{
						fNearestDis = fDis;
						pkNearestUnit = pkNearUnit;
					}
				}
			}
		}
		return pkNearestUnit;
	}
	else
	{
		return nullptr;
	}
}

int TwUnitMgr::GetGroupNearestUnit(const UnitGroupID& kGroupID, float fPosX, float fPosY) const
{
	int iNearestUnitID = 0;
	float fNearestDis = 0.0f;
	if (!kGroupID.empty())
	{
		for (int i = 0; i < (int)kGroupID.size(); ++i)
		{
			std::shared_ptr<TwUnit> pkNearUnit = gUnitMgr->GetUnitByID(kGroupID[i]);
			if (pkNearUnit && !pkNearUnit->IsDead())
			{
				float fDis = GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fPosX, fPosY);
				if (fNearestDis == 0.0f)
				{
					fNearestDis = fDis;
					iNearestUnitID = pkNearUnit->GetID();
				}
				else
				{
					if (fNearestDis > fDis)
					{
						fNearestDis = fDis;
						iNearestUnitID = pkNearUnit->GetID();
					}
				}
			}
		}
		return iNearestUnitID;
	}
	else
	{
		return 0;
	}
}

int	TwUnitMgr::GetUnitIDFromSuspend(int ITypeID)
{
	if (m_kID2SuspendUnit.empty())
	{
		return 0;
	}

	std::unordered_map<int, std::shared_ptr<TwUnit>>::iterator itr = m_kID2SuspendUnit.begin();
	for (; itr != m_kID2SuspendUnit.end(); ++itr)
	{
		std::shared_ptr<TwUnit> pkUnit = itr->second;
		if (pkUnit && pkUnit->GetTypeID() == ITypeID)
		{
			return pkUnit->GetID();
		}
	}

	return 0;
}

const std::unordered_map<int, std::shared_ptr<TwUnit>>& TwUnitMgr::GetID2Unit() const
{
	return m_kID2Unit;
}

const std::unordered_map<int, std::shared_ptr<TwUnit>>& TwUnitMgr::GetID2SuspendUnit() const
{
	return m_kID2SuspendUnit;
}

bool TwUnitMgr::GetUnitByTypeID(UnitGroup& kGroup, int iTypeID) const
{
	for (std::unordered_map<int, std::shared_ptr<TwUnit>>::const_iterator itr = m_kID2Unit.begin(); itr != m_kID2Unit.end(); ++itr)
	{
		std::shared_ptr<TwUnit> pkUnit = itr->second;
		if (pkUnit && !pkUnit->IsDead())
		{
			if (iTypeID == pkUnit->GetTypeID())
			{
				kGroup.push_back(pkUnit);
			}
		}
	}
	return true;
}
bool TwUnitMgr::GetUnitByPlayer(UnitGroup& kGroup, const std::string& kPlayerName) const
{
	return true;
}

void TwUnitMgr::PushNeedUpdateUnitID(int iID)
{
}

void TwUnitMgr::ClrPureData()
{
	m_kVPureNeedUpdateID.clear();
	m_kPureDelID.clear();
	m_kPureDelHeroBossBuildID.clear();
	m_kPureNewBossID.clear();
	m_kLoseVisionUnitGroup.clear();
}

const UnitGroupID& TwUnitMgr::PureGetAllNeedUpdateUnitID()
{
	return m_kVPureNeedUpdateID;
}

const UnitGroupID& TwUnitMgr::PureGetDelUnitID(int iCamp)
{
	return m_kLoseVisionUnitGroup[iCamp];
}

void TwUnitMgr::PushNeedDelNormalUnit(int iCamp, int iID)
{
	m_kLoseVisionUnitGroup[iCamp].push_back(iID);
}

void TwUnitMgr::PushNeedDelHeroBossBuilding(int iID)
{
	{
		m_kPureDelHeroBossBuildID.push_back(iID);
	}
}

const UnitGroupID& TwUnitMgr::PureGetDelBoosBuildingID()
{
	return m_kPureDelHeroBossBuildID;
}

const UnitGroupID& TwUnitMgr::PureGetNewBossID()
{
	return m_kPureNewBossID;
}

void TwUnitMgr::PushNewBossID(int iID)
{
	{
		m_kPureNewBossID.push_back(iID);
	}
}
