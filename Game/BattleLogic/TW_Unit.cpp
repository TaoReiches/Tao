/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Unit.h"
#include "TW_Main.h"
#include "TW_Map.h"
#include "TW_UnitMgr.h"
#include "Skill_table.hpp"
#include "TW_Skill.h"
#include "TW_TriggerEvent.h"
#include "TW_Item.h"
#include "TW_Buff.h"
#include "Buffer_table.hpp"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"
#include "Item_table.hpp"
#include "TW_MemoryObject.h"
#include "TW_Command.h"
#include <algorithm>
#include "TW_UnitLearnSkillData.h"
#include "TW_TriggerMgr.h"
#include "TW_UnitOutput.h"

void TwUnit::Link(float fX, float fY, TwEntityMgr<TwUnit>* pkMgr)
{

}

void TwUnit::Unlink()
{

}

template<class T>
void TwUnit::TrgOnPreDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr, int iItemPos)
{
	for (int i = 0; i < iCount; ++i)
	{
		auto pkSkill = kSkill[i];
		if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DAMAGETRIGGER))
		{
			TwPtParam kParam;
			kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
			kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
			kParam.SetParam(TwTrgParamID::BTP_pkAttacker, this);
			kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
			kParam.SetParam(TwTrgParamID::BTP_iItemPos, iItemPos + 1);
			kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
			kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
			kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);
			kParam.SetParam(TwTrgParamID::BTP_pkAttackAttr, &kAttr);

			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_PREONDAMAGE, kParam);
		}
	}
}

template<class T>
void TwUnit::TrgOnBeDamaged_T(T& kSkill, int iCount, int eAttackType, float& fDamage, float fRawDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos)
{
	for (int i = 0; i < iCount; ++i)
	{
		auto pkSkill = kSkill[i];
		if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_BEATTACKTRIGGER))
		{
			TwPtParam kParam;
			kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
			kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
			kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);
			kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
			kParam.SetParam(TwTrgParamID::BTP_iItemPos, iItemPos + 1);
			kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
			kParam.SetParam(TwTrgParamID::BTP_fRawDamage, fRawDamage);
			kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
			kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);
			kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iAttackSkillTypeID);

			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONBEDAMAGE, kParam);
		}
	}
}

template<class T>
void TwUnit::TrgOnDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos, int iItemID)
{
	for (int i = 0; i < iCount; ++i)
	{
		auto pkSkill = kSkill[i];
		if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DAMAGETRIGGER))
		{
			TwPtParam kParam;
			kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
			kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
			kParam.SetParam(TwTrgParamID::BTP_pkAttacker, this);
			kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
			kParam.SetParam(TwTrgParamID::BTP_iItemPos, iItemPos + 1);
			kParam.SetParam(TwTrgParamID::BTP_iItemID, iItemID);
			kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
			kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
			kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);
			kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iAttackSkillTypeID);
			kParam.SetParam(TwTrgParamID::BTP_iSkillOrgTypeID, iAttackSkillTypeID);

			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONDAMAGE, kParam);
		}
	}
}

template<class T>
void TwUnit::TrgOnAttack_T(T& kSkill, int iCount, std::shared_ptr<TwUnit> pkTarget, BeAttackingAttr* pkAttackAttr, int iItemPos)
{
	for (int i = 0; i < iCount; ++i)
	{
		auto pkSkill = kSkill[i];
		if (pkSkill && (pkSkill->HasProperty(SKILL_SKILLPROPERTY_ATTACKTRIGGER)))
		{
			TwPtParam kParam;
			kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
			kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
			kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
			kParam.SetParam(TwTrgParamID::BTP_pkAttackAttr, pkAttackAttr);
			kParam.SetParam(TwTrgParamID::BTP_iItemPos, iItemPos + 1);
			if (pkSkill->HasProperty(SKILL_SKILLPROPERTY_ATTACKTRIGGER))
			{
				kParam.SetParam(TwTrgParamID::BTP_iTrgEventBy, static_cast<int>(TwTriggerEvent::BTE_SKILL_ONATTACK));
				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONATTACK, kParam);
			}
		}
	}
}

TwUnit::TwUnit(int iID) : TwUnitCarry(iID), BeSingleLinkEntity<TwUnit>(iID)
{
	m_iOtherFlag = 0;
	m_iCarryFlag = 0;
	m_iImmunityFlag = 0;
	m_eActionName = BAN_stand;
	m_iActionStartTime = 0;

	m_bChangeSkill = false;
	m_iMoveToUnitID = 0;
	m_fMoveTarPosX = 0.0f;
	m_fMoveTarPosY = 0.0f;

	m_iMoveAllTime = 0;
	m_fMinMoveDistance = 0.0f;

	m_iGiveAttackCmdTime = 0;

	auto pkData = std::shared_ptr<TwUnitData>(mpUnitData.alloc());
	m_akUnitData.push_back(pkData);
	m_pkBackData = pkData;
	m_pkCurData = pkData;
	m_iActionStayTime = 0;
	
	m_eActorTransType = UATT_NONE_TRANS;
	m_iTransTime = 0;
	m_bNeedTrans = false;
	m_iAddTransTime = 0;
	m_bPureNeedUpdae = false;
	m_iActionState = 0;
	m_fWalkTargetX = 0.f;
	m_fWalkTargetY = 0.f;
	m_iPathFindSucessTime = 0;
	OutputFlag = TwUnitOutputFlag::BSUDCF_NULL;
	m_dwTransAlphaDec = 0;
	m_iAlphaTransTime = 0;
	m_dwStartAlpha = 0xFF;
	m_iGrassIndex = 0;
	m_bNeedUpdateObstacle = true;
	m_bSetObstacle = false;
	m_spSharePtr.reset(this);
	m_iLastAttackTime = 0;
	m_iLastAttackHeroTime = 0;
    UnitOutput = std::unique_ptr<TwUnitOutput>(new TwUnitOutput());
}

void TwUnit::OnDelete(void)
{
	m_kCommander.OnDelete();
	DelAllBuffer();

	gMap.DelUnitObstacle(gUnit, true, true);
	gUnitMgr->Unlink(this);

	m_apkNormalSkill.clear();

    mpLearnSkillData.free(m_pkBackData->akLearnSkill.get());
	mpUnitData.free(m_pkBackData.get());
	m_pkBackData = nullptr;
	m_pkCurData = nullptr;
}

TwUnit::~TwUnit(void)
{
	gTrgMgr.OnUnitDelete(this);

	m_spSharePtr.reset();
	//gMain->DelEntityPointer(GIT_ENTITY, m_iID);
}

bool TwUnit::Initialize(int iTypeID)
{
    m_kCommander.AttachUnit(gUnit);
    UnitOutput->AttachMain(gMain);
    UnitOutput->AttachUnit(gUnit);

	m_iTypeID = iTypeID;
	auto& pkRes = UnitTableMgr::Get()->GetUnitTable(iTypeID);
	if (!pkRes)
	{
		return false;
	}

	m_pkBackData->pkRes = std::shared_ptr<const UnitTable>(pkRes);
	m_iFlag = 0;
	SetCurrentTypeID(iTypeID);
	m_pkBackData->iTypeID = iTypeID;
	m_pkBackData->iUnitProperty = m_pkBackData->pkRes->uiProperty;
	OnInitAttribute(true, false);
	SetLevel(1, false);

	m_kCommander.AttachMain(pkAttachMain);

	DelAllBuffer();
	DelAllSkill();

	//mpLearnSkillData.free(m_pkBackData->akLearnSkill.get());

	TwCommand	kComm(TwCommandType::BCT_STOP);
	m_kCommander.GiveCommand(kComm);
	OnPlayerChanged();

	//SetTabInfoFlag(BTCF_UNITID);
	SetOutputFlag(TwUnitOutputFlag::BSUDCF_NEW_UNIT);

	return true;
}

void TwUnit::AppyUnitSkill(void)
{
	if (GetClass() == UNIT_CLASSTYPE_HERO)
	{
		(m_pkBackData->akLearnSkill) = std::unique_ptr<BeLearnSkillData>(mpLearnSkillData.alloc());

		//for (int i = 0; i < MAX_LEARN_SKILLS - 1; ++i)
		//{
		//	auto& rkData = (m_pkBackData->akLearnSkill)[i];
		//	rkData.iTypeID = GetAbilSkillID(i);

		//	const SkillTable* pkSkillRes = SkillTableMgr::Get()->GetSkillTable(rkData.iTypeID);
		//	if (!pkSkillRes)
		//	{
		//		continue;
		//	}

		//	rkData.iLearnLevel = 1;
		//	rkData.iPos = i;
		//	SetSkillPointSum(m_pkBackData->iSkillPointSum + pkSkillRes->iSkillMaxLevel);

		//}
	}
	else
	{
		(m_pkBackData->akLearnSkill) = std::unique_ptr<BeLearnSkillData>(mpLearnSkillData.alloc());

		int	iLastSkillTypeID = 0;
		//for (int iSkillIndex = 0; iSkillIndex < iMaxHeroSkillNum; iSkillIndex++)
		//{
		//	int		iSkillTypeID = GetAbilSkillID(iSkillIndex);
		//	if (iSkillTypeID > 0 && iSkillTypeID != iLastSkillTypeID)
		//	{
		//		iLastSkillTypeID = iSkillTypeID;
		//		AddSkill(iSkillTypeID);
		//	}
		//}
	}
}

void TwUnit::UpdateState(int iDeltaTime)
{
	if (!IsDead())
	{
		UpdateHP(iDeltaTime);
		UpdateMP(iDeltaTime);
	}

	if (m_iMoveAllTime > 0)
	{
		m_iMoveAllTime -= iDeltaTime;

		int iMoveAllTime = ((m_iMoveAllTime > 0) ? m_iMoveAllTime : 1);
		if (m_iMoveToUnitID > 0)
		{
			std::shared_ptr<TwUnit> pkMUnit = gUnitMgr->GetUnitByID(m_iMoveToUnitID);
			if (pkMUnit)
			{
				m_fMoveTarPosX = pkMUnit->GetPosX();
				m_fMoveTarPosY = pkMUnit->GetPosY();
			}
			else
			{
				m_iMoveToUnitID = 0;
				m_iMoveAllTime = 0;
			}
		}

		float fMFace = atan2f(m_fMoveTarPosY - GetPosY(), m_fMoveTarPosX - GetPosX());
		float fMDis = GetDistance(GetPosX(), GetPosY(), m_fMoveTarPosX, m_fMoveTarPosY);
		float fMMinDis = (fMDis / iMoveAllTime) * iDeltaTime;

		if (fMMinDis < fMDis && fMDis > 16.0f)
		{
			float fPosX = GetPosX() + fMMinDis * cosf(fMFace);
			float fPosY = GetPosY() + fMMinDis * sinf(fMFace);
			if (m_iMoveToUnitID == 0 && gMap.IsObstacle(fPosX, fPosY, TwGridFlag::TGF_GUTDOODAD, GetOBSize())) {

				m_iMoveAllTime = 0;
			}
			else {
				SetPosition(fPosX, fPosY, 0.0f, true);
			}
		}
		else
		{
			if (gMap.IsObstacle(GetPosX(), GetPosY(), TwGridFlag::TGF_GUTDOODAD, GetOBSize()) || gMap.IsObstacle(m_fMoveTarPosX, m_fMoveTarPosY, TwGridFlag::TGF_GUTDOODAD, GetOBSize())) {

			}
			else {
				SetPosition(m_fMoveTarPosX, m_fMoveTarPosY, 0.0f, true);
			}
			m_iMoveToUnitID = 0;
			m_iMoveAllTime = 0;
		}

		if (m_iMoveAllTime <= 0)
		{
			m_iMoveAllTime = 0;

			ClrFlag(BUF_IGNOREUNITOBS);
			ClrOtherFlag(BUOF_WEIYI);

			gMap.SetUnitPosition(gUnit, GetPosX(), GetPosY(), 0.0f, 1000.0f, false, TwGridFlag::TGF_FIXED_OTS | TwGridFlag::TGF_UNIT, TwGridFlag::TGF_NONE, true);
			SetOutputFlag(TwUnitOutputFlag::BSUDCF_RESET_POX);
		}
	}
}

