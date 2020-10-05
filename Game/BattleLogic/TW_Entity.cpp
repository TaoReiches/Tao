/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Entity.h"
#include "TW_EntityMgr.h"
#include "TW_Functions.h"

BeEntity::BeEntity(int iID)
{
	m_iID = iID;
	m_iTypeID = 0;
	m_iFlag = 0;
}

BeEntity::~BeEntity(void)
{
}


void BeEntity::Update(int iDeltaTime)
{
}

BeSingleLinkEntity::BeSingleLinkEntity(int iID)
{
}
BeSingleLinkEntity::~BeSingleLinkEntity(void)
{
}
void BeSingleLinkEntity::Link(float fX, float fY, BeEntityMgr* pkMgr)
{
	BeElement* pkEle = this;

	int iBX = TruncToInt(fX) / BLOCK_ELE_SIZE;
	int iBY = TruncToInt(fY) / BLOCK_ELE_SIZE;

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

	BeElement* pkHead = &pkMgr->GetBlock()[iBY * pkMgr->GetBlocksW() + iBX];
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

	BeElement* pkTrail = pkHead->pkPrev;
	if (pkTrail)
	{
		pkTrail->pkNext = pkEle;
		pkEle->pkPrev = pkTrail;

		pkHead->pkPrev = pkEle;
		pkEle->pkNext = pkHead;

		pkEle->pkBlock = pkHead;
	}
}

void BeSingleLinkEntity::Unlink(void)
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


BeMultiLinkEntity::BeMultiLinkEntity(int iID) : BeEntity(iID)
{
}
BeMultiLinkEntity::~BeMultiLinkEntity(void)
{
}

template<class T>
T LimitRange(T value, T minvalue, T maxvalue)
{
	if (value < minvalue)
	{
		return minvalue;
	}
	else if (value > maxvalue)
	{
		return maxvalue;
	}
	else
	{
		return value;
	}
}
void BeMultiLinkEntity::Link(float fX, float fY, BeEntityMgr* pkMgr)
{
	Unlink(); 

	TePos2 kTopLeft = GetBoundingTopLeft();
	TePos2 kBottomRight = GetBoundingBottomRight();

	int iBX = (int)kTopLeft.fX / BLOCK_ELE_SIZE;
	int iBY = (int)kTopLeft.fY / BLOCK_ELE_SIZE;

	int iEX = (int)kBottomRight.fX / BLOCK_ELE_SIZE;
	int iEY = (int)kBottomRight.fY / BLOCK_ELE_SIZE;

	iBX = LimitRange(iBX, 0, pkMgr->GetBlocksW() - 1);
	iBY = LimitRange(iBY, 0, pkMgr->GetBlocksH() - 1);

	iEX = LimitRange(iEX, iBX, pkMgr->GetBlocksW() - 1);
	iEY = LimitRange(iEY, iBY, pkMgr->GetBlocksH() - 1);

	int iCount = (iEX - iBX + 1) * (iEY - iBY + 1);

	m_kElement.resize(iCount);

	iCount = 0;
	for (int iX = iBX; iX <= iEX; ++iX)
	{
		for (int iY = iBY; iY <= iEY; ++iY, ++iCount)
		{
			BeMultiElement* pkEle = &m_kElement[iCount];
			if (!pkEle)
			{
				continue;
			}
			pkEle->m_pkEntity = this;
			BeElement* pkHead = &pkMgr->GetBlock()[iY * pkMgr->GetBlocksW() + iX];
			if (!pkHead || pkEle->pkBlock == pkHead)
			{
				continue;
			}

			BeElement* pkTrail = pkHead->pkPrev;
			if (pkTrail)
			{
				pkTrail->pkNext = pkEle;
				pkEle->pkPrev = pkTrail;

				pkHead->pkPrev = pkEle;
				pkEle->pkNext = pkHead;

				pkEle->pkBlock = pkHead;
			}
		}
	}
}

void BeMultiLinkEntity::Unlink(void)
{
	for (int i = 0; i < (int)m_kElement.size(); ++i)
	{
		BeElement* pkEle = &m_kElement[i];
		if (!pkEle)
		{
			continue;
		}
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
	m_kElement.clear();
}
