/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Unit.h"
#include "Item_table.hpp"
#include "Skill_table.hpp"
#include "Buffer_table.hpp"
#include <TW_TriggerMgr.h>
#include "TW_TriggerEvent.h"
#include "TW_Item.h"
#include "TW_Main.h"
#include "TW_Skill.h"
#include "TW_Buff.h"
#include "TW_Formula.h"
#include "TW_AttackAttr.h"
#include "TW_MemoryObject.h"

TwUnitCarry::TwUnitCarry(int iID) : TwUnitData(iID)
{

}

TwUnitCarry::~TwUnitCarry()
{

}

BeItem* TwUnit::AddItem(int iTypeID, int iPos, int iForceID, int iOrgData)
{
	auto& pkRes = ItemTableMgr::Get()->GetItemTable(iTypeID);
	if (!pkRes)
	{
		return nullptr;
	}

	int iItemPos = -1;
	if (iPos >= 0 && !m_apkItem[iPos])
	{
		iItemPos = iPos;
	}
	else
	{
		//for (int i = 0; i < UNIT_MAX_ITEM; i++)
		//{
		//	if (!m_apkItem[i])
		//	{
		//		iItemPos = i;
		//		break;
		//	}
		//}
	}

	if (iItemPos != -1)
	{
		int iID = iForceID;
		if (iID == 0)
		{
			iID = gMain->GenerateID(BeGenIDType::GIT_CARRY);
		}
		else
		{
			gMain->SetGenerateID(BeGenIDType::GIT_CARRY, iID);
		}
		auto pkItem = std::shared_ptr<BeItem>(mpItem.alloc(iID));
		pkItem->AttachMain(pkAttachMain);
		pkItem->Initialize(iTypeID);
		pkItem->SetPileCount(pkRes->uiOrgPileCount);
		pkItem->SetUseCount(pkRes->uiOrgUseCount);
		pkItem->SetData(iOrgData);
		m_apkItem[iItemPos] = pkItem;
		pkItem->SetPackagePos(iItemPos);

		UpdateAttribute(true);

		int		iLastRegSkill = 0;
		for (int i = 0; i < 7; ++i)
		{
			int iSkillID = pkRes->iItemSkill[i];
			if (iSkillID == 0)
			{
				break;
			}
			if (iSkillID == iLastRegSkill)
			{
				break;
			}
			iLastRegSkill = iSkillID;

			auto& pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillID);
			if (!pkSkillRes)
			{
				break;
			}

			//if (pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_GUANGHUAN)
			//{
			//	SetFlag(BUF_HASHALOSKILL);
			//}

			if (pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_CDTRIGGER)
			{
				SetFlag(BUF_NEEDUPDATEITEMSKILL);
			}

			//gMain->InitItemEventTriggerT(gMain->GetSkillOrgTypeID(iSkillID));
			//gMain->InitItemEventTriggerT(iSkillID);
		}

		OnItemUpDate(pkItem->GetID());

		return pkItem.get();
	}

	return nullptr;
}

void TwUnit::OnItemUpDate(int iID)
{
	int iTypeID = 0;
	BeItem* pkItem = GetItemByID(iID);
	if (pkItem)
	{
		pkItem->AttachUnit(std::shared_ptr<TwUnit>(this));
	}

	TwPtParam kParamPre;
	kParamPre.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParamPre.SetParam(TwTrgParamID::BTP_iItemID, iID);

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ADD_ITEMPRE, kParamPre);

	if (pkItem)
	{
		iTypeID = pkItem->GetTypeID();
		UpdateAttribute(pkItem->HasFlag(BCF_HAS_NORMAL_ATTR));
	}

	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParam.SetParam(TwTrgParamID::BTP_iItemTypeID, iTypeID);
	kParam.SetParam(TwTrgParamID::BTP_iItemID, iID);

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ADD_ITEM, kParam);
}

BeItem* TwUnit::GetItemByID(int iID) const
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	if (m_apkItem[i] && m_apkItem[i]->GetID() == iID)
	//	{
	//		return m_apkItem[i];
	//	}
	//}

	return nullptr;
}

BeItem* TwUnit::GetItemByPos(int iPos)
{
	//if (iPos >= 0 && iPos < UNIT_MAX_ITEM)
	//{
	//	if (m_apkItem[iPos])
	//	{
	//		return m_apkItem[iPos];
	//	}
	//}
	return nullptr;
}

BeItem* TwUnit::GetItemByTypeID(int iTypeID, int iOwnPlayer)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//{
	//	if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
	//	{
	//		if (iOwnPlayer != -1)
	//		{
	//			if (m_apkItem[i]->GetOwnerPlay() == iOwnPlayer)
	//			{
	//				return m_apkItem[i];
	//			}
	//		}
	//		else
	//		{
	//			return m_apkItem[i];
	//		}
	//	}
	//}
	return nullptr;
}

