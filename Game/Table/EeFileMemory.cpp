#include "EeFileMemory.h"
#include <iostream>
#include <string.h>

FileMemory::FileMemory(void)
{
	m_pcData = NULL;
	m_iSize = 0;
}

FileMemory::~FileMemory(void)
{
	Clear();
}

void FileMemory::Clear()
{
	if(m_pcData)
	{
		delete[] m_pcData;
		m_pcData = NULL;
	}

	m_iSize = 0;
}

bool FileMemory::ReSize(int iNewSize)
{
	Clear();

	if (iNewSize < 0)
	{
		return false;
	}

	m_pcData = new char[iNewSize + 1];
	if (m_pcData == NULL)
	{
		return false;
	}

	m_iSize = iNewSize;
	memset(m_pcData, 0, iNewSize + 1);

	return true;
}

char* FileMemory::GetData() const
{
	return m_pcData;
}

int FileMemory::GetSize() const
{
	return m_iSize;
}

FileMemory& FileMemory::operator =(const FileMemory& rkRight)
{
	ReSize(rkRight.m_iSize);
	memcpy(m_pcData, rkRight.m_pcData, m_iSize);

	return (*this);
}