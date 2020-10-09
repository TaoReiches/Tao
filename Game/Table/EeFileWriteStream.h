#pragma once

#include <stdio.h>

class FileWriteStream
{
public:
	FileWriteStream(void);
	~FileWriteStream(void);

	bool	Attach(const char* pcFileName);

	int		GetPos(void);
	void	SetPos(int iPos);

	bool	Write(void* pkData,int iSize);
	bool	WriteChar(char cValue);
	bool	WriteByte(unsigned char byValue);
	bool	WriteInt(int iValue);
	bool	WriteFloat(float fValue);
	bool	WriteWord(unsigned short wValue);
	bool	WriteDWord(unsigned long dwValue);
	bool	WriteString(const char* pcString);

protected:
	FILE*	m_pkFile;
};