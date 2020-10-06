/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Unit.h"

BeItem* BeUnit::AddItem(int iTypeID, int iPos, int iForceID, int iOrgData)
{
	const ItemTable* pkRes = gMain.GetResItem(iTypeID);
	if (!pkRes)
	{
		return NULL;
	}

	int iItemPos = -1;
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM && !m_apkItem[iPos])
	{
		iItemPos = iPos;
	}
	else
	{
		for (int i = 0; i < UNIT_MAX_ITEM; i++)
		{
			if (!m_apkItem[i])
			{
				iItemPos = i;
				break;
			}
		}
	}

	if (iItemPos != -1)
	{
		int iID = iForceID;
		if (iID == 0)
		{
			iID = gMain.GenerateID(GIT_CARRY);
		}
		else
		{
			gMain.SetGenerateID(GIT_CARRY, iID);
		}
		BeItem* pkItem = new BeItem(iID);
		pkItem->AttachMain(pkAttachMain);
		pkItem->Initialize(iTypeID);
		pkItem->CheckItemSkillCD(this);
		pkItem->SetPileCount(pkRes->iOrgPileCount);
		pkItem->SetUseCount(pkRes->iOrgUseCount);
		pkItem->SetData(iOrgData);
		m_apkItem[iItemPos] = pkItem;
		pkItem->SetPackagePos(iItemPos);

		switch (iItemPos)
		{
		case 0:SetTabInfoFlag(BTCF_ITEM1); break;
		case 1:SetTabInfoFlag(BTCF_ITEM2); break;
		case 2:SetTabInfoFlag(BTCF_ITEM3); break;
		case 3:SetTabInfoFlag(BTCF_ITEM4); break;
		case 4:SetTabInfoFlag(BTCF_ITEM5); break;
		case 5:SetTabInfoFlag(BTCF_ITEM6); break;
		default:
			break;
		}

		UpdateAttribute(true);

		int		iLastRegSkill = 0;
		for (int i = 0; i < ITEM_MAX_SKILL; ++i)
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

			const SkillTable* pkSkillRes = gMain.GetResSkill(iSkillID);
			if (!pkSkillRes)
			{
				break;
			}

			if (pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_GUANGHUAN)
			{
				SetFlag(BUF_HASHALOSKILL);
			}

			if (pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_CDCHUFA)
			{
				SetFlag(BUF_NEEDUPDATEITEMSKILL);
			}

			gMain.InitItemEventTriggerT(gMain.GetSkillOrgTypeID(iSkillID));
			gMain.InitItemEventTriggerT(iSkillID);
			gMain.InitItemEventTriggerZhanChang(gMain.GetSkillOrgTypeID(iSkillID));
			gMain.InitItemEventTriggerZhanChang(iSkillID);
		}

		OnItemUpDate(pkItem->GetID());

		return pkItem;
	}

	return NULL;
}

BeItem* BeUnit::AddZItem(int iTypeID, int iUniqueID, int iPos)
{
	const ZhanChangItemTable* pkZRes = gMain.GetResZhanChangItem(iTypeID);
	if (!pkZRes)
	{
		return NULL;
	}

	if (iPos != -1)
	{
		int iID = gMain.GenerateID(GIT_CARRY);
		BeItem* pkItem = new BeItem(iID);
		pkItem->AttachMain(pkAttachMain);
		pkItem->Initialize(iTypeID);
		pkItem->SetPileCount(pkZRes->iOrgPileCount);
		pkItem->SetUseCount(pkZRes->iOrgUseCount);
		pkItem->SetUniqueID(iUniqueID);
		pkItem->SetZhanChangResPtr(pkZRes);
		m_apkItem[iPos] = pkItem;
		pkItem->SetPackagePos(iPos);

		switch (iPos)
		{
		case 0:SetTabInfoFlag(BTCF_ITEM1); break;
		case 1:SetTabInfoFlag(BTCF_ITEM2); break;
		case 2:SetTabInfoFlag(BTCF_ITEM3); break;
		case 3:SetTabInfoFlag(BTCF_ITEM4); break;
		case 4:SetTabInfoFlag(BTCF_ITEM5); break;
		case 5:SetTabInfoFlag(BTCF_ITEM6); break;
		default:
			break;
		}

		UpdateAttribute(true);

		int		iLastRegSkill = 0;
		for (int i = 0; i < ITEM_MAX_SKILL; ++i)
		{
			int iSkillID = pkZRes->iItemSkill[i];
			if (iSkillID == 0)
			{
				break;
			}
			if (iSkillID == iLastRegSkill)
			{
				break;
			}
			iLastRegSkill = iSkillID;

			const SkillTable* pkSkillRes = gMain.GetResSkill(iSkillID);
			if (!pkSkillRes)
			{
				break;
			}

			if (pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_GUANGHUAN)
			{
				SetFlag(BUF_HASHALOSKILL);
			}

			if (pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_CDCHUFA)
			{
				SetFlag(BUF_NEEDUPDATEITEMSKILL);
			}

			gMain.InitItemEventTriggerT(gMain.GetSkillOrgTypeID(iSkillID));
			gMain.InitItemEventTriggerT(iSkillID);
			gMain.InitItemEventTriggerZhanChang(gMain.GetSkillOrgTypeID(iSkillID));
			gMain.InitItemEventTriggerZhanChang(iSkillID);
		}

		OnItemUpDate(pkItem->GetID());

		return pkItem;
	}

	return NULL;
}

void BeUnit::OnItemUpDate(int iID)
{
	int iTypeID = 0;
	BeItem* pkItem = GetItemByID(iID);
	if (pkItem)
	{
		pkItem->AttachUnit(this);
	}

	TePtParam kParamPre;
	kParamPre.SetParam(BTP_pkTrgUnit, this);
	kParamPre.SetParam(BTP_iItemID, iID);

	gTrgMgr.FireTrigger(BTE_UNIT_ADD_ITEMPRE, kParamPre);

	if (pkItem)
	{
		iTypeID = pkItem->GetTypeID();
		UpdateAttribute(pkItem->HasFlag(BCF_HAS_NORMAL_ATTR));
	}

	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_iItemTypeID, iTypeID);
	kParam.SetParam(BTP_iItemID, iID);

	gTrgMgr.FireTrigger(BTE_UNIT_ADD_ITEM, kParam);

	BeLevelMain* pkTriggerMain = (BeLevelMain*)gMain.GetTaskHandle();
	EsAssert(pkTriggerMain);
	BeTDEventArgs kArgs = BeTDEventArgs();
	kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITADDITEM;
	kArgs.kUnitAddItem.iUnitID = GetID();
	kArgs.kUnitAddItem.iItemTypeID = iTypeID;
	pkTriggerMain->GetEventMgr()->FireEvent(kArgs);
}

BeItem* BeUnit::GetItemByID(int iID) const
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetID() == iID)
		{
			return m_apkItem[i];
		}
	}

	return NULL;
}

BeItem* BeUnit::GetZItemByUniqueID(int iUniqueID) const
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetUniqueID() == iUniqueID)
		{
			return m_apkItem[i];
		}
	}

	return NULL;
}

int BeUnit::GetZItemPosByUniqueID(int iUniqueID)
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetUniqueID() == iUniqueID)
		{
			return i;
		}
	}
	return -1;
}

BeItem* BeUnit::GetItemByPos(int iPos)
{
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM)
	{
		if (m_apkItem[iPos])
		{
			return m_apkItem[iPos];
		}
	}
	return NULL;
}

