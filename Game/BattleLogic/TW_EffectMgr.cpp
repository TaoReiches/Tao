/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_EffectMgr.h"
#include "TW_Effect.h"
#include <TW_TriggerMgr.h>
#include "TW_TriggerEvent.h"
#include "TW_Main.h"
#include "TW_MemoryObject.h"

BeEffectMgr::BeEffectMgr(void)
{
	m_uiDecPureID = 0xFFFFFFFE;
	m_iIncPureID = std::numeric_limits<int>::min();
    m_iFontEffectID = 0;
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
	//BeEntityMgr::Update(iDeltaTime);

	for (auto itr = m_kID2Effect.begin(); itr != m_kID2Effect.end();)
	{
		std::shared_ptr<BeEffect>& pEffect = itr->second;
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
			TwPtParam kParam;
			kParam.SetParam(TwTrgParamID::BTP_pkEffect, pEffect.get());

			gTrgMgr.FireTrigger(TwTriggerEvent::BTE_EFFECT_DEL, kParam);

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

	for (auto itr = m_kID2PureEffect.begin(); itr != m_kID2PureEffect.end();)
	{
		std::shared_ptr<BeEffect>& pEffect = itr->second;
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
	for (auto itr = m_kID2Effect.begin(); itr != m_kID2Effect.end(); ++itr)
	{
		auto& pkEffect = itr->second;
		SafeDeleteEffect(pkEffect);
	}
	for (auto itr = m_kID2PureEffect.begin(); itr != m_kID2PureEffect.end(); ++itr)
	{
		auto& pkEffect = itr->second;
		SafeDeleteEffect(pkEffect);
	}

	m_uiDecPureID = 0xFFFFFFFE;
	m_iIncPureID = SHRT_MIN;
	m_iFontEffectID = 0;

	m_kID2Effect.clear();
	m_kID2PureEffect.clear();
}

std::shared_ptr<BeEffect> BeEffectMgr::NewEffect(int iID)
{
	auto pkEffect = std::shared_ptr<BeEffect>(mpEffect.alloc(iID));
	pkEffect->AttachMain(pkAttachMain);
	pkEffect->SetCreateTime(gTime);
	return pkEffect;
}

void BeEffectMgr::SafeDeleteEffect(std::shared_ptr<BeEffect>& pkEffect)
{
	if (pkEffect->GetModelFile() > 0)
	{
		m_aiPureDelEffect.push_back(pkEffect->GetID());
	}

	mpEffect.free(pkEffect.get());
	pkEffect.reset();
}
std::shared_ptr<BeEffect> BeEffectMgr::AddEffect(BeEffectRace iTypeID)
{
	int iID = gMain.GenerateID(GIT_EFFECT);
	auto pkEffect = NewEffect(iID);
	if (pkEffect->Initialize(iTypeID))
	{
		m_kID2Effect[iID] = pkEffect;

		//gMain.AddEntityPointer(GIT_EFFECT, iID, pkEffect.get());

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
	auto itr = m_kID2Effect.find(iID);
	if (itr != m_kID2Effect.end())
	{
		auto pkEffect = itr->second;
		if (pkEffect.get())
		{
			PushNeedRemoveEffect(iID);
			pkEffect->OnRemove();
		}
	}
}

std::shared_ptr<BeEffect> BeEffectMgr::GetEffectByID(int iID)
{
	if (iID == 0)
	{
		return NULL;
	}

	auto itr = m_kID2Effect.find(iID);
	if (itr != m_kID2Effect.end())
	{
		return itr->second;
	}
	return NULL;
}

std::unordered_map<int, std::shared_ptr<BeEffect>>& BeEffectMgr::GetID2Effect()
{
	return m_kID2Effect;
}

std::shared_ptr<BeEffect> BeEffectMgr::GetClientEffectByID(int iEffectID)
{
	auto itr = m_kID2PureEffect.find(iEffectID);
	if (itr != m_kID2PureEffect.end())
	{
		return itr->second;
	}
	return NULL;
}

void BeEffectMgr::DelClientEffect(int iEffectID)
{
	auto itr = m_kID2PureEffect.find(iEffectID);
	if (itr != m_kID2PureEffect.end())
	{
		auto pkEffect = itr->second;
		if (pkEffect)
		{
			PushNeedRemoveEffect(pkEffect->GetID());
		}
	}
}

void BeEffectMgr::DelUnitClientEffects(int iUnitID)
{
	auto iter = m_kID2PureEffect.begin();
	while (iter != m_kID2PureEffect.end())
	{
		int iEffectID = iter->first;
		auto pkEffect = iter->second;

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

std::unordered_map<int, std::shared_ptr<BeEffect>>& BeEffectMgr::GetID2ClientEffect()
{
	return m_kID2PureEffect;
}

void BeEffectMgr::PushNeedRemoveEffect(int iEffectID)
{
	bool bFind = false;
	if (iEffectID > 0)
	{
		auto pkEffect = gEffectMgr.GetEffectByID(iEffectID);
		if (pkEffect)
		{
			pkEffect->SetFlag(BEF_REMOVE);
		}
	}
	else
	{
		auto pkClientEffect = gEffectMgr.GetClientEffectByID(iEffectID);
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
