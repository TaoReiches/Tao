#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include "TW_FlagObject.h"
#include "TW_LogicBase.h"
#include "TW_Pos2.h"

class BeElement
{
public:
	BeElement()
	{
		pkPrev = nullptr;
		pkNext = nullptr;
		pkBlock = nullptr;
	}
	BeElement* pkPrev;
	BeElement* pkNext;
	BeElement* pkBlock;
};

class BeEntityMgr;
class BeEntity : public BeFlagObj, public BeMainPtr
{
public:
    BeEntity();
	BeEntity(int iID );
	virtual ~BeEntity(void);

	int GetID(void) const
	{
		return m_iID;
	}

	void SetTypeID(int iTypeID)
	{
		m_iTypeID = iTypeID;
	}

	int GetTypeID(void) const
	{
		return m_iTypeID;
	}

	virtual bool Initialize(int iTypeID) = 0;
	virtual void Update(int iDeltaTime) = 0;

	virtual void Link(float fX, float fY, BeEntityMgr* pkMgr) = 0;
	virtual void Unlink() = 0;
protected:
	int		m_iID;
	int		m_iTypeID;
};

class BeSingleLinkEntity : public BeElement
{
public:
    BeSingleLinkEntity(){};
	BeSingleLinkEntity(int iID);
	virtual ~BeSingleLinkEntity(void);
	virtual void Link(float fX, float fY, BeEntityMgr* pkMgr);
	virtual void Unlink(void);
};