BeItem* BeUnit::GetZItemByPos(int iPos)
{
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM)
	{
		if (m_apkItem[iPos])
		{
			return m_apkItem[iPos];
		}
	}
	return NULL;
}

unsigned	int	BeUnit::GetItemTypeIDByPos(INT iPos)
{
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM)
	{
		if (m_apkItem[iPos])
		{
			return m_apkItem[iPos]->GetTypeID();
		}
	}
	return 0;
}

unsigned	int	BeUnit::GetZItemTypeIDByPos(INT iPos)
{
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM)
	{
		if (m_apkItem[iPos])
		{
			return m_apkItem[iPos]->GetTypeID();
		}
	}
	return 0;
}

unsigned	int	BeUnit::GetZItemUniqueIDByPos(INT iPos)
{
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM)
	{
		if (m_apkItem[iPos])
		{
			return m_apkItem[iPos]->GetUniqueID();
		}
	}
	return 0;
}

bool BeUnit::SwapZItemPos(Byte bySrcPos, Byte byTarPos)
{
	if (bySrcPos < 0 || bySrcPos >= UNIT_MAX_ITEM || byTarPos < 0 || byTarPos >= UNIT_MAX_ITEM || bySrcPos == byTarPos)
	{
		return false;
	}

	BeItem* pkTempItem = m_apkItem[bySrcPos];
	m_apkItem[bySrcPos] = m_apkItem[byTarPos];
	m_apkItem[byTarPos] = pkTempItem;
	if (m_apkItem[bySrcPos])
	{
		m_apkItem[bySrcPos]->SetPackagePos(bySrcPos);
	}
	if (m_apkItem[byTarPos])
	{
		m_apkItem[byTarPos]->SetPackagePos(byTarPos);
	}
	switch (bySrcPos)
	{
	case 0:SetTabInfoFlag(BTCF_ITEM1); break;
	case 1:SetTabInfoFlag(BTCF_ITEM2); break;
	case 2:SetTabInfoFlag(BTCF_ITEM3); break;
	case 3:SetTabInfoFlag(BTCF_ITEM4); break;
	case 4:SetTabInfoFlag(BTCF_ITEM5); break;
	case 5:SetTabInfoFlag(BTCF_ITEM6); break;
	default:
		break;
	}
	switch (byTarPos)
	{
	case 0:SetTabInfoFlag(BTCF_ITEM1); break;
	case 1:SetTabInfoFlag(BTCF_ITEM2); break;
	case 2:SetTabInfoFlag(BTCF_ITEM3); break;
	case 3:SetTabInfoFlag(BTCF_ITEM4); break;
	case 4:SetTabInfoFlag(BTCF_ITEM5); break;
	case 5:SetTabInfoFlag(BTCF_ITEM6); break;
	default:
		break;
	}

	return true;
}

BeItem* BeUnit::GetItemByTypeID(int iTypeID, int iOwnPlayer)
{
	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
		{
			if (iOwnPlayer != -1)
			{
				if (m_apkItem[i]->GetOwnerPlay() == iOwnPlayer)
				{
					return m_apkItem[i];
				}
			}
			else
			{
				return m_apkItem[i];
			}
		}
	}
	return NULL;
}

BeItem* BeUnit::GetComposeItemByTypeID(int iTypeID)
{
	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
		{
			if (!m_apkItem[i]->IsComposeUse())
			{
				return m_apkItem[i];
			}
		}
	}
	return NULL;
}

void BeUnit::GetItemGroupByTypeID(std::vector<BeItem*>& rkItems, int iTypeID, int iOwnPlayer)
{
	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
		{
			if (iOwnPlayer != -1)
			{
				if (m_apkItem[i]->GetOwnerPlay() == iOwnPlayer)
				{
					rkItems.push_back(m_apkItem[i]);
				}
			}
			else
			{
				rkItems.push_back(m_apkItem[i]);
			}
		}
	}
}

int BeUnit::GetItemNumsByID(int iTypeID) const
{
	int iNums = 0;
	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetTypeID() == iTypeID)
		{
			iNums += m_apkItem[i]->GetPileCount();
		}
	}
	return iNums;
}

bool BeUnit::SwapItemPos(Byte bySrcPos, Byte byTarPos)
{
	if (bySrcPos < 0 || bySrcPos >= UNIT_MAX_ITEM || byTarPos < 0 || byTarPos >= UNIT_MAX_ITEM || bySrcPos == byTarPos)
	{
		return false;
	}

	BeItem* pkTempItem = m_apkItem[bySrcPos];
	m_apkItem[bySrcPos] = m_apkItem[byTarPos];
	m_apkItem[byTarPos] = pkTempItem;
	if (m_apkItem[bySrcPos])
	{
		m_apkItem[bySrcPos]->SetPackagePos(bySrcPos);
	}
	if (m_apkItem[byTarPos])
	{
		m_apkItem[byTarPos]->SetPackagePos(byTarPos);
	}
	switch (bySrcPos)
	{
	case 0:SetTabInfoFlag(BTCF_ITEM1); break;
	case 1:SetTabInfoFlag(BTCF_ITEM2); break;
	case 2:SetTabInfoFlag(BTCF_ITEM3); break;
	case 3:SetTabInfoFlag(BTCF_ITEM4); break;
	case 4:SetTabInfoFlag(BTCF_ITEM5); break;
	case 5:SetTabInfoFlag(BTCF_ITEM6); break;
	default:
		break;
	}
	switch (byTarPos)
	{
	case 0:SetTabInfoFlag(BTCF_ITEM1); break;
	case 1:SetTabInfoFlag(BTCF_ITEM2); break;
	case 2:SetTabInfoFlag(BTCF_ITEM3); break;
	case 3:SetTabInfoFlag(BTCF_ITEM4); break;
	case 4:SetTabInfoFlag(BTCF_ITEM5); break;
	case 5:SetTabInfoFlag(BTCF_ITEM6); break;
	default:
		break;
	}

	return false;
}

void BeUnit::DropItemByPos(int iPos, float iX, float iY, bool bForce)
{
	if (HasUnitCarryFlag(BUCF_ONLYCANUSEITEM))
	{
		return;
	}
}

void BeUnit::DelItemByPos(int iPos)
{
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM && m_apkItem[iPos])
	{
		int iItemID = m_apkItem[iPos]->GetID();
		int iTypeID = m_apkItem[iPos]->GetTypeID();
		bool bValid = m_apkItem[iPos]->GetValid();
		m_apkItem[iPos] = NULL;

		TePtParam kParam;
		kParam.SetParam(BTP_pkTrgUnit, this);
		kParam.SetParam(BTP_iItemID, iItemID);
		kParam.SetParam(BTP_iItemTypeID, iTypeID);
		gTrgMgr.FireTrigger(BTE_UNIT_DROP_ITEM, kParam);

		UpdateAttribute(true);

		BeLevelMain* pkTriggerMain = (BeLevelMain*)gMain.GetTaskHandle();
		EsAssert(pkTriggerMain);
		BeTDEventArgs kArgs = BeTDEventArgs();
		kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITDROPITEM;
		kArgs.kUnitDropItem.iUnitID = GetID();
		kArgs.kUnitDropItem.iItemTypeID = iTypeID;
		pkTriggerMain->GetEventMgr()->FireEvent(kArgs);

		switch (iPos)
		{
		case 0:SetTabInfoFlag(BTCF_ITEM1); break;
		case 1:SetTabInfoFlag(BTCF_ITEM2); break;
		case 2:SetTabInfoFlag(BTCF_ITEM3); break;
		case 3:SetTabInfoFlag(BTCF_ITEM4); break;
		case 4:SetTabInfoFlag(BTCF_ITEM5); break;
		case 5:SetTabInfoFlag(BTCF_ITEM6); break;
		default:
			break;
		}
	}
}

