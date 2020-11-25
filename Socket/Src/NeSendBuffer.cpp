#include "TeInclude.h"
#include "NeNetDef.h"
#include "NeSendBuffer.h"

NeSendBuffer::NeSendBuffer(void)
{
	m_iCapacity = 0;
	m_iReadOffset = 0;
	m_iWriteOffset = 0;
	m_pcData = NULL;
}

NeSendBuffer::~NeSendBuffer(void)
{
	SAFE_ARRAY_DELETE(m_pcData);
}

bool NeSendBuffer::PushData(char* pcData,int iSize)
{
	if(iSize >= MAX_PACK_SIZE)
	{
		return false;
	}

	if(!m_pcData)
	{
		m_iCapacity = MAX_PACK_SIZE;
		m_iReadOffset = 0;
		m_iWriteOffset = 0;
		m_pcData = new char[m_iCapacity];
	}

	int iNeedSize = iSize + sizeof(unsigned short);
	if(m_iWriteOffset + iNeedSize > m_iCapacity)
	{
		if(m_iCapacity > 8 * MAX_PACK_SIZE)
		{
			return false;
		}
		char* pcOld = m_pcData;
		m_iCapacity += MAX_PACK_SIZE;
		m_pcData = new char[m_iCapacity];
		memcpy(m_pcData,pcOld,m_iWriteOffset);
		SAFE_ARRAY_DELETE(pcOld);
	}

	memcpy(m_pcData + m_iWriteOffset, &iSize, sizeof(unsigned short));
	m_iWriteOffset += sizeof(unsigned short);
	memcpy(m_pcData + m_iWriteOffset,pcData,iSize);
	m_iWriteOffset += iSize;

	return true;
}

NeSendPack* NeSendBuffer::GetData(void)
{
	if(!m_pcData || m_iReadOffset >= m_iWriteOffset || m_iReadOffset >= m_iCapacity)
	{
		return NULL;
	}

	return (NeSendPack *)(m_pcData + m_iReadOffset);
}

void NeSendBuffer::PopData(void)
{
	if(!m_pcData || m_iReadOffset >= m_iWriteOffset || m_iReadOffset >= m_iCapacity)
	{
		return;
	}

	NeSendPack* pkPack = (NeSendPack *)(m_pcData + m_iReadOffset);
	m_iReadOffset += pkPack->wSize + sizeof(unsigned short);

	// 全部取完了的话就进行删除操作
	if(m_iReadOffset >= m_iWriteOffset)
	{
		m_iCapacity = 0;
		m_iReadOffset = 0;
		m_iWriteOffset = 0;
		SAFE_ARRAY_DELETE(m_pcData);
	}
}

void NeSendBuffer::Retract(void)
{
	if(!m_pcData || m_iReadOffset >= m_iWriteOffset || m_iReadOffset >= m_iCapacity)
	{
		return;
	}

	memmove(m_pcData,m_pcData + m_iReadOffset,m_iWriteOffset - m_iReadOffset);
	m_iWriteOffset -= m_iReadOffset;
	m_iReadOffset = 0;
}

void NeSendBuffer::ReleaseBuffer(void)
{
	m_iCapacity = 0;
	m_iReadOffset = 0;
	m_iWriteOffset = 0;
	SAFE_ARRAY_DELETE(m_pcData);
}