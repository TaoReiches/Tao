/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_EffectMgr.h"
#include "TW_Effect.h"

BeEffectMgr::BeEffectMgr(void)
{
	m_uiDecPureID = 0xFFFFFFFE;
	m_iIncPureID = INT_MIN;
}

BeEffectMgr::~BeEffectMgr(void)
{
	Clear();
}

bool BeEffectMgr::Initialize(void)
{
	Clear();

	return BeEntityMgr::Initialize();
}

void BeEffectMgr::Update(int iDeltaTime)
{
	BeEntityMgr::Update(iDeltaTime);

	for (std::unordered_map<int, BeEffect*>::iterator itr = m_kID2Effect.begin(); itr != m_kID2Effect.end();)
	{
		BeEffect* pEffect = itr->second;
		if (!pEffect)
		{
			++itr;
			continue;
		}
		if (!pEffect->HasFlag(BEF_REMOVE))
		{
			pEffect->Update(iDeltaTime);
		}

		if (pEffect->HasFlag(BEF_REMOVE) && !pEffect->IsNewEffect())
		{
			TePtParam kParam;
			kParam.SetParam(BTP_pkEffect, pEffect);

			gTrgMgr.FireTrigger(BTE_EFFECT_DEL, kParam);

			SafeDeleteEffect(pEffect);
			itr = m_kID2Effect.erase(itr);
		}
		else
		{
			++itr;
		}
		if (pEffect)
		{
			pEffect->SetNewEffect(false);
		}
	}

	for (std::map<int, BeEffect*>::iterator itr = m_kID2PureEffect.begin(); itr != m_kID2PureEffect.end();)
	{
		BeEffect* pEffect = itr->second;
		if (!pEffect)
		{
			++itr;
			continue;
		}
		pEffect->Update(iDeltaTime);

		if (pEffect->HasFlag(BEF_REMOVE) && !pEffect->IsNewEffect())
		{
			SafeDeleteEffect(pEffect);
			m_kID2PureEffect.erase(itr++);
		}
		else
		{
			++itr;
		}
		if (pEffect)
		{
			pEffect->SetNewEffect(false);
		}
	}
}

void BeEffectMgr::Finialize(void)
{
	Clear();

	BeEntityMgr::Finialize();
}

void BeEffectMgr::Clear()
{
	for (std::unordered_map<int, BeEffect*>::iterator itr = m_kID2Effect.begin(); itr != m_kID2Effect.end(); ++itr)
	{
		BeEffect* pkEffect = itr->second;
		SafeDeleteEffect(pkEffect);
	}
	for (std::map<int, BeEffect*>::iterator itr = m_kID2PureEffect.begin(); itr != m_kID2PureEffect.end(); ++itr)
	{
		BeEffect* pkEffect = itr->second;
		SafeDeleteEffect(pkEffect);
	}

	m_uiDecPureID = 0xFFFFFFFE;
	m_iIncPureID = SHRT_MIN;
	m_iFontEffectID = 0;

	m_kID2Effect.clear();
	m_kID2PureEffect.clear();
}

BeEffect* BeEffectMgr::NewEffect(int iID)
{
	BeEffect* pkEffect = BeEffect::NEW(iID);
	pkEffect->AttachMain(pkAttachMain);
	pkEffect->SetCreateTime(gTime);
	return pkEffect;
}

void BeEffectMgr::SafeDeleteEffect(BeEffect*& pkEffect)
{
		if (pkEffect->GetModelFile() > 0)
		{
			m_aiPureDelEffect.push_back(pkEffect->GetID());
		}

	BeEffect::DEL(pkEffect);
	pkEffect = NULL;
}
BeEffect* BeEffectMgr::AddEffect(BeEffectRace iTypeID)
{
	int iID = gMain.GenerateID(GIT_EFFECT);
	BeEffect* pkEffect = NewEffect(iID);
	if (pkEffect->Initialize(iTypeID))
	{
		m_kID2Effect[iID] = pkEffect;

		gMain.AddEntityPointer(GIT_EFFECT, iID, pkEffect);

		return pkEffect;
	}
	else
	{
		SafeDeleteEffect(pkEffect);
		return NULL;
	}
}

void BeEffectMgr::DelEffect(int iID)
{
	std::unordered_map<int, BeEffect*>::iterator itr = m_kID2Effect.find(iID);
	if (itr != m_kID2Effect.end())
	{
		BeEffect* pkEffect = itr->second;
		if (pkEffect)
		{
			PushNeedRemoveEffect(iID);
			pkEffect->OnRemove();
		}
	}
}

BeEffect* BeEffectMgr::GetEffectByID(int iID)
{
	if (iID == 0)
	{
		return NULL;
	}

	return (BeEffect*)(gMain.GetEntityPointer(GIT_EFFECT, iID));

	std::unordered_map<int, BeEffect*>::iterator itr = m_kID2Effect.find(iID);
	if (itr != m_kID2Effect.end())
	{
		return itr->second;
	}
	return NULL;
}

std::unordered_map<int, BeEffect*>& BeEffectMgr::GetID2Effect()
{
	return m_kID2Effect;
}

BeEffect* BeEffectMgr::GetClientEffectByID(int iEffectID)
{
	std::map<int, BeEffect*>::iterator itr = m_kID2PureEffect.find(iEffectID);
	if (itr != m_kID2PureEffect.end())
	{
		return itr->second;
	}
	return NULL;
}

void BeEffectMgr::DelClientEffect(int iEffectID)
{
	std::map<int, BeEffect*>::iterator itr = m_kID2PureEffect.find(iEffectID);
	if (itr != m_kID2PureEffect.end())
	{

		BeEffect* pkEffect = itr->second;
		if (pkEffect)
		{
			PushNeedRemoveEffect(pkEffect->GetID());
		}
	}
}

void BeEffectMgr::DelUnitClientEffects(int iUnitID)
{
	std::map<int, BeEffect*>::iterator iter = m_kID2PureEffect.begin();
	while (iter != m_kID2PureEffect.end())
	{
		int iEffectID = iter->first;
		BeEffect* pkEffect = iter->second;

		if (pkEffect && pkEffect->GetTargetID() == iUnitID)
		{
			PushNeedRemoveEffect(pkEffect->GetID());
		}

		iter++;
	}
}

int BeEffectMgr::GeneratePureID(void)
{
	m_uiDecPureID--;
	return (int)m_uiDecPureID;
}

int BeEffectMgr::GenerateIncPureID()
{
	m_iIncPureID++;
	return m_iIncPureID;
}

void BeEffectMgr::ClrAllPureEffectData()
{
	m_aiPureDelEffect.clear();
}

std::map<int, BeEffect*>& BeEffectMgr::GetID2ClientEffect()
{
	return m_kID2PureEffect;
}

void BeEffectMgr::PushNeedRemoveEffect(int iEffectID)
{
	bool bFind = false;
	if (iEffectID > 0)
	{
		BeEffect* pkEffect = gEffectMgr.GetEffectByID(iEffectID);
		if (pkEffect)
		{
			pkEffect->SetFlag(BEF_REMOVE);
		}
	}
	else
	{
		BeEffect* pkClientEffect = gEffectMgr.GetClientEffectByID(iEffectID);
		if (pkClientEffect)
		{
			pkClientEffect->SetFlag(BEF_REMOVE);
		}
	}
}

int BeEffectMgr::GenFontEffectID(void)
{
	return ++m_iFontEffectID;
}

std::vector<int>& BeEffectMgr::PureGetDelEffect()
{
	return m_aiPureDelEffect;
}