void BeUnit::DelZItemByPos(int iPos)
{
	if (iPos >= 0 && iPos < UNIT_MAX_ITEM && m_apkItem[iPos])
	{
		int iItemID = m_apkItem[iPos]->GetID();
		int iTypeID = m_apkItem[iPos]->GetTypeID();
		m_apkItem[iPos] = NULL;

		TePtParam kParam;
		kParam.SetParam(BTP_pkTrgUnit, this);
		kParam.SetParam(BTP_iItemID, iItemID);
		kParam.SetParam(BTP_iItemTypeID, iTypeID);
		gTrgMgr.FireTrigger(BTE_UNIT_DROP_ITEM, kParam);
		//////////////////////////////////////////////////////////////////////////
		UpdateAttribute(true);

		switch (iPos)
		{
		case 0:SetTabInfoFlag(BTCF_ITEM1); break;
		case 1:SetTabInfoFlag(BTCF_ITEM2); break;
		case 2:SetTabInfoFlag(BTCF_ITEM3); break;
		case 3:SetTabInfoFlag(BTCF_ITEM4); break;
		case 4:SetTabInfoFlag(BTCF_ITEM5); break;
		case 5:SetTabInfoFlag(BTCF_ITEM6); break;
		default:
			break;
		}
	}
}

void BeUnit::DelZItemByUniqueID(int iUniqueID)
{
	int Pos = 0;
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (m_apkItem[i]->GetUniqueID() == iUniqueID)
		{
			DelZItemByPos(i);
			return;
		}
	}
}

void BeUnit::DelAllItem(void)
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		BeItem* pkItem = m_apkItem[i];
		if (pkItem)
		{
			DelItemByPos(pkItem->GetPackagePos());
		}
	}
}

void BeUnit::DelItemByTypeID(int iTypeID, bool bFirstOnly)
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		BeItem* pkItem = m_apkItem[i];
		if (pkItem && pkItem->GetTypeID() == iTypeID)
		{
			DelItemByPos(pkItem->GetPackagePos());
			if (bFirstOnly)
			{
				return;
			}
		}
	}
}

bool BeUnit::HasItem(int iTypeID, bool bOwner)
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		BeItem* pkItem = m_apkItem[i];
		if (pkItem && pkItem->GetTypeID() == iTypeID)
		{
			if (bOwner)
			{
				if (pkItem->GetOwnerPlay() == GetPlayer())
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

int BeUnit::HasItemNum(int iTypeID, bool bOwner)
{
	int iTotal = 0;
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		BeItem* pkItem = m_apkItem[i];
		if (pkItem && pkItem->GetTypeID() == iTypeID)
		{
			int iOrgPileCount = pkItem->GetOrgPileCount();
			if (bOwner)
			{
				if (pkItem->GetOwnerPlay() == GetPlayer())
				{
					if (iOrgPileCount >= 1)
					{
						iTotal += pkItem->GetPileCount() / iOrgPileCount;
					}
					else
					{
						iTotal += 1;
					}
				}
			}
			else
			{
				if (iOrgPileCount >= 1)
				{
					iTotal += pkItem->GetPileCount() / iOrgPileCount;
				}
				else
				{
					iTotal += 1;
				}
			}
		}
	}

	return iTotal;
}

bool BeUnit::HasItemSkill(int iTypeID)
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		BeItem* pkItem = m_apkItem[i];
		if (pkItem /*&& pkItem->GetOwnerPlay() == GetPlayer()*/)
		{
			const ItemTable* pkItemRes = gMain.GetResItem(pkItem->GetTypeID());
			if (!pkItemRes)
			{
				continue;
			}

			for (int j = 0; j < 6; ++j)
			{
				int iSkill = pkItemRes->iItemSkill[j];
				if (iSkill == iTypeID)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int BeUnit::GetFreeItemPos()
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (!m_apkItem[i])
		{
			return i;
		}
	}

	return -1;
}

bool BeUnit::IsPackageFull(void) const
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (!m_apkItem[i])
		{
			return false;
		}
	}
	return true;
}

bool BeUnit::IsPackageEmpty(void) const
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (m_apkItem[i])
		{
			return false;
		}
	}

	return true;
}

bool BeUnit::PickAroundItem(void)
{
	if (IsGhost() || IsDividMan() || HasOtherFlag(BUOF_72BIAN))
	{
		return false;
	}
	if (IsDead())
	{
		return false;
	}

	std::vector<BeMapItem*> kMapItemGroup;
	gMapItemMgr.GetAreaGroup(kMapItemGroup, GetPosX(), GetPosY(), 350.0f, -1);
	if (!kMapItemGroup.empty())
	{
		for (int i = 0; i < (int)kMapItemGroup.size(); ++i)
		{
			BeMapItem* pkMapItem = kMapItemGroup[i];
			if (pkMapItem)
			{
				PickMapItem(pkMapItem);
			}
		}
	}

	return true;
}

BeSkill* BeUnit::AddSkill(int iTypeID, int iLevel, bool bCurrent, bool bGenus, bool bGenusSkill, int iForceID)
{
	const SkillTable* pkRes = SkillTableMgr::Get()->GetSkillTable(iTypeID);
	if (!pkRes)
	{
		return NULL;
	}

	BeSkill* pkOldSkill = GetSkill(iTypeID);
	if (pkOldSkill)
	{
		return pkOldSkill;
	}

	gMain.InitSkillEventTrigger(gMain.GetSkillOrgTypeID(iTypeID));
	gMain.InitSkillEventTrigger(iTypeID);

	BeSkill* pkSkill = NULL;
	BeSkill* pkRet = NULL;

	int iID = iForceID;
	if (iID == 0)
	{
		iID = gMain.GenerateID(GIT_CARRY);
	}
	else
	{
		gMain.SetGenerateID(GIT_CARRY, iID);
	}
	pkSkill = BeSkill::NEW(iID);
	pkSkill->AttachMain(pkAttachMain);
	pkSkill->AttachUnit(this);
	pkSkill->Initialize(iTypeID);
	pkRet = pkSkill;

	if (pkRes->iUIShowPos - 1 >= 0 && pkRes->iUIShowPos - 1 < iMaxHeroSkillNum)
	{
		bool bAdd = false;
		int iCount = (int)m_akUnitData.size();
		for (int i = iCount - 1; i >= 0; --i)
		{
			BeSkill* pkOldSkill = (m_akUnitData[i]->apkUISkill)[pkRes->iUIShowPos - 1];
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
				pkRet = NULL;
			}
			pkSkill = NULL;
		}
	}
	else
	{
		bool bAdd = false;
		for (std::vector<BeSkill*>::iterator itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			BeSkill* pkOldSkill = *itr;
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
				pkRet = NULL;
			}
			pkSkill = NULL;
		}
	}

	if (!pkRet)
	{
		return NULL;
	}
	pkSkill = pkRet;
	pkSkill->SetLevel(iLevel);
	UpdateAttribute(true);

	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_pkSkill, pkSkill);
	kParam.SetParam(BTP_iIsLearnSkill, 1);

	gTrgMgr.FireTrigger(BTE_LEARN_SPELL, kParam);

	if (pkSkill->IsHaloSkill())
	{
		SetFlag(BUF_HASHALOSKILL);
	}

	if (pkRes->iCoolDown[iLevel - 1] > 0)
	{
		SetFlag(BUF_NEEDUPDATESKILL);
	}

	BeTDEventArgs kArgs = BeTDEventArgs();
	kArgs.iEventType = BeTDEventArgs::BeTDEVENT_SPELL_LEARN;
	kArgs.kSpellLearn.iUnitID = GetID();
	kArgs.kSpellLearn.iSkillTypeID = pkSkill->GetTypeID();
	kArgs.kSpellLearn.iSkillLevel = pkSkill->GetLevel();
	gLevelMain.GetEventMgr()->FireEvent(kArgs);

	return pkSkill;
}

