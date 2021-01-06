#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <memory>
#include <vector>
#include "TW_FlagObject.h"
#include "TW_LogicBase.h"
#include "TW_Pos2.h"

template<class T>
class BeElement
{
public:
    BeElement()
    {
        pkPrev = nullptr;
        pkNext = nullptr;
        pkBlock = nullptr;
    }
    std::shared_ptr<BeElement> pkPrev;
    std::shared_ptr<BeElement> pkNext;
    std::shared_ptr<T> pkBlock;
};

template<class T>
class TwEntityMgr;

template<class T>
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

	virtual void Link(float fX, float fY, TwEntityMgr<T>* pkMgr) = 0;
	virtual void Unlink() = 0;
protected:
	int		m_iID;
	int		m_iTypeID;
};

template<class T>
class BeSingleLinkEntity : public BeElement<T>
{
public:
    BeSingleLinkEntity(){};
	BeSingleLinkEntity(int iID);
	virtual ~BeSingleLinkEntity(void);
	virtual void Link(float fX, float fY, TwEntityMgr<T>* pkMgr);
	virtual void Unlink(void);
};
