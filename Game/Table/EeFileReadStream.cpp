#include "EeFileReadStream.h"

#include <iostream>

#define TOOL_MAX_STRING	(8 * 1024)

FileReadStream::FileReadStream()
{
	m_pcData = nullptr;
	m_iSize	= 0;
	m_iPos = 0;
	m_bCopy = false;

	m_acString = new char[TOOL_MAX_STRING];
}

FileReadStream::~FileReadStream()
{
	if(m_bCopy)
	{
		//SAFE_ARRAY_DELETE(m_pcData);
		if(m_pcData)
		{
			delete[] m_pcData;
			m_pcData = nullptr;
		}
	}

	//SAFE_ARRAY_DELETE(m_acString);
	if(m_acString)
	{
		delete[] m_acString;
		m_acString = nullptr;
	}
}

bool FileReadStream::Attach(FileMemory& rkMemory,bool bCopy)
{
	if(m_bCopy)
	{
		//SAFE_ARRAY_DELETE(m_pcData);
		if(m_pcData)
		{
			delete[] m_pcData;
			m_pcData = nullptr;
		}
	}

	m_bCopy = false;
	m_iPos = 0;
	m_iSize = rkMemory.GetSize();
	if(m_iSize <= 0)
	{
		return false;
	}

	m_bCopy = bCopy;
	if(bCopy)
	{
		m_pcData = new char[m_iSize];
		memcpy(m_pcData,rkMemory.GetData(),m_iSize);
	}
	else
	{
		m_pcData = rkMemory.GetData();
	}
	return true;
}

bool FileReadStream::Eof(void)
{
	if(m_iPos >= m_iSize)
	{
		return true;
	}
	return false;
}

int FileReadStream::GetPos(void)
{
	return m_iPos;
}

void FileReadStream::SetPos(int iPos)
{
	if(iPos >= m_iSize)
	{
		m_iPos = m_iSize;
	}
	else
	{
		m_iPos = iPos;
	}
}

void FileReadStream::GoPos(int iOff)
{
	if(m_iPos + iOff >= m_iSize)
	{
		m_iPos = m_iSize;
	}
	else
	{
		m_iPos += iOff;
	}
}

char* FileReadStream::GetCurBuf(void)
{
	return &m_pcData[m_iPos];
}

bool FileReadStream::Read(void* pkVoid,int iSize)
{
	if(iSize <= 0)
	{
		return false;
	}

	if(m_iPos + iSize > m_iSize)
	{
		return false;
	}

	memcpy(pkVoid,m_pcData + m_iPos,iSize);
	m_iPos += iSize;
	return true;
}

char FileReadStream::ReadChar(void)
{
	char cTemp = 0;
	Read(&cTemp,1);
	return cTemp;
}

unsigned char FileReadStream::ReadByte(void)
{
	unsigned char byTemp = 0;
	Read(&byTemp,1);
	return byTemp;
}

int FileReadStream::ReadInt(void)
{
	int iTemp = 0;
	Read(&iTemp,4);
	return iTemp;
}

long FileReadStream::ReadInt64(void)
{
	long iTemp = 0;
	Read(&iTemp, sizeof(long));
	return iTemp;
}

float FileReadStream::ReadFloat(void)
{
	float fTemp = 0.0f;
	Read(&fTemp,4);
	return fTemp;
}

unsigned short FileReadStream::ReadWord(void)
{
	unsigned short wTemp = 0;
	Read(&wTemp,2);
	return wTemp;
}

unsigned long FileReadStream::ReadDWord(void)
{
	unsigned long dwTemp = 0;
	Read(&dwTemp,4);
	return dwTemp;
}

char* FileReadStream::ReadString(void)
{
	int iLen = ReadInt();
	if(iLen > 0 && iLen < TOOL_MAX_STRING)
	{
		Read(m_acString,iLen);
		m_acString[iLen] = '\0';
	}
	else
	{
		m_acString[0] = '\0';
	}
	return m_acString;
}

char* FileReadStream::ReadLineString(void)
{
	m_acString[0] = '\0';
	if(m_iPos >= m_iSize)
	{
		return m_acString;
	}

	char* pcBegin = &m_pcData[m_iPos];
	char* pcFinally = m_pcData + m_iSize;
	char* pcEnd = pcBegin;
	while(pcEnd <= pcFinally)
	{
		if (*pcEnd == 0 || (unsigned char)(*pcEnd) == 0x0D || (unsigned char)(*pcEnd) == 0x0A)
		{
			if(pcEnd > pcBegin && pcBegin + TOOL_MAX_STRING > pcEnd)
			{
				memcpy(m_acString,pcBegin,pcEnd - pcBegin);
				m_acString[pcEnd - pcBegin] = '\0';
			}

			if ((unsigned char)(*pcEnd) == 0x0D && (unsigned char)(*(pcEnd + 1)) == 0x0A)
			{
				pcEnd += 2;
			}
			else
			{
				pcEnd++;
			}
			break;
		}
		pcEnd++;
	}

	m_iPos += (int)(pcEnd - pcBegin);

	return m_acString;
}