int BeUnit::GetCanLearnSkillByIdx(int iPos) const
{
	return 0;
}

int BeUnit::GetCanLearnSkillIDByPos(int iPos) const
{
	return 0;
}

BeSkill* BeUnit::GetSkill(int iTypeID, bool bCurrent) const
{
	BeUnitData* pkUnitData = 0;
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
		return NULL;
	}
	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		BeSkill* pkSkill = (pkUnitData->apkUISkill)[i];
		if (pkSkill && pkSkill->GetTypeID() == iTypeID)
		{
			return pkSkill;
		}
	}
	if (!m_apkNormalSkill.empty())
	{
		for (std::vector<BeSkill*>::const_iterator itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			BeSkill* pkSkill = *itr;
			if (pkSkill->GetTypeID() == iTypeID)
			{
				return pkSkill;
			}
		}
	}

	return NULL;
}

BeSkill* BeUnit::GetSkillByPos(int iPos, bool bOrg)
{
	if (m_pkCurData && iPos >= 0 && iPos < iMaxHeroSkillNum)
	{
		BeSkill* pkOldSkill = (m_pkCurData->apkUISkill)[iPos];
		if (pkOldSkill)
		{
			return pkOldSkill;
		}
	}

	return NULL;
}

BeSkill* BeUnit::GetSkillByUIPos(int iPos) const
{
	if (m_pkCurData)
	{
		for (int iIdx = 0; iIdx < iMaxHeroSkillNum; iIdx++)
		{
			BeSkill* pkSkill = (m_pkCurData->apkUISkill)[iIdx];
			if (pkSkill && pkSkill->GetUIShowPos() == iPos)
			{
				return pkSkill;
			}
		}
	}
	return NULL;
}

void BeUnit::DelSkill(int iTypeID)
{
	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		BeSkill* pkSkill = (m_pkCurData->apkUISkill)[i];
		if (pkSkill && pkSkill->GetTypeID() == iTypeID)
		{
			if (!m_apkNormalSkill.empty())
			{
				for (std::vector<BeSkill*>::iterator itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
				{
					BeSkill* pkNormalSkill = *itr;
					if (pkNormalSkill && pkNormalSkill == pkSkill)
					{
						m_apkNormalSkill.erase(itr);
						pkNormalSkill = NULL;
						break;
					}
				}
			}

			bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
			(m_pkCurData->apkUISkill)[i] = NULL;
			for (int j = 0; j < (int)m_akUnitData.size(); ++j)
			{
				if (m_akUnitData[j]->apkUISkill[i] == pkSkill)
				{
					m_akUnitData[j]->apkUISkill[i] = NULL;
				}
			}
			UpdateAttribute(true);
			return;
		}
	}

	if (!m_apkNormalSkill.empty())
	{
		for (std::vector<BeSkill*>::iterator itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			BeSkill* pkSkill = *itr;
			if (pkSkill && pkSkill->GetTypeID() == iTypeID)
			{
				bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
				pkSkill = NULL;
				m_apkNormalSkill.erase(itr);
				UpdateAttribute(true);
				return;
			}
		}
	}
}

void BeUnit::DelAllSkill(bool bHeroSkillOnly)
{
	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		BeSkill* pkSkill = (m_pkCurData->apkUISkill)[i];
		if (pkSkill)
		{
			if (!m_apkNormalSkill.empty())
			{
				bool bHasPoint = false;
				for (std::vector<BeSkill*>::iterator itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
				{
					BeSkill* pkNormalSkill = *itr;
					if (pkNormalSkill && pkNormalSkill == pkSkill)
					{
						(m_pkCurData->apkUISkill)[i] = NULL;
						bHasPoint = true;
						break;
					}
				}

				if (bHasPoint)
				{
					continue;
				}
			}

			int iSkillTypeID = pkSkill->GetTypeID();
			if (iSkillTypeID == 'S306' || iSkillTypeID == 'M188')
			{
				SetAttackRange(GetOrgAttackRange());
			}

			bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
			(m_pkCurData->apkUISkill)[i] = NULL;

			for (int j = 0; j < (int)m_akUnitData.size(); ++j)
			{
				if (m_akUnitData[j]->apkUISkill[i] == pkSkill)
				{
					m_akUnitData[j]->apkUISkill[i] = NULL;
				}
			}
			UpdateAttribute(true);
		}
	}

	if (bHeroSkillOnly)
	{
		DelAllBuffer();
	}

	if (!bHeroSkillOnly && !m_apkNormalSkill.empty())
	{
		for (std::vector<BeSkill*>::iterator itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			BeSkill* pkSkill = *itr;
			if (pkSkill)
			{
				bool bNeedUpdate = pkSkill->HasFlag(BCF_HAS_NORMAL_ATTR);
				pkSkill = NULL;
				UpdateAttribute(true);
			}
		}
		m_apkNormalSkill.clear();
	}
	SetUsedSkillPoint(0);
}

void BeUnit::SetSkillLevel(int iTypeID, int iLevel)
{
	BeSkill* pkSkill = GetSkill(iTypeID);
	if (pkSkill)
	{
		pkSkill->SetLevel(iLevel);
		UpdateAttribute(true);
	}
}

int BeUnit::GetSkillLevel(int iTypeID)
{
	BeSkill* pkSkill = GetSkill(iTypeID);
	if (pkSkill)
	{
		return pkSkill->GetLevel();
	}
	return 0;
}

const std::vector<BeSkill*>& BeUnit::GetNormalSkill(void)
{
	return m_apkNormalSkill;
}

bool BeUnit::ActiveSkill(const BeActiveSkill& kCmd)
{
	int iSkillTypeID = kCmd.iSkillTypeID;
	bool bActive = kCmd.bActive;

	BeSkill* pkSkill = GetSkill(iSkillTypeID);
	if (!pkSkill)
	{
		return false;
	}
	if (bActive)
	{
		pkSkill->SetActive(true);
	}
	else
	{
		pkSkill->SetActive(false);
	}

	return true;
}

int	BeUnit::GetItemUseSkill(int iItemTypeID)
{
	const ItemTable* pkItemRes = gMain.GetResItem(iItemTypeID);
	if (!pkItemRes)
	{
		return 0;
	}

	for (int i = 0; i < 6; i++)
	{
		int iItemSkillTypeID = pkItemRes->iItemSkill[i];
		const SkillTable* pkSkillTable = gMain.GetResSkill(iItemSkillTypeID);
		if (pkSkillTable)
		{
			if (pkSkillTable->uiOperateType != SKILL_OPERATETYPE_BEIDONG)
			{
				return iItemSkillTypeID;
			}
		}
	}
	return 0;
}

