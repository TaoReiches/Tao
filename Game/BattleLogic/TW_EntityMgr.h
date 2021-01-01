#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include "TW_LogicBase.h"

class BeEntity;
class BeElement;

class TwEntityMgr : public BeMainPtr
{
public:
	TwEntityMgr(void);
	virtual ~TwEntityMgr(void);

	virtual bool Initialize(void);
	virtual void Update(int iDeltaTime) = 0;
	virtual void Finialize(void);

	void Link(float fX, float fY, BeEntity* pkEnt);
	void Unlink(BeEntity* pkEnt);
	void GetBlockArea(float fX, float fY, float fRadius, int& iBX, int& iBY, int& iEX, int& iEY) const;

	int GetBlocksW(void) const
	{
		return m_iBlocksW;
	}
	int GetBlocksH(void) const
	{
		return m_iBlocksH;
	}
	const std::vector<std::shared_ptr<BeElement>>& GetBlock(void) const
	{
		return m_akBlock;
	}

public:
	static const int BLOCK_ELE_SIZE = 512;

protected:
	int							m_iBlocksW;
	int							m_iBlocksH;
	std::vector<std::shared_ptr<BeElement>>	m_akBlock;
};