BeItem* TwUnit::GetComposeItemByTypeID(int iTypeID)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//{
	//	if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
	//	{
	//		if (!m_apkItem[i]->IsComposeUse())
	//		{
	//			return m_apkItem[i];
	//		}
	//	}
	//}
	return nullptr;
}

void TwUnit::GetItemGroupByTypeID(std::vector<BeItem*>& rkItems, int iTypeID, int iOwnPlayer)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//{
	//	if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
	//	{
	//		if (iOwnPlayer != -1)
	//		{
	//			if (m_apkItem[i]->GetOwnerPlay() == iOwnPlayer)
	//			{
	//				rkItems.push_back(m_apkItem[i]);
	//			}
	//		}
	//		else
	//		{
	//			rkItems.push_back(m_apkItem[i]);
	//		}
	//	}
	//}
}

int TwUnit::GetItemNumsByID(int iTypeID) const
{
	int iNums = 0;
	//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//{
	//	if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
	//	{
	//		iNums += m_apkItem[i]->GetPileCount();
	//	}
	//}
	return iNums;
}

void TwUnit::DropItemByPos(int iPos, float iX, float iY, bool bForce)
{
	if (HasUnitCarryFlag(BUCF_ONLYCANUSEITEM))
	{
		return;
	}
}

void TwUnit::DelItemByPos(int iPos)
{
	//if (iPos >= 0 && iPos < UNIT_MAX_ITEM && m_apkItem[iPos])
	//{
	//	int iItemID = m_apkItem[iPos]->GetID();
	//	int iTypeID = m_apkItem[iPos]->GetTypeID();
	//	bool bValid = m_apkItem[iPos]->GetValid();
	//	m_apkItem[iPos] = nullptr;

	//	TePtParam kParam;
	//	kParam.SetParam(BTP_pkTrgUnit, this);
	//	kParam.SetParam(BTP_iItemID, iItemID);
	//	kParam.SetParam(BTP_iItemTypeID, iTypeID);
	//	gTrgMgr.FireTrigger(BTE_UNIT_DROP_ITEM, kParam);

	//	UpdateAttribute(true);
	//}
}

void TwUnit::DelAllItem(void)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	BeItem* pkItem = m_apkItem[i];
	//	if (pkItem)
	//	{
	//		DelItemByPos(pkItem->GetPackagePos());
	//	}
	//}
}

void TwUnit::DelItemByTypeID(int iTypeID, bool bFirstOnly)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	BeItem* pkItem = m_apkItem[i];
	//	if (pkItem && pkItem->GetTypeID() == iTypeID)
	//	{
	//		DelItemByPos(pkItem->GetPackagePos());
	//		if (bFirstOnly)
	//		{
	//			return;
	//		}
	//	}
	//}
}

bool TwUnit::HasItem(int iTypeID, bool bOwner)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	BeItem* pkItem = m_apkItem[i];
	//	if (pkItem && pkItem->GetTypeID() == iTypeID)
	//	{
	//		if (bOwner)
	//		{
	//			if (pkItem->GetOwnerPlay() == GetPlayer())
	//			{
	//				return true;
	//			}
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//}
	return false;
}

int TwUnit::HasItemNum(int iTypeID, bool bOwner)
{
	int iTotal = 0;
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	BeItem* pkItem = m_apkItem[i];
	//	if (pkItem && pkItem->GetTypeID() == iTypeID)
	//	{
	//		int iOrgPileCount = pkItem->GetOrgPileCount();
	//		if (bOwner)
	//		{
	//			if (pkItem->GetOwnerPlay() == GetPlayer())
	//			{
	//				if (iOrgPileCount >= 1)
	//				{
	//					iTotal += pkItem->GetPileCount() / iOrgPileCount;
	//				}
	//				else
	//				{
	//					iTotal += 1;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			if (iOrgPileCount >= 1)
	//			{
	//				iTotal += pkItem->GetPileCount() / iOrgPileCount;
	//			}
	//			else
	//			{
	//				iTotal += 1;
	//			}
	//		}
	//	}
	//}

	return iTotal;
}

bool TwUnit::HasItemSkill(int iTypeID)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	BeItem* pkItem = m_apkItem[i];
	//	if (pkItem /*&& pkItem->GetOwnerPlay() == GetPlayer()*/)
	//	{
	//		const ItemTable* pkItemRes = ItemTableMgr::Get()->GetItemTable(pkItem->GetTypeID());
	//		if (!pkItemRes)
	//		{
	//			continue;
	//		}

	//		for (int j = 0; j < 6; ++j)
	//		{
	//			int iSkill = pkItemRes->iItemSkill[j];
	//			if (iSkill == iTypeID)
	//			{
	//				return true;
	//			}
	//		}
	//	}
	//}
	return false;
}

