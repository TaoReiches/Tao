/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Entity.h"
#include "TW_EntityMgr.h"
#include "TW_Functions.h"


BeEntity::BeEntity()
{

}

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

	int iBX = TruncToInt(fX) / BeEntityMgr::BLOCK_ELE_SIZE;
	int iBY = TruncToInt(fY) / BeEntityMgr::BLOCK_ELE_SIZE;

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

	auto pkHead = &pkMgr->GetBlock()[iBY * pkMgr->GetBlocksW() + iBX];
	if (!pkHead || pkEle->pkBlock == pkHead->get())
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

	auto pkTrail = pkHead->get()->pkPrev;
	if (pkTrail)
	{
		pkTrail->pkNext = pkEle;
		pkEle->pkPrev = pkTrail;

		pkHead->get()->pkPrev = pkEle;
		pkEle->pkNext = pkHead->get();

		pkEle->pkBlock = pkHead->get();
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
