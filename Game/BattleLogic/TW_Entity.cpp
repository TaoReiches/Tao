/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Entity.h"
#include "TW_EntityMgr.h"
#include "TW_Functions.h"

template<class T>
BeEntity<T>::BeEntity()
{

}

template<class T>
BeEntity<T>::BeEntity(int iID)
{
	m_iID = iID;
	m_iTypeID = 0;
	m_iFlag = 0;
}

template<class T>
BeEntity<T>::~BeEntity(void)
{
}

template<class T>
void BeEntity<T>::Update(int iDeltaTime)
{
}

template<class T>
BeSingleLinkEntity<T>::BeSingleLinkEntity(int iID)
{
}

template<class T>
BeSingleLinkEntity<T>::~BeSingleLinkEntity(void)
{
}

template<class T>
void BeSingleLinkEntity<T>::Link(float fX, float fY, TwEntityMgr<T>* pkMgr)
{
	auto pkEle = std::shared_ptr<BeElement>(this);

	int iBX = TruncToInt(fX) / TwEntityMgr::BLOCK_ELE_SIZE;
	int iBY = TruncToInt(fY) / TwEntityMgr::BLOCK_ELE_SIZE;

	if (iBX < 0)
	{
		iBX = 0;
	}
	else if (iBX >= pkMgr->GetBlocksW())
	{
		iBX = pkMgr->GetBlocksW() - 1;
	}

	if (iBY < 0)
	{
		iBY = 0;
	}
	else if (iBY >= pkMgr->GetBlocksH())
	{
		iBY = pkMgr->GetBlocksH() - 1;
	}

	auto pkHead = pkMgr->GetBlock()[iBY * pkMgr->GetBlocksW() + iBX];
	if (!pkHead || pkEle->pkBlock == pkHead)
	{
		return;
	}

	if (pkEle->pkPrev)
	{
		pkEle->pkPrev->pkNext = pkEle->pkNext;
	}
	if (pkEle->pkNext)
	{
		pkEle->pkNext->pkPrev = pkEle->pkPrev;
	}

	auto pkTrail = pkHead->pkPrev;
	if (pkTrail)
	{
		pkTrail->pkNext = pkEle;
		pkEle->pkPrev = pkTrail;

		pkHead->pkPrev = pkEle;
		pkEle->pkNext = pkHead;

		pkEle->pkBlock = pkHead;
	}
}

template<class T>
void BeSingleLinkEntity<T>::Unlink(void)
{
	BeElement* pkEle = this;
	if (pkEle->pkPrev)
	{
		pkEle->pkPrev->pkNext = pkEle->pkNext;
	}
	if (pkEle->pkNext)
	{
		pkEle->pkNext->pkPrev = pkEle->pkPrev;
	}

	pkEle->pkPrev = nullptr;
	pkEle->pkNext = nullptr;
}