int BeUnit::GetItemSkillTypeID(int iItemID)
{
	BeItem* pkItem = GetItemByID(iItemID);
	if (!pkItem)
	{
		return 0;
	}
	const ItemTable* pkItemRes = gMain.GetResItem(pkItem->GetTypeID());
	const ZhanChangItemTable* pkZItemRes = gMain.GetResZhanChangItem(pkItem->GetTypeID());
	if (!pkItemRes && !pkZItemRes)
	{
		return 0;
	}

	int iSkillTypeID = 0;
	for (int i = 0; i < ITEM_MAX_SKILL; ++i)
	{
		int iTypeID = 0;
		if (pkItemRes)
		{
			iTypeID = pkItemRes->iItemSkill[i];
		}
		else
		{
			iTypeID = pkZItemRes->iItemSkill[i];
		}
		const SkillTable* pkRes = GetResSkill(iTypeID);
		if (pkRes)
		{
			if (pkRes->uiOperateType != SKILL_OPERATETYPE_BEIDONG)
			{
				iSkillTypeID = iTypeID;
				break;
			}
		}
	}
	const SkillTable* pkSkillRes = GetResSkill(iSkillTypeID);
	if (!pkSkillRes)
	{
		return 0;
	}
	return iSkillTypeID;
}

void BeUnit::SetMP(float fMP, bool bChange/* = true*/)
{
	float fPreMP = m_pkCurData->fMP;

	if (fMP > m_pkCurData->fMaxMP)
	{
		m_pkCurData->fMP = m_pkCurData->fMaxMP;
	}
	else if (fMP <= 0.0f)
	{
		m_pkCurData->fMP = 0.0f;
	}
	else
	{
		m_pkCurData->fMP = fMP;
	}

	if (gData.IsWTF())
	{
		m_pkCurData->fMP = m_pkCurData->fMaxMP;
	}

	if (fPreMP != m_pkCurData->fMP && bChange)
	{
		SetShareUnitChangeFlag(BSUDCF_CURMP);
	}
}

void BeUnit::SetMaxMP(float fMaxMP)
{
	if (fMaxMP != m_pkCurData->fMaxMP)
	{
		SetShareUnitChangeFlag(BSUDCF_MAXMP);
	}
	m_pkCurData->fMaxMP = fMaxMP;
}

bool BeUnit::IsForbidSkill(BeSkill* pkSkill) const
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

		if (pkSkill->GetOperateType() == SKILL_OPERATETYPE_BEIDONG)
		{
			bForbid = false;
		}
	}

	return bForbid;
}

BeBuffer* BeUnit::AddBufferBegin(int iTypeID, int iOrgUnitID, int iLevel, int iUnitID, bool bAddDead, int iForceID)
{
	const BufferTable* pkRes = BufferTableMgr::Get()->GetBufferTable(iTypeID);
	if (!pkRes)
	{
		return NULL;
	}
	if (!bAddDead && !(pkRes->uiProperty & BUFFER_PROPERTY_DEATHADD) && IsDead())
	{
		return NULL;
	}
	if (m_apkBuffer.Size() >= UNIT_MAX_BUFFER || HasUnitCarryFlag(BUCF_NO_BUFFER_EFFECT))
	{
		return NULL;
	}

	if (HasUnitCarryFlag(BUCF_IMMUNITY_CONTROL) || GetClass() == UNIT_CLASSTYPE_BOSS) {

		if (iTypeID == 'BA01' || iTypeID == 'BA02' || iTypeID == 'BA04' || iTypeID == 'BA05' || iTypeID == 'BA06' || iTypeID == 'B571') {

			return NULL;
		}
	}

	BeBuffer* pkRet = 0;
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && !pkBuffer->GetHasDel())
			{
				if (pkBuffer->GetSingleSpliceNum() == 1 && pkBuffer->GetMultiSpliceNum() == 1)
				{
					pkBuffer->SetHasDel(false);
					pkRet = pkBuffer;
					pkRet->SetLevel(iLevel);

					break;
				}
				else
				{
					BeBuffer* pkNewBuffer = SuperposeBuffer(iTypeID, pkBuffer->GetSingleSpliceNum(), pkBuffer->GetMultiSpliceNum(), iLevel, iUnitID);
					if (!pkNewBuffer)
					{
						return NULL;
					}
					pkRet = pkNewBuffer;
					pkRet->SetLevel(iLevel);

					m_apkBuffer.PushBack(pkRet);

					break;
				}
			}
		}
	}

	if (!pkRet)
	{
		int iID = iForceID;
		if (iID == 0)
		{
			iID = gMain.GenerateID(GIT_CARRY);
		}
		else
		{
			gMain.SetGenerateID(GIT_CARRY, iID);
		}
		pkRet = BeBuffer::NEW(iID);
		if (!pkRet)
		{
			return pkRet;
		}
		pkRet->AttachMain(pkAttachMain);
		pkRet->AttachUnit(this);

		pkRet->SetUnitID(iUnitID);
		pkRet->SetOrgUnitID(iOrgUnitID);
		pkRet->Initialize(iTypeID);
		pkRet->SetRemoveTime(-1);
		pkRet->SetLevel(iLevel);
		pkRet->SetStartTime((int)gTime);

		m_apkBuffer.PushBack(pkRet);
	}

	pkRet->SetDecreased(false);

	return pkRet;
}

void BeUnit::AddBufferEnd(BeBuffer* pkBuffer)
{
	if (!pkBuffer || HasUnitCarryFlag(BUCF_NO_BUFFER_EFFECT))
	{
		return;
	}
	if (GetClass() == UNIT_CLASSTYPE_BOSS)
	{
		pkBuffer->ClrCarryFlag(BUCF_DIZZY | BUCF_ISFORBIDITEM | BUCF_ISFORBIDSKILL | BUCF_CANNOTMOVE | BUCF_CANNOTATTACK | BUCF_CANNOTCONTROL);
	}

	if (HasUnitCarryFlag(BUCF_IMMUNITY_CONTROL))
	{
		pkBuffer->ClrCarryFlag(BUCF_DIZZY | BUCF_ISFORBIDITEM | BUCF_ISFORBIDSKILL | BUCF_CANNOTMOVE);
	}

	int iRemoveTime = (int)pkBuffer->GetRemoveTime();
	if (pkBuffer->IsHaloBuffer())
	{
		iRemoveTime = -1;
	}

	OnAddBuffer(pkBuffer);

	BeShareBufferData	kData;
	kData.bRemove = false;
	kData.iBufferLogicID = pkBuffer->GetID();
	kData.iBufferTypeID = pkBuffer->GetTypeID();
	kData.iRemoveTime = pkBuffer->GetRemoveTime();
	kData.iUnitID = GetID();
	kData.byNum = pkBuffer->GetLevel();
	kData.iAttackUnitID = pkBuffer->GetOrgUnitID();

	gMain.AddBufferShowData(kData);
}

BeBuffer* BeUnit::GetBuffer(int iTypeID, int iUnitID)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID) && (pkBuffer->GetRemoveTime() > gTime || pkBuffer->GetRemoveTime() == -1))
			{
				return pkBuffer;
			}
		}
	}

	return NULL;
}

void	BeUnit::RemoveBufferAttr(BeNormalAttType eType)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
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

const BeBuffer* BeUnit::GetBuffer(int iTypeID, int iUnitID) const
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID) && (pkBuffer->GetRemoveTime() > gTime || pkBuffer->GetRemoveTime() == -1)) //
			{
				return pkBuffer;
			}
		}
	}

	return NULL;
}