int TwUnit::GetFreeItemPos()
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	if (!m_apkItem[i])
	//	{
	//		return i;
	//	}
	//}

	return -1;
}

bool TwUnit::IsPackageFull(void) const
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	if (!m_apkItem[i])
	//	{
	//		return false;
	//	}
	//}
	return true;
}

bool TwUnit::IsPackageEmpty(void) const
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	if (m_apkItem[i])
	//	{
	//		return false;
	//	}
	//}

	return true;
}

bool TwUnit::PickAroundItem(void)
{
	if (IsDead())
	{
		return false;
	}

	//std::vector<BeMapItem*> kMapItemGroup;
	//gMapItemMgr.GetAreaGroup(kMapItemGroup, GetPosX(), GetPosY(), 350.0f, -1);
	//if (!kMapItemGroup.empty())
	//{
	//	for (int i = 0; i < (int)kMapItemGroup.size(); ++i)
	//	{
	//		BeMapItem* pkMapItem = kMapItemGroup[i];
	//		if (pkMapItem)
	//		{
	//			PickMapItem(pkMapItem);
	//		}
	//	}
	//}

	return true;
}

BeSkill* TwUnit::AddSkill(int iTypeID, int iLevel, bool bCurrent, bool bGenus, bool bGenusSkill, int iForceID)
{
	auto& pkRes = SkillTableMgr::Get()->GetSkillTable(iTypeID);
	if (!pkRes)
	{
		return nullptr;
	}

	auto pkOldSkill = GetSkill(iTypeID);
	if (pkOldSkill)
	{
		return pkOldSkill;
	}

	//gMain->InitSkillEventTrigger(gMain->GetSkillOrgTypeID(iTypeID));
	//gMain->InitSkillEventTrigger(iTypeID);

	std::shared_ptr<BeSkill> pkSkill;
	std::shared_ptr<BeSkill> pkRet;

	int iID = iForceID;
	if (iID == 0)
	{
		iID = gMain->GenerateID(BeGenIDType::GIT_CARRY);
	}
	else
	{
		gMain->SetGenerateID(BeGenIDType::GIT_CARRY, iID);
	}
	pkSkill.reset(mpSkill.alloc(iID));
	pkSkill->AttachMain(pkAttachMain);
	pkSkill->AttachUnit(std::shared_ptr<TwUnit>(this));
	pkSkill->Initialize(iTypeID);
	pkRet = pkSkill;

	if (pkRes->iUIShowPos - 1 >= 0 && pkRes->iUIShowPos - 1 < iMaxHeroSkillNum)
	{
		bool bAdd = false;
		int iCount = (int)m_akUnitData.size();
		for (int i = iCount - 1; i >= 0; --i)
		{
			auto pkOldSkill = (m_akUnitData[i]->apkUISkill)[pkRes->iUIShowPos - 1];
			if (pkOldSkill)
			{
				if (pkOldSkill->GetTypeID() == iTypeID)
				{
					pkRet = pkOldSkill;
				}
			}
			else
			{
				bAdd = true;
				pkSkill->SetUnitSkillType(OtherSkill);
				if (i == 0)
				{
					pkSkill->SetUnitSkillType(UIShowSkill);
				}
				(m_akUnitData[i]->apkUISkill)[pkRes->iUIShowPos - 1] = pkSkill;
			}
		}
		if (!bAdd)
		{
			if (pkRet == pkSkill)
			{
				pkRet = nullptr;
			}
			pkSkill = nullptr;
		}
	}
	else
	{
		bool bAdd = false;
		for (auto itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			auto pkOldSkill = *itr;
			if (pkOldSkill->GetTypeID() == iTypeID)
			{
				bAdd = true;
				pkRet = pkOldSkill;
				break;
			}
		}
		if (!bAdd)
		{
			pkSkill->SetUnitSkillType(UnShowSkill);
			m_apkNormalSkill.push_back(pkSkill);
		}
		else
		{
			if (pkRet == pkSkill)
			{
				pkRet = nullptr;
			}
			pkSkill = nullptr;
		}
	}

	if (!pkRet)
	{
		return nullptr;
	}
	pkSkill = pkRet;
	pkSkill->SetLevel(iLevel);
	UpdateAttribute(true);

	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
	kParam.SetParam(TwTrgParamID::BTP_iIsLearnSkill, 1);

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_LEARN_SPELL, kParam);

	if (pkSkill->IsHaloSkill())
	{
		SetFlag(BUF_HASHALOSKILL);
	}

	if (pkRes->iCoolDown[iLevel - 1] > 0)
	{
		SetFlag(BUF_NEEDUPDATESKILL);
	}

	return pkSkill.get();
}

