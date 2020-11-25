#pragma once

struct NeSendPack
{
	unsigned short	wSize;
	char			acData[2];
};

class NeSendBuffer
{
public:
	NeSendBuffer(void);
	~NeSendBuffer(void);

	bool PushData(char* pcData,int iSize);
	NeSendPack* GetData(void);
	void PopData(void);
	void Retract(void);
	void ReleaseBuffer(void);

protected:
	int		m_iCapacity;
	int		m_iReadOffset;
	int		m_iWriteOffset;
	char*	m_pcData;
};