void TwUnit::Update(int iDeltaTime)
{
	BeEntity::Update(iDeltaTime);

	TrgOnUpdate(iDeltaTime);

	gMap.DelUnitObstacle(gUnit, false, false);

	m_kCommander.ExecuteCmd(iDeltaTime);

	UpdateState(iDeltaTime);

	if (IsDead())
	{
	}
	else
	{
		gMap.AddUnitObstacle(gUnit, false, false);
	}

	float fPerCDTime = GetPerCDTime();
	if (fPerCDTime > 0.0f)
	{
		float fAddTime = (float)iDeltaTime / (1.0f - fPerCDTime);
		int iDecTime = (int)fAddTime - iDeltaTime;
		for (int i = 0; i < iMaxHeroSkillNum; i++)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill)
			{
				if (!pkSkill->CDComplete(gUnit))
				{
					int iLastTime = pkSkill->GetLastUseTime(gUnit);
					pkSkill->SetLastUseTime(gUnit, iLastTime - iDecTime);
				}
			}
		}
	}

	UpdateBuffer(iDeltaTime);

	UpdateLiveTime(iDeltaTime);

	gUnitMgr->Link(GetPosX(), GetPosY(), this);

	UpdateTransrate(iDeltaTime);

    UnitOutput->Update();

	//int iTempIndex = gMap.GetGrassIndex(GetPosX(), GetPosY());
	//if (m_iGrassIndex != iTempIndex)
	//{
	//	SetShareUnitChangeFlag(BSUDCF_GRASS);
	//	if (m_iGrassIndex > 0)
	//	{
	//		gMain->RemoveGrassUnit(GetID(), m_iGrassIndex);
	//	}
	//	if (iTempIndex > 0)
	//	{
	//		gMain->SetGrassUnit(GetID(), iTempIndex);
	//	}
	//}
	//m_iGrassIndex = iTempIndex;

	//for (int iIndex = 0; iIndex < iPureCampNum; iIndex++)
	//{
	//	if (HasProperty(UNIT_PROPERTY_HIDEEVER))
	//	{
	//		m_bVisionForCamp[iIndex] = false;
	//	}
	//	else if (GetClass() == UNIT_CLASSTYPE_BUILDING)
	//	{
	//		m_bVisionForCamp[iIndex] = true;
	//		if (!m_bVisionForCampLast[iIndex] && m_bVisionForCamp[iIndex])
	//		{
	//			m_bNewUnit[iIndex] = true;
	//		}
	//		m_bVisionForCampLast[iIndex] = m_bVisionForCamp[iIndex];
	//	}
	//	else
	//	{
	//		m_bVisionForCamp[iIndex] = gMap.IsInSight(SRPC_CAMPA + iIndex, this);
	//		if (gTime < m_iCastVisionTime)
	//		{
	//			m_bVisionForCamp[iIndex] = true;
	//			m_bGrassVisionForCamp[iIndex] = true;
	//		}
	//		if (!m_bVisionForCamp[iIndex])
	//		{
	//			if (!HasFlag(BUF_HASINVISIBLE))
	//			{
	//				if (gMain->HasGrassVisionForCamp(m_iGrassIndex, GetCamp(), SRPC_CAMPA + iIndex))
	//				{
	//					m_bVisionForCamp[iIndex] = true;
	//					m_bGrassVisionForCamp[iIndex] = true;
	//				}
	//			}
	//		}

	//		if (GetClass() == UNIT_CLASSTYPE_SMALLMONST || GetClass() == UNIT_CLASSTYPE_BIGMONST)
	//		{
	//			m_bGrassVisionForCamp[iIndex] = true;
	//		}
	//		if (!m_bVisionForCampLast[iIndex] && m_bVisionForCamp[iIndex])
	//		{
	//			m_bNewUnit[iIndex] = true;
	//		}
	//		else if (m_bVisionForCampLast[iIndex] && !m_bVisionForCamp[iIndex])
	//		{
	//			gUnitMgr->PushNeedDelNormalUnit(iIndex, m_iID);
	//		}
	//		m_bVisionForCampLast[iIndex] = m_bVisionForCamp[iIndex];

	//		if (m_bVisionForCamp[iIndex] && GetCamp() != iIndex + 1)
	//		{
	//			if (m_bGrassVisionForCampLast[iIndex] && !m_bGrassVisionForCamp[iIndex])
	//			{
	//				gUnitMgr->PushNeedDelNormalUnit(iIndex, m_iID);
	//			}

	//			if (!m_bGrassVisionForCampLast[iIndex] && m_bGrassVisionForCamp[iIndex])
	//			{
	//				m_bNewUnit[iIndex] = true;
	//			}
	//		}

	//		m_bGrassVisionForCampLast[iIndex] = m_bGrassVisionForCamp[iIndex];
	//	}
	//}
}

void TwUnit::SetPlayer(std::uint64_t iPlayerIdx)
{
	int iOldPlayer = GetPlayer();
	//SeRoomPlayerCamp eOldCamp = SeRoomPlayerCamp(m_eCamp);
	//gMap.ReleaseUnitVision(this);
	if (m_pkBackData->iOrgPlayer == 0)
	{
		if (m_pkBackData->Player == 0)
		{
			m_pkBackData->iOrgPlayer = iPlayerIdx;
		}
	}

	m_pkBackData->Player = iPlayerIdx;
	SetOutputFlag(TwUnitOutputFlag::BSUDCF_CAMP);
	//m_eCamp = gMain->GetPlayerCamp(iPlayerIdx);
	SetControl(iPlayerIdx, true, false);

	OnPlayerChanged();
}

void TwUnit::OnPlayerChanged(void)
{
}

void TwUnit::OnSuspend(void)
{
	ClrFlag(BUF_REMOVE);
	SetDead();
	DelAllBuffer(HasFlag(BUF_RELIVE));
	SetUnitvisible(false);
}

void TwUnit::OnResume(void)
{
	ClrFlag(BUF_RELIVE | BUF_BUYLIVE | BUF_REMOVE | BUF_DEAD);
	ClrOtherFlag(BUOF_SPECIAL_RELIVE);
	TwCommand beCommand(TwCommandType::BCT_STOP);
	GiveCommand(beCommand, TwGiveCmdType::BCT_DEATH);
	SetUnitvisible(true);
	ClrOtherFlag(BUOF_WEIYI);
	m_iMoveAllTime = 0;
}

void TwUnit::SetUnitReliveTime(unsigned int uiUnitReliveTime)
{
	if (m_pkBackData->uiUnitReliveTime < gTime)
	{
		m_pkBackData->uiUnitReliveTime = uiUnitReliveTime;
	}
	else
	{
		m_pkBackData->uiUnitReliveTime = m_pkBackData->uiUnitReliveTime > uiUnitReliveTime ? uiUnitReliveTime : m_pkBackData->uiUnitReliveTime;
	}
}

bool TwUnit::IsIdle(void) const
{
	return m_kCommander.IsIdle();
}

bool TwUnit::GiveCommand(TwCommand& kCmd, TwGiveCmdType eType, bool bPlayerControl, bool bNeedHangCurrent, bool bCheckChaoFeng)
{
	if (HasUnitImmunityFlag(BUIF_BATI) && kCmd.eCmdType == TwCommandType::BCT_STOP)
	{
		return false;
	}

	switch (kCmd.eCmdType)
	{
	case TwCommandType::BCT_ATTACK:
	{
		if (kCmd.iUnitID && kCmd.iUnitID == GetID())
		{
			return false;
		}
		if (GetResUnitAttackType() == UNIT_ATTACKTYPE_NONE)
		{
			return false;
		}

		std::shared_ptr<TwUnit> pkUnit = gUnitMgr->GetUnitByID(kCmd.iUnitID);
		if (pkUnit)
		{
			if (eType == TwGiveCmdType::BCT_SYSTEM_SHIFT)
			{
				break;
			}
			if (pkUnit->IsDead())
			{
				return false;
			}
		}
		else
		{
			if (kCmd.iUnitID != 0)
			{
				return false;
			}
		}
		m_iGiveAttackCmdTime = gTime;
		break;
	}
	case TwCommandType::BCT_STOP:
	{
		if (HasFlag(BUF_ISPERSISTSKILL) && eType != TwGiveCmdType::BCT_DEATH)
		{
			return false;
		}
		break;
	}
	case TwCommandType::BCT_MOVE:
	case TwCommandType::BCT_MOVE_DIRECT:
	{
		if (m_iGiveAttackCmdTime == gTime)
		{
			return false;
		}
		if (GetClass() == UNIT_CLASSTYPE_BUILDING)
		{
			return false;
		}
		if (!UnitCanMove(true))
		{
			return false;
		}
		break;
	}
	case TwCommandType::BCT_SPELL:
	{
		BeSkill* pkSkill = GetSkill(kCmd.iData);

		if (HasUnitCarryFlag(BUCF_ISFORBIDSKILL))
		{
			return false;
		}

		if (!pkSkill || IsForbidSkill(pkSkill))
		{
			return false;
		}

		if (!pkSkill->CDComplete(gUnit))
		{
			return false;
		}

		if (pkSkill->GetOperateType() == SKILL_OPERATETYPE_PASSIVE)
		{
			return false;
		}
		if (UnitCanUseSkill(kCmd.iData, gUnitMgr->GetUnitByID(kCmd.iUnitID), false, false, kCmd.kPos.fX, kCmd.kPos.fY) != UUSRT_OK)
		{
			return false;
		}

		if (kCmd.eTargetType == BeCommandTargetType::BCTT_UNIT)
		{
			std::shared_ptr<TwUnit> pkUnit = gUnitMgr->GetUnitByID(kCmd.iUnitID);
			if (pkUnit)
			{
				UnitUseSkillResultType eResult = UnitCanUseSkill(pkSkill->GetTypeID(), pkUnit, true);
				if (eResult != UUSRT_OK)
				{
					return false;
				}
				if (!IsSkillTargetType(pkSkill->GetSkillRes(), pkUnit))
				{
					return false;
				}

				if (pkUnit->IsDead())
				{
					return false;
				}
			}
		}

		if (m_kLastCommand.iData == kCmd.iData && kCmd.iData == 'SA02')
		{
			return false;
		}
		break;
	}
	case TwCommandType::BCT_USE_ITEM:
	{
		BeItem* pkItem = GetItemByID(kCmd.iData);
		if (!pkItem)
		{
			return false;
		}

		int iSkillTypeID = GetItemSkillTypeID(kCmd.iData);
		auto& pkSkill = pkItem->GetSkillByTypeID(iSkillTypeID);
		if (!pkSkill)
		{
			return false;
		}

		if (!pkSkill->CDComplete(gUnit))
		{
			return false;
		}

		auto pkSkillRes = pkSkill->GetSkillRes();
		if (pkSkillRes)
		{
			//if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_TAKEDRUGS)
			{
				if (!pkSkill->CDComplete(gUnit))
				{
					return false;
				}
				if (HasUnitCarryFlag(BUCF_DIZZY) || HasUnitCarryFlag(BUCF_ISFORBIDSKILL) || HasUnitCarryFlag(BUCF_ISTETANIC))
				{
					return false;
				}

				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
				kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, pkSkill->GetTypeID());
				kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, pkSkill->GetLevel());
				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONSPELL, kParam);
				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SPELL_EFFECT, kParam);

				pkSkill->SetLastUseTime(gUnit, gTime);
				return true;
			}
		}
		break;
	}
	}

	m_kCommander.GiveCommand(kCmd, eType, GetID(), bNeedHangCurrent);
	m_kLastCommand = kCmd;

	return true;
}

int	TwUnit::GetCommandCount() const
{
	return m_kCommander.GetCommandCount();
}

void TwUnit::SetPosition(float fX, float fY, float fZ, bool bNoRecordChange)
{
	if (HasOtherFlag(BUOF_CANNOT_WEIYI))
	{
		return;
	}

	if (!bNoRecordChange)
	{
		if (fabs(m_pkCurData->fPosX - fX) > 1.f)
		{
			SetOutputFlag(TwUnitOutputFlag::BSUDCF_CURRENT_POS);
		}
		if (fabs(m_pkCurData->fPosY - fY) > 1.f)
		{
			SetOutputFlag(TwUnitOutputFlag::BSUDCF_CURRENT_POS);
		}
	}

	m_pkCurData->fPosX = fX;
	m_pkCurData->fPosY = fY;
}