int TwUnit::GetCanLearnSkillByIdx(int iPos) const
{
	return 0;
}

int TwUnit::GetCanLearnSkillIDByPos(int iPos) const
{
	return 0;
}

BeSkill* TwUnit::GetSkill(int iTypeID, bool bCurrent) const
{
    std::shared_ptr<BeUnitData> pkUnitData;
	if (bCurrent)
	{
		pkUnitData = m_pkCurData;
	}
	else
	{
		pkUnitData = m_pkBackData;
	}
	if (!pkUnitData)
	{
		return nullptr;
	}
	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		auto pkSkill = (pkUnitData->apkUISkill)[i];
		if (pkSkill && pkSkill->GetTypeID() == iTypeID)
		{
			return pkSkill.get();
		}
	}
	if (!m_apkNormalSkill.empty())
	{
		for (auto itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			auto pkSkill = *itr;
			if (pkSkill->GetTypeID() == iTypeID)
			{
				return pkSkill.get();
			}
		}
	}

	return nullptr;
}

BeSkill* TwUnit::GetSkillByPos(int iPos, bool bOrg)
{
	if (m_pkCurData && iPos >= 0 && iPos < iMaxHeroSkillNum)
	{
		auto pkOldSkill = (m_pkCurData->apkUISkill)[iPos];
		if (pkOldSkill)
		{
			return pkOldSkill.get();
		}
	}

	return nullptr;
}

BeSkill* TwUnit::GetSkillByUIPos(int iPos) const
{
	if (m_pkCurData)
	{
		for (int iIdx = 0; iIdx < iMaxHeroSkillNum; iIdx++)
		{
            auto pkSkill = (m_pkCurData->apkUISkill)[iIdx];
			if (pkSkill && pkSkill->GetUIShowPos() == iPos)
			{
				return pkSkill.get();
			}
		}
	}
	return nullptr;
}

void TwUnit::DelSkill(int iTypeID)
{
	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		auto pkSkill = (m_pkCurData->apkUISkill)[i];
		if (pkSkill && pkSkill->GetTypeID() == iTypeID)
		{
			if (!m_apkNormalSkill.empty())
			{
				for (auto itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
				{
                    auto pkNormalSkill = *itr;
					if (pkNormalSkill && pkNormalSkill == pkSkill)
					{
						m_apkNormalSkill.erase(itr);
						pkNormalSkill = nullptr;
						break;
					}
				}
			}

			bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
			(m_pkCurData->apkUISkill)[i] = nullptr;
			for (int j = 0; j < (int)m_akUnitData.size(); ++j)
			{
				if (m_akUnitData[j]->apkUISkill[i] == pkSkill)
				{
					m_akUnitData[j]->apkUISkill[i] = nullptr;
				}
			}
			UpdateAttribute(true);
			return;
		}
	}

	if (!m_apkNormalSkill.empty())
	{
		for (auto itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
            auto pkSkill = *itr;
			if (pkSkill && pkSkill->GetTypeID() == iTypeID)
			{
				bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
				pkSkill = nullptr;
				m_apkNormalSkill.erase(itr);
				UpdateAttribute(true);
				return;
			}
		}
	}
}

void TwUnit::DelAllSkill(bool bHeroSkillOnly)
{
	//for (auto& pkSkill : m_pkCurData->apkUISkill)
	//{
	//	if (pkSkill)
	//	{
	//		if (!m_apkNormalSkill.empty())
	//		{
	//			bool bHasPoint = false;
	//			for (auto itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
	//			{
	//				auto pkNormalSkill = *itr;
	//				if (pkNormalSkill && pkNormalSkill == pkSkill)
	//				{
	//					(m_pkCurData->apkUISkill)[i] = nullptr;
	//					bHasPoint = true;
	//					break;
	//				}
	//			}

	//			if (bHasPoint)
	//			{
	//				continue;
	//			}
	//		}

	//		int iSkillTypeID = pkSkill->GetTypeID();

	//		bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
	//		(m_pkCurData->apkUISkill)[i] = nullptr;

	//		for (int j = 0; j < (int)m_akUnitData.size(); ++j)
	//		{
	//			if (m_akUnitData[j]->apkUISkill[i] == pkSkill)
	//			{
	//				m_akUnitData[j]->apkUISkill[i] = nullptr;
	//			}
	//		}
	//		UpdateAttribute(true);
	//	}
	//}

	if (bHeroSkillOnly)
	{
		DelAllBuffer();
	}

	if (!bHeroSkillOnly && !m_apkNormalSkill.empty())
	{
		for (auto itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			auto pkSkill = *itr;
			if (pkSkill)
			{
				bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
				pkSkill = nullptr;
				UpdateAttribute(true);
			}
		}
		m_apkNormalSkill.clear();
	}
	SetUsedSkillPoint(0);
}

