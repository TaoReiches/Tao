/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskAction.h"
#include "TW_ShareUnitData.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include <TW_MapDefine.h>
#include "TW_Map.h"
#include "TW_Item.h"
#include "TW_MapItem.h"
#include "TW_MapItemMgr.h"
#include "TW_Effect.h"
#include "TW_EffectMgr.h"
#include "TW_IRandom.h"
#include "TW_TriggerEvent.h"
#include "Skill_table.hpp"
#include "TW_Skill.h"

#define SKILL_BUFFER (400.0f)

inline float f_line_y(float x, float x1, float y1, float x2, float y2)
{
	return ((y1 - y2) * x + (x1 * y2 - x2 * y1)) / (x1 - x2);
}

inline float f_line_x(float y, float x1, float y1, float x2, float y2)
{
	return ((x1 - x2) * y + (x2 * y1 - x1 * y2)) / (y1 - y2);
}



BeTaskActionWalk::BeTaskActionWalk()
{
	m_eType = STT_ACTION_WALK;
	m_bBlocked = false;
	m_iWalkTime = 0;
}

BeTaskActionWalk::~BeTaskActionWalk()
{
	m_eType = STT_ACTION_WALK;
	m_bBlocked = false;
	m_iWalkTime = 0;
	if (pkAttachUnit)
	{
		gUnit.SetActionState(0);
	}
}

void BeTaskActionWalk::SetTargetPos(const TePos2& kTargetPos, bool bTurn)
{
	float fUnitX = gUnit.GetPosX();
	float fUnitY = gUnit.GetPosY();
	float fTarFace = atan2f(kTargetPos.fY - fUnitY, kTargetPos.fX - fUnitX);
	gUnit.SetTarFace(fTarFace);

	m_kTargetPos = kTargetPos;
	m_kSrcPos = TePos2(fUnitX, fUnitY);
	m_iWalkTime = 0;

	gUnit.SetTarPosX(m_kTargetPos.fX);
	gUnit.SetTarPosY(m_kTargetPos.fY);
}

bool	BeTaskActionWalk::IsBlocked(void)
{
	return	m_bBlocked;
}

