/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Buff.h"
#include "TW_Main.h"
#include "TW_Unit.h"
#include "TW_UnitMgr.h"
#include "TW_ShareUnitData.h"
#include <TW_TriggerMgr.h>
#include "TW_TriggerEvent.h"
#include <Buffer_table.hpp>

BeBuffer::BeBuffer(int iID) : BeCarry(iID)
{
	m_bNeedUpdate = false;
	m_eType = BeCarryType::BCT_BUFFER;
	m_iRefreshTime = 0;
	m_pkRes.reset();
	m_iBufferEffectPlayer = -1;
	m_iBUfferEffectGroup = -1;
	m_fBufferEffectScale = 1.0f;
	m_bDeadNoRemove = false;
	m_bCopyFromOhers = false;
	m_bSkillRealize = false;
	m_bHasDecreased = false;
	m_bHasBufVoice = false;
	m_bIsInterrupt = false;
}

bool BeBuffer::Initialize(int iTypeID)
{
	BeCarry::Initialize(iTypeID);

	m_pkRes = BufferTableMgr::Get()->GetBufferTable(iTypeID);

	return true;
}

BeBuffer::~BeBuffer(void)
{

}

void BeBuffer::SetRemoveTime(unsigned int uiTime, bool bForceChange, bool bNeedRecordChange)
{
	bool	bCalToughness = false;
	if (HasCarryFlag(BUCF_CANNOTATTACK) || HasCarryFlag(BUCF_DIZZY) || HasCarryFlag(BUCF_ISFORBIDSKILL))
	{
		bCalToughness = true;
	}
	if (GetNormalAttrValue(NAT_PER_MOVE_SPEED) < 0 || GetNormalAttrValue(NAT_ABS_MOVE_SPEED))
	{
		bCalToughness = true;
	}
	float	fToughness = gUnit.GetToughness();
	if (fToughness > 0)
	{
		if (bCalToughness)
		{
			int		iOldLiveTime = uiTime - gTime;
			if (iOldLiveTime > 0)
			{
				int iNewLiveTime = (int)(iOldLiveTime * (1.0f - fToughness));
				uiTime = gTime + iNewLiveTime;
			}
		}
	}
	if (!bForceChange)
	{
		if (m_kData.uiRemoveTime == -1 || uiTime > m_kData.uiRemoveTime || uiTime == gTime) 
		{
			m_kData.uiRemoveTime = uiTime;
			SetAllLiveTime(uiTime - gTime);
		}
	}
	else
	{
		m_kData.uiRemoveTime = uiTime;
		SetAllLiveTime(uiTime - gTime);
	}
}

void BeBuffer::Update(BeUnit* pkUnit, int iDeltaTime)
{
	if (!pkUnit)
	{
		return;
	}

	if (m_kData.iCDTime > 0)
	{
		m_kData.iUpdateTime += iDeltaTime;
		if (m_kData.iUpdateTime >= m_kData.iCDTime)
		{
			m_kData.iUpdateTime -= m_kData.iCDTime;

			int iRemoveTime = (int)m_kData.uiRemoveTime;
			if ((int)gTime - iRemoveTime <= iDeltaTime && (int)gTime - iRemoveTime >= -iDeltaTime && !m_kData.bLastTimeUpdate)
			{
				return;
			}

			if (HasProperty(BUFFER_PROPERTY_CDTRIGGER))
			{
				TwPtParam kParam;
				kParam.SetParam(TwTrgParamID::BTP_pkTrgUnit, pkUnit);
				kParam.SetParam(TwTrgParamID::BTP_pkBuffer, this);

				gTrgMgr.FireTrigger(TwTriggerEvent::BTE_BUFFER_ONCDCOMPLETE, kParam);
			}
		}
	}
}

bool BeBuffer::HasProperty(int iProperty) const
{
	return ((m_pkRes->uiProperty & iProperty) == iProperty) ? true : false;
}

void BeBuffer::SetUnitID(int iUnitID)
{
	m_kData.iUnitID = iUnitID;
	BeUnit* pkUnit = gUnitMgr->GetUnitByID(iUnitID, true);
	if (pkUnit)
	{
		m_kData.kUnit.reset(pkUnit);
	}
}
