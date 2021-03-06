/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Entity.h"
#include "TW_EntityMgr.h"
#include "TW_Define.h"
#include "TW_Main.h"
#include "TW_Map.h"

template<class T>
TwEntityMgr<T>::TwEntityMgr(void)
{
	m_iBlocksW = 0;
	m_iBlocksH = 0;
}

template<class T>
TwEntityMgr<T>::~TwEntityMgr(void)
{
	//if (m_akBlock.get() != nullptr)
	//{
	//	delete[] m_akBlock.get();
	//}
	m_akBlock.clear();
}

template<class T>
bool TwEntityMgr<T>::Initialize(void)
{
	Finialize();

	m_iBlocksW = (int)gMap.GetWidth() / BLOCK_ELE_SIZE;
	m_iBlocksH = (int)gMap.GetHeight() / BLOCK_ELE_SIZE;

	int iBlocks = m_iBlocksW * m_iBlocksH;
    for (auto i = 0; i < iBlocks; ++i)
    {
        auto block = std::shared_ptr<BeElement>(new BeElement);
        block->pkBlock = nullptr;
        block->pkNext = block;
        block->pkPrev = block;
        m_akBlock.push_back(block);
    }

	//auto pkEle = m_akBlock.begin();
	//for (int i = 0; i < iBlocks; i++, pkEle++)
	//{
	//	(*pkEle)->pkBlock = nullptr;
 //       (*pkEle)->pkNext = (*pkEle);
 //       (*pkEle)->pkPrev = (*pkEle);
	//}

	return true;
}

template<class T>
void TwEntityMgr<T>::Finialize(void)
{
	m_iBlocksW = 0;
	m_iBlocksH = 0;
	//if (m_akBlock.get() != nullptr)
	//{
	//	delete[] m_akBlock.get();
	//}
	m_akBlock.clear();
}

template<class T>
void TwEntityMgr<T>::Link(float fX, float fY, BeEntity<T>* pkEnt)
{
	pkEnt->Link(fX, fY, this);
}

template<class T>
void TwEntityMgr<T>::Unlink(BeEntity<T>* pkEnt)
{
	pkEnt->Unlink();
}

template<class T>
void TwEntityMgr<T>::GetBlockArea(float fX, float fY, float fRadius, int& iBX, int& iBY, int& iEX, int& iEY) const
{
	int iPosX = (int)(fX);
	int iPosY = (int)(fY);

	iBX = (iPosX - (int)fRadius) / BLOCK_ELE_SIZE;
	iBY = (iPosY - (int)fRadius) / BLOCK_ELE_SIZE;
	iEX = (iPosX + (int)fRadius) / BLOCK_ELE_SIZE;
	iEY = (iPosY + (int)fRadius) / BLOCK_ELE_SIZE;

	if (iBX < 0)
	{
		iBX = 0;
	}
	if (iBY < 0)
	{
		iBY = 0;
	}
	if (iEX >= m_iBlocksW)
	{
		iEX = m_iBlocksW - 1;
	}
	if (iEY >= m_iBlocksH)
	{
		iEY = m_iBlocksH - 1;
	}
}