void TwUnit::SetUnitAction(BeUnitAction eAction, int eActionName, int iAllTime, bool bSetShow, bool bForceRefresh)
{
	if (HasFlag(BUF_ISPERSIST))
	{
		return;
	}

	int iTime = gTime;

	if (m_iActionStayTime > iTime && eAction != BUA_DEATH)
	{
		return;
	}

	switch (eAction)
	{
	case BUA_BIRTH:
	{
		if (m_pkCurData->eActionType != eAction)
		{
			m_pkCurData->eActionType = eAction;
			m_pkCurData->iActionCurTime = 0;
			SetActionAllTime(iAllTime);
			SetActionName(BAN_birth, gTime);
		}
		break;
	}
	case BUA_STAND:
	{
		if (m_pkCurData->eActionType != eAction || bForceRefresh)
		{
			if (m_pkCurData->eActionType != BUA_WALK)
			{
				SetActionName(BAN_stand, gTime);
			}
			m_pkCurData->eActionType = eAction;
			m_pkCurData->iActionCurTime = 0;
		}
		break;
	}
	case BUA_WALK:
	{
		if (m_pkCurData->eActionType != eAction || bForceRefresh)
		{
			m_pkCurData->eActionType = eAction;
			m_pkCurData->iActionCurTime = 0;

			SetActionName(BAN_walk, gTime);

			if (HasUnitCarryFlag(BUCF_NEW_WALK))
			{
				SetActionName(BAN_run, gTime);
			}
		}
		break;
	}
	case BUA_ATTACK:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(GetAttackCD());
		break;
	}
	case BUA_SPELL:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(iAllTime);
		break;
	}
	case BUA_DEATH:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;

		if (m_pkCurData->iActionAllTime <= 0)
		{
			SetActionAllTime(10);
		}
		break;
	}
	case BUA_DISSIPATE:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(2000);
		break;
	}
	case BUA_DECAY:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(12000);
		break;
	}
	default:
	{
		break;
	}
	}

	if (eActionName != BAN_Null)
	{
		SetActionName(eActionName, gTime);
	}
}

void TwUnit::DamagedByAbsNum(BeAttackType eAttackType, float fDamage, float fRawDamage, const std::shared_ptr<TwUnit> kAttacker, int iPlayer, int iFlag, bool bCanDead, bool bFirstAttack, int iSkillTypeID, int iSkillLevel, float fLeech, float fMultiDamage, int iBeDamagedEffect)
{
	if (IsDead())
	{
		return;
	}

	fDamage *= 1.0f - GetPerDamageReduce();

	std::shared_ptr<TwUnit> pkAttacker = kAttacker;
	int iAttacker = 0;
	if (!kAttacker)
	{
		iAttacker = kAttacker->GetID();
	}

	if (fMultiDamage > 0.0f)
	{
		iFlag |= BAF_HAS_BAOJI;
	}

	TrgOnBeDamaged(eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iSkillTypeID);

	if (bFirstAttack)
	{
		TwPtParam kParamAttackPre;
		kParamAttackPre.SetParam(TwTrgParamID::BTP_iAttackType, (int)eAttackType);
		kParamAttackPre.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
		kParamAttackPre.SetParam(TwTrgParamID::BTP_iAttackerPlayer, iPlayer);
		kParamAttackPre.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
		kParamAttackPre.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
		kParamAttackPre.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
		kParamAttackPre.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
		kParamAttackPre.SetParam(TwTrgParamID::BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ATTACKED_PRE, kParamAttackPre);

		TwPtParam kParamAttack;
		kParamAttack.SetParam(TwTrgParamID::BTP_iAttackType, (int)eAttackType);
		kParamAttack.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
		kParamAttack.SetParam(TwTrgParamID::BTP_iAttackerPlayer, iPlayer);
		kParamAttack.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
		kParamAttack.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
		kParamAttack.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
		kParamAttack.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
		kParamAttack.SetParam(TwTrgParamID::BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ATTACKED, kParamAttack);
	}
	else
	{
		TwPtParam kParamAttack;
		kParamAttack.SetParam(TwTrgParamID::BTP_iAttackType, (int)eAttackType);
		kParamAttack.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
		kParamAttack.SetParam(TwTrgParamID::BTP_iAttackerPlayer, iPlayer);
		kParamAttack.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
		kParamAttack.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
		kParamAttack.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
		kParamAttack.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
		kParamAttack.SetParam(TwTrgParamID::BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_ATTACKED_NOTFIRST, kParamAttack);
	}

	BeShareDamageData	kData;
	kData.fValue = fDamage;
	kData.iAttackerID = iAttacker;
	kData.iUnitID = GetID();
	kData.iDamageType = eAttackType;
	if (iFlag & BAF_HAS_BAOJI)
	{
		kData.eType = BSDT_CRITILE;
	}

	//gMain->AddDamageData(kData);

	int iPreHP = (int)ceil(m_pkCurData->fHP);

	if (fDamage >= m_pkCurData->fHP)
	{
		fDamage = m_pkCurData->fHP;
	}
	else if (fDamage < 0.0f)
	{
		fDamage = 0.0f;
	}

	if ((iFlag & BAF_NODAMAGE) != BAF_NODAMAGE)
	{
		SetHP(GetHP() - fDamage);
	}

	if (bFirstAttack)
	{
		TwPtParam kParamAttack;
		kParamAttack.SetParam(TwTrgParamID::BTP_iAttackType, (int)eAttackType);
		kParamAttack.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
		kParamAttack.SetParam(TwTrgParamID::BTP_iAttackerPlayer, iPlayer);
		kParamAttack.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
		kParamAttack.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
		kParamAttack.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
		kParamAttack.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
		kParamAttack.SetParam(TwTrgParamID::BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_AFTERATTACKED, kParamAttack);
	}

		if (!bCanDead && m_pkCurData->fHP <= 0.0f)
		{
			SetHP(1.0f);
		}
		if (HasOtherFlag(BUOF_NEVEDEATH) && m_pkCurData->fHP <= 0.0f)
		{
			SetHP(1.0f);
		}
		if (HasUnitCarryFlag(BUCF_CANNOTDEATH) && m_pkCurData->fHP <= 0.0f)
		{
			SetHP(1.0f);
		}

	if (iBeDamagedEffect != 0)
	{
		AddAttach(iBeDamagedEffect, 100, iAttacker);
	}
	if (m_pkCurData->fHP <= 0.0f)
	{
		OperateUnitDead(eAttackType, fDamage, pkAttacker, iPlayer, iFlag, false, iSkillTypeID);
	}
}

void TwUnit::OperateUnitDead(BeAttackType eAttackType, float fDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, bool bIsTimeOver, int iSkillTypeID)
{
	if (IsDead())
	{
		return;
	}
	if (HasOtherFlag(BUOF_NEVEDEATH))
	{
		SetHP(1.0f);
		return;
	}

	SetHP(0.0f);

	if (!bIsTimeOver)
	{
		//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		//{
		//	BeItem* pkItem = GetItemByPos(i);
		//	if (!pkItem)
		//	{
		//		continue;
		//	}
		//	const std::vector<BeSkill*>& rkItemSkill = pkItem->GetAllSkill();
		//	for (int iSkillIndex = 0; iSkillIndex < rkItemSkill.size(); iSkillIndex++)
		//	{
		//		BeSkill* pkSkill = rkItemSkill[iSkillIndex];
		//		if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEATHRELIVE) && pkSkill->CDComplete(this))
		//		{
		//			if ((IsDividMan() || IsGhost()))
		//			{
		//				continue;
		//			}
		//			TePtParam kParam;
		//			kParam.SetParam(BTP_pkTrgUnit, this);
		//			if (pkAttacker)
		//			{
		//				kParam.SetParam(BTP_pkAttacker, pkAttacker);
		//			}

		//			kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
		//			kParam.SetParam(BTP_pkTarget, this);
		//			kParam.SetParam(BTP_pkSkill, pkSkill);
		//			kParam.SetParam(BTP_pfDamage, &fDamage);
		//			kParam.SetParam(BTP_iFlag, iFlag);
		//			kParam.SetParam(BTP_iAttackType, eAttackType);

		//			gTrgMgr.FireTrigger(BTE_SKILL_SKIP_DEATH, kParam);

		//			pkSkill->SetLastUseTime(this, (int)gTime);
		//			pkItem->SetLastUseTime(gTime);
		//			pkItem->SetItemCDTime(pkSkill->GetCDTime(this));
		//			return;
		//		}
		//	}
		//}

		for (int i = 0; i < 4; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEATHRELIVE) && pkSkill->CDComplete(gUnit))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				if (pkAttacker)
				{
					kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
				}

				kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill);
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_SKIP_DEATH, kParam);

				pkSkill->SetLastUseTime(gUnit, (int)gTime);
				return;
			}
		}
	}

	SetDead();

	if (!bIsTimeOver)
	{
		TrgOnDead(eAttackType, fDamage, pkAttacker, iPlayer, iFlag, iSkillTypeID);
	}

	DelAllBuffer(HasFlag(BUF_RELIVE));

	m_kCommander.ClearAllCommands();

	SetFlag(BUF_REMOVE);
}

void TwUnit::OperateUnitRemove()
{
	if (IsDead())
	{
		return;
	}

	SetHP(0.0f);
	SetDead();
	DelAllBuffer(HasFlag(BUF_RELIVE));
	SetFlag(BUF_REMOVE);
}

float TwUnit::GetDamagedByFormula(const std::shared_ptr<TwUnit> pkAttacker, const BeAttackingAttr& rkAttackingAttr, float fDamage) const
{
	fDamage -= m_pkCurData->fDecDamage;
	if (fDamage < 0.0f)
	{
		return 0.0f;
	}
	float	fAttackerDamagePer = pkAttacker->GetDamagePer();
	fDamage *= fAttackerDamagePer;
	float	fSelfBeDamagePer = GetBeDamagePer();
	fDamage *= fSelfBeDamagePer;
	if (rkAttackingAttr.iSkillTypeID != 0)
	{
		float	fSkillDamagePer = pkAttacker->GetSkillDamagePer();
		fDamage *= fSkillDamagePer;
	}

	if (pkAttacker->GetClass() == UNIT_CLASSTYPE_SOLIDER)
	{
		if (GetClass() == UNIT_CLASSTYPE_BUILDING)
		{
			//if (pkAttacker->HasProperty(UNIT_PROPERTY_DECPOWDAMAGE))
			//{
			//	fDamage *= 1.8f;
			//}
			//else
			{
				fDamage *= 1.0f;
			}
		}
		else if (GetClass() == UNIT_CLASSTYPE_HERO)
		{
			fDamage *= 0.6f;
		}
	}
	else if (pkAttacker->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		//if (HasProperty(UNIT_PROPERTY_DECPOWDAMAGE))
		//{
		//	fDamage *= 0.7f;
		//}
	}

	if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_MAGIC)
	{
		float	fAntiMagic = 0.0f;
		GetAttackedMagicAttr(fAntiMagic, pkAttacker, pkAttacker->GetDecMagicArmor(), pkAttacker->GetPerDecMagicArmor());
		fDamage *= fAntiMagic;
	}
	else if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_PHYSICS)
	{
		float fAntiPhysic = 1.0f;
		GetAttackedPhysicAttr(fAntiPhysic, pkAttacker, pkAttacker->GetDecArmor(), pkAttacker->GetPerDecArmor());
		fDamage *= fAntiPhysic;
	}
	else if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_ABSOLUTE)
	{
	}
	else
	{
	}
	return fDamage;
}
void TwUnit::SetDead(void)
{
	TwCommand	kComm(TwCommandType::BCT_STOP);
	GiveCommand(kComm, TwGiveCmdType::BCT_DEATH);

	SetFlag(BUF_DEAD);
	gMap.DelUnitObstacle(gUnit, true);
	gUnitMgr->OnUnitDead(gUnit);
}

