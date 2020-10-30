/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Effect.h"
#include "TW_Main.h"
#include "TW_TriggerEvent.h"
#include "TW_Functions.h"
#include "TW_UnitMgr.h"
#include "TW_Unit.h"
#include "TW_IRandom.h"
#include "TW_MapItemMgr.h"
#include "TW_MapItem.h"
#include "TW_EffectMgr.h"
#include "TW_MemoryObject.h"

TePos2  GetPointOnBezier(TePos2* pkPos, float fValueT)
{
	float   fAx, fBx, fCx;
	float   fAy, fBy, fCy;
	float   fTSquared, fTCubed;
	TePos2	kResult;

	fCx = 3.0f * (pkPos[1].fX - pkPos[0].fX);
	fBx = 3.0f * (pkPos[2].fX - pkPos[1].fX) - fCx;
	fAx = pkPos[3].fX - pkPos[0].fX - fCx - fBx;

	fCy = 3.0f * (pkPos[1].fY - pkPos[0].fY);
	fBy = 3.0f * (pkPos[2].fY - pkPos[1].fY) - fCy;
	fAy = pkPos[3].fY - pkPos[0].fY - fCy - fBy;

	fTSquared = fValueT * fValueT;
	fTCubed = fTSquared * fValueT;

	kResult.fX = (fAx * fTCubed) + (fBx * fTSquared) + (fCx * fValueT) + pkPos[0].fX;
	kResult.fY = (fAy * fTCubed) + (fBy * fTSquared) + (fCy * fValueT) + pkPos[0].fY;

	return kResult;
}

BeEffect::BeEffect(int iID)
{
	m_pkAttr.reset(mpAttackingAttr.alloc());
	m_iRace = 0;
	m_kEffectData.uiRemoveTime = -1;
	m_kEffectData.fMoveSpeed = 0.0f;
	m_kEffectData.eTargetType = BET_POSITION;
	m_eCurveStyle = BECS_LEFTBIG;
	m_fNeedMoveDistance = 0.0f;
	m_fCanMoveDistance = 0.0f;
	m_bPureShow = false;
	m_fOrgScale = 1.0f;
	m_bChangeScale = false;
	m_eCurveType = BECT_CURVELEFT;
	m_bHasCulCurve = false;
	m_iCurveDividNum = 2;
	m_fPrePosX = 0.0f;
	m_fPrePosY = 0.0f;
	m_fPrePosZ = 0.0f;
	m_fCycleAngle = 0.0f;
	m_bTurning = true;
	m_bRotate = false;
	m_fOwnerFace = 0.0f;
	m_fAllDistance = 0.0f;
	m_bNewEffect = true;
	m_birthTime = 0;
	m_bSearchTargetSelf = false;
	m_iMaxTargetNum = 99;
	m_bNoDelayDelete = false;
	m_eAttachNode = BAP_ChestRef;
	m_iModelID = 0;
}

BeEffect::~BeEffect(void)
{
	mpAttackingAttr.free(m_pkAttr.get());
	m_pkAttr.release();

	gMain.DelEntityPointer(GIT_EFFECT, m_iID);
}

bool BeEffect::Initialize(int iTypeID)
{
	if (iTypeID != 0)
	{
		SetChangeFlag(BECF_RACE);
	}
	m_iRace = iTypeID;

	m_iTypeID = iTypeID;
	m_iFlag = 0;

	if (OnCycleRound())
	{
		SetFlag(BEF_IGNORE_INVINCIBLE | BEF_IGNORE_INVISIBLE | BEF_IGNORE_MAGIC_IMMUNE, false);
	}

	return true;
}

void BeEffect::Update(int iDeltaTime)
{
	if (m_birthTime != 0 && gTime < m_birthTime)
	{
		return;
	}
	if (m_birthTime != 0 && !m_bSearchTargetSelf)
	{
		SetSearchTarget(true);
	}

	if (m_kEffectData.uiRemoveTime <= gTime)
	{
		SetFlag(BEF_REMOVE, false);
		OnRemove();

		return;
	}

	switch (GetRace())
	{
	case BER_ORIGIN:
	{
		break;
	}
	case BER_ATTACH:
	case BER_ATTACH_TRCE:
		//case BET_DIRECT_UNIT:
	{
		UpdateAttach(iDeltaTime);
		break;
	}
	case BER_LINE_TRACE:
	case BER_CURVE_TRACE_MID:
	case BER_CAST_TRACE:
	case BER_LINE_FOLLOWEARTH:
	{
		UpdateLineTrace(iDeltaTime);
		break;
	}
	case BET_CURVE_TRACE_FRANK:
	{
		UpdateCurveTraceFrank(iDeltaTime);
		break;
	}
	case BER_LINE_TRACE_COLLISION:
	case BER_CAST_TRACE_COLLISION:
	{
		UpdateLineTraceCollision(iDeltaTime);
		break;
	}
	default:break;
	}
}

void BeEffect::SetModelFile(int iModelID, BeAttachPos eAttachPos)
{
	m_eAttachNode = eAttachPos;
	m_iModelID = iModelID;

	if (iModelID > 0)
	{
		SetChangeFlag(BECF_MODEL);
	}
	if (eAttachPos != BAP_ChestRef)
	{
		SetChangeFlag(BECF_ATTACHNODE);
	}
}

