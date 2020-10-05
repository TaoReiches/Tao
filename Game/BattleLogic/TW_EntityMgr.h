#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_LogicBase.h"

#define BLOCK_ELE_SIZE	512

class BeEntity;
struct BeElement;

class BeEntityMgr : public BeMainPtr
{
public:
	BeEntityMgr(void);
	virtual ~BeEntityMgr(void);

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
	BeElement* GetBlock(void) const
	{
		return m_akBlock;
	}
protected:
	int			m_iBlocksW;
	int			m_iBlocksH;
	BeElement* m_akBlock;
};