BeBuffer* BeUnit::GetLilithBuffer()
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer
				&& (pkBuffer->GetTypeID() == 'B258' || pkBuffer->GetTypeID() == 'B207' || pkBuffer->GetTypeID() == 'B221' || pkBuffer->GetTypeID() == 'B439')
				&& (pkBuffer->GetRemoveTime() > gTime || pkBuffer->GetRemoveTime() == -1)) //
			{
				return pkBuffer;
			}
		}
	}

	return NULL;
}

void BeUnit::RemoveLilithBuffer(int iUnitID)
{
	BeBuffer* pkBuffer = GetBufferByInnerID(iUnitID);

	if (pkBuffer) {

		int iNowRemoveTime = pkBuffer->GetRemoveTime();

		if (iNowRemoveTime > gTime + iLogicFrameTime) {
			pkBuffer->SetInterrupt(true);
			pkBuffer->SetRemoveTime(gTime + iLogicFrameTime, true);
		}
	}
}

const std::vector<BeBuffer*>& BeUnit::GetBufferByMulty(int iTypeID, int iUnitID)
{
	m_apkMultyBuffer.clear();
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID) && (pkBuffer->GetRemoveTime() > gTime || pkBuffer->GetRemoveTime() == -1))
			{
				m_apkMultyBuffer.push_back(pkBuffer);
			}
		}
	}

	return m_apkMultyBuffer;
}

const int BeUnit::GetMultyBufferSize(int iTypeID, int iUnitID) const
{
	int iCount = 0;
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID) && (pkBuffer->GetRemoveTime() > gTime || pkBuffer->GetRemoveTime() == -1))
			{
				++iCount;
			}
		}
	}

	return iCount;
}

const std::vector<BeBuffer*>& BeUnit::GetBufferByNegative(void)
{
	m_apkMultyBuffer.clear();
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->HasProperty(BUFFER_PROPERTY_DEBUFFER))
			{
				m_apkMultyBuffer.push_back(pkBuffer);
			}
		}
	}
	return m_apkMultyBuffer;
}

Es::Array<BeBuffer*> BeUnit::GetAllBuffer(void) const
{
	return m_apkBuffer;
}

bool BeUnit::DelBuffer(int iTypeID, int iUnitID)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
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

bool BeUnit::DelBufferByID(int iBufferID)
{
	if (m_apkBuffer.IsEmpty())
	{
		return false;
	}

	for (int i = 0; i < (int)m_apkBuffer.Size(); i++)
	{
		BeBuffer* pkBuffer = m_apkBuffer[i];
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

bool BeUnit::DelBufferByType(int iTypeID, int iUnitID)
{
	bool bFlag = false;
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && (0 == iUnitID || pkBuffer->GetBufferUnitID() == iUnitID))
			{
				OnDelBuffer(pkBuffer);
				pkBuffer->SetRemoveTime(gTime);
				bFlag = true;
			}
		}
	}

	return bFlag;
}

void BeUnit::DelAllBuffer(bool bRelive)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
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

void BeUnit::DelAllBufferFinal(void)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			SafeDelBuf(pkBuffer);
		}
		m_apkBuffer.Clear();
	}
}

void BeUnit::DelBufferByClean(bool bNegative, bool bGood)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && !pkBuffer->HasProperty(BUFFER_PROPERTY_NOJINGHUA))
			{
				if (pkBuffer->HasProperty(BUFFER_PROPERTY_DEBUFFER))
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

void BeUnit::DelBufferByAutoClean(void)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->HasProperty(BUFFER_PROPERTY_AUTOREMOVE))
			{
				OnDelBuffer(pkBuffer);
				pkBuffer->SetRemoveTime(gTime);
			}
		}
	}
}

bool BeUnit::DelNegativeBuffer(void)
{
	if (!m_apkBuffer.IsEmpty())
	{
		bool bHadDeld = false;
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->IsBadBuffer(GetCamp()))
			{
				if (!pkBuffer->HasProperty(BUFFER_PROPERTY_NONVYAO)) {

					OnDelBuffer(pkBuffer);
					pkBuffer->SetRemoveTime(gTime);
					bHadDeld = true;
				}
			}
		}

		if (bHadDeld)
		{
			return true;
		}
	}
	return false;
}

bool BeUnit::DelNegativeBufferExceptNoInvins(void)
{
	if (!m_apkBuffer.IsEmpty())
	{
		bool bHadDeld = false;
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->HasProperty(BUFFER_PROPERTY_DEBUFFER) && pkBuffer->GetTypeID() != 'BA30')
			{
				OnDelBuffer(pkBuffer);
				pkBuffer->SetRemoveTime(gTime);
				bHadDeld = true;
			}
		}

		if (bHadDeld)
		{
			return true;
		}
	}
	return false;
}

void BeUnit::OnAddBuffer(BeBuffer* pkBuffer)
{
	if (!pkBuffer)
	{
		return;
	}
	pkBuffer->SetHasDel(false);
	TePtParam kParam;

	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_pkBuffer, pkBuffer);
	gTrgMgr.FireTrigger(BTE_UNIT_ADDBUFFER, kParam);

	UpdateAttribute(pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR));

	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_pkBuffer, pkBuffer);

	gTrgMgr.FireTrigger(BTE_UNIT_ADDBUFFER_AFTER, kParam);

	if (pkBuffer->HasCarryFlag(BUCF_ISINVISIBLE))
	{
		gTrgMgr.FireTrigger(BTE_BUFFER_INVISIBLE_ENTER, kParam);
	}

	BeTDEventArgs kArgs = BeTDEventArgs();
	kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITADDBUFFER;
	kArgs.kUnitAddBuffer.iBufferTypeID = pkBuffer->GetTypeID();
	kArgs.kUnitAddBuffer.iUnitID = GetID();
	gLevelMain.GetEventMgr()->FireEvent(kArgs);

	BeUnit* pkAttacker = gUnitMgr.GetUnitByID(pkBuffer->GetOrgUnitID());
	if (IsHero() && pkAttacker && pkAttacker->IsHero())
	{
		if (pkAttacker->GetID() != GetID()) {

			if (GetCamp() != pkAttacker->GetCamp()) {
				gMain.SetPlayerAddBufferTime(GetPlayer(), pkAttacker->GetPlayer());
			}
			else {
				gMain.SetPlayerHelpTime(GetPlayer(), pkAttacker->GetPlayer());
			}
		}
	}
}

void BeUnit::OnDelBuffer(BeBuffer* pkBuffer, bool bUpdate, bool bDelEffect)
{
	if (!pkBuffer || pkBuffer->GetHasDel())
	{
		return;
	}

	pkBuffer->SetHasDel(true);
	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_pkBuffer, pkBuffer);

	gTrgMgr.FireTrigger(BTE_UNIT_DELBUFFER, kParam);

	if (bUpdate)
	{
		UpdateAttribute(pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR));
	}

	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_pkBuffer, pkBuffer);

	gTrgMgr.FireTrigger(BTE_UNIT_DELBUFFER_AFTER, kParam);

	BeTDEventArgs kArgs = BeTDEventArgs();
	kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITDELBUFFER;
	kArgs.kUnitDelBuffer.iBufferTypeID = pkBuffer->GetTypeID();
	kArgs.kUnitDelBuffer.iUnitID = GetID();
	gLevelMain.GetEventMgr()->FireEvent(kArgs);

	BeShareBufferData kData;
	kData.bRemove = true;
	kData.iBufferLogicID = pkBuffer->GetID();
	kData.iBufferTypeID = pkBuffer->GetTypeID();
	kData.iRemoveTime = pkBuffer->GetRemoveTime();
	kData.iUnitID = GetID();
	kData.iAttackUnitID = pkBuffer->GetOrgUnitID();

	gMain.AddBufferShowData(kData);

	BeUnit* pkAttacker = gUnitMgr.GetUnitByID(pkBuffer->GetOrgUnitID());

	if (IsHero() && pkAttacker && pkAttacker->IsHero())
	{
		if (pkAttacker->GetID() != GetID()) {

			if (GetCamp() != pkAttacker->GetCamp()) {
				gMain.SetPlayerAddBufferTime(GetPlayer(), pkAttacker->GetPlayer());
			}
			else {
				gMain.SetPlayerHelpTime(GetPlayer(), pkAttacker->GetPlayer());
			}
		}
	}
}