void BeEffect::OnRemove(void)
{
	if (m_bPureShow)
	{
		return;
	}

	BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
	if (GetDamageScope() > 10.0f)
	{
		if (m_kEffectData.iSkillTypeID)
		{
			gRandNum.RandInt();
			TePtParam kParam;
			kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
			kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
			if (pkAttacker)
			{
				kParam.SetParam(BTP_pkAttacker, pkAttacker);
			}

			kParam.SetParam(BTP_fSpellOrgPosX, m_kEffectData.fOrgPosX);
			kParam.SetParam(BTP_fSpellOrgPosY, m_kEffectData.fOrgPosY);
			kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fTarPosX);
			kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fTarPosY);
			kParam.SetParam(BTP_iIntData, m_kEffectData.iData);
			kParam.SetParam(BTP_fFloatData, m_kEffectData.fData);
			kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);
			kParam.SetParam(BTP_pkEffect, this);

			gTrgMgr.FireTrigger(BTE_EFFECT_AFFECT, kParam);
		}
	}
	else
	{
		if (m_kEffectData.eTargetType == BET_UNIT)
		{
			BeUnit* pkUnit = gUnitMgr.GetUnitByID(GetTargetID());
			if (pkUnit && !pkUnit->IsDead())
			{
				if (m_kEffectData.iSkillTypeID)
				{
					if (-1 != m_kEffectData.iSkillLevel)
					{
						m_akBumpAttackedID.push_back(pkUnit->GetID());

						TePtParam kParam;
						kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
						kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
						kParam.SetParam(BTP_pkTrgUnit, pkUnit);
						if (pkAttacker)
						{
							kParam.SetParam(BTP_pkAttacker, pkAttacker);
						}

						kParam.SetParam(BTP_fSpellOrgPosX, m_kEffectData.fOrgPosX);
						kParam.SetParam(BTP_fSpellOrgPosY, m_kEffectData.fOrgPosY);
						if (GetRace() != BER_LINE_TRACE_COLLISION && GetRace() != BER_CAST_TRACE_COLLISION)
						{
							kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fTarPosX);
							kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fTarPosY);
						}
						else
						{
							kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fPosX);
							kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fPosY);
						}

						kParam.SetParam(BTP_iIntData, m_kEffectData.iData);
						kParam.SetParam(BTP_fFloatData, m_kEffectData.fData);
						kParam.SetParam(BTP_pkEffect, this);
						kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

						gTrgMgr.FireTrigger(BTE_EFFECT_AFFECT, kParam);

						if (m_pkAttr)
						{
							pkUnit->OnBeDamaged(*m_pkAttr);
						}

						gTrgMgr.FireTrigger(BTE_EFFECT_AFFECT_AFTER, kParam);
					}
				}
				else
				{
					if (m_pkAttr)
					{
						pkUnit->OnBeDamaged(*m_pkAttr);
					}
				}
			}
		}
	}
}

void BeEffect::SetOwnerID(int iOwnerID)
{
	SetChangeFlag(BECF_OWNERID);
	m_kEffectData.iOwnerID = iOwnerID;
	if (!m_kEffectData.iSourceID)
	{
		m_kEffectData.iSourceID = iOwnerID;
	}

	BeUnit* pkOwner = gUnitMgr.GetUnitByID(iOwnerID);
	if (pkOwner)
	{
		m_kEffectData.iPlayer = pkOwner->GetPlayer();
	}
	else
	{
		m_kEffectData.iPlayer = -1;
	}
}

void  BeEffect::SetEffectPosition(int iTargetID, float fOrgPosX, float fOrgPosY, float fTarPosX, float fTarPosY, float fMissileArc, float fOrgPosZ, float fTarPosZ, BeEffectCurveType eCurveType)
{
	SetChangeFlag(BECF_POS);
	if (iTargetID != 0)
	{
		SetChangeFlag(BECF_TARUNITID);
	}

	m_kEffectData.eTargetType = BET_UNIT;
	m_kEffectData.fPosX = fOrgPosX;
	m_kEffectData.fPosY = fOrgPosY;
	m_kEffectData.fOrgPosX = fOrgPosX;
	m_kEffectData.fOrgPosY = fOrgPosY;
	m_kEffectData.fTarPosX = fTarPosX;
	m_kEffectData.fTarPosY = fTarPosY;
	m_kEffectData.iTargetID = iTargetID;
	BeUnit* pkTarget = gUnitMgr.GetUnitByID(iTargetID, true);
	if (pkTarget)
	{
		m_kEffectData.fTarPosX = pkTarget->GetPosX();
		m_kEffectData.fTarPosY = pkTarget->GetPosY();
	}

	m_kEffectData.fPosZ = fOrgPosZ;
	m_kEffectData.fOrgPosZ = m_kEffectData.fPosZ;
	m_kEffectData.fTarPosZ = fTarPosZ;

	if (fMissileArc <= 0.0f || fMissileArc >= 1.57f)
	{
		fMissileArc = 0.0f;
	}
	m_kEffectData.fMissileArc = fMissileArc;

	m_eCurveType = eCurveType;

	m_fOwnerFace = atan2f(m_kEffectData.fTarPosY - m_kEffectData.fPosY, m_kEffectData.fTarPosX - m_kEffectData.fPosX);

	m_fPrePosX = m_kEffectData.fPosX;
	m_fPrePosY = m_kEffectData.fPosY;
	m_fPrePosZ = m_kEffectData.fPosZ;
}

void BeEffect::SetTargetID(int iTargetID)
{
	SetChangeFlag(BECF_TARUNITID);
	m_kEffectData.eTargetType = BET_UNIT;
	m_kEffectData.iTargetID = iTargetID;
}

void BeEffect::SetTargetItem(int iItemID)
{
	SetChangeFlag(BECF_TARUNITID);
	m_kEffectData.eTargetType = BET_ITEM;
	m_kEffectData.iTargetID = iItemID;
}

void BeEffect::SetTargetEffect(int iEffectID)
{
	SetChangeFlag(BECF_TARUNITID);
	m_kEffectData.eTargetType = BET_EFFECT;
	m_kEffectData.iTargetID = iEffectID;
}