BeExeResult BeTaskActionWalk::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	m_bBlocked = false;
	if (!gUnit.UnitCanMove(true))
	{
		if (gUnit.GetActionState() == 1)
		{
			gUnit.SetActionState(0);
		}
	}
	else
	{
		if (m_bStopAction)
		{
			m_bStopAction = false;
			gUnit.SetActionState(1);
		}
	}

	if (!gUnit.IsActionNow(BUA_WALK))
	{
		m_iWalkTime = 0;
		gUnit.SetUnitAction(BUA_WALK, BAN_walk);
	}

	float fUnitX = gUnit.GetPosX();
	float fUnitY = gUnit.GetPosY();

	gUnit.IncActionCurTime(iDeltaTime);

	float fMoveSpeed = gUnit.GetMoveSpeed();
	if (fMoveSpeed <= 0)
	{
		return BER_EXE_END;
	}

	float fCanMoveDistance = fMoveSpeed * iDeltaTime / 1000.0f;
	float fMaxMoveDistance = 64.0f;

	float fWalkX = 0.0f;
	float fWalkY = 0.0f;

	int		iObs = TGF_FIXED_OTS;
	if (gUnit.GetClass() == UNIT_CLASSTYPE_SOLIDER)
	{
		iObs = TGF_FIXED_OTS | TGF_SOLIDER;
	}
	else
	{
		iObs = TGF_FIXED_OTS | TGF_UNIT;
	}

	int bRet = gMap.GetFirstCanStay(&gUnit, m_kTargetPos.fX, m_kTargetPos.fY, fWalkX, fWalkY, fMaxMoveDistance, iObs);

	float fNeedMoveDistance = GetDistance(fUnitX, fUnitY, fWalkX, fWalkY);

	if (fNeedMoveDistance > fCanMoveDistance)
	{
		if (fNeedMoveDistance > 0.0f)
		{
			fWalkX = (float)(fUnitX + fCanMoveDistance * (fWalkX - fUnitX) / fNeedMoveDistance);
			fWalkY = (float)(fUnitY + fCanMoveDistance * (fWalkY - fUnitY) / fNeedMoveDistance);
		}
		else
		{
			fWalkX = (float)(fUnitX + fCanMoveDistance * (fWalkX - fUnitX) / 1.0f);
			fWalkY = (float)(fUnitY + fCanMoveDistance * (fWalkY - fUnitY) / 1.0f);
		}

		if (fabs(fWalkX - fUnitX) > fabs(fWalkY - fUnitY))
		{
			float fY = f_line_y(fWalkX, m_kSrcPos.fX, m_kSrcPos.fY, m_kTargetPos.fX, m_kTargetPos.fY);
			fWalkY = fY;
		}
		else
		{
			float fX = f_line_x(fWalkY, m_kSrcPos.fX, m_kSrcPos.fY, m_kTargetPos.fX, m_kTargetPos.fY);
			fWalkX = fX;
		}

		float fCanMoveX = fUnitX;
		float fCanMoveY = fUnitY;

		int bRet1 = gMap.GetFirstCanStay(&gUnit, fWalkX, fWalkY, fCanMoveX, fCanMoveY, fMaxMoveDistance, iObs);
		if (fWalkX != fCanMoveX || fWalkY != fCanMoveY)
		{
			m_bBlocked = true;
		}
	}
	else
	{
		if (fWalkX != m_kTargetPos.fX || fWalkY != m_kTargetPos.fY)
		{
			m_bBlocked = true;
		}
	}

	if (m_bBlocked)
	{
		bool bFound = false;
		float afOffset[][2] =
		{
			{-1.0f, 0.0f},
			{1.0f, 0.0f},
			{0.0f, 1.0f},
			{0.0f, -1.0f},
			{-1.0f, 1.0f},
			{1.0f, -1.0f},
			{1.0f, 1.0f},
			{-1.0f, -1.0f},
		};
		float fOX = fWalkX;
		float fOY = fWalkY;

		float fCanMoveX = fUnitX;
		float fCanMoveY = fUnitY;

		float fDist2 = GetDistance2(fUnitX, fUnitY, m_kTargetPos.fX, m_kTargetPos.fY);
		for (int iRange = 0; iRange < fCanMoveDistance; ++iRange)
		{
			for (int i = 0; i < sizeof(afOffset) / sizeof(afOffset[0]); ++i)
			{
				float fX = fOX + afOffset[i][0] * (iRange + 1);
				float fY = fOY + afOffset[i][1] * (iRange + 1);
				int		iCheckRet = gMap.GetFirstCanStay(fX, fY, gUnit.GetOBSize(), m_kTargetPos.fX, m_kTargetPos.fY, fCanMoveX, fCanMoveY, fMaxMoveDistance, iObs);
				if (iCheckRet == 3 || iCheckRet == 5)
				{
					float fD2 = GetDistance2(fX, fY, m_kTargetPos.fX, m_kTargetPos.fY);
					if (fD2 < fDist2)
					{
						fDist2 = fD2;
						fWalkX = fX;
						fWalkY = fY;
						bFound = true;
					}
				}
			}

			if (bFound)
			{
				m_bBlocked = false;
				break;
			}
		}
	}

	if (m_bBlocked)
	{
		gUnit.SetActionState(0);
		m_iWalkTime = 0;
		return BER_EXE_END;
	}

	int iActionTime = iDeltaTime;
	if (m_kTargetPos.fX != fWalkX || m_kTargetPos.fY != fWalkY)
	{
		iDeltaTime = 0;
	}
	else
	{
		if (fMoveSpeed > 0)
		{
			iActionTime = (int)(fNeedMoveDistance * 1000.0f / fMoveSpeed);
		}

		iDeltaTime -= iActionTime;
	}

	m_iWalkTime += iActionTime;
	if (m_iWalkTime > 200)
	{
		gUnit.SetFlag(BUF_MOVING);
	}

	gUnit.SetActionState(1);

	gUnit.SetPosition(fWalkX, fWalkY, 0.0f, true);

	if (iDeltaTime > 0)
	{
		m_iWalkTime = 0;
		m_bBlocked = false;
		return BER_EXE_END;
	}

	return BER_TIME_OUT;
}
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

BeTaskActionSpell::BeTaskActionSpell()
{
	m_eType = STT_ACTION_SPELL;

	m_iSkillTypeID = 0;
	m_iSkillLevel = 1;
	m_bExpendMP = true;
	m_iTargetID = 0;
	m_iTargetType = 0;


	m_ePhase = BSP_INIT;
	m_iActionTime = 0;
	m_iPrepareTime = 0;
	m_iCastTime = 0;
	m_iEffectTime = 0;
	m_iShakesTime = 0;
	m_iItemID = 0;
	m_iItemOwnerPlayer = -1;

	m_iManaSpend = 0;

	m_iOtherID = 0;
	m_iItemTypeID = 0;

	m_pkSkillRes = NULL;
	m_iPersitTime = 0;
	m_iPersitDeltaTime = 100;
	m_iUsePlayer = -1;

	m_bSkipEffectTime = false;
}

