#pragma once

struct NePack
{
	int		iSize;
	int		iAddSize;
	char*	pcData;
};

// 某个连接的属性
enum TeLenType
{
	TLT_16BIT_WIN	= 0	,	// 16位长度的win系统
	TLT_32BIT_WIN		,	// 32位长度的win系统
	TLT_16BIT_LINUX		,	// 16位长度的linux系统
	TLT_32BIT_LINUX		,	// 32位长度的linux系统
};

class NeBuffer
{
public:
	NeBuffer(void);
	~NeBuffer(void);

	void	InitBuffer(int iCapacity,int iFlag);
	void	ReleaseBuffer(void);
	bool	PushData(char* pcData,int iSize);
	bool	Append(int iSize);
	bool	GetPack(int iPos,NePack& rkPack);
	void	Retract(int iSize);


	inline char* GetBuffer(void)
	{
		return m_pcData;
	}

	inline int GetSize(void)
	{
		return m_iSize;
	}

	inline char* GetFreeBuffer(void)
	{
		return m_pcData + m_iSize;
	}

	inline int GetFreeSize(void)
	{
		int iLen = m_iCapacity - m_iSize;
		if (iLen <= 0)
		{
			return 0;
		}
		return iLen;
	}

	inline void Clear(void)
	{
		m_iSize = 0;
	}

protected:
	int		m_iLenType;
	int		m_iPSize;
	int		m_iCapacity;
	int		m_iSize;
	char*	m_pcData;
};