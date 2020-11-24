/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TaskActionAttack.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include "TW_MemoryObject.h"
#include "TW_TriggerEvent.h"
#include "TW_Effect.h"
#include "TW_EffectMgr.h"
#include "TW_IRandom.h"
#include "TW_TriggerMgr.h"

BeTaskActionAttack::BeTaskActionAttack()
{
	m_eType = BeTaskType::STT_ACTION_ATTACK;

	m_bAttacking = false;
	m_bDamaged = false;
	m_bCanCancel = false;
	m_iSkillTypeID = 0;
	m_iSkillLevel = 0;
	m_iAttackCount = 0;
}

void BeTaskActionAttack::SetTargetID(int iID, bool bIsOrb, int iSkillTypeID, int iSkillLevel, bool bAttackPos)
{
	if (iID != gUnit.GetAttackingUnitID()
		|| m_iSkillTypeID != iSkillTypeID
		|| m_iSkillLevel != iSkillLevel
		)
	{
		gUnit.SetAttackingUnitID(iID, bIsOrb, bAttackPos);

		BeUnit* pkTarget = gUnitMgr.GetUnitByID(iID);
		if (pkTarget && (gUnit.GetClass() != UNIT_CLASSTYPE_BUILDING || gUnit.HasOtherFlag(BUOF_BUILDING_TURN)))
		{
			float fTarFace = atan2f(pkTarget->GetPosY() - gUnit.GetPosY(), pkTarget->GetPosX() - gUnit.GetPosX());
			gUnit.SetTarFace(fTarFace);
		}

		m_bAttacking = false;
		m_bDamaged = false;
		m_bBeginAttack = true;

		m_iSkillTypeID = iSkillTypeID;
		m_iSkillLevel = iSkillLevel;
		m_iAttackCount = 0;
		gUnit.SetActionCurTime(0);
	}

	m_bBeginAttack = true;
	m_bCanCancel = true;
}

int BeTaskActionAttack::GetTargetID() const
{
	return gUnit.GetAttackingUnitID();
}

bool BeTaskActionAttack::IsAttacking(void) const
{
	return m_bAttacking;
}

bool BeTaskActionAttack::IsDamaged(void) const
{
	return m_bDamaged;
}

bool BeTaskActionAttack::IsCanCancel(void) const
{
	return true;
}

bool	BeTaskActionAttack::IsPerDamageEnd(void)	const
{
	return m_bCanCancel;
}

int BeTaskActionAttack::GetAttackCount(void) const
{
	return m_iAttackCount;
}