BeTaskActionSpell::~BeTaskActionSpell()
{
	if (((m_ePhase == BSP_EFFECT) || (m_ePhase == BSP_FINISH) || (m_ePhase == BSP_END)))
	{
		TePtParam kParam;
		kParam.SetParam(BTP_pkTrgUnit, &gUnit);
		kParam.SetParam(BTP_iSkillTypeID, m_iSkillTypeID);
		kParam.SetParam(BTP_iSkillLevel, m_iSkillLevel);
		kParam.SetParam(BTP_iSpellTargetID, m_iTargetID);
		kParam.SetParam(BTP_fSpellTargetPosX, m_kTargetPos.fX);
		kParam.SetParam(BTP_fSpellTargetPosY, m_kTargetPos.fY);
		kParam.SetParam(BTP_iTargetType, m_iTargetType);

		gTrgMgr.FireTrigger(BTE_SPELL_CMDEND, kParam);

		if (m_ePhase == BSP_EFFECT)
		{
			gTrgMgr.FireTrigger(BTE_SPELL_EFFECTEND, kParam);
		}

		if ((m_pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_SENDBAR) != 0)
		{
			BeShareUIWindowData	kWindowData;
			kWindowData.eWindowType = BWT_SPELL_EFFECT;
			kWindowData.iSkillID = m_iSkillTypeID;
			kWindowData.iRemoveTime = m_iEffectTime;
			kWindowData.iUnitID = gUnit.GetID();
			kWindowData.bRemove = true;
			//gMain.AddWindowData(kWindowData);
		}
	}
	else if (m_ePhase == BSP_CAST)
	{
		TePtParam kParam;
		kParam.SetParam(BTP_pkTrgUnit, &gUnit);
		kParam.SetParam(BTP_iSkillTypeID, m_iSkillTypeID);
		kParam.SetParam(BTP_iSkillLevel, m_iSkillLevel);
		kParam.SetParam(BTP_iSpellTargetID, m_iTargetID);
		kParam.SetParam(BTP_fSpellTargetPosX, m_kTargetPos.fX);
		kParam.SetParam(BTP_fSpellTargetPosY, m_kTargetPos.fY);
		kParam.SetParam(BTP_iTargetType, m_iTargetType);

		gTrgMgr.FireTrigger(BTE_SPELL_CASTEND, kParam);

		BeShareUIWindowData	kWindowData;
		kWindowData.eWindowType = BWT_SPELL_CAST;
		kWindowData.iSkillID = m_iSkillTypeID;
		kWindowData.iRemoveTime = 0;
		kWindowData.iUnitID = gUnit.GetID();
		kWindowData.bRemove = true;
		//gMain.AddWindowData(kWindowData);
	}

	gUnit.ClrFlag(BUF_ISPERSISTSKILL);
}

bool BeTaskActionSpell::SpellTargetID(int iSkillTypeID, int iSkillLevel, bool bExpendMP, int iTargetID, const TePos2& kPos, int iItemID, int iUsePlayer, int iTargetType)
{
	m_iSkillTypeID = iSkillTypeID;
	m_iSkillLevel = iSkillLevel;
	m_bExpendMP = bExpendMP;
	m_iTargetID = iTargetID;
	m_iTargetType = iTargetType;
	m_iUsePlayer = iUsePlayer;
	m_kTargetPos = kPos;

	if (iItemID != 0)
	{
		m_iItemID = iItemID;
		BeItem* pkItem = gUnit.GetItemByID(iItemID);
		if (pkItem)
		{
			if (!pkItem->GetValid())
			{
				return false;
			}
			m_iItemOwnerPlayer = pkItem->GetOwnerPlay();
			m_iItemTypeID = pkItem->GetTypeID();
		}
	}

	m_pkSkillRes = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);///gMain.GetResSkill(iSkillTypeID,(BeUnit*)pkAttachUnit);

	gUnit.SetTarFace(gUnit.GetFace());
	if (iTargetID && iTargetID != gUnit.GetID())
	{
		if (!m_pkSkillRes)
		{
			return false;
		}
		if (m_iTargetType == BCTT_UNIT)
		{
			BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
			if (pkTarget)
			{
				if (m_iTargetID != gUnit.GetID() && gUnit.GetClass() != UNIT_CLASSTYPE_BUILDING)
				{
					float fTarFace = atan2f(pkTarget->GetPosY() - gUnit.GetPosY(), pkTarget->GetPosX() - gUnit.GetPosX());
					gUnit.SetTarFace(fTarFace);
				}
			}
		}
	}

	return true;
}