void TwUnit::SetSkillLevel(int iTypeID, int iLevel)
{
	auto pkSkill = GetSkill(iTypeID);
	if (pkSkill)
	{
		pkSkill->SetLevel(iLevel);
		UpdateAttribute(true);
	}
}

int TwUnit::GetSkillLevel(int iTypeID)
{
	auto pkSkill = GetSkill(iTypeID);
	if (pkSkill)
	{
		return pkSkill->GetLevel();
	}
	return 0;
}

const std::vector<std::shared_ptr<BeSkill>>& TwUnit::GetNormalSkill(void)
{
	return m_apkNormalSkill;
}

int	TwUnit::GetItemUseSkill(int iItemTypeID)
{
	auto& pkItemRes = ItemTableMgr::Get()->GetItemTable(iItemTypeID);
	if (!pkItemRes)
	{
		return 0;
	}

	for (int i = 0; i < 6; i++)
	{
		int iItemSkillTypeID = pkItemRes->iItemSkill[i];
		auto& pkSkillTable = SkillTableMgr::Get()->GetSkillTable(iItemSkillTypeID);
		if (pkSkillTable)
		{
			if (pkSkillTable->uiOperateType != SKILL_OPERATETYPE_PASSIVE)
			{
				return iItemSkillTypeID;
			}
		}
	}
	return 0;
}

int TwUnit::GetItemSkillTypeID(int iItemID)
{
	BeItem* pkItem = GetItemByID(iItemID);
	if (!pkItem)
	{
		return 0;
	}
	auto& pkItemRes = ItemTableMgr::Get()->GetItemTable(pkItem->GetTypeID());
	if (!pkItemRes)
	{
		return 0;
	}

	int iSkillTypeID = 0;
	for (int i = 0; i < 7; ++i)
	{
		int iTypeID = 0;
		if (pkItemRes)
		{
			iTypeID = pkItemRes->iItemSkill[i];
		}
		auto& pkRes = SkillTableMgr::Get()->GetSkillTable(iTypeID);
		if (pkRes)
		{
			if (pkRes->uiOperateType != SKILL_OPERATETYPE_PASSIVE)
			{
				iSkillTypeID = iTypeID;
				break;
			}
		}
	}
	auto& pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (!pkSkillRes)
	{
		return 0;
	}
	return iSkillTypeID;
}

bool TwUnit::IsForbidSkill(BeSkill* pkSkill) const
{
	if (!pkSkill)
	{
		return false;
	}

	bool bForbid = false;
	if (HasUnitCarryFlag(BUCF_ISFORBIDSKILL))
	{
		if (pkSkill->GetCanForbid())
		{
			bForbid = true;
		}

		if (pkSkill->GetOperateType() == SKILL_OPERATETYPE_PASSIVE)
		{
			bForbid = false;
		}
	}

	return bForbid;
}

