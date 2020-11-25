#include "TeInclude.h"
#include "NeNetDef.h"
#include "NeRecvBuffer.h"

#define ADD_BUF_SIZE (512 * 1024)


NeRecvBuffer::NeRecvBuffer(void)
{
	m_iSize = 2 * 1024 * 1024;
	m_pcBuffer = new char[m_iSize];
	m_iReadOffset = 0;
	m_iWriteOffset = 0;
	m_pcGetBuffer = new char[MAX_PACK_SIZE];
}

NeRecvBuffer::~NeRecvBuffer(void)
{
	delete []m_pcBuffer;
	delete []m_pcGetBuffer;
}

int NeRecvBuffer::PushData(char* pcData,int iSize)
{
	if(iSize >= MAX_PACK_SIZE)
	{
		return 0;
	}

	m_kLock.Lock();

	if(m_iWriteOffset + iSize > m_iSize)
	{
		char* pcOld = m_pcBuffer;
		m_iSize += ADD_BUF_SIZE;
		m_pcBuffer = new char[m_iSize];
		memcpy(m_pcBuffer,pcOld,m_iWriteOffset);
		delete [] pcOld;
	}
	memcpy(m_pcBuffer + m_iWriteOffset,pcData,iSize);
	m_iWriteOffset += iSize;

	m_kLock.UnLock();

	return iSize;
}

char* NeRecvBuffer::GetData(int iSize)
{
	if(iSize <= 0 || iSize >= MAX_PACK_SIZE)
	{
		return NULL;
	}

	m_kLock.Lock();
	int iReadOffset = m_iReadOffset + iSize;
	if(iReadOffset > m_iWriteOffset)
	{
		// 这种情况不应该出现
		m_iReadOffset = 0;
		m_iWriteOffset = 0;
		m_kLock.UnLock();
		return NULL;
	}

	memcpy(m_pcGetBuffer,m_pcBuffer + m_iReadOffset,iSize);
	m_iReadOffset += iSize;

	if(m_iReadOffset == m_iWriteOffset)
	{
		// 数据都取完了
		m_iReadOffset = 0;
		m_iWriteOffset = 0;
	}
	else if(m_iReadOffset > (ADD_BUF_SIZE * 4))
	{
		memmove(m_pcBuffer,m_pcBuffer + m_iReadOffset,m_iWriteOffset - m_iReadOffset);
		m_iWriteOffset -= m_iReadOffset;
		m_iReadOffset = 0;
	}

	m_kLock.UnLock();
	return m_pcGetBuffer;
}

void NeRecvBuffer::Clear(void)
{
	m_kLock.Lock();
	m_iReadOffset = 0;
	m_iWriteOffset = 0;
	m_kLock.UnLock();
}