void	TwUnit::OnBeDamaged(BeAttackingAttr& rkAttackingAttr, bool bCanDead, bool bIgnoreInvincible, float fMaxHeroAbsDamage)
{
	//	gRandNum.RandInteger(RANDINFO);
	if (IsDead())
	{
		return;
	}

	if (!bIgnoreInvincible && IsInvincible())
	{
		return;
	}

	std::shared_ptr<TwUnit> pkAttacker = rkAttackingAttr.kAttacker;
	if (!pkAttacker)
	{
		return;
	}
	if (!pkAttacker->m_pkCurData)
	{
		return;
	}

	if (rkAttackingAttr.fDamage <= 0)
	{
		return;
	}

	float fMultiDamage = 0.f;
	if (rkAttackingAttr.eAttackType == BAT_NORMAL && GetClass() != UNIT_CLASSTYPE_BUILDING)
	{
		if (rkAttackingAttr.GetFlag() & BeAttackingFlag::BAF_NOT_BAOJI)
		{
		}
		else if (rkAttackingAttr.GetFlag() & BeAttackingFlag::BAF_BAOJI)
		{
			float	fBaoJiPerDamage = 0;
			if (m_pkCurData->fBaoJiDecDamage > 0.1 && m_pkCurData->fBaoJiDecDamage < 1.0) {
				fBaoJiPerDamage = 2.0f * pkAttacker->GetBaoJiDamagePer() * m_pkCurData->fBaoJiDecDamage;
			}
			else {
				fBaoJiPerDamage = 2.0f * pkAttacker->GetBaoJiDamagePer();
			}
			fMultiDamage = rkAttackingAttr.fDamage;
			rkAttackingAttr.fDamage *= fBaoJiPerDamage;

			rkAttackingAttr.SetFlag(BeAttackingFlag::BAF_NOT_BAOJI);
		}
	}

	if (rkAttackingAttr.HasFlag(BAF_FIRST) && rkAttackingAttr.HasFlag(BAF_AVOID) && !rkAttackingAttr.HasFlag(BAF_CANTAVOID))
	{
		TwPtParam kParam;
		kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
		kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
		kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_ATTACK_AVOIDED, kParam);
	}

	TrgOnPreBeDamaged(rkAttackingAttr.eAttackType, rkAttackingAttr.fDamage, rkAttackingAttr.fDamage, pkAttacker, rkAttackingAttr.iPlayer, rkAttackingAttr.GetFlag(), bCanDead, rkAttackingAttr.iSkillTypeID, rkAttackingAttr);

	float fDamage = rkAttackingAttr.fDamage;
	float fExtDamage = 0.0f;

	if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_ABSOLUTE)
	{
		float fDamageAbs = fDamage;
		DamagedByAbsNum(rkAttackingAttr.eAttackType, fDamageAbs, fDamageAbs, rkAttackingAttr.kAttacker, rkAttackingAttr.iPlayer, rkAttackingAttr.GetFlag(), bCanDead, rkAttackingAttr.bFirstAttack, rkAttackingAttr.iSkillTypeID, rkAttackingAttr.iSkillLevel, 0.f, 0.f, rkAttackingAttr.iBeDamagedEffect);
		return;
	}

	fDamage = GetDamagedByFormula(pkAttacker, rkAttackingAttr, fDamage);

	int		iAttackFlag = rkAttackingAttr.GetFlag();
	if ((iAttackFlag & BAF_FIRST) && (iAttackFlag & BAF_NORMAL))
	{
		if (GetClass() == UNIT_CLASSTYPE_BUILDING)
		{
			float	fAp = pkAttacker->GetMagicDamage() * 0.5f;
			float	fAddDamage = pkAttacker->GetAddDamage();
			if (fAp > fAddDamage)
			{
				BeAttackingAttr		kAPDamageAttr;
				kAPDamageAttr.fDamage = fAp;
				kAPDamageAttr.eAttackType = BAT_SKILL;
				kAPDamageAttr.kAttacker = pkAttacker->GetSharePtr();

				OnBeDamaged(kAPDamageAttr);
			}
		}
	}

	DamagedByAbsNum(rkAttackingAttr.eAttackType, fDamage, 1, rkAttackingAttr.kAttacker, rkAttackingAttr.iPlayer, rkAttackingAttr.GetFlag(), bCanDead, rkAttackingAttr.bFirstAttack, rkAttackingAttr.iSkillTypeID, rkAttackingAttr.iSkillLevel, rkAttackingAttr.fLeech, fMultiDamage, rkAttackingAttr.iBeDamagedEffect);
}

bool TwUnit::UnitCanUseActiveSkill(int iAttackedID, int& iSkillTypeID, int& iSkillLevel)
{
	std::shared_ptr<TwUnit> pkAttacked = gUnitMgr->GetUnitByID(iAttackedID);
	if (!pkAttacked)
	{
		return false;
	}

	return false;
}

void TwUnit::ResetSkill(void)
{
	for (int iPos = 0; iPos < iMaxHeroSkillNum; iPos++)
	{
		BeSkill* pkSkill = GetSkillByPos(iPos, true);
		if (pkSkill)
		{
			pkSkill->SetLastUseTime(gUnit, 0);
		}
	}
	DelBufferByClean();
	DelAllSkill();
    mpLearnSkillData.free(m_pkBackData->akLearnSkill.get());
}

bool TwUnit::UnitCanBeControl(int iPlayer) const
{
	if (GetControl(iPlayer))
	{
		return true;
	}
	return false;
}

bool TwUnit::UnitCanMove(bool bTemp) const
{
	if (bTemp && HasUnitCarryFlag(BUCF_CANNOTMOVE))
	{
		return false;
	}

	return true;
}

bool TwUnit::UnitCanAttack(const std::shared_ptr<TwUnit> pkTarget, bool bTemp, bool bForceAttackSameGroup) const
{
	if (bTemp && HasUnitCarryFlag(BUCF_CANNOTATTACK))
	{
		return false;
	}
	else if (pkTarget)
	{
		if (GetID() == pkTarget->GetID())
		{
			return false;
		}
		if (HasOtherFlag(BUOF_ONLYATTACKBUILD) && pkTarget->GetClass() != UNIT_CLASSTYPE_BUILDING)
		{
			return false;
		}
		if (pkTarget->IsInvincible() || pkTarget->HasFlag(BUF_ISINVINCIBLE))
		{
			return false;
		}
		else if (pkTarget->HasUnitCarryFlag(BUCF_ISPHYSICIMMUNITY) && !HasUnitCarryFlag(BUCF_IGNORE_PHYSICIMMUNITY))
		{
			return false;
		}
		else if (pkTarget->IsDead())
		{
			return false;
		}

		if (GetClass() == UNIT_CLASSTYPE_BUILDING)
		{
			return false;
		}
	}
	return true;
}

UnitUseSkillResultType TwUnit::UnitCanUseSkill(int iSkillTypeID, const std::shared_ptr<TwUnit> pkTarget, bool bTemp, bool bCheckPos, float fTargetPosX, float fTargetPosY, bool bArea, std::shared_ptr<TwUnit>* ppkTarget)
{
	BeSkill* pkSkill = GetSkill(iSkillTypeID);
	if (!pkSkill)
	{
		return UUSRT_EER_COMMON;
	}

	if (!pkSkill)
	{
		return UUSRT_EER_COMMON;
	}

	auto& pkRes = pkSkill->GetSkillRes();
	if (!pkRes)
	{
		return UUSRT_EER_COMMON;
	}

	SeCalSkillLvlData kData;
	if (!GetSkillLvlData(kData, iSkillTypeID))
	{
		return UUSRT_EER_COMMON;
	}

	if (pkSkill->IsSwitch() && pkSkill->IsActive())
	{
		return UUSRT_OK;
	}

	int iNeedMana = 0;
	auto& pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (pkSkillRes)
	{
		iNeedMana = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID)->iManaSpend[pkSkill->GetLevel() - 1];
	}

	if (GetMP() < iNeedMana)
	{
		return UUSRT_EER_MANA_NOT_ENOUGH;
	}

	if (!pkSkill->CDComplete(gUnit))
	{
		return UUSRT_EER_CD;
	}

	if (IsDead())
	{
		return UUSRT_EER_COMMON;
	}

	if (IsForbidSkill(pkSkill))
	{
		return UUSRT_EER_FORBID_SKILL;
	}

	return UUSRT_OK;
}

bool TwUnit::IsSkillTargetType(const std::shared_ptr<const SkillTable> pkRes, const std::shared_ptr<TwUnit> pkTarget) const
{
	if (!pkRes)
	{
		return false;
	}

	if (pkTarget->IsDead())
	{
		return false;
	}

	int iSkillTargetFlag = pkRes->uiTargetType;
	int iStaticProcFlag = 0;
	int iDynaPropFlag = 0;
	TranslateSkillTargetType(iSkillTargetFlag, iStaticProcFlag, iDynaPropFlag);

	return IsTargetUnit(pkTarget, iStaticProcFlag, iDynaPropFlag);
}

void TwUnit::TranslateSkillTargetType(int iSkillTargetFlag, int& iStaticProcFlag, int& iDynaPropFlag) const
{
	iStaticProcFlag = 0;

	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_HERO ? BUSP_CLASS_HERO : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_BUILDING ? BUSP_CLASS_BUILDING : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_BOSS ? BUSP_CLASS_BOSS : 0;

	iDynaPropFlag = 0;
	iDynaPropFlag |= BUDP_SUMMON | BUDP_NOTSUMMON;

	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_ENEMY ? BUDP_CAMP_ENEMY : 0;
	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_SELF ? BUDP_CAMP_SELF : 0;
	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_FRIEND ? BUDP_CAMP_ALLY : 0;
}

int TwUnit::GetAttackCD(void) const
{
	return m_pkCurData->iAttackCD;
}

bool TwUnit::IsTargetUnit(const std::shared_ptr<TwUnit> pkUnit, int iStaticProcFlag, int iDynaPropFlag) const
{
	if (!pkUnit)
	{
		return false;
	}

	int iStaticFlag = 0;
	if ((iStaticFlag & iStaticProcFlag) != iStaticFlag)
	{
		return false;
	}

	if (pkUnit->GetClass() == UNIT_CLASSTYPE_HERO)
	{
		iStaticFlag |= BUSP_CLASS_HERO;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		iStaticFlag |= BUSP_CLASS_BUILDING;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
	{
		iStaticFlag |= BUSP_CLASS_SOLIDER;
	}

	if ((iStaticFlag & iStaticProcFlag) != iStaticFlag)
	{
		return false;
	}

	if (pkUnit->HasProperty(UNIT_PROPERTY_NONE))
	{
		iStaticFlag |= BUSP_PROP_NONE;
	}

	if ((iStaticFlag & iStaticProcFlag) != iStaticFlag)
	{
		return false;
	}

	int iDynaFlag = 0;
	{
		iDynaFlag |= BUDP_NOTSUMMON;
	}

	if (pkUnit->IsInvincible() || pkUnit->HasFlag(BUF_ISINVINCIBLE))
	{
		iDynaFlag |= BUDP_INVINCIBLE;
	}

	if (pkUnit->HasUnitCarryFlag(BUCF_ISMAGICIMMUNITY))
	{
		iDynaFlag |= BUDP_MAGIC_IMMUNITY;
	}

	if (pkUnit->HasUnitCarryFlag(BUCF_ISPHYSICIMMUNITY) && !HasUnitCarryFlag(BUCF_IGNORE_PHYSICIMMUNITY))
	{
		iDynaFlag |= BUDP_PHYSIC_IMMUNITY;
	}

	if ((iDynaFlag & iDynaPropFlag) != iDynaFlag)
	{
		return false;
	}
	return true;
}

void TwUnit::TrgOnPreBeDamaged(int eAttackType, float& fDamage, float fRawDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, BeAttackingAttr& kAttr)
{
	if (pkAttacker && pkAttacker->m_pkCurData)
	{
		pkAttacker->TrgOnPreDamage(eAttackType, fDamage, gUnit, iPlayer, iFlag, bCanDead, kAttr);
	}
}

void TwUnit::TrgOnPreDamage(int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr)
{
	{
		TrgOnPreDamage_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, kAttr);
	}

	TrgOnPreDamage_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, kAttr);

	{
		//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		//{
		//	BeItem* pkItem = m_apkItem[i];
		//	if (pkItem && pkItem->GetOrgValid())
		//	{
		//		TrgOnPreDamage_T(pkItem->GetAllSkill(), (int)pkItem->GetAllSkill().size(), eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, kAttr, i);
		//	}
		//}
	}

	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_DAMAGETRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
				kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONPREDAMAGE, kParam);
			}
		}
	}
}

void TwUnit::TrgOnBeDamaged(int eAttackType, float& fDamage, float fRawDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID)
{
	{
		TrgOnBeDamaged_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);
	}

	TrgOnBeDamaged_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);

	{
		//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		//{
		//	BeItem* pkItem = m_apkItem[i];
		//	if (pkItem && pkItem->GetOrgValid())
		//	{
		//		TrgOnBeDamaged_T(pkItem->GetAllSkill(), (int)pkItem->GetAllSkill().size(), eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iAttackSkillTypeID, i);
		//	}
		//}
	}

	if (!m_apkBuffer.empty())
	{

		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_BEATTACKTRIGGER))
			{

				TwPtParam kParam;

				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);
				kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_fRawDamage, fRawDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);
				kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iAttackSkillTypeID);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONBEDAMAGE, kParam);
			}
		}
	}
	if (pkAttacker)
	{
		pkAttacker->TrgOnDamage(eAttackType, fDamage, gUnit, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);
	}
}

void TwUnit::TrgOnDamage(int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID)
{
	{
		TrgOnDamage_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);
	}

	TrgOnDamage_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);

	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_DAMAGETRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
				kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iFlagPtr, &iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);
				kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iAttackSkillTypeID);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONDAMAGE, kParam);
			}
		}
	}
}

