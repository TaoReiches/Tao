#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "TW_EntityMgr.h"
#include "TW_ShareUnitData.h"

class BeEffect;

class BeEffectMgr : public TwEntityMgr<BeEffect>
{
public:
	BeEffectMgr(void);
	~BeEffectMgr(void);

	bool Initialize(void);
	void Update(int iDeltaTime);
	void ClientUpdate(int iDeltaTime) {};
	void Finialize(void);
	void Clear();
	std::shared_ptr<BeEffect> AddEffect(BeEffectRace iTypeID);
	void		DelEffect(int iID);
	std::shared_ptr<BeEffect> GetEffectByID(int iID);
	std::unordered_map<int, std::shared_ptr<BeEffect>>& GetID2Effect();
	std::unordered_map<int, std::shared_ptr<BeEffect>>& GetID2ClientEffect();
	int			GeneratePureID(void);
	int			GenerateIncPureID(void);
	std::shared_ptr<BeEffect>	GetClientEffectByID(int iEffectID);
	void		DelClientEffect(int iEffectID);
	void		DelUnitClientEffects(int iUnitID);
	void		ClrAllPureEffectData();
	int			GenFontEffectID(void);
	void		PushNeedRemoveEffect(int iEffectID);
	std::vector<int>& PureGetDelEffect();

protected:
	virtual std::shared_ptr<BeEffect> NewEffect(int iID);
	virtual void SafeDeleteEffect(std::shared_ptr<BeEffect>& pkEffect);

protected:
	std::unordered_map<int, std::shared_ptr<BeEffect>>	m_kID2Effect;
	std::unordered_map<int, std::shared_ptr<BeEffect>>			m_kID2PureEffect;
	unsigned int						m_uiDecPureID;
	int									m_iIncPureID;

private:
	int					m_iFontEffectID;
	std::vector<int>	m_aiPureDelEffect;
};