BeBuffer* TwUnit::AddBufferBegin(int iTypeID, int iOrgUnitID, int iLevel, int iUnitID, bool bAddDead, int iForceID)
{
	auto& pkRes = BufferTableMgr::Get()->GetBufferTable(iTypeID);
	if (!pkRes)
	{
		return nullptr;
	}
	//if (!bAddDead && !(pkRes->uiProperty & BUFFER_PROPERTY_DEATHADD) && IsDead())
	//{
	//	return nullptr;
	//}
	if (m_apkBuffer.size() >= UNIT_MAX_BUFFER || HasUnitCarryFlag(BUCF_NO_BUFFER_EFFECT))
	{
		return nullptr;
	}

	BeBuffer* pkRet = 0;
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && !pkBuffer->GetHasDel())
			{
				//if (pkBuffer->GetSingleSpliceNum() == 1 && pkBuffer->GetMultiSpliceNum() == 1)
				{
					pkBuffer->SetHasDel(false);
					pkRet = pkBuffer.get();
					pkRet->SetLevel(iLevel);

					break;
				}
				//else
				//{
				//	BeBuffer* pkNewBuffer = SuperposeBuffer(iTypeID, pkBuffer->GetSingleSpliceNum(), pkBuffer->GetMultiSpliceNum(), iLevel, iUnitID);
				//	if (!pkNewBuffer)
				//	{
				//		return nullptr;
				//	}
				//	pkRet = pkNewBuffer;
				//	pkRet->SetLevel(iLevel);

				//	m_apkBuffer.push_back(pkRet);

				//	break;
				//}
			}
		}
	}

	if (!pkRet)
	{
		int iID = iForceID;
		if (iID == 0)
		{
			iID = gMain->GenerateID(BeGenIDType::GIT_CARRY);
		}
		else
		{
			gMain->SetGenerateID(BeGenIDType::GIT_CARRY, iID);
		}
		pkRet = mpBuffer.alloc(iID);
		if (!pkRet)
		{
			return pkRet;
		}
		pkRet->AttachMain(pkAttachMain);
		pkRet->AttachUnit(std::shared_ptr<TwUnit>(this));

		pkRet->SetUnitID(iUnitID);
		pkRet->SetOrgUnitID(iOrgUnitID);
		pkRet->Initialize(iTypeID);
		pkRet->SetRemoveTime(-1);
		pkRet->SetLevel(iLevel);
		pkRet->SetStartTime((int)gTime);

		m_apkBuffer.push_back(std::shared_ptr<BeBuffer>(pkRet));
	}

	pkRet->SetDecreased(false);

	return pkRet;
}

void TwUnit::AddBufferEnd(BeBuffer* pkBuffer)
{
	if (!pkBuffer || HasUnitCarryFlag(BUCF_NO_BUFFER_EFFECT))
	{
		return;
	}

	if (HasUnitCarryFlag(BUCF_IMMUNITY_CONTROL))
	{
		pkBuffer->ClrCarryFlag(BUCF_DIZZY | BUCF_ISFORBIDITEM | BUCF_ISFORBIDSKILL | BUCF_CANNOTMOVE);
	}

	int iRemoveTime = (int)pkBuffer->GetRemoveTime();

	OnAddBuffer(pkBuffer);

	BeShareBufferData	kData;
	kData.bRemove = false;
	kData.iBufferLogicID = pkBuffer->GetID();
	kData.iBufferTypeID = pkBuffer->GetTypeID();
	kData.iRemoveTime = pkBuffer->GetRemoveTime();
	kData.iUnitID = GetID();
	kData.byNum = pkBuffer->GetLevel();
	kData.iAttackUnitID = pkBuffer->GetOrgUnitID();

	//gMain->AddBufferShowData(kData);
}

BeBuffer* TwUnit::GetBuffer(int iTypeID, int iUnitID)
{
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID) && (pkBuffer->GetRemoveTime() > gTime || pkBuffer->GetRemoveTime() == -1))
			{
				return pkBuffer.get();
			}
		}
	}

	return nullptr;
}

void	TwUnit::RemoveBufferAttr(BeNormalAttType eType)
{
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (pkBuffer)
			{
				float	fValue = pkBuffer->GetNormalAttrValue(eType);
				if (fValue < 0.0f)
				{
					pkBuffer->SetNormalAttr(eType, 0.0f, true);
				}
			}
		}
	}
}

const BeBuffer* TwUnit::GetBuffer(int iTypeID, int iUnitID) const
{
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
            auto pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID) && (pkBuffer->GetRemoveTime() > gTime || pkBuffer->GetRemoveTime() == -1)) //
			{
				return pkBuffer.get();
			}
		}
	}

	return nullptr;
}

bool TwUnit::DelBuffer(int iTypeID, int iUnitID)
{
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID) && !pkBuffer->GetHasDel())
			{
				pkBuffer->SetInterrupt(true);
				OnDelBuffer(pkBuffer);
				pkBuffer->SetRemoveTime(gTime);
				return true;
			}
		}
	}

	return false;
}

bool TwUnit::DelBufferByID(int iBufferID)
{
	if (m_apkBuffer.empty())
	{
		return false;
	}

	for (int i = 0; i < (int)m_apkBuffer.size(); i++)
	{
		auto pkBuffer = m_apkBuffer[i];
		if (!pkBuffer)
		{
			continue;
		}
		if (pkBuffer->GetHasDel())
		{
			continue;
		}
		if (pkBuffer->GetID() != iBufferID)
		{
			continue;
		}
		OnDelBuffer(pkBuffer);
		pkBuffer->SetRemoveTime(gTime);
		return true;
	}

	return false;
}

void TwUnit::DelAllBuffer(bool bRelive)
{
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (!pkBuffer || bRelive && pkBuffer->IsDeadNoRemove())
			{
				continue;
			}
			OnDelBuffer(pkBuffer);
			pkBuffer->SetRemoveTime(gTime);
		}

		UpdateAttribute(true);
	}
}