void TwUnit::TrgOnDead(int eAttackType, float fDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, int iSkillTypeID)
{
	if (m_pkBackData->akLearnSkill)
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (!HasFlag(BUF_RELIVE) && pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEADTRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				if (pkAttacker)
				{
					kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
				}

				kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill);
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONDEAD, kParam);
			}
		}
	}

	if (!m_apkNormalSkill.empty())
	{
		for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
		{
			auto pkSkill = m_apkNormalSkill[i];
			if (!HasFlag(BUF_RELIVE) && pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEADTRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				if (pkAttacker)
				{
					kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
				}

				kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONDEAD, kParam);
			}
		}
	}

	if (!HasFlag(BUF_RELIVE))
	{
		if (!m_apkBuffer.empty())
		{
			for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
			{
				auto pkBuffer = m_apkBuffer[i];
				if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_DEADTRIGGER))
				{
					TwPtParam kParam;
					kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
					if (pkAttacker)
					{
						kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
					}

					kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);
					kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());
					kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
					kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
					kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);

					gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONDEAD_PRE, kParam);
				}
			}
		}
	}

	if (!HasFlag(BUF_RELIVE))
	{
		TwPtParam kParamDeath;
		kParamDeath.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
		if (pkAttacker)
		{
			kParamDeath.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
		}
		kParamDeath.SetParam(TwTrgParamID::BTP_iAttackerPlayer, iPlayer);
		kParamDeath.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
		kParamDeath.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
		kParamDeath.SetParam(TwTrgParamID::BTP_fDamageFace, pkAttacker->GetFace());
		kParamDeath.SetParam(TwTrgParamID::BTP_iFlag, iFlag);

		gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_DEATH, kParamDeath);

		if (pkAttacker)
		{
			pkAttacker->TrgOnKill(eAttackType, fDamage, gUnit, GetPlayer(), iFlag, iSkillTypeID);
		}
		if (!m_apkBuffer.empty())
		{
			for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
			{
				auto pkBuffer = m_apkBuffer[i];
				if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_DEADTRIGGER))
				{
					TwPtParam kParam;
					kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
					if (pkAttacker)
					{
						kParam.SetParam(TwTrgParamID::BTP_pkAttacker, pkAttacker.get());
					}

					kParam.SetParam(TwTrgParamID::BTP_pkTarget, this);
					kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());
					kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
					kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
					kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);

					gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONDEAD, kParam);
				}
			}
		}
	}

	//if (IsHero())
	//{
	//	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//	{
	//		BeItem* pkItem = m_apkItem[i];
	//		if (!pkItem)
	//		{
	//			continue;
	//		}
	//		if (!pkItem->GetValid())
	//		{
	//			continue;
	//		}
	//		for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
	//		{
	//			BeSkill* pkSkill = pkItem->m_akSkill[iSkill];
	//			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEADTRIGGER))
	//			{
	//				TePtParam kParam;
	//				kParam.SetParam(BTP_pkTrgUnit, this);
	//				if (pkAttacker)
	//				{
	//					kParam.SetParam(BTP_pkAttacker, pkAttacker);
	//				}
	//				kParam.SetParam(BTP_pkTarget, this);
	//				kParam.SetParam(BTP_pkSkill, pkSkill);
	//				kParam.SetParam(BTP_iItemPos, i + 1);
	//				kParam.SetParam(BTP_iTriggerItemID, pkItem->GetID());
	//				kParam.SetParam(BTP_pfDamage, &fDamage);
	//				kParam.SetParam(BTP_iFlag, iFlag);
	//				kParam.SetParam(BTP_iAttackType, eAttackType);

	//				gTrgMgr.FireTrigger(BTE_SKILL_ONDEAD, kParam);
	//				break;
	//			}
	//		}
	//	}
	//}
}


void TwUnit::TrgOnPreAttack(std::shared_ptr<TwUnit> pkTarget, BeAttackingAttr* pkAttackAttr)
{
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_ATTACKTRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill);
				kParam.SetParam(TwTrgParamID::BTP_pkAttackAttr, pkAttackAttr);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONPREATTACK, kParam);

				UpdateAttribute(true);
			}
		}
	}

	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_ATTACKTRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
				kParam.SetParam(TwTrgParamID::BTP_pkAttackAttr, pkAttackAttr);
				kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONPREATTACK, kParam);
			}
		}
	}
}

void TwUnit::TrgOnAttack(int iTargetID, BeAttackingAttr* pkAttackAttr)
{
	std::shared_ptr<TwUnit> pkTarget = gUnitMgr->GetUnitByID(iTargetID);

	{
		TrgOnAttack_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, pkTarget, pkAttackAttr);
	}

	TrgOnAttack_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), pkTarget, pkAttackAttr);

	//{
	//	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//	{
	//		BeItem* pkItem = m_apkItem[i];
	//		if (pkItem && pkItem->GetOrgValid())
	//		{
	//			TrgOnAttack_T(pkItem->GetAllSkill(), (int)pkItem->GetAllSkill().size(), pkTarget, pkAttackAttr, i);
	//		}
	//	}
	//}

	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_ATTACKTRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
				kParam.SetParam(TwTrgParamID::BTP_pkAttackAttr, pkAttackAttr);
				kParam.SetParam(TwTrgParamID::BTP_pkBuffer, pkBuffer.get());

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONATTACK, kParam);
			}
			//if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_YINSHENTUICHU))
			//{
			//	if (pkBuffer->HasCarryFlag(BUCF_ISINVISIBLE) && HasFlag(BUF_HASINVISIBLE))
			//	{
			//		TePtParam kParam;
			//		kParam.SetParam(BTP_pkTrgUnit, this);
			//		kParam.SetParam(BTP_pkTarget, pkTarget);
			//		kParam.SetParam(BTP_pkAttackAttr, pkAttackAttr);
			//		kParam.SetParam(BTP_pkBuffer, pkBuffer.get());

			//		gTrgMgr.FireTrigger(BTE_BUFFER_INVISIBLEBREAK, kParam);
			//	}
			//}
		}
	}

	if (HasFlag(BUF_HASINVISIBLE))
	{
		ClrFlag(BUF_HASINVISIBLE);
	}
}

void TwUnit::TrgOnSpell(int iSkillTypeID, int iSkillLevel, int iItemID, int iTargetID, float fTargetPosX, float fTargetPosY)
{
	auto& pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_PASSIVE)
	{
		return;
	}

	if (pkSkillRes->iUIShowPos < 1 || pkSkillRes->iUIShowPos > 4)
	{
		return;
	}

		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && (pkSkill->HasProperty(SKILL_SKILLPROPERTY_SKILLTRIGGER)))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill);
				kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
				kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, iSkillLevel);
				kParam.SetParam(TwTrgParamID::BTP_iItemID, iItemID);
				kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, iTargetID);
				kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, fTargetPosX);
				kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, fTargetPosY);

				if (pkSkill->HasProperty(SKILL_SKILLPROPERTY_SKILLTRIGGER))
				{
					gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONSPELL, kParam);
				}
			}
		}

	if (!m_apkNormalSkill.empty())
	{
		for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
		{
			auto pkSkill = m_apkNormalSkill[i];
			if (pkSkill && (pkSkill->HasProperty(SKILL_SKILLPROPERTY_SKILLTRIGGER)))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
				kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
				kParam.SetParam(TwTrgParamID::BTP_iSkillLevel, iSkillLevel);
				kParam.SetParam(TwTrgParamID::BTP_iItemID, iItemID);
				kParam.SetParam(TwTrgParamID::BTP_iSpellTargetID, iTargetID);
				kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosX, fTargetPosX);
				kParam.SetParam(TwTrgParamID::BTP_fSpellTargetPosY, fTargetPosY);
				if (pkSkill->HasProperty(SKILL_SKILLPROPERTY_SKILLTRIGGER))
				{
					gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONSPELL, kParam);
				}
			}
		}
	}

	//if (IsHero())
	//{
	//	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//	{
	//		BeItem* pkItem = m_apkItem[i];
	//		if (!pkItem || !pkItem->GetOrgValid())
	//		{
	//			continue;
	//		}
	//		for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
	//		{
	//			BeSkill* pkSkill = pkItem->m_akSkill[iSkill];
	//			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_SKILLTRIGGER))
	//			{
	//				if ((IsDividMan() || IsGhost()))
	//				{
	//					continue;
	//				}

	//				TePtParam kParam;
	//				kParam.SetParam(BTP_pkTrgUnit, this);
	//				kParam.SetParam(BTP_pkSkill, pkSkill);
	//				kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
	//				kParam.SetParam(BTP_iSkillLevel, iSkillLevel);
	//				kParam.SetParam(BTP_iItemID, iItemID);
	//				kParam.SetParam(BTP_iItemPos, i + 1);
	//				kParam.SetParam(BTP_iTriggerItemID, pkItem->GetID());
	//				kParam.SetParam(BTP_iSpellTargetID, iTargetID);
	//				kParam.SetParam(BTP_fSpellTargetPosX, fTargetPosX);
	//				kParam.SetParam(BTP_fSpellTargetPosY, fTargetPosY);

	//				gTrgMgr.FireTrigger(BTE_SKILL_ONSPELL, kParam);
	//				break;
	//			}
	//		}
	//	}
	//}

	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			//if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_SHIFA))
			//{
			//	auto pkAttacker = pkBuffer->GetUnit();
			//	TePtParam kParam;
			//	kParam.SetParam(BTP_pkTrgUnit, this);
			//	kParam.SetParam(BTP_pkAttacker, pkAttacker.get());
			//	kParam.SetParam(BTP_pkBuffer, pkBuffer.get());
			//	kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
			//	kParam.SetParam(BTP_iSkillLevel, iSkillLevel);
			//	kParam.SetParam(BTP_iItemID, iItemID);
			//	kParam.SetParam(BTP_iItemPos, i + 1);
			//	kParam.SetParam(BTP_iSpellTargetID, iTargetID);
			//	kParam.SetParam(BTP_fSpellTargetPosX, fTargetPosX);
			//	kParam.SetParam(BTP_fSpellTargetPosY, fTargetPosY);

			//	gTrgMgr.FireTrigger(BTE_BUFFER_ONSPELL, kParam);
			//}
			//if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_YINSHENTUICHU))
			//{
			//	if (pkBuffer->HasCarryFlag(BUCF_ISINVISIBLE) && HasFlag(BUF_HASINVISIBLE))
			//	{
			//		auto pkAttacker = pkBuffer->GetUnit();
			//		TePtParam kParam;
			//		kParam.SetParam(BTP_pkTrgUnit, this);
			//		kParam.SetParam(BTP_pkAttacker, pkAttacker.get());
			//		kParam.SetParam(BTP_pkBuffer, pkBuffer.get());

			//		gTrgMgr.FireTrigger(BTE_BUFFER_INVISIBLEBREAK, kParam);
			//	}
			//}
		}
	}

	if (HasFlag(BUF_HASINVISIBLE))
	{
		ClrFlag(BUF_HASINVISIBLE);
	}
}

void TwUnit::TrgOnUpdate(int iDeltaTime)
{
	if (HasFlag(BUF_NEEDUPDATESKILL) && (gTime % 100 == 0))
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			if (m_pkCurData)
			{
				auto pkSkill = (m_pkCurData->apkUISkill)[i];
				if (pkSkill)
				{
					pkSkill->Update(gUnit);
				}
			}
		}

		if (!m_apkNormalSkill.empty())
		{
			for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
			{
				auto pkSkill = m_apkNormalSkill[i];
				if (pkSkill)
				{
					pkSkill->Update(gUnit);
				}
			}
		}
	}

	//if (HasFlag(BUF_NEEDUPDATEITEMSKILL))
	//{
	//	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//	{
	//		BeItem* pkItem = m_apkItem[i];
	//		if (pkItem)
	//		{
	//			pkItem->Update(this, iDeltaTime);
	//		}
	//	}
	//}

	if (!m_apkBuffer.empty())
	{
		for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		{
			auto pkBuffer = m_apkBuffer[i];
			int iBufferCount = (int)m_apkBuffer.size();
			if (pkBuffer)
			{
				pkBuffer->Update(gUnit, iDeltaTime);
				if (iBufferCount != m_apkBuffer.size())
				{
					break;
				}
			}
		}
	}
}