BeExeResult BeTaskActionAttack::Execute(int& iDeltaTime)
{
	BeTask::Execute(iDeltaTime);

	gUnit.ClrFlag(BUF_MOVING);

	BeUnit* pkTarget = nullptr;
	float fPosX = gUnit.GetPosX();
	float fPosY = gUnit.GetPosY();

	pkTarget = gUnitMgr.GetUnitByID(GetTargetID());
	if (pkTarget)
	{
		fPosX = pkTarget->GetPosX();
		fPosY = pkTarget->GetPosY();
	}

	if (!m_bAttacking)
	{
		if ((gUnit.GetAttackElapseTime() + iDeltaTime) <= gUnit.GetAttackBackPt())
		{
			gUnit.IncActionCurTime(iDeltaTime);

			iDeltaTime = 0;
			m_bCanCancel = false;

			return BeExeResult::BER_TIME_OUT;
		}

		if (pkTarget)
		{
			if (gUnit.GetClass() == UNIT_CLASSTYPE_BUILDING)
			{
			}
			else
			{
				float fTarFace = atan2f(pkTarget->GetPosY() - gUnit.GetPosY(), pkTarget->GetPosX() - gUnit.GetPosX());
				gUnit.SetTarFace(fTarFace);
			}
		}

		if (pkTarget)
		{
			BeAttackingAttr* kAttr = mpAttackingAttr.alloc();
			gUnit.UpdateAttribute();
			gUnit.GetAttackingAttr(pkTarget, kAttr);

			m_kAttr = kAttr;
			m_kAttr->SetFlag(BeAttackingFlag::BAF_FIRST);
			m_kAttr->SetFlag(BeAttackingFlag::BAF_NORMAL);

			if (m_kAttr->HasFlag(BAF_FIRST) && pkTarget->GetAttackedAvoid())
			{
				m_kAttr->SetFlag(BAF_AVOID);
			}
			gUnit.TrgOnPreAttack(pkTarget, m_kAttr);
			m_kAttr->eAttackType = BAT_NORMAL;

			if (!gUnit.HasUnitCarryFlag(BUCF_NO_DAMAGE_EFFECT))
			{
				m_kAttr->iBeDamagedEffect = gUnit.GetDamageModle();
			}
		}
		m_bAttacking = true;
	}

	if (m_bBeginAttack)
	{
		if (pkTarget)
		{
			if (!gUnit.UnitCanAttack(pkTarget, true, true))
			{
				gUnit.SetAttackingUnitID(0);
				m_bCanCancel = true;
				return BeExeResult::BER_EXE_END;
			}
		}
		float	fBaoJiRate = gRandNum.RandFloat(0.0f, 1.0f);
		if (fBaoJiRate < gUnit.GetBaoJiRate())
		{
			m_kAttr->SetFlag(BAF_BAOJI);
			{
				gUnit.SetUnitAction(BUA_ATTACK, BAN_attack);
			}
		}
		else
		{
			m_kAttr->SetFlag(BAF_NOT_BAOJI);
			gUnit.SetUnitAction(BUA_ATTACK, BAN_attack);
		}

		gUnit.SetLastAttackTime(gTime);

		m_bBeginAttack = false;

		if (pkTarget)
		{
			if (gUnit.GetClass() == UNIT_CLASSTYPE_HERO || gUnit.GetClass() == UNIT_CLASSTYPE_SOLIDER)
			{
				if (pkTarget->GetClass() == UNIT_CLASSTYPE_SOLIDER || pkTarget->GetClass() == UNIT_CLASSTYPE_HERO)
				{
					//gUnit.SetCastVisionTime(gTime + 3000);
				}
			}
		}
	}
	if (!gUnit.IsDamageTime(iDeltaTime))
	{
		gUnit.IncActionCurTime(iDeltaTime);
		iDeltaTime = 0;
		m_bCanCancel = false;

		return BeExeResult::BER_TIME_OUT;
	}

	if (!m_bDamaged)
	{
		gUnit.ResetAttackElapseTime();

		if (pkTarget)
		{
			if (!gUnit.UnitCanAttack(pkTarget, true, true))
			{
				gUnit.SetAttackingUnitID(0);
				m_bCanCancel = true;
				return BeExeResult::BER_EXE_END;
			}

			float fTargetDistance2 = GetDistance2(gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY());
			float fMaxAttackRange = gUnit.GetAttackRange(pkTarget) + 250.0f;
			if (fTargetDistance2 > fMaxAttackRange * fMaxAttackRange)
			{
				return BeExeResult::BER_EXE_END;
			}

			if (!pkTarget->IsDead())
			{
				m_iAttackCount += 1;
			}
			else
			{
				gUnit.SetAttackingUnitID(0);
				return BeExeResult::BER_EXE_END;
			}

			if (gUnit.HasFlag(BUF_IGNOREPHYSICRESIST))
			{
				m_kAttr->bIgnorePhysicResist = true;
			}

			if (gUnit.GetWeaponType() == 0)
			{
				gUnit.TrgOnAttack(GetTargetID(), m_kAttr);
				pkTarget->OnBeDamaged(*m_kAttr);
			}
			else if (gUnit.GetWeaponType() == 1)
			{
				auto pkEffect = gEffectMgr.AddEffect(BER_LINE_TRACE);
				if (pkEffect)
				{
					pkEffect->SetChangeFlag(BECF_CASTMISSILE);
					pkEffect->SetOwnerID(gUnit.GetID());
					pkEffect->SetEffectPosition(GetTargetID(), gUnit.GetPosX(), gUnit.GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY(), gUnit.GetMissileArc(), gUnit.GetPosZ());
					pkEffect->SetMoveSpeed(gUnit.GetMissileSpeed());
					if (m_kAttr->iMissileModel != 0)
					{
						pkEffect->SetFlag(BEF_ATTRMISSLE);
						pkEffect->SetModelFile(m_kAttr->iMissileModel, BAP_Weapon);
					}
					else
					{
						pkEffect->SetModelFile(gUnit.GetMissleModel(), BAP_Weapon);
					}

					pkEffect->SetFlag(BEF_IGNORE_MAGIC_IMMUNE);

					gUnit.SetEffectID(pkEffect->GetID());
				}

				gUnit.TrgOnAttack(GetTargetID(), m_kAttr);
			}

			//if (gUnit.IsHero())
			//{
				TwPtParam kParamHero;
				kParamHero.SetParam(TwTrgParamID::BTP_pkTrgUnit, &gUnit);
				kParamHero.SetParam(TwTrgParamID::BTP_iTargetID, GetTargetID());

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_HERO_ATTACK, kParamHero);
			//}
			//else if (gUnit.GetClass() == UNIT_CLASSTYPE_BUILDING)
			//{
			//	TePtParam kParam;
			//	kParam.SetParam(BTP_pkTrgUnit, &gUnit);
			//	kParam.SetParam(BTP_iTargetID, GetTargetID());

			//	gTrgMgr.FireTrigger(BTE_BUILDING_ATTACK, kParam);
			//}
			//else
			//{
			//	TePtParam kParam;
			//	kParam.SetParam(BTP_pkTrgUnit, &gUnit);
			//	kParam.SetParam(BTP_iTargetID, GetTargetID());

			//	gTrgMgr.FireTrigger(BTE_NOT_HERO_ATTACK, kParam);
			//}
		}
		else
		{
			gUnit.SetAttackingUnitID(0);
			return BeExeResult::BER_EXE_END;
		}
		m_bDamaged = true;
	}

	if (gUnit.IsActionCurTimeOut(iDeltaTime))
	{
		m_bDamaged = false;
		m_bAttacking = false;
		m_bCanCancel = true;
		m_bBeginAttack = true;
		return BeExeResult::BER_EXE_END;
	}

	if (gUnit.GetAttackElapseTime() <= gUnit.GetAttackBackPt() / 3.0f)
	{
		m_bCanCancel = false;
	}
	else
	{
		m_bCanCancel = true;
	}

	gUnit.IncActionCurTime(iDeltaTime);

	iDeltaTime = 0;

	return BeExeResult::BER_TIME_OUT;
}
