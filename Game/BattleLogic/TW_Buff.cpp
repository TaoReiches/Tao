/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Buff.h"

BeBuffer::BeBuffer(int iID) : BeCarry(iID)
{
	m_bNeedUpdate = false;
	m_eType = BCT_BUFFER;
	m_iRefreshTime = 0;
	m_pkRes = NULL;
	m_iBufferEffectPlayer = -1;
	m_iBUfferEffectGroup = -1;
	m_fBufferEffectScale = 1.0f;
	m_bDeadNoRemove = false;
	m_bCopyFromOhers = false;
	m_bSkillRealize = false;
	m_bHasDecreased = false;
	m_bHasBufVoice = false;
	m_bIsInterrupt = false;
	m_iOrgCamp = 0;
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

void BeBuffer::SetRemoveTime(UInt uiTime, bool bForceChange, bool bNeedRecordChange)
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
				int iNewLiveTime = iOldLiveTime * (1.0f - fToughness);
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

			if (HasProperty(BUFFER_PROPERTY_CDWANCHENG))
			{
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, pkUnit);
				kParam.SetParam(BTP_pkBuffer, this);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONCDCOMPLETE, kParam);
			}
		}
	}
}

SeCalSkillLvlData* BeBuffer::GetSkillLvlData(void)
{
	return &m_kData.kSkillLvlData;
}

void BeBuffer::SetUnitID(int iUnitID)
{
	m_kData.iUnitID = iUnitID;
	BeUnit* pkUnit = gUnitMgr.GetUnitByID(iUnitID, true);
	if (pkUnit)
	{
		m_kData.kUnit = pkUnit->GetSharePtr();
	}
	else if (!m_kData.kUnit.IsNull())
	{
		m_kData.kUnit = SeSharedPtr<BeUnit>();
	}
	//m_kData.iUnitID = iUnitID;
}

int BeBuffer::GetBufferUnitID() const
{
	return m_kData.iUnitID;
}

void BeBuffer::SetOrgUnitID(int iUnitID)
{
	m_kData.iOrgUnitID = iUnitID;

	BeUnit* pkUnit = gUnitMgr.GetUnitByID(iUnitID);
	if (pkUnit)
	{
		m_iOrgCamp = pkUnit->GetCamp();
	}
}

bool BeBuffer::IsClientNeed() const
{
	return false;
}