void TwUnit::TrgOnKill(int eAttackType, float fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, int iSkillTypeID)
{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_KILLTRIGGER))
			{

				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkAttacker, this);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill);
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONKILL, kParam);
			}
		}

	if (m_apkNormalSkill.size())
	{
		for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
		{
			auto pkSkill = m_apkNormalSkill[i];
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_KILLTRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
				kParam.SetParam(TwTrgParamID::BTP_pkAttacker, this);
				kParam.SetParam(TwTrgParamID::BTP_pkTarget, pkTarget.get());
				kParam.SetParam(TwTrgParamID::BTP_pkSkill, pkSkill.get());
				kParam.SetParam(TwTrgParamID::BTP_pfDamage, &fDamage);
				kParam.SetParam(TwTrgParamID::BTP_iFlag, iFlag);
				kParam.SetParam(TwTrgParamID::BTP_iAttackType, eAttackType);
				kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_SKILL_ONKILL, kParam);
			}
		}
	}

	//if (IsHero())
	//{
	//	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//	{
	//		BeItem* pkItem = m_apkItem[i];
	//		if (!pkItem || !pkItem->GetOrgValid())
	//		{
	//			continue;
	//		}
	//		for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
	//		{
	//			BeSkill* pkSkill = pkItem->m_akSkill[iSkill];
	//			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_KILLTRIGGER))
	//			{
	//				if ((IsDividMan() || IsGhost()))
	//				{
	//					continue;
	//				}

	//				TePtParam kParam;
	//				kParam.SetParam(BTP_pkTrgUnit, this);
	//				kParam.SetParam(BTP_pkAttacker, this);
	//				kParam.SetParam(BTP_pkTarget, pkTarget);
	//				kParam.SetParam(BTP_pkSkill, pkSkill);
	//				kParam.SetParam(BTP_iItemPos, i + 1);
	//				kParam.SetParam(BTP_iTriggerItemID, pkItem->GetID());
	//				kParam.SetParam(BTP_pfDamage, &fDamage);
	//				kParam.SetParam(BTP_iFlag, iFlag);
	//				kParam.SetParam(BTP_iAttackType, eAttackType);

	//				gTrgMgr.FireTrigger(BTE_SKILL_ONKILL, kParam);
	//			}
	//		}
	//	}
	//}

	//if (!m_apkBuffer.empty())
	//{
	//	/////////////////////////////////////////////////////////
	//	for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
	//	{
	//		auto pkBuffer = m_apkBuffer[i];
	//		if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_JISHA))
	//		{

	//			TePtParam kParam;

	//			kParam.SetParam(BTP_pkTrgUnit, this);
	//			kParam.SetParam(BTP_pkAttacker, this);
	//			kParam.SetParam(BTP_pkTarget, pkTarget);
	//			kParam.SetParam(BTP_pkBuffer, pkBuffer.get());
	//			kParam.SetParam(BTP_pfDamage, &fDamage);
	//			kParam.SetParam(BTP_iFlag, iFlag);
	//			kParam.SetParam(BTP_iAttackType, eAttackType);

	//			gTrgMgr.FireTrigger(BTE_BUFFER_ONKILL, kParam);
	//		}
	//	}
	//}
}

bool TwUnit::PickMapItem(BeMapItem* pkMapItem)
{
	if (!pkMapItem)
	{
		return false;
	}

	if (HasUnitCarryFlag(BUCF_ONLYCANUSEITEM))
	{
		return false;
	}

	int iItemTypeID = pkMapItem->GetTypeID();
	int iSkillTypeID = 0;
	auto& pkItemRes = ItemTableMgr::Get()->GetItemTable(iItemTypeID);
	if (pkItemRes)
	{
		for (int i = 0; i < 7; ++i)
		{
			int iID = pkItemRes->iItemSkill[i];
			auto& pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iID);
			if (pkSkillRes)
			{
				if (pkSkillRes->uiOperateType != SKILL_OPERATETYPE_PASSIVE)
				{
					iSkillTypeID = iID;
					break;
				}
			}
		}
	}

	TwPtParam kParam;
	kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, this);
	kParam.SetParam(TwTrgParamID::BTP_iItemTypeID, iItemTypeID);
	kParam.SetParam(TwTrgParamID::BTP_iSkillTypeID, iSkillTypeID);
	kParam.SetParam(TwTrgParamID::BTP_pkMapItem, pkMapItem);

	gTrgMgr.FireTrigger(TwTriggerEvent::BTE_UNIT_PICKUP_ITEM, kParam);

	//if (pkItemRes)
	//{
	//	if (pkItemRes->uiItemProperty & ITEM_ITEMPROPERTY_PICKUSE)
	//	{
	//		BeSkill* pkSkill = GetSkill(iSkillTypeID);
	//		if (!pkSkill)
	//		{
	//			AddSkill(iSkillTypeID);
	//		}

	//		BeCommand	kComm(BeCommandType::BCT_SPELL, GetID(), 0.0f, 0.0f, iSkillTypeID);
	//		GiveCommand(kComm);
	//	}
	//}

	BeItemData kData = pkMapItem->GetItemData();
	//if (pkItemRes && (pkItemRes->uiItemProperty & ITEM_ITEMPROPERTY_PICKUSE))
	//{
	//	gMapItemMgr.DelMapItem(pkMapItem->GetID());

	//	return true;
	//}
	//else
	{
		gMapItemMgr.DelMapItem(pkMapItem->GetID());
	}

	return true;
}

void TwUnit::AddIgnoreUnit(int iUnitID, int iTime)
{

}

bool TwUnit::IsIgnoreUnit(int iUnitID) const
{
	return false;
}

void TwUnit::SetCheatAttr(void)
{
	BeBuffer* pkBuffer = this->AddBufferBegin('BG02', -1);
	if (pkBuffer)
	{
		pkBuffer->SetRemoveTime(-1);
		pkBuffer->SetNeedUpdate(true);
	}

	UpdateAttribute(true);
}

void TwUnit::SetAllLiveTime(int iAllTime)
{
	SetUnitAllLiveTime(iAllTime);
	SetUnitCurLiveTime(iAllTime);
	SetUnitCreateTime(gTime);
}

void TwUnit::SetChangeScale(int iBufferID, float fChangeScale, int iChangeTime /*= 0*/)
{
	//BufChangeScaleData kData;
	//kData.fScale = fChangeScale;
	//kData.iAllTime = iChangeTime;
	//kData.iCurTime = iChangeTime;

	//	m_kMapChangeScale[iBufferID] = kData;
}

void TwUnit::SetColorWithBuf(int iBufferID, unsigned int dwColor)
{

}

void TwUnit::UpdateTransrate(int iDeltaTime)
{
	if (HasUnitCarryFlag(BUCF_ISINVISIBLE))
	{
		m_eActorTransType = UATT_WHOLE_TRANS;
		m_bNeedTrans = true;
	}
	else
	{
		m_eActorTransType = UATT_NONE_TRANS;
	}

	if (m_bNeedTrans)
	{
		switch (m_eActorTransType)
		{
		case UATT_NONE_TRANS:
		{
			if (m_iAddTransTime >= m_iTransTime)
			{
				ClrFlag(BUF_HASINVISIBLE);
				m_iAddTransTime = 0;
				m_iTransTime = 0;
			}
			else
			{
				m_iAddTransTime += iDeltaTime;
			}

			break;
		}
		case UATT_WHOLE_TRANS:
		{
			if (HasFlag(BUF_HASINVISIBLE))
			{
				break;
			}
			if (m_iAddTransTime >= m_iTransTime)
			{
				SetFlag(BUF_HASINVISIBLE);
				//m_iTransTime = 0;
				m_iAddTransTime = 0;
			}
			else
			{
				m_iAddTransTime += iDeltaTime;
			}
			break;
		}
		default:
		{
			ClrFlag(BUF_HASINVISIBLE);
			m_iTransTime = 0;
			m_iAddTransTime = 0;
			break;
		}
		}
	}
}

void TwUnit::SetUnitvisible(bool bVisible)
{
	if (!bVisible)
	{
		m_eActorTransType = UATT_WHOLE_TRANS;
	}
	else
	{
		m_eActorTransType = UATT_NONE_TRANS;
	}
}

void	TwUnit::AddAttach(int pcMdlFile, int iRemoveTime, int iAttackerID, bool bChangeFace)
{
	BeShareAttachData kData;
	kData.iRemoveTime = gTime + iRemoveTime;
	kData.iUnitLogicID = GetID();
	kData.iModleName = pcMdlFile;
	kData.bChangeFace = bChangeFace;
	kData.iAttackerID = iAttackerID;

	//gMain->AddAttachData(kData);
}

bool TwUnit::DelAttachByName(int iModelID)
{
	BeShareAttachData kData;
	kData.iRemoveTime = gTime;
	kData.iUnitLogicID = GetID();
	kData.iModleName = iModelID;
	kData.bRemove = true;

	//gMain->AddAttachData(kData);

	return true;
}

void TwUnit::AddControlSummonUnitID(int iUnitID)
{
	//	m_akControlSummonID.PushBack(iUnitID);
}

void TwUnit::SafeDelBuf(BeBuffer* pkBuffer, bool bNeedRecordChange)
{
	if (!pkBuffer)
	{
		return;
	}

	int iID = pkBuffer->GetID();
	mpBuffer.free(pkBuffer);
}

void TwUnit::ClearOutputData()
{
	//for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
	//{
	//	m_abEverInPlayerVision[i] = m_abInPlayerVision[i];
	//}

	m_kShareSelfDataLast = m_kShareSelfDataCur;
	OutputFlag = TwUnitOutputFlag::BSUDCF_NULL;

	m_iPathFindSucessTime = 0;
}

#define CLIENTNONEEDFLAG (BUF_IGNOREDOODADOBS|BUF_NOOBSTACLE|BUF_IGNOREUNITOBS|BUF_IGNOREFIXEDOBS|BUF_HASHALOSKILL|BUF_OBSTACLESET|BUF_MOVING|BUF_ISBLOCKED |BUF_ISPERSISTSKILL)

void TwUnit::SetFlag(int iFlag, bool bNeedRecordChange)
{
	int iTempFlag = iFlag;
	iTempFlag &= ~CLIENTNONEEDFLAG;
	if (iTempFlag && !HasFlag(iTempFlag))
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_FLAG);
	}
	//	}
	m_iFlag |= iFlag;
}

void TwUnit::ClrFlag(int iFlag, bool bNeedRecordChange)
{
	if (/*gMain->IsServerMode() && */bNeedRecordChange)
	{
		int iTempFlag = iFlag;
		iTempFlag &= ~CLIENTNONEEDFLAG;
		if (iTempFlag && (m_iFlag & iTempFlag) != 0)
		{
			SetOutputFlag(TwUnitOutputFlag::BSUDCF_FLAG);
		}
	}
	m_iFlag &= ~iFlag;
}

void TwUnit::UpdateLiveTime(int iDeltaTime)
{
	if (GetUnitAllLiveTime() != 0 && GetUnitCurLiveTime() > 0)
	{
		if (!HasFlag(BUF_ISPERSIST))
		{
			DecUnitCurTime(iDeltaTime);
		}

		if (GetUnitCreateTime() != 0 && ((int)gTime - (int)GetUnitCreateTime() > GetUnitAllLiveTime()))
		{
			DecUnitCurTime(GetUnitCurLiveTime());
		}
	}
}

void TwUnit::UpdateHP(int iDeltaTime)
{
	if (IsDead())
	{
		return;
	}
	if (gTime % 100 != 0)
	{
		return;
	}
	if (!HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		if (m_pkCurData->fRegenHP && m_pkCurData->fHP < m_pkCurData->fMaxHP)
		{
			SetHP(GetHP() + (m_pkCurData->fRegenHP * 100) / 1000.0f, false);
		}
	}
}

void TwUnit::UpdateMP(int iDeltaTime)
{
	if (IsDead())
	{
		return;
	}
	if (gTime % 100 != 0)
	{
		return;
	}

	if (!HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		if (m_pkCurData->fRegenMP && m_pkCurData->fMP < m_pkCurData->fMaxMP)
		{
			SetMP(GetMP() + (m_pkCurData->fRegenMP * 100) / 1000.0f, false);
			if (m_pkCurData->fMP > m_pkCurData->fMaxMP)
			{
				SetMP(m_pkCurData->fMaxMP, false);
			}
		}
	}
}

void TwUnit::UpdateBuffer(int iDeltaTime)
{
	if (!m_apkBuffer.empty())
	{
		bool bNeedUpdate = false;
		bool bUpdateNormal = false;

		int iDelNum = 0;

		int iHasDelNum = 0;

		int iReserveNum = 0;

		//for (int i = 0; i < (int)m_apkBuffer.size(); ++i)
		//{
		//	BeBuffer* pkBuffer = m_apkBuffer[i];
		//	if (!pkBuffer)
		//	{
		//		continue;
		//	}

		//	if (pkBuffer->GetHasDel())
		//	{
		//		bNeedUpdate = true;
		//		if (pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR))
		//		{
		//			bUpdateNormal = true;
		//		}

		//		bool bNeedRecordChange = false;
		//		{
		//			bNeedRecordChange = true;
		//		}
		//		SafeDelBuf(pkBuffer, bNeedRecordChange);

		//		m_apkBuffer.erase(i);
		//		i--;
		//	}
		//	else if (pkBuffer->GetRemoveTime() != -1 && pkBuffer->GetRemoveTime() <= gTime)
		//	{
		//		bNeedUpdate = true;
		//		if (pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR))
		//		{
		//			bUpdateNormal = true;
		//		}

		//		bool bNeedRecordChange = false;
		//		OnDelBuffer(pkBuffer);
		//		SafeDelBuf(pkBuffer, bNeedRecordChange);

		//		m_apkBuffer.EraseIndexSwap(i);
		//		i--;
		//	}
		//	else if (pkBuffer->GetNeedUpdate())
		//	{
		//		bNeedUpdate = true;
		//		bUpdateNormal = true;
		//		pkBuffer->SetNeedUpdate(false);
		//	}
		//}

		if (bNeedUpdate)
		{
			UpdateAttribute(bUpdateNormal);
		}
	}
}

