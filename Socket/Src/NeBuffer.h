#pragma once

#include "TeNetMgr.h"

class NePack
{
public:
	int		iSize;
	int		iAddSize;
	char*	pcData;
};

enum class TeLenType
{
	TLT_16BIT_WIN	= 0	,
	TLT_32BIT_WIN		,
	TLT_16BIT_LINUX		,
	TLT_32BIT_LINUX		,
};

class NeBuffer
{
public:
	NeBuffer(void);
	~NeBuffer(void);

	void	InitBuffer(int iCapacity, TeSockFlag iFlag);
	void	ReleaseBuffer(void);
	bool	PushData(char* pcData,int iSize);
	bool	Append(int iSize);
	bool	GetPack(int iPos,NePack& rkPack);
	void	Retract(int iSize);

	char* GetBuffer(void)
	{
		return m_pcData;
	}

	int GetSize(void)
	{
		return m_iSize;
	}

	char* GetFreeBuffer(void)
	{
		return m_pcData + m_iSize;
	}

	int GetFreeSize(void)
	{
		int iLen = m_iCapacity - m_iSize;
		if (iLen <= 0)
		{
			return 0;
		}
		return iLen;
	}

	void Clear(void)
	{
		m_iSize = 0;
	}

protected:
    TeLenType		m_iLenType;
	int		m_iPSize;
	int		m_iCapacity;
	int		m_iSize;
	char*	m_pcData;
};