bool BeTaskActionSpell::SpellTargetItem(int iSkillTypeID, int iSkillLevel, bool bExpendMP, const TePos2& kPos, int iItemID, int iOtherID, int iUsePlayer)
{
	gUnit.SetAttackingUnitID(0);
	m_iSkillTypeID = iSkillTypeID;
	m_iSkillLevel = iSkillLevel;
	m_bExpendMP = bExpendMP;
	m_kTargetPos = kPos;
	m_iTargetID = 0;
	m_iUsePlayer = iUsePlayer;
	m_iOtherID = iOtherID;

	if (iItemID != 0)
	{
		m_iItemID = iItemID;

		BeItem* pkItem = gUnit.GetItemByID(iItemID);
		if (pkItem)
		{
			if (!pkItem->GetValid())
			{
				return false;
			}
			m_iItemOwnerPlayer = pkItem->GetOwnerPlay();
			m_iItemTypeID = pkItem->GetTypeID();
		}
	}

	m_pkSkillRes = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);

	gUnit.SetTarFace(gUnit.GetFace());

	if (iItemID)
	{
		BeMapItem* pkMapItem = gMapItemMgr.GetMapItemByID(iOtherID);
		if (pkMapItem && gUnit.GetClass() != UNIT_CLASSTYPE_BUILDING)
		{
			float fTarFace = atan2f(pkMapItem->GetPosY() - gUnit.GetPosY(), pkMapItem->GetPosX() - gUnit.GetPosX());
			gUnit.SetTarFace(fTarFace);
		}
	}

	return true;
}

bool BeTaskActionSpell::SpellTargetPos(int iSkillTypeID, int iSkillLevel, bool bExpendMP, const TePos2& kPos, const TePos2& kDirPos, int iItemID, int iUsePlayer)
{
	gUnit.SetAttackingUnitID(0);
	m_iSkillTypeID = iSkillTypeID;
	m_iSkillLevel = iSkillLevel;
	m_bExpendMP = bExpendMP;
	m_kTargetPos = kPos;
	m_iUsePlayer = iUsePlayer;
	m_iTargetID = 0;
	m_kDirectPos = kDirPos;
	m_iItemID = iItemID;

	m_pkSkillRes = SkillTableMgr::Get()->GetSkillTable(m_iSkillTypeID);

	if ((m_pkSkillRes->uiOperateType == SKILL_OPERATETYPE_LIJISHIFA))
	{
		m_kTargetPos.fX = 0.0f;
		m_kTargetPos.fY = 0.0f;
	}

	if (!(m_pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_NOJULI)) {

		if (m_pkSkillRes->uiOperateType == SKILL_OPERATETYPE_QUYU || m_pkSkillRes->uiOperateType == SKILL_OPERATETYPE_XIANXING)
		{
			SeCalSkillLvlData	rkData;
			if (gUnit.GetSkillLvlData(rkData, iSkillTypeID))
			{
				float	fDis = rkData.GetSpellDistance();
				if (GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), m_kTargetPos.fX, m_kTargetPos.fY) > fDis * fDis)
				{
					float fTarFace = atan2f(kPos.fY - gUnit.GetPosY(), kPos.fX - gUnit.GetPosX());
					m_kTargetPos.fX = gUnit.GetPosX() + fDis * cosf(fTarFace);
					m_kTargetPos.fY = gUnit.GetPosY() + fDis * sinf(fTarFace);
				}
			}
		}
	}

	if (!m_pkSkillRes)
	{
		gUnit.SetTarFace(gUnit.GetFace());
	}
	else if (gUnit.GetClass() != UNIT_CLASSTYPE_BUILDING && m_kTargetPos.fX != 0.0f && m_kTargetPos.fY != 0.0f)
	{
		float fTarFace = atan2f(kPos.fY - gUnit.GetPosY(), kPos.fX - gUnit.GetPosX());
		gUnit.SetTarFace(fTarFace);
	}

	return true;
}
bool BeTaskActionSpell::CanSkip(void) const
{
	return false;
}
BeSpellPhase BeTaskActionSpell::GetPhase(void)
{
	return m_ePhase;
}