void TwUnit::UpdateAttribute(bool bUpdateNormal)
{
	auto pkOrgData = m_pkBackData;
	auto pkCurData = m_pkCurData;
	float afChange[NAT_MAX_NUM];
	float afChangeTemp[BCT_NUM][NAT_MAX_NUM][2];

	const int iCommon1Size = 10;
	float akCommonInfo[iCommon1Size] = { 0.f };
	akCommonInfo[0] = pkCurData->fHP;
	akCommonInfo[1] = pkCurData->fMP;
	akCommonInfo[2] = pkCurData->fMaxHP;
	akCommonInfo[3] = pkCurData->fMaxMP;
	akCommonInfo[4] = pkCurData->fMoveSpeed;
	akCommonInfo[5] = pkCurData->fRegenHP;
	akCommonInfo[6] = pkCurData->fRegenMP;
	akCommonInfo[7] = pkCurData->iAttackCD;

	const int iSelfSize = 1;
	float akSelfData[iSelfSize] = { 0.f };
	akSelfData[0] = pkCurData->fDecCDTime;
	float temp1, temp2 = 0.f;

	int iOldCarryFlag = m_iCarryFlag;
	int iOldImmunityFlag = m_iImmunityFlag;

	m_apkCarry.clear();

	iOldCarryFlag = m_iCarryFlag;
	m_iCarryFlag = 0;
	iOldImmunityFlag = TwUnit::m_iImmunityFlag;
	m_iImmunityFlag = 0;

	UpdateValidItem();
	UpdateValidBuffer();
	UpdateValidSkill();

	int iMaskCarryFlag = m_iCarryFlag ^ iOldCarryFlag;

	if (!bUpdateNormal)
	{
		if (m_iCarryFlag != iOldCarryFlag)
		{
			SetOutputFlag(TwUnitOutputFlag::BSUDCF_CARRYFLAG);
		}
		return;
	}

	memset(afChange, 0, sizeof(afChange));
	float afAntiMagic[BCT_NUM];
	memset(afAntiMagic, 0, sizeof(afAntiMagic));

	float afDecAntiMagic[BCT_NUM];
	memset(afDecAntiMagic, 0, sizeof(afDecAntiMagic));

	memset(afChangeTemp, 0, sizeof(afChangeTemp));
	for (auto itr = m_apkCarry.begin(); itr != m_apkCarry.end(); ++itr)
	{
		auto pkCarry = *itr;
		{
			pkCarry->ApplyNormalAttr(afChangeTemp, false, m_iImmunityFlag);
			float fAntiRate = pkCarry->GetAttackedAntiMagic();
			if (fAntiRate > 0.0f && afAntiMagic[pkCarry->GetType()] <= fAntiRate)
			{
				afAntiMagic[pkCarry->GetType()] = fAntiRate;
			}
			else if (fAntiRate < 0.0f)
			{
				afDecAntiMagic[pkCarry->GetType()] += fAntiRate;
			}
		}
	}
	for (int i = 0; i < NAT_MAX_NUM; ++i)
	{
		switch (i)
		{
		case NAT_PER_SKILLBLAST:
		{
			for (int j = 0; j < BCT_NUM; ++j)
			{
				afChange[i] = 1.0f - (1.0f - afChange[i]) * (1.0f - (afChangeTemp[j][i][0] + afChangeTemp[j][i][1]));
			}
			break;
		}
		case NAT_PER_BAOJI:
		{
			afChange[i] = 1;

			for (int j = 0; j < BCT_NUM; ++j)
			{
				if (afChangeTemp[j][i][0] > 0)
				{
					afChange[i] *= afChangeTemp[j][i][0];
				}
			}
			break;
		}
		case NAT_PER_MOVE_SPEED:
		{
			afChange[i] = 1;
			temp1 = 0;
			temp2 = 1;

			for (int j = 0; j < BCT_NUM; ++j)
			{
				if (afChangeTemp[j][i][0] > 0) {

					temp1 += afChangeTemp[j][i][0];

				}

				if (afChangeTemp[j][i][1] > 0) {

					temp2 *= afChangeTemp[j][i][1];
				}
			}

			afChange[i] = temp1 - (1 - temp2);
			break;
		}
		case NAT_TOUGHNESS:
		{
			afChange[i] = 1;

			for (int j = 0; j < BCT_NUM; ++j)
			{
				if (afChangeTemp[j][i][0] > 0)
				{
					afChange[i] *= (1 - afChangeTemp[j][i][0]);
				}
			}

			afChange[i] = 1 - afChange[i];
			break;
		}
		case NAT_DEC_CDTIME:
		case NAT_DEC_MPCOST:
		case NAT_PER_CDTIME:
		default:
		{
			for (int j = 0; j < BCT_NUM; ++j)
			{
				afChange[i] += afChangeTemp[j][i][0] + afChangeTemp[j][i][1];
			}
			break;
		}
		}
	}
	pkCurData->fMagicArmor = (pkCurData->fOrgMagicArmor + afChange[NAT_MAGIC_ARMOR]) * (1.0f + afChange[NAT_PER_MAGIC_ARMOR]);
	pkCurData->fDecAntiMagic = afChange[NAT_DEC_MAGIC_RESISTANCE];

	pkCurData->fAddArmorPer = afChange[NAT_PER_ARMOR];
	pkCurData->fAddArmor = afChange[NAT_ABS_ARMOR];
	pkCurData->fAddDamage = afChange[NAT_ABS_DAMAGE] + afChange[NAT_PER_DAMAGE_BASE] * (pkCurData->fBaseDamage);
	if (pkCurData->fAddDamage < -pkCurData->fBaseDamage)
	{
		pkCurData->fAddDamage = -pkCurData->fBaseDamage;
	}
	pkCurData->fDamagePer = (1.0f + afChange[NAT_PER_DEMAGE]) * pkCurData->fDamagePerOrg;
	pkCurData->fBeDamagePer = (1.0f + afChange[NAT_PER_BEDEMAGED]) * pkCurData->fBeDamagePerOrg;
	pkCurData->fMagicDamage = (pkOrgData->fOrgMagicDamage + afChange[NAT_ABS_MAGIC]) * (1.0f + afChange[NAT_PER_MAGIC]);
	pkCurData->fSkillBlastPer = (afChange[NAT_PER_SKILLBLAST]);
	pkCurData->fSkillDamagePer = 1.0f + afChange[NAT_PER_DAMAGE_SKILL];

	pkCurData->fArmor = (pkCurData->fBaseArmor + pkCurData->fAddArmor) * (1 + pkCurData->fAddArmorPer);

	pkCurData->fAttackRange = m_pkBackData->pkRes->fAttackRange + afChange[NAT_ABS_ATTACKRANGE];
	pkCurData->fMaxHP = (pkCurData->fBaseMaxHP * (1.0f + afChange[NAT_PER_BASE_MAXHP]) + afChange[NAT_ABS_MAX_HP]);

	pkCurData->fDecDamage = afChange[NAT_ABS_DECDAMAGE];
	pkCurData->fRegenHP = pkCurData->fBaseRegenHP + afChange[NAT_ABS_REGEN_HP] + pkCurData->fBaseRegenHP * afChange[NAT_PER_REGEN_HP];

	{
		pkCurData->fMaxMP = pkCurData->fBaseMaxMP + afChange[NAT_ABS_MAX_MP];
		pkCurData->fRegenMP = pkCurData->fBaseRegenMP + afChange[NAT_ABS_REGEN_MP] + pkCurData->fBaseRegenMP * afChange[NAT_PER_REGEN_MP];
	}
	{
		pkCurData->fMoveSpeed = (pkCurData->fBaseMoveSpeed + afChange[NAT_BASE_MOVE_SPEED]) * (1.0f + afChange[NAT_PER_MOVE_SPEED]) + afChange[NAT_ABS_MOVE_SPEED];
	}
	float	fTempMoveSpeed = pkCurData->fMoveSpeed;
	if (fTempMoveSpeed > 490.0f)
	{
		pkCurData->fMoveSpeed = (fTempMoveSpeed * 0.5f) + 230.0f;
	}
	else if (fTempMoveSpeed > 415.0f && fTempMoveSpeed <= 490.0f)
	{
		pkCurData->fMoveSpeed = (fTempMoveSpeed * 0.8f) + 83;
	}
	else if (fTempMoveSpeed < 220.0f)
	{
		pkCurData->fMoveSpeed = (fTempMoveSpeed * 0.5f) + 110.0f;
	}
	if (pkCurData->fMoveSpeed > 900.0f)
	{
		pkCurData->fMoveSpeed = 900.0f;
	}
	if (pkCurData->fMoveSpeed < 140.0f)
	{
		pkCurData->fMoveSpeed = 140.0f;
	}
	pkCurData->fLeech = afChange[NAT_LEECH];
	pkCurData->fMagicLeech = afChange[NAT_MAGIC_LEECH];
	pkCurData->fDecArmor = afChange[NAT_DEC_ARMOR];
	pkCurData->fPerDecArmor = std::min(afChange[NAT_PER_DEC_ARMOR], 1.0f);
	pkCurData->fPerDecMagicArmor = afChange[NAT_PER_FASHUCHUANTOU];
	pkCurData->fToughness = afChange[NAT_TOUGHNESS];
	pkCurData->fDecMPCost = afChange[NAT_DEC_MPCOST];
	pkCurData->fBaoJi = afChange[NAT_BAOJI];
	if (pkCurData->fBaoJi > 1.0f)
	{
		pkCurData->fBaoJi = 1.0f;
	}
	pkCurData->fBaoJiDamagePer = afChange[NAT_PER_BAOJI];
	pkCurData->fBaoJiDecDamage = afChange[NAT_PER_DEC_BAOJI];

	{
		pkCurData->fDecCDTime = std::min(afChange[NAT_DEC_CDTIME], 0.4f + afChange[NAT_DEC_CDTIME_MAX]);
	}

	pkCurData->fPerCDTime = std::min(afChange[NAT_PER_CDTIME], 0.9f);
	pkCurData->fEnmityPoint = afChange[NAT_ABS_ENMITY_POINT];
	pkCurData->fPerDamageReduce = afChange[NAT_PER_DAMAGE_REDUCE];
	pkCurData->fEvadeRate = afChange[NAT_PER_AVOIDPHYSIC];
	pkCurData->fTreatment = afChange[NAT_PER_ADDHP];

	float fPrimaryAddDamage = 0.0f;
	pkCurData->fRegenHP += (pkCurData->fMaxHP * afChange[NAT_PER_REGEN_MAXHP]);

	pkCurData->fAddDamage += afChange[NAT_PER_DAMAGE_ADD] * (pkCurData->fAddDamage + pkCurData->fBaseDamage);
	pkCurData->fDamage = pkCurData->fBaseDamage + pkCurData->fAddDamage;

		if (pkCurData->fHP <= 0.0f)
		{
			pkCurData->fHP = 1.0f;
		}
		if (pkCurData->fMP <= 0.0f)
		{
			pkCurData->fMP = 1.0f;
		}

	float fDivAttackSpeed = afChange[NAT_PER_ATTACK_SPEED];

	pkCurData->iAttackCD = m_pkCurData->iOrgAttackCD;

	float	fBaseNum = 1000.0f / m_pkCurData->iOrgAttackCD;
	float	fAddNum = 1000.0f / pkCurData->pkRes->fAttackBackPt * fDivAttackSpeed;
	float	fLevelNum = 1000.0f / pkCurData->pkRes->fAttackBackPt * 0.01f * pkCurData->pkRes->fAttackSpeedAddUp * (GetLevel() - 1);
	pkCurData->iAttackCD = 1000.0f / (fBaseNum + fAddNum + fLevelNum);

	if (pkCurData->iAttackCD < 400)
	{
		pkCurData->iAttackCD = 400;
	}

	pkCurData->iAttackDamagePt = (int)((float)(pkCurData->pkRes)->uiAttackDamagePt * (pkCurData->iAttackCD / pkCurData->pkRes->fAttackBackPt));
	if (pkCurData->iAttackDamagePt > pkCurData->iAttackCD)
	{
		pkCurData->iAttackDamagePt = pkCurData->iAttackCD;
	}
	pkCurData->iAttackBackPt = pkCurData->iAttackCD - pkCurData->iAttackDamagePt;

	if (akCommonInfo[0] != pkCurData->fHP)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_CURRENT_HP);
	}
	if (akCommonInfo[1] != pkCurData->fMP)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_CURRENT_MP);
	}
	if (akCommonInfo[2] != pkCurData->fMaxHP)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_MAXHP);
	}
	if (akCommonInfo[3] != pkCurData->fMaxMP)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_MAXMP);
	}
	if (akCommonInfo[4] != pkCurData->fMoveSpeed)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_MOVESPEED);
	}
	if (akCommonInfo[5] != pkCurData->fRegenHP)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_REGENHP);
	}
	if (akCommonInfo[6] != pkCurData->fRegenMP)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_REGENMP);
	}
	if ((int)akCommonInfo[7] != pkCurData->iAttackCD)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_ATTACKCD);
	}

	if (m_iCarryFlag != iOldCarryFlag)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_CARRYFLAG);
	}
}

