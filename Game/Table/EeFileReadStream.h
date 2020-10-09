#pragma once

#include "EeFileMemory.h"

//#ifndef _WIN32
//typedef long UInt64;
//#endif

class FileReadStream
{
public:
	FileReadStream();
	~FileReadStream();

	bool	Attach(FileMemory& rkMemory, bool bCopy = false);

	bool	Eof(void);
	int		GetPos(void);
	void	SetPos(int iPos);
	void	GoPos(int iOff);
	char*	GetCurBuf(void);

	bool	Read(void* pkVoid, int iSize);
	char	ReadChar(void);
	unsigned char	ReadByte(void);
	int		ReadInt(void);
	long	ReadInt64(void);
	float	ReadFloat(void);
	unsigned short	ReadWord(void);
	unsigned long	ReadDWord(void);
	char*	ReadString(void);
	char*	ReadLineString(void);

protected:
	char*	m_pcData;
	int		m_iSize;
	int		m_iPos;
	bool	m_bCopy;

	char*	m_acString;
};