/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <stdio.h>
#include <cstring>
#include "TW_MemoryPool.h"

NotifyMempoolAlloc	TeMemoryPool::m_fAllocCallBack = NULL;

TeMemoryPool::TeMemoryPool(int iEleSize, int iBaseEles, int iAddEles)
{
	m_iEleSize = iEleSize < sizeof(void*) ? sizeof(void*) : iEleSize;
	m_iEleSize += sizeof(unsigned int) * 2;

	static int s_iIdx = 0;
	s_iIdx++;
	s_iIdx %= 1000;
	char acMagic[16] = { 0 };
	sprintf_s(acMagic, "B%03dB%03d", s_iIdx, s_iIdx);
	std::memcpy(&m_stMagicBegin, acMagic, sizeof(unsigned int));
	sprintf_s(acMagic, "E%03dE%03d", s_iIdx, s_iIdx);
	std::memcpy(&m_stMagicEnd, acMagic, sizeof(unsigned int));

	m_iBaseEles = iBaseEles;
	m_iTotalSize = 0;
	m_iAddEles = iAddEles;

	Reset();
	ExtendPool();
}

TeMemoryPool::~TeMemoryPool(void)
{
	Clear();
}

void TeMemoryPool::Reset(void)
{
	m_iAllocEles = 0;
	m_iAllocMaxEles = 0;
	m_pkFreeFirst = NULL;

	m_kHeadList.pkNext = m_kHeadList.pkPrev = &m_kHeadList;
}

void* TeMemoryPool::NewEle(void)
{
	if (m_pkFreeFirst == NULL && !ExtendPool())
	{
		return NULL;
	}

	++m_iAllocEles;
	if (m_iAllocMaxEles < m_iAllocEles)
	{
		m_iAllocMaxEles = m_iAllocEles;
	}

	char* pbyMem = (char*)m_pkFreeFirst;
	m_pkFreeFirst = *((void**)m_pkFreeFirst);

	*((unsigned int*)pbyMem) = m_stMagicBegin;
	*((unsigned int*)(pbyMem + m_iEleSize - sizeof(unsigned int))) = m_stMagicEnd;

	return (pbyMem + sizeof(unsigned int));
}

void TeMemoryPool::DelEle(void* pkEle)
{
	if (!pkEle || m_iAllocEles <= 0)
	{
		return;
	}

	pkEle = ((char*)pkEle) - sizeof(unsigned int);

	--m_iAllocEles;

	*((void**)pkEle) = m_pkFreeFirst;
	m_pkFreeFirst = pkEle;
}

bool TeMemoryPool::IsValid(void* pkEle)
{
	if (!pkEle)
	{
		return false;
	}

	char* pbyMem = (char*)pkEle;
	pbyMem -= sizeof(unsigned int);

	unsigned int stMagic = *((unsigned int*)pbyMem);
	if (stMagic != m_stMagicBegin)
	{
		return false;
	}

	stMagic = *((unsigned int*)(pbyMem + m_iEleSize - sizeof(unsigned int)));
	if (stMagic != m_stMagicEnd)
	{
		return false;
	}

	return true;
}

bool TeMemoryPool::ExtendPool(void)
{
	m_iAddEles = m_iBaseEles > 64 ? m_iBaseEles / 4 : m_iAddEles;
	int iEles = (m_kHeadList.pkNext == &m_kHeadList) ? m_iBaseEles : m_iAddEles;
	int iSize = iEles * m_iEleSize + sizeof(SeHeadList) - 8;
	m_iTotalSize += iSize;

	SeHeadList* pkHeadList = (SeHeadList*)(new char[iSize]);
	if (pkHeadList == NULL)
	{
		return false;
	}

	if (m_fAllocCallBack)
	{
		m_fAllocCallBack(m_acName, iSize);
	}

	pkHeadList->iSize = iSize;
	pkHeadList->pkNext = &m_kHeadList;
	pkHeadList->pkPrev = pkHeadList->pkNext->pkPrev;
	pkHeadList->pkNext->pkPrev = pkHeadList->pkPrev->pkNext = pkHeadList;

	m_pkFreeFirst = pkHeadList->acData;

	char* pkData = pkHeadList->acData;
	for (int i = 0; i < iEles - 1; i++, pkData += m_iEleSize)
	{
		*((void**)pkData) = (void*)(pkData + m_iEleSize);
	}
	*((void**)pkData) = NULL;

	return true;
}

int TeMemoryPool::GetAllocSize(void)
{
	return m_iAllocEles * m_iEleSize;
}

int	TeMemoryPool::GetTotalSize(void)
{
	return m_iTotalSize;
}

int	TeMemoryPool::GetAllocMaxEles(void)
{
	return m_iAllocMaxEles;
}

void TeMemoryPool::Clear(void)
{
	SeHeadList* pkNext = NULL;
	for (SeHeadList* pkCur = m_kHeadList.pkNext; pkCur != &m_kHeadList; pkCur = pkNext)
	{
		pkNext = pkCur->pkNext;
		delete (char*)pkCur;
		pkCur = NULL;
	}
	Reset();
}

void TeMemoryPool::RegisterAllocCallBack(NotifyMempoolAlloc fCallBack)
{
	m_fAllocCallBack = fCallBack;
}