void TwUnit::DelBufferByClean(bool bNegative, bool bGood)
{
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (pkBuffer && !pkBuffer->HasProperty(BUFFER_PROPERTY_NOCLEAN))
			{
				if (pkBuffer->HasProperty(BUFFER_PROPERTY_DEBUFF))
				{
					if (bNegative)
					{
						OnDelBuffer(pkBuffer);
						pkBuffer->SetRemoveTime(gTime);
					}
				}
				else
				{
					if (bGood)
					{
						OnDelBuffer(pkBuffer);
						pkBuffer->SetRemoveTime(gTime);
					}
				}
			}
		}
	}
}

void TwUnit::DelBufferByAutoClean(void)
{
	//if (!m_apkBuffer.empty())
	//{
	//	for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
	//	{
	//		auto pkBuffer = m_apkBuffer[i];
	//		if (pkBuffer && pkBuffer->HasProperty(BUFFER_PROPERTY_AUTOREMOVE))
	//		{
	//			OnDelBuffer(pkBuffer);
	//			pkBuffer->SetRemoveTime(gTime);
	//		}
	//	}
	//}
}

void TwUnit::OnAddBuffer(BeBuffer* pkBuffer)
{
	if (!pkBuffer)
	{
		return;
	}
	pkBuffer->SetHasDel(false);
	TwPtParam kParam;

	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer);
	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ADDBUFFER, kParam);

	UpdateAttribute(pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR));

	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer);

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ADDBUFFER_AFTER, kParam);

	if (pkBuffer->HasCarryFlag(BUCF_ISINVISIBLE))
	{
		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_INVISIBLE_ENTER, kParam);
	}
}

void TwUnit::OnDelBuffer(std::shared_ptr<BeBuffer> pkBuffer, bool bUpdate, bool bDelEffect)
{
	if (!pkBuffer || pkBuffer->GetHasDel())
	{
		return;
	}

	pkBuffer->SetHasDel(true);
	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_DELBUFFER, kParam);

	if (bUpdate)
	{
		UpdateAttribute(pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR));
	}

	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_DELBUFFER_AFTER, kParam);

	BeShareBufferData kData;
	kData.bRemove = true;
	kData.iBufferLogicID = pkBuffer->GetID();
	kData.iBufferTypeID = pkBuffer->GetTypeID();
	kData.iRemoveTime = pkBuffer->GetRemoveTime();
	kData.iUnitID = GetID();
	kData.iAttackUnitID = pkBuffer->GetOrgUnitID();

	//gMain->AddBufferShowData(kData);
}

void TwUnit::UpdateValidItem(void)
{
	//for (int i = 0; i < UNIT_MAX_ITEM; i++)
	//{
	//	if (m_apkItem[i] && m_apkItem[i]->GetOrgValid())
	//	{
	//		m_apkCarry.push_back(m_apkItem[i]);
	//		SetUnitCarryFlag(m_apkItem[i]->GetCarryFlag());
	//		SetUnitImmunityFlag(m_apkItem[i]->GetImmunityFlag());
	//	}
	//}
}

void TwUnit::UpdateValidBuffer(void)
{
	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (pkBuffer->GetHasDel())
			{
				continue;
			}

			if (pkBuffer->HasCarryFlag(BUCF_DIZZY) && HasOtherFlag(BUOF_CANT_DIZZY))
			{
				OnDelBuffer(pkBuffer);
				pkBuffer->SetRemoveTime(gTime);
				continue;
			}

			m_apkCarry.push_back(pkBuffer);
			SetUnitCarryFlag(pkBuffer->GetCarryFlag());
			SetUnitImmunityFlag(pkBuffer->GetImmunityFlag());
		}
	}
}

