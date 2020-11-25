#pragma once

class NeRecvBuffer
{
public:
	NeRecvBuffer(void);
	~NeRecvBuffer(void);

	int PushData(char* pcData,int iSize);
	char* GetData(int iSize);

	void Clear(void);

protected:
	SeLock	m_kLock;
	int		m_iSize;
	char*	m_pcBuffer;
	int		m_iReadOffset;
	int		m_iWriteOffset;
	char*	m_pcGetBuffer;
};