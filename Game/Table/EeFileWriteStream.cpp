#include "EeFileWriteStream.h"

#include <iostream>

FileWriteStream::FileWriteStream(void)
{
	m_pkFile = nullptr;
}

FileWriteStream::~FileWriteStream(void)
{
	if(m_pkFile)
	{
		fclose(m_pkFile);
	}
}

bool FileWriteStream::Attach(const char* pcFileName)
{
#ifdef _WIN32
    if(m_pkFile)
	{
		fclose(m_pkFile);
	}

	fopen_s(&m_pkFile,pcFileName, "wb");
	if(!m_pkFile)
	{
		return false;
	}
#else
    if (m_pkFile)
    {
        fclose(m_pkFile);
    }
    m_pkFile = fopen(pcFileName, "wb");
    if (!m_pkFile)
    {
        return false;
    }
#endif

	return true;
}

int FileWriteStream::GetPos(void)
{
#ifdef _WIN32
	if(m_pkFile)
	{
		__int64 iCurPos;
		fgetpos(m_pkFile,&iCurPos);
		return (int)iCurPos;
	}
	else
	{
		return -1;
	}
#else
    if (m_pkFile)
    {
        fpos_t iCurPos;
        fgetpos(m_pkFile, &iCurPos);
        return (int)iCurPos;
    }
#endif
    return -1;
}

void FileWriteStream::SetPos(int iPos)
{
	if(m_pkFile)
	{
		fseek(m_pkFile,iPos,SEEK_SET);
	}
}

bool FileWriteStream::Write(void* pkData,int iSize)
{
	if(m_pkFile)
	{
		fwrite(pkData,iSize,1,m_pkFile);
		return true;
	}
	return false;
}

bool FileWriteStream::WriteChar(char cValue)
{
	return Write(&cValue,sizeof(char));
}

bool FileWriteStream::WriteByte(unsigned char byValue)
{
	return Write(&byValue, sizeof(unsigned char));
}

bool FileWriteStream::WriteInt(int iValue)
{
	return Write(&iValue,sizeof(int));
}

bool FileWriteStream::WriteFloat(float fValue)
{
	return Write(&fValue,sizeof(float));
}

bool FileWriteStream::WriteWord(unsigned short wValue)
{
	return Write(&wValue, sizeof(unsigned short));
}

bool FileWriteStream::WriteDWord(unsigned long dwValue)
{
	return Write(&dwValue, sizeof(unsigned long));
}

bool FileWriteStream::WriteString(const char* pcString)
{
	if(!m_pkFile)
	{
		return false;
	}

	int iLen = (int)strlen(pcString);
	WriteInt(iLen);
	if(iLen > 0)
	{
		Write((void *)pcString,iLen);
	}
	return true;
}