void TwUnit::UpdateValidSkill(bool bReset)
{
	//for (int i = 0; i < iMaxHeroSkillNum; i++)
	//{
	//	auto pkSkill = (m_pkCurData->apkUISkill)[i];
	//	if (pkSkill)
	//	{
	//		if (bReset)
	//		{
	//			pkSkill->SetLevel(pkSkill->GetLevel());
	//		}

	//		bool bHasSame = false;
	//		for (int j = 0; j < (int)m_apkCarry.size(); j++)
	//		{
	//			auto pkCarry = m_apkCarry[j];
	//			if (!pkCarry || pkCarry->GetID() == pkSkill->GetID())
	//			{
	//				bHasSame = true;
	//				break;
	//			}
	//		}

	//		if (bHasSame)
	//		{
	//			continue;
	//		}

	//		m_apkCarry.push_back(pkSkill);
	//		SetUnitCarryFlag(pkSkill->GetCarryFlag());
	//		SetUnitImmunityFlag(pkSkill->GetImmunityFlag());
	//	}
	//}

	//if (!m_apkNormalSkill.empty())
	//{
	//	for (auto itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
	//	{
	//		auto pkSkill = *itr;
	//		if (bReset)
	//		{
	//			pkSkill->SetLevel(pkSkill->GetLevel());
	//		}

	//		bool bHasSame = false;
	//		for (int i = 0; i < (int)m_apkCarry.size(); i++)
	//		{
	//			auto pkCarry = m_apkCarry[i];
	//			if (!pkCarry || pkCarry->GetID() == pkSkill->GetID())
	//			{
	//				bHasSame = true;
	//				break;
	//			}
	//		}

	//		if (bHasSame)
	//		{
	//			continue;
	//		}

	//		m_apkCarry.push_back(pkSkill);
	//		SetUnitCarryFlag(pkSkill->GetCarryFlag());
	//		SetUnitImmunityFlag(pkSkill->GetImmunityFlag());
	//	}
	//}
}

void TwUnit::GetAttackingAttr(std::shared_ptr<TwUnit> pkTarget, BeAttackingAttr*& rkAttackingAttr)
{
	rkAttackingAttr->eAttackType = GetAttackType();
	rkAttackingAttr->fDamage = GetDamageNum();
	rkAttackingAttr->kAttacker = m_spSharePtr;
	rkAttackingAttr->iPlayer = GetPlayer();
	rkAttackingAttr->bNeedCulculateArmor = true;
	rkAttackingAttr->fLeech = GetLeech();
	rkAttackingAttr->SetFlag(static_cast<unsigned int>(BeAttackingFlag::BAF_REBOUND));

	if (GetAttackingMiss())
	{
		rkAttackingAttr->SetFlag(static_cast<unsigned int>(BeAttackingFlag::BAF_AVOID));
		return;
	}
}

bool TwUnit::GetAttackingMiss(void) const
{
	if (!m_apkCarry.empty())
	{
		for (auto itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			auto pkCarry = *itr;
			if (pkCarry->GetAttackMissPhysic())
			{
				return true;
			}
		}
	}

	return false;
}

bool TwUnit::GetAttackedAvoid(void) const
{
	if (!m_apkCarry.empty())
	{
		float fMaxAvoidRate = GetEvadeRate();
		for (auto itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			auto pkCarry = *itr;

			if (fMaxAvoidRate < pkCarry->GetAttackedAvoidPhysic())
			{
				fMaxAvoidRate = pkCarry->GetAttackedAvoidPhysic();
			}
		}

		//if (gMain->RandFloatInRate(fMaxAvoidRate, m_iID + (int)fMaxAvoidRate))
		//{
		//	return true;
		//}
	}

	return false;
}

void TwUnit::GetAttackedPhysicAttr(float& fAntiPhysic, const std::shared_ptr<TwUnit> pkAttacker, float fDecArmorValue, float fDecArmorPer) const
{
	float fFinalAntiArmor = GetArmor() * (1.0f - fDecArmorPer) - fDecArmorValue;
	fAntiPhysic = BeFormula::GetAmorForDamage(fFinalAntiArmor);
}

void TwUnit::GetAttackedMagicAttr(float& fAntiMagic, const std::shared_ptr<TwUnit> pkAttacker, float fDecEnemyAntiMagic, float fDecMagicArmorPer) const
{
	float fFinalAntiArmor = GetMagicArmor() * (1.0f - fDecMagicArmorPer) - fDecEnemyAntiMagic;

	fAntiMagic = BeFormula::GetAmorForDamage(fFinalAntiArmor);
}

float TwUnit::GetAttackedAntiLeech(void) const
{
	float fPercent = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (auto itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			auto pkCarry = *itr;

			fPercent += pkCarry->GetAttackedAntiLeech();
		}
	}

	return fPercent;
}

float TwUnit::GetAttackedAttackCDAttr(void) const
{
	float fTotalACD = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (auto itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			auto pkCarry = *itr;
			float fAttackCD = 0.0f;
			fAttackCD = pkCarry->GetNormalAttrValue(NAT_PER_ATTACK_SPEED);
			fTotalACD += fAttackCD;
		}
	}
	return fTotalACD;
}

float TwUnit::GetAttackedBaoJiAttr(void) const
{
	float fTotalBaoji = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (auto itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			auto pkCarry = *itr;
			float fBaoji = 0.0f;
			fBaoji = pkCarry->GetNormalAttrValue(NAT_BAOJI);
			fTotalBaoji += fBaoji;
		}
	}
	return fTotalBaoji;
}
