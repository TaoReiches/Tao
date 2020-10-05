#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include "TW_EntityMgr.h"

class BeEffect;

class BeEffectMgr : public BeEntityMgr
{
public:
	BeEffectMgr(void);
	~BeEffectMgr(void);

	bool Initialize(void);
	void Update(int iDeltaTime);
	void ClientUpdate(int iDeltaTime) {};
	void Finialize(void);


	void Clear();

	BeEffect* AddEffect(BeEffectRace iTypeID);
	void		DelEffect(int iID);
	BeEffect* GetEffectByID(int iID);

	std::unordered_map<int, BeEffect*>& GetID2Effect();
	std::map<int, BeEffect*>& GetID2ClientEffect();

protected:
	virtual BeEffect* NewEffect(int iID);
	virtual void SafeDeleteEffect(BeEffect*& pkEffect);

protected:
	std::unordered_map<int, BeEffect*>	m_kID2Effect;

	// sm
public:
	int			GeneratePureID(void);
	int			GenerateIncPureID(void);

	BeEffect* GetClientEffectByID(int iEffectID);
	void		DelClientEffect(int iEffectID);
	void		DelUnitClientEffects(int iUnitID);

protected:
	std::map<int, BeEffect*> m_kID2PureEffect;
	unsigned int							m_uiDecPureID;
	int								m_iIncPureID;
public:
	void	ClrAllPureEffectData();

public:
	int					GenFontEffectID(void);


private:
	int					m_iFontEffectID;

public:
	void PushNeedRemoveEffect(int iEffectID);

public:
	std::vector<int>& PureGetDelEffect();
	std::vector<int>	m_aiPureDelEffect;
};