BeBuffer* BeUnit::SuperposeBuffer(int iTypeID, int iSingleCount, int iMultiCount, int iLevel, int iUnitID)
{
	if (m_apkBuffer.Size() >= UNIT_MAX_BUFFER)
	{
		return NULL;
	}

	BeBuffer* pkBuffer = NULL;
	std::vector<BeBuffer*> akBuffer;

	if (iSingleCount != 1)
	{
		GetSingleSuperposeBuffer(iTypeID, akBuffer, iUnitID);
		if ((int)akBuffer.size() < iSingleCount)
		{
			pkBuffer = BeBuffer::NEW(gMain.GenerateID(GIT_CARRY));
			if (!pkBuffer)
			{
				return pkBuffer;
			}

			pkBuffer->AttachMain(pkAttachMain);
			pkBuffer->AttachUnit(this);
			pkBuffer->Initialize(iTypeID);
			pkBuffer->SetRemoveTime(-1);
			pkBuffer->SetLevel(iLevel);
			pkBuffer->SetUnitID(iUnitID);
			pkBuffer->SetStartTime((int)gTime);
			return pkBuffer;
		}
		else
		{
			for (std::vector<BeBuffer*>::iterator itr = akBuffer.begin(); itr != akBuffer.end(); ++itr)
			{
				BeBuffer* pkTempBuffer = *itr;
				if (pkTempBuffer && pkTempBuffer->GetLevel() <= iLevel && pkTempBuffer->GetRemoveTime() != gTime)
				{
					if (!pkBuffer || pkBuffer->GetRemoveTime() > pkTempBuffer->GetRemoveTime())
					{
						pkBuffer = pkTempBuffer;
						continue;
					}
				}
			}
			for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
			{
				BeBuffer* pkOrgBuffer = m_apkBuffer[i];
				if (pkBuffer && pkOrgBuffer && pkOrgBuffer->GetID() == pkBuffer->GetID())
				{
					OnDelBuffer(pkOrgBuffer);
					pkOrgBuffer->SetRemoveTime(gTime);
					break;
				}
			}

			BeBuffer* pkNewBuffer = BeBuffer::NEW(gMain.GenerateID(GIT_CARRY));
			if (!pkNewBuffer)
			{
				return pkNewBuffer;
			}
			pkNewBuffer->AttachMain(pkAttachMain);
			pkNewBuffer->AttachUnit(this);
			pkNewBuffer->Initialize(iTypeID);
			pkNewBuffer->SetRemoveTime(-1);
			pkNewBuffer->SetLevel(iLevel);
			pkNewBuffer->SetUnitID(iUnitID);

			return pkNewBuffer;
		}
	}

	if (iMultiCount != 1)
	{
		if (!m_apkBuffer.IsEmpty())
		{
			for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
			{
				BeBuffer* pkOrgBuffer = m_apkBuffer[i];
				if (pkOrgBuffer && pkOrgBuffer->GetTypeID() == iTypeID && pkOrgBuffer->GetBufferUnitID() == iUnitID && iSingleCount == 1)
				{
					return pkOrgBuffer;
				}
			}
		}

		GetMultiSuperposeBuffer(iTypeID, akBuffer, iUnitID);
		if ((int)akBuffer.size() < iMultiCount)
		{
			pkBuffer = BeBuffer::NEW(gMain.GenerateID(GIT_CARRY));
			if (!pkBuffer)
			{
				return pkBuffer;
			}
			pkBuffer->AttachMain(pkAttachMain);
			pkBuffer->AttachUnit(this);
			pkBuffer->Initialize(iTypeID);
			pkBuffer->SetRemoveTime(-1);
			pkBuffer->SetLevel(iLevel);
			pkBuffer->SetUnitID(iUnitID);
			return pkBuffer;
		}
		else
		{
			for (std::vector<BeBuffer*>::iterator itr = akBuffer.begin(); itr != akBuffer.end(); ++itr)
			{
				BeBuffer* pkTempBuffer = *itr;
				if (pkTempBuffer && pkTempBuffer->GetLevel() <= iLevel)
				{
					if (!pkBuffer || pkBuffer->GetRemoveTime() > pkTempBuffer->GetRemoveTime())
					{
						pkBuffer = pkTempBuffer;
						continue;
					}
				}
			}

			for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
			{
				BeBuffer* pkOrgBuffer = m_apkBuffer[i];
				if (pkOrgBuffer && pkBuffer && pkOrgBuffer->GetID() == pkBuffer->GetID())
				{
					OnDelBuffer(pkOrgBuffer);
					pkOrgBuffer->SetRemoveTime(gTime);
					break;
				}
			}

			BeBuffer* pkNewBuffer = BeBuffer::NEW(gMain.GenerateID(GIT_CARRY));
			if (!pkNewBuffer)
			{
				return pkNewBuffer;
			}

			pkNewBuffer->AttachMain(pkAttachMain);
			pkNewBuffer->AttachUnit(this);
			pkNewBuffer->Initialize(iTypeID);
			pkNewBuffer->SetRemoveTime(-1);
			pkNewBuffer->SetLevel(iLevel);
			pkNewBuffer->SetUnitID(iUnitID);

			return pkNewBuffer;
		}
	}
	return NULL;
}

void BeUnit::GetSingleSuperposeBuffer(int iTypeID, std::vector<BeBuffer*>& rakBuffer, int iUnitID)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && !pkBuffer->GetHasDel())
			{
				rakBuffer.push_back(pkBuffer);
			}
		}
	}
}

void BeUnit::GetMultiSuperposeBuffer(int iTypeID, std::vector<BeBuffer*>& rakBuffer, int iUnitID)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetTypeID() == iTypeID && pkBuffer->GetBufferUnitID() != iUnitID && !pkBuffer->GetHasDel())
			{
				rakBuffer.push_back(pkBuffer);
			}
		}
	}
}

BeBuffer* BeUnit::GetBufferByInnerID(int iID)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (pkBuffer && pkBuffer->GetID() == iID)
			{
				return pkBuffer;
			}
		}
	}
	return NULL;
}

void BeUnit::UpdateValidItem(void)
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (m_apkItem[i] && m_apkItem[i]->GetOrgValid())
		{
			m_apkCarry.push_back(m_apkItem[i]);
			SetUnitCarryFlag(m_apkItem[i]->GetCarryFlag());
			SetUnitImmunityFlag(m_apkItem[i]->GetImmunityFlag());
		}
	}
}

void BeUnit::UpdateValidBuffer(void)
{
	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
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
			SetUnitNotInvisFlag(pkBuffer->GetNotInvisCampFlag());
		}
	}
}

