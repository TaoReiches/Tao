#include "TeInclude.h"
#include "NeBuffer.h"
#include "TeNetMgr.h"

#ifdef  _VISION_ANDROID
#include <sys/types.h>  
#include <sys/socket.h>  

  
#include <sys/un.h>  

 
#include <netinet/in.h>  
  
#include <arpa/inet.h>  
  
#include <unistd.h>  
 
#include <stddef.h>  

#endif

NeBuffer::NeBuffer(void)
{
	m_iLenType = TLT_16BIT_WIN;
	m_iPSize = 2;
	m_iCapacity = 0;
	m_iSize = 0;
	m_pcData = NULL;
}

NeBuffer::~NeBuffer(void)
{
	SAFE_ARRAY_DELETE(m_pcData);
}

void NeBuffer::InitBuffer(int iCapacity,int iFlag)
{
	if((iFlag & TSF_32BIT_WIN))
	{
		m_iLenType = TLT_32BIT_WIN;
		m_iPSize = 4;
	}
	else if((iFlag & TSF_16BIT_LINUX))
	{
		m_iLenType = TLT_16BIT_LINUX;
		m_iPSize = 2;
	}
	else if((iFlag & TSF_32BIT_LINUX))
	{
		m_iLenType = TLT_32BIT_LINUX;
		m_iPSize = 4;
	}
	else
	{
		m_iLenType = TLT_16BIT_WIN;
		m_iPSize = 2;
	}

	m_iSize = 0;

	if(!m_pcData || iCapacity != m_iCapacity)
	{
		SAFE_ARRAY_DELETE(m_pcData);
		m_pcData = new char[iCapacity];
		m_iCapacity = iCapacity;
	}
}

void NeBuffer::ReleaseBuffer(void)
{
	m_iLenType = TLT_16BIT_WIN;
	m_iPSize = 2;
	m_iSize = 0;

	if(m_iCapacity >= 128 * 1024)
	{
		m_iCapacity = 0;
		SAFE_ARRAY_DELETE(m_pcData);
	}
}

bool NeBuffer::PushData(char* pcData,int iSize)
{
	if(iSize < 0)
	{
		return false;
	}

	int iNewSize = m_iSize + m_iPSize + iSize;
	if(iNewSize > m_iCapacity)
	{
		return false;
	}

	//	flash回发安全策略  特殊处理
	if (*pcData == '<' && *(pcData + 1) == '?' &&  *(pcData + 2) == 'x' &&  *(pcData + 3) == 'm' &&  *(pcData + 4) == 'l')
	//if (*((int*)pcData) == 1010792557)		//	'<?xm'
	{
		memcpy(m_pcData + m_iSize, pcData, iSize);
		*(m_pcData + m_iSize + iSize) = '\0';
		iSize++;		
		m_iSize = iSize;
		return true;
	}

	if(m_iLenType == TLT_16BIT_LINUX)
	{
		unsigned short wLen = htons((unsigned short)iSize);
		memcpy(m_pcData + m_iSize,&wLen,m_iPSize);
	}
	else if(m_iLenType == TLT_16BIT_LINUX)
	{
		unsigned int dwLen = htonl((unsigned int)iSize);
		memcpy(m_pcData + m_iSize,&dwLen,m_iPSize);
	}
	else
	{
		memcpy(m_pcData + m_iSize,&iSize,m_iPSize);
	}
	memcpy(m_pcData + m_iSize + m_iPSize,pcData,iSize);

	m_iSize = iNewSize;
	return true;
}

bool NeBuffer::Append(int iSize)
{
	// 非法的数据格式(不应该出现)
	if(iSize <= 0 || (m_iSize + iSize) > m_iCapacity)
	{
		return false;
	}

	m_iSize += iSize;
	return true;
}

bool NeBuffer::GetPack(int iPos,NePack& rkPack)
{
	memset(&rkPack,0,sizeof(NePack));

	// 接收的大小不够表示长度的字节数
	int iSize = m_iSize - iPos;
	if(m_pcData == NULL || iSize < m_iPSize )
	{
		return false;
	}
	iSize -= m_iPSize;

	// 数据流错误了(不应该出现)
	int iNeedSize = 0;
	memcpy(&iNeedSize,m_pcData + iPos,m_iPSize);
	//	flash的安全策略会系统发一个 需要特殊处理
	if (iNeedSize == 28732)
	{
		iNeedSize = m_iSize;
		iSize += m_iPSize;
		m_iPSize = 0;		
	}
	if(TLT_16BIT_LINUX == m_iLenType)
	{
		iNeedSize = ntohs((unsigned short)iNeedSize);
	}
	else if(TLT_32BIT_LINUX == m_iLenType)
	{
		iNeedSize = ntohl((unsigned int)iNeedSize);
	}
	if(iNeedSize <= 0)
	{
		Clear();
		return false;
	}

	// 数据包已经完整了
	if(iNeedSize <= iSize)
	{
		rkPack.iSize = iNeedSize;
		rkPack.pcData = m_pcData + iPos + m_iPSize;
		rkPack.iAddSize = iNeedSize + m_iPSize;
		return true;
	}

	// 数据包还未完整
	return false;
}

void NeBuffer::Retract(int iSize)
{
	// 异常情况下清空(不应该出现)
	if(iSize <= 0 || iSize > m_iSize)
	{
		Clear();
		return;
	}

	m_iSize -= iSize;
	if(m_iSize > 0)
	{
		memmove(m_pcData,m_pcData + iSize,m_iSize);
	}
}