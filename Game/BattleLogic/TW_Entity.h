#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include "TW_FlagObject.h"
#include "TW_LogicBase.h"
#include "TW_Pos2.h"

struct BeElement
{
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
	BeEntity(int iID);
	virtual ~BeEntity(void);

	inline int GetID(void) const
	{
		return m_iID;
	}

	inline void SetTypeID(int iTypeID)
	{
		m_iTypeID = iTypeID;
	}

	inline int GetTypeID(void) const
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
	BeSingleLinkEntity(int iID);
	virtual ~BeSingleLinkEntity(void);
	virtual void Link(float fX, float fY, BeEntityMgr* pkMgr);
	virtual void Unlink(void);
};

class BeMultiElement : public BeElement
{
public:
	BeMultiElement()
	{
		m_pkEntity = nullptr;
	}
	BeEntity* m_pkEntity;
};
class BeMultiLinkEntity : public BeEntity
{
public:
	BeMultiLinkEntity(int iID);
	virtual ~BeMultiLinkEntity(void);
	virtual void Link(float fX, float fY, BeEntityMgr* pkMgr);
	virtual void Unlink(void);

	virtual const TePos2& GetBoundingTopLeft(void) const = 0;
	virtual const TePos2& GetBoundingBottomRight(void) const = 0;
protected:
	std::vector<BeMultiElement> m_kElement;
};