void TwUnit::CopyAttribute(std::shared_ptr<TwUnit> pkUnit)
{
	SetPlayer(pkUnit->GetPlayer());
	SetLevel(pkUnit->GetLevel());
	SetCurExp(pkUnit->GetCurExp());

	m_pkBackData->fSkillDamagePer = pkUnit->GetSkillDamagePer();
	OnPropertyUpdate(GetLevel());

	m_pkCurData->fBaseMaxHP = pkUnit->GetBaseMaxHP();
	m_pkCurData->fBaseMaxMP = pkUnit->GetBaseMaxMP();
	SetBaseArmor(pkUnit->GetBaseArmor());
	SetMaxHP(pkUnit->GetMaxHP());
	m_pkCurData->fMaxMP = pkUnit->GetMaxMP();
	SetHP(pkUnit->GetHP());
	SetMP(pkUnit->GetMP());
	SetFace(pkUnit->GetFace());
	SetTarFace(pkUnit->GetTarFace());
	SetUsedSkillPoint(pkUnit->GetUsedSkillPoint());
}



bool TwUnit::IsDamageTime(int iDeltaTime)
{
	int iRealTime = GetRealDamagePt();
	int iTime = m_pkCurData->iActionCurTime + iDeltaTime;

	return iTime > iRealTime;
}

void	TwUnit::SellItem(int iPos)
{
	BeItem* pkItem = GetItemByPos(iPos);
	if (!pkItem)
	{
		return;
	}

	int	iPrice = pkItem->GetPriceOfBuy() * 7 / 10;

	if (pkItem->IsCanSplice())
	{
		if (pkItem->GetPileCount() > 1)
		{
			pkItem->SetPileCount(pkItem->GetPileCount() - 1);
			pkItem->SetUseCount(pkItem->GetOrgUseCount());
		}
		else
		{
			DelItemByPos(pkItem->GetPackagePos());
		}
	}
	else
	{
		DelItemByPos(iPos);
	}

	int iPlayer = GetPlayer();
	//gMain->AddPlayerGold(iPlayer, iPrice, false);

	UpdateItemPassiveSkill();
}

void	TwUnit::BuyItem(int iItemTypeID)
{
	//const ItemTable* pkItemRes = gMain->GetResItem(iItemTypeID);
	//if (!pkItemRes)
	//{
	//	return;
	//}
	//int iPlayer = GetPlayer();
	//int iHasMoney = gMain->GetPlayerGold(iPlayer);
	//int iPrice = pkItemRes->iItemPrice;
	//int	iHasItemAllPrice = 0;

	//if (pkItemRes->iPileCount > 1)
	//{
	//	BeItem* pkHasItem = GetItemByTypeID(iItemTypeID);
	//	if (pkHasItem)
	//	{
	//		if (iHasMoney < iPrice)
	//		{
	//			return;
	//		}

	//		if (pkHasItem->GetPileCount() > pkItemRes->iPileCount)
	//		{
	//			return;
	//		}
	//		pkHasItem->SetPileCount(pkHasItem->GetPileCount() + 1);

	//		gMain->AddPlayerGold(iPlayer, -iPrice);

	//		return;
	//	}
	//}

	//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//{
	//	if (m_apkItem[i])
	//	{
	//		m_apkItem[i]->SetComposeUse(false);
	//	}
	//}
	//std::vector<int>	kHasItemVec;
	//const	ItemComposeTable* pkComposeRes = gMain->GetResItemCompose(iItemTypeID);
	//if (pkComposeRes)
	//{
	//	int		iNeedItem1 = pkComposeRes->uiNeedItemTypeID1;
	//	BeItem* pkFindItem = GetComposeItemByTypeID(iNeedItem1);
	//	if (pkFindItem)
	//	{
	//		kHasItemVec.push_back(pkFindItem->GetPackagePos());
	//		iHasItemAllPrice += pkFindItem->GetPriceOfBuy();
	//		pkFindItem->SetComposeUse(true);
	//	}
	//	else
	//	{
	//		const	ItemComposeTable* pkSecondComposeRes = gMain->GetResItemCompose(iNeedItem1);
	//		if (pkSecondComposeRes)
	//		{
	//			int		iSecondNeed1 = pkSecondComposeRes->uiNeedItemTypeID1;
	//			BeItem* pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed1);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//			int		iSecondNeed2 = pkSecondComposeRes->uiNeedItemTypeID2;
	//			pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed2);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//			int		iSecondNeed3 = pkSecondComposeRes->uiNeedItemTypeID3;
	//			pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed3);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//		}
	//	}

	//	int		iNeedItem2 = pkComposeRes->uiNeedItemTypeID2;
	//	pkFindItem = GetComposeItemByTypeID(iNeedItem2);
	//	if (pkFindItem)
	//	{
	//		kHasItemVec.push_back(pkFindItem->GetPackagePos());
	//		iHasItemAllPrice += pkFindItem->GetPriceOfBuy();
	//		pkFindItem->SetComposeUse(true);
	//	}
	//	else
	//	{
	//		const	ItemComposeTable* pkSecondComposeRes = gMain->GetResItemCompose(iNeedItem2);
	//		if (pkSecondComposeRes)
	//		{
	//			int		iSecondNeed1 = pkSecondComposeRes->uiNeedItemTypeID1;
	//			BeItem* pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed1);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//			int		iSecondNeed2 = pkSecondComposeRes->uiNeedItemTypeID2;
	//			pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed2);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//			int		iSecondNeed3 = pkSecondComposeRes->uiNeedItemTypeID3;
	//			pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed3);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//		}
	//	}

	//	int		iNeedItem3 = pkComposeRes->uiNeedItemTypeID3;
	//	pkFindItem = GetComposeItemByTypeID(iNeedItem3);
	//	if (pkFindItem)
	//	{
	//		kHasItemVec.push_back(pkFindItem->GetPackagePos());
	//		iHasItemAllPrice += pkFindItem->GetPriceOfBuy();
	//		pkFindItem->SetComposeUse(true);
	//	}
	//	else
	//	{
	//		const	ItemComposeTable* pkSecondComposeRes = gMain->GetResItemCompose(iNeedItem3);
	//		if (pkSecondComposeRes)
	//		{
	//			int		iSecondNeed1 = pkSecondComposeRes->uiNeedItemTypeID1;
	//			BeItem* pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed1);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//			int		iSecondNeed2 = pkSecondComposeRes->uiNeedItemTypeID2;
	//			pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed2);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//			int		iSecondNeed3 = pkSecondComposeRes->uiNeedItemTypeID3;
	//			pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed3);
	//			if (pkSecondFindItem)
	//			{
	//				kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
	//				iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
	//				pkSecondFindItem->SetComposeUse(true);
	//			}
	//		}
	//	}
	//}

	//iPrice -= iHasItemAllPrice;
	//if (iHasMoney < iPrice)
	//{
	//	return;
	//}
	//int iOrgItemData = 0;
	//for (int iDelItemIndex = 0; iDelItemIndex < kHasItemVec.size(); iDelItemIndex++)
	//{
	//	if (GetItemByPos(kHasItemVec[iDelItemIndex])->GetTypeID() == 'IP06') {
	//		iOrgItemData = GetItemByPos(kHasItemVec[iDelItemIndex])->GetData();
	//	}

	//	DelItemByPos(kHasItemVec[iDelItemIndex]);
	//}

	//bool	bHasPos = false;
	//for (int iItemIndex = 0; iItemIndex < iMaxBuyItem; iItemIndex++)
	//{
	//	if (!m_apkItem[iItemIndex])
	//	{
	//		bHasPos = true;
	//		break;
	//	}
	//}
	//if (!bHasPos)
	//{
	//	return;
	//}
	//AddItem(iItemTypeID, -1, 0, iOrgItemData);
	//gMain->AddPlayerGold(iPlayer, -iPrice);

	//UpdateItemPassiveSkill();

	//SetTabInfoFlag(BTCF_BIGSKILL);
}

void	TwUnit::UpdateItemPassiveSkill()
{
	//ClearItemPassiveSkill();

	//for (int iIndex = 0; iIndex < iMaxBuyItem; ++iIndex)
	//{
	//	BeItem* pkItem = m_apkItem[iIndex];
	//	if (pkItem)
	//	{
	//		pkItem->ClearForbidSkill();
	//	}
	//}

	//for (int iIndex = 0; iIndex < iMaxBuyItem; ++iIndex)
	//{
	//	BeItem* pkItem = m_apkItem[iIndex];
	//	if (pkItem)
	//	{
	//		if (gMain->IsPveMap())
	//		{
	//			const	ZhanChangItemTable* pkItemRes = pkItem->GetZhanChangResPtr();
	//			if (pkItemRes)
	//			{
	//				for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
	//				{
	//					int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
	//					const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
	//					if (pkItemPassiveRes)
	//					{
	//						for (int iPassiveSkillIndex = 0; iPassiveSkillIndex < 10; ++iPassiveSkillIndex)
	//						{
	//							int		iPassiveSkillTypeID = pkItemPassiveRes->iKillSkill[iPassiveSkillIndex];
	//							if (iPassiveSkillTypeID > 'S000')
	//							{
	//								AddItemPassiveSkill(iPassiveSkillTypeID);
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			const	ItemTable* pkItemRes = pkItem->GetResPtr();
	//			if (pkItemRes)
	//			{
	//				for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
	//				{
	//					int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
	//					const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
	//					if (pkItemPassiveRes)
	//					{
	//						for (int iPassiveSkillIndex = 0; iPassiveSkillIndex < 10; ++iPassiveSkillIndex)
	//						{
	//							int		iPassiveSkillTypeID = pkItemPassiveRes->iKillSkill[iPassiveSkillIndex];
	//							if (iPassiveSkillTypeID > 'S000')
	//							{
	//								AddItemPassiveSkill(iPassiveSkillTypeID);
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//for (int iIndex = 0; iIndex < iMaxBuyItem; ++iIndex)
	//{
	//	BeItem* pkItem = m_apkItem[iIndex];
	//	if (pkItem)
	//	{
	//		if (gMain->IsPveMap())
	//		{
	//			const	ZhanChangItemTable* pkItemRes = pkItem->GetZhanChangResPtr();
	//			if (pkItemRes)
	//			{
	//				for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
	//				{
	//					int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
	//					if (!CanAddItemPassiveSkill(iSkillTypeID))
	//					{
	//						pkItem->AddForbidSkill(iSkillTypeID);

	//						continue;
	//					}
	//					const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
	//					if (pkItemPassiveRes)
	//					{
	//						AddItemPassiveSkill(iSkillTypeID);
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			const	ItemTable* pkItemRes = pkItem->GetResPtr();
	//			if (pkItemRes)
	//			{
	//				for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
	//				{
	//					int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
	//					if (!CanAddItemPassiveSkill(iSkillTypeID))
	//					{
	//						pkItem->AddForbidSkill(iSkillTypeID);

	//						continue;
	//					}
	//					const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
	//					if (pkItemPassiveRes)
	//					{
	//						AddItemPassiveSkill(iSkillTypeID);
	//					}
	//				}
	//			}
	//		}

	//	}
	//}

	//for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	//{
	//	BeItem* pkOrgItem = GetItemByPos(i);
	//	if (pkOrgItem)
	//	{
	//		pkOrgItem->InitItemSkill();
	//	}
	//}
	//UpdateAttribute(true);
}

void TwUnit::SetFace(float fFace, bool bChange/* = true*/)
{
	AdjustRadian(fFace);

	// 	if(fFace != m_pkCurData->fFace && bChange)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_FACE);
	}
	m_pkCurData->fFace = fFace;
}

void TwUnit::SetPreTurnFace(float fFace)
{
	AdjustRadian(fFace);
	m_pkCurData->fFace = fFace;
}

void TwUnit::SetCurAttackCD(int iCurAttackCD)
{
	if (iCurAttackCD != m_pkCurData->iAttackCD)
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_ATTACKCD);
	}
	m_pkCurData->iAttackCD = iCurAttackCD;
}