void BeUnit::UpdateValidSkill(bool bReset)
{
	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		BeSkill* pkSkill = (m_pkCurData->apkUISkill)[i];
		if (pkSkill)
		{
			if (bReset && !m_bIsUpdate)
			{
				pkSkill->SetLevel(pkSkill->GetLevel());
			}

			bool bHasSame = false;
			for (int j = 0; j < (int)m_apkCarry.size(); j++)
			{
				BeCarry* pkCarry = m_apkCarry[j];
				if (!pkCarry || pkCarry->GetID() == pkSkill->GetID())
				{
					bHasSame = true;
					break;
				}
			}

			if (bHasSame)
			{
				continue;
			}

			m_apkCarry.push_back(pkSkill);
			SetUnitCarryFlag(pkSkill->GetCarryFlag());
			SetUnitImmunityFlag(pkSkill->GetImmunityFlag());
		}
	}

	if (!m_apkNormalSkill.empty())
	{
		for (std::vector<BeSkill*>::iterator itr = m_apkNormalSkill.begin(); itr != m_apkNormalSkill.end(); ++itr)
		{
			BeSkill* pkSkill = *itr;
			if (bReset && !m_bIsUpdate)
			{
				pkSkill->SetLevel(pkSkill->GetLevel());
			}

			bool bHasSame = false;
			for (int i = 0; i < (int)m_apkCarry.size(); i++)
			{
				BeCarry* pkCarry = m_apkCarry[i];
				if (!pkCarry || pkCarry->GetID() == pkSkill->GetID())
				{
					bHasSame = true;
					break;
				}
			}

			if (bHasSame)
			{
				continue;
			}

			m_apkCarry.push_back(pkSkill);
			SetUnitCarryFlag(pkSkill->GetCarryFlag());
			SetUnitImmunityFlag(pkSkill->GetImmunityFlag());
		}
	}
}

void BeUnit::GetAttackingAttr(BeUnit* pkTarget, BeAttackingAttr& rkAttackingAttr)
{
	rkAttackingAttr.eAttackType = GetAttackType();
	rkAttackingAttr.fDamage = GetDamageNum();
	rkAttackingAttr.kAttacker = m_spSharePtr;
	rkAttackingAttr.iPlayer = GetPlayer();
	rkAttackingAttr.bNeedCulculateArmor = true;
	rkAttackingAttr.fLeech = GetLeech();
	rkAttackingAttr.SetFlag(BAF_REBOUND);

	if (GetAttackingMiss())
	{
		rkAttackingAttr.SetFlag(BAF_AVOID);
		return;
	}
}

bool BeUnit::GetAttackingMiss(void) const
{
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;
			if (pkCarry->GetAttackMissPhysic())
			{
				return true;
			}
		}
	}

	return false;
}

bool BeUnit::GetAttackedAvoid(void) const
{
	if (!m_apkCarry.empty())
	{
		float fMaxAvoidRate = GetEvadeRate();
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;

			if (fMaxAvoidRate < pkCarry->GetAttackedAvoidPhysic())
			{
				fMaxAvoidRate = pkCarry->GetAttackedAvoidPhysic();
			}
		}

		if (gMain.RandFloatInRate(fMaxAvoidRate, m_iID + (int)fMaxAvoidRate))
		{
			return true;
		}
	}

	return false;
}

void BeUnit::GetAttackedPhysicAttr(float& fAntiPhysic, const BeUnit* pkAttacker, float fDecArmorValue, float fDecArmorPer) const
{
	float fFinalAntiArmor = GetArmor() * (1.0f - fDecArmorPer) - fDecArmorValue;
	fAntiPhysic = BeFormula::GetAmorForDamage(fFinalAntiArmor);
}

void BeUnit::GetAttackedMagicAttr(float& fAntiMagic, const BeUnit* pkAttacker, float fDecEnemyAntiMagic, float fDecMagicArmorPer) const
{
	float fFinalAntiArmor = GetMagicArmor() * (1.0f - fDecMagicArmorPer) - fDecEnemyAntiMagic;

	fAntiMagic = BeFormula::GetAmorForDamage(fFinalAntiArmor);
}

float BeUnit::GetAttackedReboundSkill(void) const
{
	float fPercent = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;
			fPercent += pkCarry->GetAttackedReboundSkill();
		}
	}

	return fPercent;
}

float BeUnit::GetAttackedReboundSkillAll(void) const
{
	float fPercent = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;
			fPercent += pkCarry->GetAttackedReboundSkillAll();
		}
	}

	return fPercent;
}

float BeUnit::GetAttackedAntiLeech(void) const
{
	float fPercent = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;

			fPercent += pkCarry->GetAttackedAntiLeech();
		}
	}

	return fPercent;
}

float BeUnit::GetAttackedReboundPhysicShorRange(int& iSkill) const
{
	float fValue = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;
			if (fValue <= 0.0f || fValue < pkCarry->GetAttackedReboundPhysicShortRange())
			{
				fValue = pkCarry->GetAttackedReboundPhysicShortRange();
				iSkill = pkCarry->GetTypeID();
			}
		}
	}

	return fValue;
}

void BeUnit::GetAttackedReboundPhysicBoth(float& fShortRange, float& fEject, int& iShortSkill, int& iEjectSkill)
{
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;
			float fShortRangeTure = 0.0f;
			float fEjectTure = 0.0f;
			pkCarry->GetAttackedReboundPhysicBoth(fShortRangeTure, fEjectTure);

			if (fShortRange <= 0.0f || fShortRange < fShortRangeTure)
			{
				fShortRange = fShortRangeTure;
				iShortSkill = pkCarry->GetTypeID();
			}

			if (fEject <= 0.0f || fEject < fEjectTure)
			{
				fEject = fEjectTure;
				iShortSkill = pkCarry->GetTypeID();
			}
		}
	}
}

float BeUnit::GetAttackedAttackCDAttr(void) const
{
	float fTotalACD = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;
			float fAttackCD = 0.0f;
			fAttackCD = pkCarry->GetNormalAttrValue(NAT_PER_ATTACK_SPEED);
			fTotalACD += fAttackCD;
		}
	}
	return fTotalACD;
}

float BeUnit::GetAttackedBaoJiAttr(void) const
{
	float fTotalBaoji = 0.0f;
	if (!m_apkCarry.empty())
	{
		for (std::vector<BeCarry*>::const_iterator itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
		{
			BeCarry* pkCarry = *itr;
			float fBaoji = 0.0f;
			fBaoji = pkCarry->GetNormalAttrValue(NAT_BAOJI);
			fTotalBaoji += fBaoji;
		}
	}
	return fTotalBaoji;
}

void BeUnit::SetSummonOption(BeUnitAIType eAiType, int iPlayer, int iControlPlayer, int iConnectUnitID, float fPosX, float fPosY, float fDistance, int iFlag, int iAllLiveTime, int iSkillOrItem)
{
	BeUnit* pkConnectUnit = gUnitMgr.GetUnitByID(iConnectUnitID, true);

	AISetType(eAiType);
	AISetControlType(BACT_PLAYER);
	SetPlayer(iPlayer);
	SetControl(iControlPlayer);
	SetConnectUnit(pkConnectUnit);
	gMap.SetUnitPosition(this, fPosX, fPosY, 0.0f, fDistance, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);
	SetHP(this->GetMaxHP());
	SetFlag(iFlag);
	SetAllLiveTime(iAllLiveTime);
}