void BeTaskActionSpell::OnSpellPrepare(void)
{
	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(BTP_iSkillLevel, m_iSkillLevel);
	kParam.SetParam(BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(BTP_fSpellDirectPosX, m_kDirectPos.fX);
	kParam.SetParam(BTP_fSpellDirectPosY, m_kDirectPos.fY);
	kParam.SetParam(BTP_iTargetType, m_iTargetType);

	gTrgMgr.FireTrigger(BTE_SPELL_PREPARE, kParam);
}

void BeTaskActionSpell::OnSpellCast(void)
{
	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(BTP_iSkillLevel, m_iSkillLevel);
	kParam.SetParam(BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(BTP_iTargetType, m_iTargetType);

	if (m_iItemID == 0)
	{
		gTrgMgr.FireTrigger(BTE_SPELL_CAST, kParam);
	}
	else
	{
		kParam.SetParam(BTP_iSkillLevel, 1);
		kParam.SetParam(BTP_iItemID, m_iItemID);
		kParam.SetParam(BTP_iOtherID, m_iOtherID);

		gTrgMgr.FireTrigger(BTE_UNIT_USE_ITEMCAST, kParam);
	}
}

void BeTaskActionSpell::OnSpellEffect(void)
{
	if (m_iSkillTypeID != 'SA02' && m_iSkillTypeID != 'SA06')
	{
		gUnit.TrgOnSpell(m_iSkillTypeID, m_iSkillLevel, m_iItemID, m_iTargetID, m_kTargetPos.fX, m_kTargetPos.fY);
	}

	m_iPersitTime = 100;
	m_iPersitDeltaTime = 100;

	if (m_iTargetType == BCTT_UNIT && m_iTargetID)
	{
		BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
		if (pkTarget)
		{
			m_kTargetPos.fX = pkTarget->GetPosX();
			m_kTargetPos.fY = pkTarget->GetPosY();
		}
	}

	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(BTP_piSpellTargetID, &m_iTargetID);
	kParam.SetParam(BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(BTP_fSpellDirectPosX, m_kDirectPos.fX);
	kParam.SetParam(BTP_fSpellDirectPosY, m_kDirectPos.fY);
	kParam.SetParam(BTP_piPersistTime, &m_iPersitTime);
	kParam.SetParam(BTP_piPersistDeltaTime, &m_iPersitDeltaTime);
	kParam.SetParam(BTP_iTargetType, m_iTargetType);

	if (m_iItemID == 0)
	{
		kParam.SetParam(BTP_iSkillLevel, m_iSkillLevel);

		gTrgMgr.FireTrigger(BTE_SPELL_EFFECT, kParam);
	}
	else
	{
		int iItemTypeID = m_iItemTypeID;
		BeItem* pkItem = gUnit.GetItemByID(m_iItemID);
		if (pkItem)
		{
			iItemTypeID = pkItem->GetTypeID();
			pkItem->SetBuyTime(0);
			kParam.SetParam(BTP_iItemTypeID, iItemTypeID);
		}
		else
		{
			kParam.SetParam(BTP_iItemTypeID, iItemTypeID);
		}
		kParam.SetParam(BTP_iSkillLevel, 1);
		kParam.SetParam(BTP_iItemID, m_iItemID);
		kParam.SetParam(BTP_iItemOwnerPlayer, m_iItemOwnerPlayer);
		kParam.SetParam(BTP_iOtherID, m_iOtherID);

		gTrgMgr.FireTrigger(BTE_UNIT_USE_ITEM, kParam);
	}
}

void BeTaskActionSpell::OnSpellFinish(void)
{
	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, &gUnit);
	kParam.SetParam(BTP_iSkillTypeID, m_iSkillTypeID);
	kParam.SetParam(BTP_iSpellTargetID, m_iTargetID);
	kParam.SetParam(BTP_iSkillUsePlayer, m_iUsePlayer);
	kParam.SetParam(BTP_fSpellTargetPosX, m_kTargetPos.fX);
	kParam.SetParam(BTP_fSpellTargetPosY, m_kTargetPos.fY);
	kParam.SetParam(BTP_iTargetType, m_iTargetType);

	if (m_iItemID == 0)
	{
		kParam.SetParam(BTP_iSkillLevel, m_iSkillLevel);

		gTrgMgr.FireTrigger(BTE_SPELL_FINISH, kParam);
	}
	else
	{
		BeItem* pkItem = gUnit.GetItemByID(m_iItemID);
		if (pkItem)
		{
			if (pkItem->IsCanSplice())
			{
				if (pkItem->GetPileCount() > 1)
				{
					pkItem->SetPileCount(pkItem->GetPileCount() - 1);
					pkItem->SetUseCount(pkItem->GetOrgUseCount());
				}
				else
				{
					gUnit.DelItemByPos(pkItem->GetPackagePos());
				}
			}
		}

		if (pkItem)
		{
			if (pkItem->IsCanSplice())
			{
				if (pkItem->GetPileCount() > 1)
				{
					pkItem->SetPileCount(pkItem->GetPileCount() - 1);
					pkItem->SetUseCount(pkItem->GetOrgUseCount());
				}
				else
				{
					gUnit.DelItemByPos(pkItem->GetPackagePos());
				}
			}
		}

		kParam.SetParam(BTP_iSkillLevel, 1);
		kParam.SetParam(BTP_iItemID, m_iItemID);
		kParam.SetParam(BTP_iOtherID, m_iOtherID);

		gTrgMgr.FireTrigger(BTE_UNIT_USEITEM_FINISH, kParam);
	}
}

bool BeTaskActionSpell::CanUseSpell(void)
{
	SeCalSkillLvlData	rkData;
	if (!gUnit.GetSkillLvlData(rkData, m_iSkillTypeID, m_iSkillLevel))
	{
		return false;
	}
	int iManaSpend = rkData.GetSkillManaSpend();
	if (m_bExpendMP && gUnit.GetMP() < iManaSpend)
	{
		return false;
	}

	if (m_iItemID)
	{
		if (gUnit.HasUnitCarryFlag(BUCF_ISFORBIDITEM))
		{
			return false;
		}

		BeItem* pkItem = gUnit.GetItemByID(m_iItemID);
		if (pkItem)
		{
			BeSkill* pkSkill = pkItem->GetSkillByTypeID(m_iSkillTypeID);
			if (pkSkill)
			{
				int iSkillCD = rkData.GetSkillCoolDown();

				if (!gUnit.CommonCDComplete(pkItem->GetCDSkillID(), iSkillCD))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		if (m_iTargetID)
		{
			BeUnit* pkTarget = gUnitMgr.GetUnitByID(m_iTargetID);
			if (!pkTarget)
			{
				return false;
			}
			if (!gUnit.IsSkillTargetType(m_pkSkillRes, pkTarget))
			{
				return false;
			}
		}
	}
	else
	{
		BeSkill* pkSkill = gUnit.GetSkill(m_iSkillTypeID);
		if (!pkSkill)
		{
			return false;
		}
		if (pkSkill && pkSkill->IsSwitch() && pkSkill->IsActive())
		{
			return true;
		}

		int iSkillCD = rkData.GetSkillCoolDown();
		int iSkillCount = pkSkill->GetUICounter();

		if (pkSkill->GetCurPileNums() > 0 || (iSkillCount > 0 && iSkillCount != 234))
		{
		}
		else if (!gUnit.CommonCDComplete(m_iSkillTypeID, iSkillCD))
		{
			return false;
		}


		UnitUseSkillResultType eResult = gUnit.UnitCanUseSkill(m_iSkillTypeID, NULL, true, true, m_kTargetPos.fX, m_kTargetPos.fY);
		if (eResult != UUSRT_OK)
		{
			return false;
		}

		if (gUnit.IsForbidSkill(pkSkill))
		{
			if (m_ePhase < BSP_CAST)
			{
				return false;
			}
		}
	}

	return true;
}

bool BeTaskActionSpell::WillEffectBreak(void)
{
	if (gUnit.HasUnitCarryFlag(BUCF_DIZZY))
	{

		return true;

	}

	BeSkill* pkSkill = gUnit.GetSkill(m_iSkillTypeID);
	if (pkSkill)
	{
		if (gUnit.IsForbidSkill(pkSkill))
		{
			return true;
		}
	}

	return false;
}

bool BeTaskActionSpell::UseUpItem()
{
	if (m_bExpendMP)
	{
		m_iManaSpend = m_pkSkillRes->fManaSpend[m_iSkillLevel];
		gUnit.SetMP(gUnit.GetMP() - m_iManaSpend);
	}

	BeItem* pkItem = gUnit.GetItemByID(m_iItemID);
	if (pkItem)
	{
		OperateItem(pkItem);
		return true;
	}
	return false;
}

void BeTaskActionSpell::OperateItem(BeItem* pkItem)
{
	if (!pkItem)
	{
		return;
	}

	if (pkItem->GetOrgUseCount() != -1)
	{
		pkItem->SetUseCount(pkItem->GetUseCount() - 1);
	}

	gUnit.SetCommonCDLastUseTime(pkItem->GetCDSkillID(), (int)gTime);
	for (int i = 0; i < 6; i++)
	{
		BeItem* pkOldItem = gUnit.GetItemByPos(i);
		if (pkOldItem && pkOldItem->GetCDSkillID() == pkItem->GetCDSkillID())
		{
			{
				pkOldItem->SetLastUseTime((int)gTime);
			}
		}
	}

	if (pkItem->GetUseCount() == 0 && pkItem->IsUsable())
	{

		if (pkItem->IsCanSplice() && pkItem->GetPileCount() > 1)
		{
			pkItem->SetPileCount(pkItem->GetPileCount() - 1);

			pkItem->SetUseCount(pkItem->GetOrgUseCount());
		}
		else
		{
			int iItemID = pkItem->GetID();
			gUnit.DelItemByPos(pkItem->GetPackagePos());
			pkItem = gUnit.GetItemByID(iItemID);
		}
	}

	if (pkItem && pkItem->GetUseCount() == 0 && pkItem->IsUsable())
	{
		pkItem->SetUseCount(pkItem->GetOrgUseCount());
	}

}

bool BeTaskActionSpell::UseUpSkill()
{
	BeSkill* pkSkill = gUnit.GetSkill(m_iSkillTypeID);
	if (!pkSkill)
	{
		return false;
	}
	SeCalSkillLvlData	rkData;
	if (!gUnit.GetSkillLvlData(rkData, m_iSkillTypeID, m_iSkillLevel))
	{
		return false;
	}
	if (pkSkill->IsSwitch() && pkSkill->IsActive())
	{
		return true;
	}
	if (pkSkill->GetTypeID() != 'S209')
	{
		pkSkill->SetLastUseTime(&gUnit, (int)gTime);
	}

	if (m_bExpendMP)
	{
		int iManaSpend = rkData.GetSkillManaSpend();
		m_iManaSpend = iManaSpend;
		gUnit.SetMP(gUnit.GetMP() - iManaSpend);
	}

	return true;
}

BeExeResult BeTaskActionSpell::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);
	{
		gUnit.ClrFlag(BUF_MOVING);
	}
	if (!m_pkSkillRes)
	{
		return BER_EXE_END;
	}

	int iLoopCount = 10;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		switch (m_ePhase)
		{
		case BSP_INIT:
		{
			if (!CanUseSpell())
			{
				return BER_EXE_END;
			}

			m_iPrepareTime = 0;

			m_iActionTime = 0;
			m_ePhase = BSP_PREPARE;

			OnSpellPrepare();
			break;
		}
		case BSP_PREPARE:
		{
			if (!CanUseSpell())
			{
				return BER_EXE_END;
			}

			SeCalSkillLvlData	rkData;
			if (!gUnit.GetSkillLvlData(rkData, m_iSkillTypeID, m_iSkillLevel))
			{
				return BER_EXE_END;
			}
			m_iCastTime = rkData.GetCastTime();
			m_iEffectTime = rkData.GetEffectTime();
			m_iShakesTime = rkData.GetShakesTime();

			if (m_iCastTime < 0)
			{
				if (m_iItemID)
				{
					m_iCastTime = 0;
				}
				else
				{
					m_iCastTime = 100;
				}
			}

			if (m_iEffectTime < 0)
			{
				if (m_iItemID)
				{
					m_iEffectTime = 0;
					m_bSkipEffectTime = true;

				}
				else
				{
					m_iEffectTime = 100;
					m_bSkipEffectTime = true;
				}
			}

			m_iActionTime = 0;
			m_ePhase = BSP_CAST;

			gUnit.SetUnitAction(BUA_SPELL, m_pkSkillRes->uiSpellAction);

			OnSpellCast();

			if (m_iCastTime > 500)
			{
				BeShareUIWindowData	kWindowData;
				kWindowData.eWindowType = BWT_SPELL_CAST;
				kWindowData.iSkillID = m_iSkillTypeID;
				kWindowData.iRemoveTime = m_iCastTime;
				kWindowData.iUnitID = gUnit.GetID();
				//gMain.AddWindowData(kWindowData);
			}

			break;
		}
		case BSP_CAST:
		{
			if (!CanUseSpell())
			{
				return BER_EXE_END;
			}

			if ((m_iActionTime + iDeltaTime) >= m_iCastTime)
			{
				int iTime = m_iCastTime - m_iActionTime;
				iDeltaTime -= iTime;

				gUnit.IncActionCurTime(iTime);

				m_iActionTime = 0;
				m_ePhase = BSP_EFFECT;

				bool bEnterCD = false;
				if (m_iItemID)
				{
					bEnterCD = UseUpItem();
				}
				else
				{
					bEnterCD = UseUpSkill();
				}

				if (!bEnterCD)
				{
					return BER_EXE_END;
				}

				OnSpellEffect();

				do
				{
					if ((m_pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_SENDBAR) != 0)
					{
						BeShareUIWindowData	kWindowData;
						kWindowData.eWindowType = BWT_SPELL_EFFECT;
						kWindowData.iSkillID = m_iSkillTypeID;
						kWindowData.iRemoveTime = m_iEffectTime;
						kWindowData.iUnitID = gUnit.GetID();
						//gMain.AddWindowData(kWindowData);
					}
				} while (0);

				break;
			}

			m_iActionTime += iDeltaTime;
			gUnit.IncActionCurTime(iDeltaTime);
			iDeltaTime = 0;
			break;
		}
		case BSP_EFFECT:
		{
			if (WillEffectBreak() || (gUnit.GetSkill(m_iSkillTypeID) == NULL && m_iItemID == 0))
			{
				m_iActionTime = 0;
				return BER_EXE_END;
			}

			if ((m_iActionTime + iDeltaTime) >= m_iEffectTime)
			{
				int iTime = m_iEffectTime - m_iActionTime;
				iDeltaTime -= iTime;

				gUnit.IncActionCurTime(iTime);

				m_iActionTime = 0;
				m_ePhase = BSP_SHAKES;
				break;
			}
			else
			{
				gUnit.SetFlag(BUF_ISPERSISTSKILL);
			}

			m_iActionTime += iDeltaTime;
			gUnit.IncActionCurTime(iDeltaTime);
			iDeltaTime = 0;
			break;
		}

		case BSP_SHAKES:
		{
			if (gUnit.GetSkill(m_iSkillTypeID) == NULL && m_iItemID == 0)
			{
				m_iActionTime = 0;
				return BER_EXE_END;
			}

			if ((m_iActionTime + iDeltaTime) >= m_iShakesTime)
			{
				int iTime = m_iShakesTime - m_iActionTime;
				iDeltaTime -= iTime;

				gUnit.IncActionCurTime(iTime);

				m_iActionTime = 0;
				m_ePhase = BSP_FINISH;
				break;
			}

			gUnit.SetFlag(BUF_SPELL_SHARK);
			m_iActionTime += iDeltaTime;
			gUnit.IncActionCurTime(iDeltaTime);
			iDeltaTime = 0;
			break;
		}
		case BSP_FINISH:
		{
			gUnit.ClrFlag(BUF_ISPERSISTSKILL);
			gUnit.ClrFlag(BUF_SPELL_SHARK);
			OnSpellFinish();
			m_ePhase = BSP_END;
			return BER_EXE_END;
		}
		case BSP_END:
		{
			return BER_EXE_END;
		}
		default:break;
		}
	}
	return BER_TIME_OUT;
}

BeTaskActionDeath::BeTaskActionDeath()
{
	m_eType = STT_ACTION_DEATH;

	m_bInit = false;
}

BeExeResult BeTaskActionDeath::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);
	gUnit.ClrFlag(BUF_MOVING);
	if (!m_bInit)
	{
		gUnit.SetUnitAction(BUA_DEATH, BAN_death);
		m_bInit = true;
	}

	int iLoopCount = 10;
	while (iDeltaTime > 0)
	{
		--iLoopCount;
		if (iLoopCount <= 0)
		{
			iDeltaTime = 0;
			break;
		}
		if (gUnit.IsActionNow(BUA_DEATH))
		{
			if (gUnit.IsActionCurTimeOut(iDeltaTime))
			{
				iDeltaTime -= gUnit.GetActionCurTimeNeed();

				return BER_ALL_OVER;
			}
			else
			{
				gUnit.IncActionCurTime(iDeltaTime);
				iDeltaTime = 0;

				return BER_TIME_OUT;
			}
		}
		else
		{
			if (gUnit.IsActionCurTimeOut(iDeltaTime))
			{
				return BER_ALL_OVER;
			}
			else
			{
				gUnit.IncActionCurTime(iDeltaTime);
				iDeltaTime = 0;

				return BER_TIME_OUT;
			}
		}
	}

	return BER_TIME_OUT;
}