void* BeEffect::UpdateTargetPos(void)
{
	switch (m_kEffectData.eTargetType)
	{
	case BET_POSITION:break;
	case BET_UNIT:
	{
		if (GetTargetID() == 0)
		{
			return nullptr;
		}

		BeUnit* pkTarget = gUnitMgr.GetUnitByID(GetTargetID());
		if (!pkTarget)
		{
			return nullptr;
		}

		if (HasFlag(BEF_REMOVE_WHENUNITDEAD) && pkTarget->IsDead())
		{
			OnRemove();

			SetFlag(BEF_REMOVE);
			return pkTarget;
		}

		BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);

		if (pkTarget->HasUnitCarryFlag(BUCF_ISMAGICIMMUNITY) && !HasFlag(BEF_IGNORE_MAGIC_IMMUNE))
		{
			if (m_kEffectData.iSkillTypeID)
			{
				m_kEffectData.iSkillLevel = -1;
			}
		}
		else if (pkTarget->HasFlag(BUF_HASINVISIBLE) && !HasFlag(BEF_IGNORE_INVISIBLE))
		{
			BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
			if (pkAttacker)
			{
				if (m_kEffectData.iSkillTypeID)
				{
					m_kEffectData.iSkillLevel = -1;
				}
				else
				{
					if (m_pkAttr)
					{
						m_pkAttr->eAttackType = BAT_NONE;
					}
				}
			}
		}
		else if (pkTarget->HasUnitCarryFlag(BUCF_ISINVINCIBLE) && !HasFlag(BEF_IGNORE_INVINCIBLE))
		{
			m_kEffectData.iSkillTypeID = 0;
			m_kEffectData.iTargetID = 0;
			pkTarget = nullptr;

			OnRemove();
			SetFlag(BEF_REMOVE);
			return pkTarget;
		}
		else if (pkTarget->HasUnitCarryFlag(BUCF_ISPHYSICIMMUNITY) && !HasFlag(BEF_IGNORE_PHYSIC_IMMUNE))
		{
			BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
			if (!pkAttacker || (pkAttacker && !pkAttacker->HasUnitCarryFlag(BUCF_IGNORE_PHYSICIMMUNITY)))
			{
				if (!m_kEffectData.iSkillTypeID)
				{
					if (m_pkAttr)
					{
						m_pkAttr->eAttackType = BAT_NONE;
					}
				}
			}
		}

		if (pkTarget)
		{
			m_kEffectData.fTarPosX = pkTarget->GetPosX();
			m_kEffectData.fTarPosY = pkTarget->GetPosY();
		}

		return pkTarget;
	}
	case BET_ITEM:
	{
		auto pkTarget = gMapItemMgr.GetMapItemByID(GetTargetID());
		if (pkTarget)
		{
			m_kEffectData.fTarPosX = pkTarget->GetPosX();
			m_kEffectData.fTarPosY = pkTarget->GetPosY();
			m_kEffectData.fTarPosZ = 0.0f;
		}
		return pkTarget.get();
		break;
	}
	case BET_EFFECT:
	{
		auto pkEffect = gEffectMgr.GetEffectByID(m_kEffectData.iTargetID);
		if (pkEffect)
		{
			m_kEffectData.fTarPosX = pkEffect->GetPosX();
			m_kEffectData.fTarPosY = pkEffect->GetPosY();
			m_kEffectData.fTarPosZ = 0.0f;
		}
		break;
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}

void BeEffect::UpdateAttach(int iDeltaTime)
{
	UpdateTargetPos();

	if (OnCycleRound())
	{
		int iRadius = 80;

		m_kEffectData.fPosX = m_kEffectData.fTarPosX + iRadius * cosf(m_fCycleAngle);
		m_kEffectData.fPosY = m_kEffectData.fTarPosY + iRadius * sinf(m_fCycleAngle);

		if (m_bTurning)
		{
			const float fAngle = D3DX_2PI / 200;
			m_fCycleAngle += fAngle;
		}
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}

	switch (m_kEffectData.eTargetType)
	{
	case BET_UNIT:
	{
		BeUnit* pkUnit = gMain.m_pkUnitMgr->GetUnitByID(GetTargetID());
		if (pkUnit && !pkUnit->IsDead())
		{
			{
				m_kEffectData.fPosZ = pkUnit->GetPosZ();
			}
		}
		break;
	}
	case BET_EFFECT:
	{
		auto pkEffect = gEffectMgr.GetEffectByID(GetTargetID());
		if (pkEffect)
		{
			m_kEffectData.fPosZ = pkEffect->GetPosZ();
		}
		else
		{
			SetFlag(BEF_REMOVE);
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

void BeEffect::UpdateLineFollowEarth(int iDeltaTime)
{
	void* pkTarget = UpdateTargetPos();

	m_fNeedMoveDistance = GetDistance(m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	m_fCanMoveDistance = GetMoveSpeed() * iDeltaTime / 1000.0f;

	if (m_fCanMoveDistance >= m_fNeedMoveDistance || m_fNeedMoveDistance <= 16.0f)
	{
		if (m_kEffectData.iSkillTypeID)
		{
			if (!m_bPureShow)
			{
				BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
				if (pkAttacker)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					kParam.SetParam(BTP_pkTrgUnit, pkTarget);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fTarPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fTarPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}
		OnRemove();

		ParseAttack();
		return;
	}
	if (m_fCanMoveDistance > 0.01f && m_fNeedMoveDistance != 0)
	{
		m_kEffectData.fPosX += m_fCanMoveDistance * (m_kEffectData.fTarPosX - m_kEffectData.fPosX) / m_fNeedMoveDistance;
		m_kEffectData.fPosY += m_fCanMoveDistance * (m_kEffectData.fTarPosY - m_kEffectData.fPosY) / m_fNeedMoveDistance;
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}
}

void BeEffect::UpdateLineTrace(int iDeltaTime)
{
	void* pkTarget = UpdateTargetPos();

	m_fNeedMoveDistance = GetDistance(m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	m_fCanMoveDistance = GetMoveSpeed() * iDeltaTime / 1000.0f;

	if (m_fCanMoveDistance >= m_fNeedMoveDistance || m_fNeedMoveDistance <= 16.0f)
	{
		BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
		if (m_kEffectData.iSkillTypeID)
		{
			if (!m_bPureShow)
			{
				if (pkAttacker)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					kParam.SetParam(BTP_pkTrgUnit, pkTarget);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fTarPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fTarPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}

		OnRemove();

		ParseAttack();
		return;
	}

	if (m_fCanMoveDistance > 0.01f && m_fNeedMoveDistance != 0)
	{
		m_kEffectData.fPosX += m_fCanMoveDistance * (m_kEffectData.fTarPosX - m_kEffectData.fPosX) / m_fNeedMoveDistance;
		m_kEffectData.fPosY += m_fCanMoveDistance * (m_kEffectData.fTarPosY - m_kEffectData.fPosY) / m_fNeedMoveDistance;
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}
}

void BeEffect::UpdateCurveTraceFrank(int iDeltaTime)
{
	void* pkTarget = UpdateTargetPos();

	int spendTime = gTime - m_kEffectData.dwCreateTime;
	float needDis = GetDistance(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	float needTime = needDis * 1000.0f / GetMoveSpeed();
	float fSpendTime = (float)spendTime / needTime;
	float t = 1.0f;//std::min(1, fSpendTime);
	if (t > fSpendTime)
	{
		t = fSpendTime;
	}

	float sinH = sin(m_kEffectData.fFace - D3DX_PI * 0.5f);
	float cosH = cos(m_kEffectData.fFace - D3DX_PI * 0.5f);
	float fOrgPosX, fOrgPosY, fTarPosX, fTarPosY, fResDis, fCurDis, fParaPer = 0.0f;
	fOrgPosX = m_kEffectData.fOrgPosX;
	fOrgPosY = m_kEffectData.fOrgPosY;
	fTarPosX = m_kEffectData.fTarPosX;
	fTarPosY = m_kEffectData.fTarPosY;
	BeUnit* pkAttacker = nullptr;

	SeCalSkillLvlData rkData;
	pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);

	if (!pkAttacker)
	{
		return;
	}

	if (!pkAttacker->GetSkillLvlData(rkData, m_kEffectData.iSkillTypeID)) {

		return;
	}

	fResDis = rkData.GetSpellDistance();
	fCurDis = GetDistance(fOrgPosX, fOrgPosY, fTarPosX, fTarPosY);
	fParaPer = float(fCurDis / fResDis);

	float offset1X = rkData.GetDataValue(VALUE_D) * GetMissileArc() * fParaPer;
	float offset1Y = rkData.GetDataValue(VALUE_E);
	float offset2X = rkData.GetDataValue(VALUE_F) * GetMissileArc() * fParaPer;
	float offset2Y = rkData.GetDataValue(VALUE_G);

	float p2x = fOrgPosX + cosH * offset1X;
	float p2y = fOrgPosY + sinH * offset1X;
	float p3x = fTarPosX + cosH * offset2X;
	float p3y = fTarPosY + sinH * offset2X;

	if (t >= 1)
	{
		if (m_kEffectData.iSkillTypeID)
		{
			if (!m_bPureShow)
			{
				if (pkAttacker)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					kParam.SetParam(BTP_pkTrgUnit, pkTarget);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fTarPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fTarPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}

		OnRemove();
		ParseAttack();
		return;
	}

	m_kEffectData.fPosX = pow(1 - t, 3) * fOrgPosX + 3 * pow(1 - t, 2) * t * p2x + 3 * (1 - t) * t * t * p3x + t * t * t * fTarPosX;
	m_kEffectData.fPosY = pow(1 - t, 3) * fOrgPosY + 3 * pow(1 - t, 2) * t * p2y + 3 * (1 - t) * t * t * p3y + t * t * t * fTarPosY;
}

void BeEffect::UpdateLineTraceCollision(int iDeltaTime)
{
	m_fNeedMoveDistance = GetDistance(m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	m_fCanMoveDistance = GetMoveSpeed() * iDeltaTime / 1000.0f;

	UnitGroup kGroup;
	BeUnit* pkAttacker = nullptr;
	if (gTime % 100 == 0)
	{
		pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
		if (!pkAttacker)
		{
			return;
		}
		int iSkillTargetFlag = m_kEffectData.iTargetFlag;
		int iStaticProcFlag = 0;
		int iDynaPropFlag = 0;
		pkAttacker->TranslateSkillTargetType(iSkillTargetFlag, iStaticProcFlag, iDynaPropFlag);
	}

	if (m_fCanMoveDistance >= m_fNeedMoveDistance || m_fNeedMoveDistance <= 16.0f || !kGroup.empty())
	{
		if (m_kEffectData.iSkillTypeID && !kGroup.empty())
		{
			BeUnit* pkAttacked = gUnitMgr.GetGroupNearestUnit(kGroup, m_kEffectData.fPosX, m_kEffectData.fPosY);
			if (pkAttacked)
			{
				if (!pkAttacker)
				{
					pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
				}

				if (!pkAttacker)
				{
					return;
				}

				SetTargetID(pkAttacked->GetID());

				if (!m_bPureShow)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					kParam.SetParam(BTP_pkTrgUnit, pkAttacked);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}

		OnRemove();

		SetFlag(BEF_REMOVE);
		return;
	}

	if (m_fCanMoveDistance > 0.01f && m_fNeedMoveDistance != 0)
	{
		m_kEffectData.fPosX += m_fCanMoveDistance * (m_kEffectData.fTarPosX - m_kEffectData.fPosX) / m_fNeedMoveDistance;
		m_kEffectData.fPosY += m_fCanMoveDistance * (m_kEffectData.fTarPosY - m_kEffectData.fPosY) / m_fNeedMoveDistance;
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}
}

void BeEffect::UpdateCastTrace(int iDeltaTime)
{
	void* pkTarget = UpdateTargetPos();

	m_fNeedMoveDistance = GetDistance(m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	m_fCanMoveDistance = GetMoveSpeed() * iDeltaTime / 1000.0f;
	if (m_fCanMoveDistance >= m_fNeedMoveDistance || m_fNeedMoveDistance <= 16.0f)
	{
		if (m_kEffectData.iSkillTypeID)
		{
			BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
			if (pkAttacker)
			{
				if (!m_bPureShow)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					kParam.SetParam(BTP_pkTrgUnit, pkTarget);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fTarPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fTarPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}

		OnRemove();

		ParseAttack();
		return;
	}

	if (m_fCanMoveDistance > 0.01f && m_fNeedMoveDistance != 0)
	{
		m_kEffectData.fPosX += m_fCanMoveDistance * (m_kEffectData.fTarPosX - m_kEffectData.fPosX) / m_fNeedMoveDistance;
		m_kEffectData.fPosY += m_fCanMoveDistance * (m_kEffectData.fTarPosY - m_kEffectData.fPosY) / m_fNeedMoveDistance;
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}

	m_fPrePosX = m_kEffectData.fPosX;
	m_fPrePosY = m_kEffectData.fPosY;
	m_fPrePosZ = m_kEffectData.fPosZ;
}

void BeEffect::UpdateCastTraceCollision(int iDeltaTime)
{
	m_fNeedMoveDistance = GetDistance(m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	m_fCanMoveDistance = GetMoveSpeed() * iDeltaTime / 1000.0f;

	UnitGroup kGroup;
	BeUnit* pkAttacker = nullptr;
	if (gTime % 100 == 0)
	{
		pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
		if (!pkAttacker)
		{
			return;
		}
		int iSkillTargetFlag = m_kEffectData.iTargetFlag;
		int iStaticProcFlag = 0;
		int iDynaPropFlag = 0;
		pkAttacker->TranslateSkillTargetType(iSkillTargetFlag, iStaticProcFlag, iDynaPropFlag);

		gUnitMgr.GetAreaGroup(kGroup, m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fCollisionScope, pkAttacker, iDynaPropFlag, iStaticProcFlag);
	}

	if (m_fCanMoveDistance >= m_fNeedMoveDistance || m_fNeedMoveDistance <= 16.0f || !kGroup.empty())
	{
		if (m_kEffectData.iSkillTypeID && !kGroup.empty())
		{
			BeUnit* pkAttacked = gUnitMgr.GetGroupNearestUnit(kGroup, m_kEffectData.fPosX, m_kEffectData.fPosY);
			if (pkAttacked)
			{
				if (!pkAttacker)
				{
					pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
				}

				if (!pkAttacker)
				{
					return;
				}

				SetTargetID(pkAttacked->GetID());

				if (!m_bPureShow)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					kParam.SetParam(BTP_pkTrgUnit, pkAttacked);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}

		OnRemove();

		SetFlag(BEF_REMOVE);
		return;
	}

	if (m_fCanMoveDistance > 0.01f && m_fNeedMoveDistance != 0)
	{
		m_kEffectData.fPosX += m_fCanMoveDistance * (m_kEffectData.fTarPosX - m_kEffectData.fPosX) / m_fNeedMoveDistance;
		m_kEffectData.fPosY += m_fCanMoveDistance * (m_kEffectData.fTarPosY - m_kEffectData.fPosY) / m_fNeedMoveDistance;
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}

	float fDisZ = 0;
	if (m_fNeedMoveDistance > 0)
	{
		fDisZ = m_fCanMoveDistance * (m_kEffectData.fTarPosZ - m_kEffectData.fPosZ) / m_fNeedMoveDistance;
	}

	m_fPrePosX = m_kEffectData.fPosX;
	m_fPrePosY = m_kEffectData.fPosY;
	m_fPrePosZ = m_kEffectData.fPosZ;
}

void BeEffect::UpdateCurveTrace(int iDeltaTime)
{
	void* pkTarget = UpdateTargetPos();

	m_fNeedMoveDistance = GetDistance(m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	m_fCanMoveDistance = GetMoveSpeed() * iDeltaTime / 1000.0f;

	if (m_fCanMoveDistance >= m_fNeedMoveDistance || m_fNeedMoveDistance <= 16.0f)
	{
		if (m_kEffectData.iSkillTypeID)
		{
			BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
			if (pkAttacker)
			{
				if (!m_bPureShow)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					kParam.SetParam(BTP_pkTrgUnit, pkTarget);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fTarPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fTarPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}

		OnRemove();

		ParseAttack();
		return;
	}

	if (m_fCanMoveDistance > 0.01f && m_fNeedMoveDistance != 0)
	{
		m_kEffectData.fPosX += m_fCanMoveDistance * (m_kEffectData.fTarPosX - m_kEffectData.fPosX) / m_fNeedMoveDistance;
		m_kEffectData.fPosY += m_fCanMoveDistance * (m_kEffectData.fTarPosY - m_kEffectData.fPosY) / m_fNeedMoveDistance;
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}

	ParseCurveTrace();
}

void BeEffect::UpdateCurveTraceCollision(int iDeltaTime)
{
	void* pkTarget = UpdateTargetPos();

	m_fNeedMoveDistance = GetDistance(m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY);
	m_fCanMoveDistance = GetMoveSpeed() * iDeltaTime / 1000.0f;

	UnitGroup kGroup;
	BeUnit* pkAttacker = nullptr;
	if (gTime % 100 == 0)
	{
		pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
		if (!pkAttacker)
		{
			return;
		}
		int iSkillTargetFlag = m_kEffectData.iTargetFlag;
		int iStaticProcFlag = 0;
		int iDynaPropFlag = 0;
		pkAttacker->TranslateSkillTargetType(iSkillTargetFlag, iStaticProcFlag, iDynaPropFlag);

		gUnitMgr.GetAreaGroup(kGroup, m_kEffectData.fPosX, m_kEffectData.fPosY, m_kEffectData.fCollisionScope, pkAttacker, BeCommandType::BCT_SPELL, m_kEffectData.iSkillTypeID);
	}

	if (m_fCanMoveDistance >= m_fNeedMoveDistance || m_fNeedMoveDistance <= 16.0f || !kGroup.empty())
	{
		if (m_kEffectData.iSkillTypeID && !kGroup.empty())
		{
			BeUnit* pkAttacked = gUnitMgr.GetGroupNearestUnit(kGroup, m_kEffectData.fPosX, m_kEffectData.fPosY);
			if (pkAttacked)
			{
				if (!pkAttacker)
				{
					pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID(), true);
				}

				if (!pkAttacker)
				{
					return;
				}

				if (pkAttacked)
				{
					SetTargetID(pkAttacked->GetID());
				}

				if (!m_bPureShow)
				{
					TePtParam kParam;
					kParam.SetParam(BTP_iSkillTypeID, m_kEffectData.iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, m_kEffectData.iSkillLevel);
					if (pkAttacked)
					{
						kParam.SetParam(BTP_pkTrgUnit, pkAttacked);
					}
					else
					{
						kParam.SetParam(BTP_pkTrgUnit, pkTarget);
					}
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_fSpellTargetPosX, m_kEffectData.fPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, m_kEffectData.fPosY);
					kParam.SetParam(BTP_pkEffect, this);
					kParam.SetParam(BTP_iTargetType, m_kEffectData.eTargetType);

					gTrgMgr.FireTrigger(BTE_EFFECT_ARRIVED, kParam);
				}
			}
		}

		OnRemove();

		SetFlag(BEF_REMOVE);
		return;
	}

	if (m_fCanMoveDistance > 0.01f && m_fNeedMoveDistance != 0)
	{
		m_kEffectData.fPosX += m_fCanMoveDistance * (m_kEffectData.fTarPosX - m_kEffectData.fPosX) / m_fNeedMoveDistance;
		m_kEffectData.fPosY += m_fCanMoveDistance * (m_kEffectData.fTarPosY - m_kEffectData.fPosY) / m_fNeedMoveDistance;
	}
	else
	{
		m_kEffectData.fPosX = m_kEffectData.fTarPosX;
		m_kEffectData.fPosY = m_kEffectData.fTarPosY;
	}

	ParseCurveTrace();
}


void BeEffect::ParseAttack(void)
{
	switch (m_kEffectData.eTargetType)
	{
	case BET_POSITION:
		SetFlag(BEF_REMOVE);
		break;
	case BET_UNIT:
	{
		if (m_bPureShow)
		{
			SetFlag(BEF_REMOVE);
			break;
		}

		BeUnit* pkTarget = gUnitMgr.GetUnitByID(GetTargetID());

		if (m_pkAttr && m_pkAttr->iBumpCount > 0 && pkTarget /*&& !pkTarget->IsDead()*/)
		{
			m_pkAttr->ClrFlag(BAF_NORMAL);
			m_pkAttr->ClrFlag(BAF_FIRST);

			if (pkTarget->HasUserData(UDK_NONE))
			{
				SetFlag(BEF_REMOVE);
				break;
			}

			if (m_pkAttr->bFirstAttack)
			{
				m_kEffectData.fData = atan2f(pkTarget->GetPosY() - m_kEffectData.fOrgPosY, pkTarget->GetPosX() - m_kEffectData.fOrgPosX);
			}
			bool bNeedRemove = false;
			m_pkAttr->bFirstAttack = false;

			if (m_kEffectData.iBumpCount < m_pkAttr->iBumpCount)
			{
				BeUnit* pkAttacker = gUnitMgr.GetUnitByID(GetOwnerID());
				if (pkAttacker && !pkAttacker->IsDead())
				{
					UnitGroup kGroup;
					gUnitMgr.GetAreaGroup(kGroup, m_kEffectData.fPosX, m_kEffectData.fPosY, m_pkAttr->fBumpRadius, pkAttacker, BeCommandType::BCT_ATTACK, 0);
					if (kGroup.empty())
					{
						bNeedRemove = true;
					}
					else
					{
						float fOrgFace = m_kEffectData.fData;

						std::vector<BeUnit*> akAttacked;
						m_akBumpAttackedID.push_back(m_kEffectData.iTargetID);
						for (int i = 0; i < (int)kGroup.empty(); ++i)
						{
							BeUnit* pkAttacked = kGroup[i];
							if (pkAttacked && !pkAttacked->IsDead() && !pkAttacked->HasUserData(UDK_NONE))
							{
								float fTarFace = atan2f(pkAttacked->GetPosY() - pkTarget->GetPosY(), pkAttacked->GetPosX() - pkTarget->GetPosX());
								float fDelFace = fTarFace - fOrgFace;
								if (fDelFace < 0)
								{
									fDelFace += D3DX_2PI;
								}

								bool bHasAttacked = false;
								for (int j = 0; j < (int)m_akBumpAttackedID.size(); ++j)
								{
									if (pkAttacked->GetID() == m_akBumpAttackedID[j])
									{
										bHasAttacked = true;
										break;
									}
								}
								if (!bHasAttacked)
								{
									akAttacked.push_back(pkAttacked);
								}
							}
						}

						if (!akAttacked.empty())
						{
							BeUnit* pkNewAttacked = gUnitMgr.GetGroupNearestUnit(akAttacked, m_kEffectData.fPosX, m_kEffectData.fPosY);

							if (pkNewAttacked && pkNewAttacked->GetID() != m_kEffectData.iTargetID)
							{
								SetEffectPosition(pkNewAttacked->GetID(), m_kEffectData.fPosX, m_kEffectData.fPosY, 0.0f, 0.0f, m_kEffectData.fMissileArc, m_kEffectData.fPosZ);
								SetSkillBumpCount(m_kEffectData.iBumpCount + 1);
								m_pkAttr->fDamage *= 1.0f - m_pkAttr->fDamageDecPer;
								m_akBumpAttackedID.push_back(pkNewAttacked->GetID());
								break;
							}
						}
						else
						{
							BeUnit* pkNewAttacked = gUnitMgr.GetGroupNearestUnit(kGroup, m_kEffectData.fPosX, m_kEffectData.fPosY);
							if (pkNewAttacked && !pkNewAttacked->IsDead() && pkNewAttacked->GetID() != m_kEffectData.iTargetID && !pkNewAttacked->HasUserData(UDK_NONE))
							{
								SetEffectPosition(pkNewAttacked->GetID(), m_kEffectData.fPosX, m_kEffectData.fPosY, 0.0f, 0.0f, m_kEffectData.fMissileArc, m_kEffectData.fPosZ);
								SetSkillBumpCount(m_kEffectData.iBumpCount + 1);
								m_pkAttr->fDamage *= 1.0f - m_pkAttr->fDamageDecPer;
								m_akBumpAttackedID.push_back(pkNewAttacked->GetID());
								break;
							}
						}

						bNeedRemove = true;
					}
				}
				else
				{
					bNeedRemove = true;
				}
			}
			else
			{
				bNeedRemove = true;
			}

			SetFlag(BEF_REMOVE);
			SetSkillBumpCount(m_pkAttr->iBumpCount);
		}
		else if (m_kEffectData.iBumpCount > 0)
		{
			SetSkillBumpCount(m_kEffectData.iBumpCount - 1);
		}
		else
		{
			SetFlag(BEF_REMOVE);
		}
	}break;
	case BET_ITEM:
	{
		if (m_bPureShow)
		{
			SetFlag(BEF_REMOVE);
			break;
		}
		auto pkTarget = gMapItemMgr.GetMapItemByID(GetTargetID());
		if (pkTarget)
		{
			pkTarget->BeDamage();
			if (pkTarget->IsDead())
			{
				gMapItemMgr.DelMapItem(GetTargetID());
			}
		}
		SetFlag(BEF_REMOVE);
	}break;
	case BET_EFFECT:
	{
		SetFlag(BEF_REMOVE);
		break;
	}
	default:
	{
		SetFlag(BEF_REMOVE);
	}break;
	}
}

void BeEffect::SetOrgScale(float fOrgScale, bool bChangeScale)
{
	m_fOrgScale = fOrgScale;
	m_bChangeScale = bChangeScale;
}

void BeEffect::GetCurvePos(float fOrgX, float fOrgY, float fTarX, float fTarY, float fFace, std::vector<TePos2>& akCurvePos, int iPointNum /*= 100*/, BeEffectCurveType eCurveType /*= BECT_CURVELEFT*/, BeEffectCurveStyle eCurveStyle /*= BECS_LEFTBIG*/)
{
	akCurvePos.clear();
	const float fAllDistance = GetDistance(fOrgX, fOrgY, fTarX, fTarY);
	const float fCosValue = cosf(fFace);
	const float fSinValue = sinf(fFace);
	TePos2 akPos[4];

	akPos[0].fX = fOrgX;
	akPos[0].fY = fOrgY;
	akPos[3].fX = fTarX;
	akPos[3].fY = fTarY;

	float fTempDis = 0.0f;
	float fDisAX = 0.0f;
	float fDisAY = 0.0f;
	float fDisBX = 0.0f;
	float fDisBY = 0.0f;

	switch (eCurveStyle)
	{
	case BECS_LEFTBIG:
	{
		if (m_kCurvePointMul2.fX == 0.0f)
		{
			m_kCurvePointMul2.fX = 1.0f / 20.0f;
		}

		if (m_kCurvePointMul2.fY == 0.0f)
		{
			m_kCurvePointMul2.fY = 1.0f / 10.0f;
		}

		fDisAX = fAllDistance * m_kCurvePointMul2.fX;
		fDisAY = fAllDistance * m_kCurvePointMul2.fY;
		if (m_kCurvePointMul3.fX == 0.0f)
		{
			m_kCurvePointMul3.fX = 1.0f / 20.0f;
		}

		if (m_kCurvePointMul3.fY == 0.0f)
		{
			m_kCurvePointMul3.fY = 9.0f / 10.0f;
		}

		fDisBX = fAllDistance * m_kCurvePointMul3.fX;
		fDisBY = fAllDistance * m_kCurvePointMul3.fY;
		break;
	}
	case BECS_RIGHTBIG:
	{
		int iRand = rand() % 5;
		int iPointRand = rand() % 4;
		if (m_kCurvePointMul2.fX == 0.0f)
		{
			m_kCurvePointMul2.fX = iPointRand / 10.0f;
		}

		if (m_kCurvePointMul2.fY == 0.0f)
		{
			m_kCurvePointMul2.fY = (2.0f + iRand) / 10.0f;
		}

		fDisAX = fAllDistance * m_kCurvePointMul2.fX;
		fDisAY = fAllDistance * m_kCurvePointMul2.fY;
		if (m_kCurvePointMul3.fX == 0.0f)
		{
			m_kCurvePointMul3.fX = iPointRand / 10.0f;
		}

		if (m_kCurvePointMul3.fY == 0.0f)
		{
			m_kCurvePointMul3.fY = (3.0f + iRand) / 10.0f;
		}

		fDisBX = fAllDistance * m_kCurvePointMul3.fX;
		fDisBY = fAllDistance * m_kCurvePointMul3.fY;
		break;
	}
	case BECS_MIDBIG:
	{
		fDisAX = fAllDistance / 2;
		fDisAY = fAllDistance / 2;

		fDisBX = fAllDistance / 2;
		fDisBY = fAllDistance / 2;
		break;
	}
	case BECS_WAVE:
	{
		int iRand = rand() % 10 + 1;
		fDisAX = fAllDistance / (iRand * rand());
		fDisAY = fAllDistance / (iRand * rand());

		fDisBX = fAllDistance / iRand;
		fDisBY = fAllDistance / iRand;
		break;
	}
	default:
		break;
	}

	switch (eCurveType)
	{
	case BECT_CURVELEFT:
	case BECT_CURVESLOWLEFT:
	{
		akPos[1].fX = akPos[0].fX + fDisAX * fCosValue - fDisAY * fSinValue;
		akPos[1].fY = akPos[0].fY + fDisAX * fSinValue + fDisAY * fCosValue;

		akPos[2].fX = akPos[0].fX + fDisBX * fCosValue - fDisBY * fSinValue;
		akPos[2].fY = akPos[0].fY + fDisBX * fSinValue + fDisBY * fCosValue;
		break;
	}
	case BECT_CURVERIGHT:
	case BECT_CURVESLOWRIGHT:
	{
		akPos[1].fX = akPos[0].fX + fDisAX * fCosValue + fDisAY * fSinValue;
		akPos[1].fY = akPos[0].fY + fDisAX * fSinValue - fDisAY * fCosValue;

		akPos[2].fX = akPos[0].fX + fDisBX * fCosValue + fDisBY * fSinValue;
		akPos[2].fY = akPos[0].fY + fDisBX * fSinValue - fDisBY * fCosValue;
		break;
	}
	case BECT_CURVERMID:
	{
		akPos[1].fX = akPos[0].fX + 1 * fCosValue - 1 * fSinValue;
		akPos[1].fY = akPos[0].fY + 1 * fSinValue + 1 * fCosValue;

		akPos[2].fX = akPos[0].fX + 1 * fCosValue - 1 * fSinValue;
		akPos[2].fY = akPos[0].fY + 1 * fSinValue + 1 * fCosValue;
		break;
	}
	default:
		break;
	}

	float fAddT = 1.0f / (iPointNum - 1);
	float fValueT = 0.0f;
	for (int i = 0; i < iPointNum; ++i)
	{
		fValueT += fAddT;
		TePos2 kPos = GetPointOnBezier(akPos, fValueT);
		akCurvePos.push_back(kPos);
	}
}

void BeEffect::SetCycleAngle(float fAngle)
{
	if (m_fCycleAngle != fAngle)
	{
		SetChangeFlag(BECF_CYCLEANGLE);
	}
	m_fCycleAngle = fAngle;
}

float BeEffect::GetCycleAngle(void)
{
	return m_fCycleAngle;
}

void BeEffect::SetCurvePointMul2(float fPointMulX, float fPointMulY)
{
	m_kCurvePointMul2.fX = fPointMulX;
	m_kCurvePointMul2.fY = fPointMulY;
}

void BeEffect::SetCurvePointMul3(float fPointMulX, float fPointMulY)
{
	m_kCurvePointMul3.fX = fPointMulX;
	m_kCurvePointMul3.fY = fPointMulY;
}

void BeEffect::SetTurning(bool bTurn)
{
	m_bTurning = bTurn;
}

void BeEffect::SetRotateOption(float fRad, bool bDeasil)
{
	m_bRotate = true;
}

void BeEffect::ParseCurveTrace(void)
{
	float fDisZ = m_fCanMoveDistance * (m_kEffectData.fTarPosZ - m_kEffectData.fPosZ) / m_fNeedMoveDistance;

	int iTotalNum = 100;
	if (m_eCurveType == BECT_CURVESLOWLEFT || m_eCurveType == BECT_CURVESLOWRIGHT)
	{
		iTotalNum = 1000;
	}

	m_kEffectData.fOrgPosZ += fDisZ;
	if (m_kEffectData.iTargetID == 0)
	{
		if (!m_bHasCulCurve)
		{
			switch (m_eCurveType)
			{
			case BECT_CURVELEFT:
			case BECT_CURVESLOWLEFT:
			{
				GetCurvePos(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY, m_fOwnerFace, m_akCurvePosW, iTotalNum, m_eCurveType, m_eCurveStyle);
				GetCurvePos(0, 0, m_fAllDistance / 2, 0, m_fOwnerFace + D3DX_PI / 2, m_akCurvePosH, iTotalNum, BECT_CURVELEFT, BECS_MIDBIG);
				break;
			}
			case BECT_CURVERIGHT:
			case BECT_CURVESLOWRIGHT:
			{
				GetCurvePos(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY, m_fOwnerFace, m_akCurvePosW, iTotalNum, m_eCurveType, m_eCurveStyle);
				GetCurvePos(0, 0, m_fAllDistance / 2, 0, m_fOwnerFace + D3DX_PI / 2, m_akCurvePosH, iTotalNum, BECT_CURVELEFT, BECS_MIDBIG);
				break;
			}
			case BECT_CURVERMID:
			{
				GetCurvePos(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY, m_fOwnerFace, m_akCurvePosW, iTotalNum, m_eCurveType, m_eCurveStyle);
				GetCurvePos(0, 0, m_fAllDistance, 0, m_fOwnerFace + D3DX_PI / 2, m_akCurvePosH, iTotalNum, BECT_CURVELEFT, m_eCurveStyle);
				break;
			}
			default:
				break;
			}
		}

		if (m_fAllDistance < 4.0f)
		{
			return;
		}
		int iPerDis = abs((int)((m_fAllDistance - m_fNeedMoveDistance) * iTotalNum / m_fAllDistance));
		if (iPerDis < 0)
		{
			return;
		}
		iPerDis %= iTotalNum;

		TePos2& rkPosW = m_akCurvePosW[iPerDis];

		float fTempDis = GetDistance(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fPosX, m_kEffectData.fPosY);
		iPerDis = abs((int)((m_fAllDistance - fTempDis) * iTotalNum / m_fAllDistance));
		iPerDis %= iTotalNum;

		m_kEffectData.fPosZ = m_kEffectData.fOrgPosZ + m_akCurvePosH[iPerDis].fY;

		m_fPrePosX = rkPosW.fX;
		m_fPrePosY = rkPosW.fY;

		m_fPrePosZ = m_kEffectData.fPosZ;
	}
	else
	{
		float fAllDistance = 0.0f;
		BeUnit* pkUnit = gMain.m_pkUnitMgr->GetUnitByID(m_kEffectData.iTargetID);
		if (pkUnit && !pkUnit->IsDead())
		{
			fAllDistance = GetDistance(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, pkUnit->GetPosX(), pkUnit->GetPosY());
		}

		if (!m_bHasCulCurve)
		{
			switch (m_eCurveType)
			{
			case BECT_CURVELEFT:
			case BECT_CURVESLOWLEFT:
			{
				GetCurvePos(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY, m_fOwnerFace, m_akCurvePosW, iTotalNum, m_eCurveType, m_eCurveStyle);
				GetCurvePos(0, 0, fAllDistance / 2, 0, m_fOwnerFace + D3DX_PI / 2, m_akCurvePosH, iTotalNum, BECT_CURVELEFT, BECS_MIDBIG);
				break;
			}
			case BECT_CURVERIGHT:
			case BECT_CURVESLOWRIGHT:
			{
				GetCurvePos(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY, m_fOwnerFace, m_akCurvePosW, iTotalNum, m_eCurveType, m_eCurveStyle);
				GetCurvePos(0, 0, fAllDistance / 2, 0, m_fOwnerFace + D3DX_PI / 2, m_akCurvePosH, iTotalNum, BECT_CURVELEFT, BECS_MIDBIG);
				break;
			}
			case BECT_CURVERMID:
			{
				GetCurvePos(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fTarPosX, m_kEffectData.fTarPosY, m_fOwnerFace, m_akCurvePosW, iTotalNum, m_eCurveType, m_eCurveStyle);
				GetCurvePos(0, 0, fAllDistance, 0, m_fOwnerFace + D3DX_PI / 2, m_akCurvePosH, iTotalNum, BECT_CURVELEFT, m_eCurveStyle);
				break;
			}
			default:
				break;
			}

			m_fAllDistance = fAllDistance;
			m_bHasCulCurve = true;
		}

		if (0 == m_iCurveDividNum)
		{
			return;
		}
		if (m_fNeedMoveDistance <= (m_fAllDistance / m_iCurveDividNum) && m_bHasCulCurve)
		{
			m_bHasCulCurve = false;
			m_iCurveDividNum = m_iCurveDividNum << 1;
		}

		if (fAllDistance < 4.0f)
		{
			return;
		}
		int iPerDis = abs((int)((fAllDistance - m_fNeedMoveDistance) * iTotalNum / fAllDistance));
		if (iPerDis < 0)
		{
			return;
		}
		iPerDis %= iTotalNum;
		TePos2& rkPosW = m_akCurvePosW[iPerDis];

		float fTempDis = GetDistance(m_kEffectData.fOrgPosX, m_kEffectData.fOrgPosY, m_kEffectData.fPosX, m_kEffectData.fPosY);
		iPerDis = abs((int)((fAllDistance - fTempDis) * iTotalNum / fAllDistance));
		iPerDis %= iTotalNum;

		m_kEffectData.fPosZ = m_kEffectData.fOrgPosZ + m_akCurvePosH[iPerDis].fY;

		m_fPrePosX = rkPosW.fX;
		m_fPrePosY = rkPosW.fY;

		m_fPrePosZ = m_kEffectData.fPosZ;
	}
}

float BeEffect::GetEffecttrueFace(int iDY, int iDX, int iDeltaTime)
{
	return atan2f(iDY, iDX);
}

void BeEffect::SetNewEffect(bool bNew)
{
	m_bNewEffect = bNew;
}

bool BeEffect::IsNewEffect()
{
	return m_bNewEffect;
}

void BeEffect::CopyBumpAttackeID(const BeEffect* pEffect)
{
	if (!pEffect)
	{
		return;
	}
	m_akBumpAttackedID.clear();
	m_BumpCount.clear();

	m_akBumpAttackedID.assign(pEffect->m_akBumpAttackedID.begin(), pEffect->m_akBumpAttackedID.end());
	m_BumpCount.assign(pEffect->m_BumpCount.begin(), pEffect->m_BumpCount.end());
}

void BeEffect::AddBumCount(int iUnitId)
{
	std::vector<BumpCount>::iterator iter = m_BumpCount.begin();
	bool bAdded = false;
	for (; iter != m_BumpCount.end(); iter++)
	{
		BumpCount bc = *iter;
		if (bc.iUnitID == iUnitId)
		{
			(*iter).iBumpCount++;
			bAdded = true;
			break;
		}
	}

	if (!bAdded)
	{
		BumpCount newBc;
		newBc.iUnitID = iUnitId;
		newBc.iBumpCount = 1;
		m_BumpCount.push_back(newBc);
	}
}

bool BeEffect::bBumCountOk(int iUnitId, int iMaxCount)
{
	std::vector<BumpCount>::iterator iter = m_BumpCount.begin();
	for (; iter != m_BumpCount.end(); iter++)
	{
		BumpCount bc = *iter;
		if (bc.iUnitID == iUnitId)
		{
			if (bc.iBumpCount >= iMaxCount)
			{
				return false;
			}
		}
	}

	return true;
}
