#pragma once
#include "EeFileMemory.h"

class TeMemExcerption
{
public:
	TeMemExcerption(const char* pcName,int iSize) : 
	  m_pcName(pcName), m_iBufferSize(iSize) {}
	int         GetRequestSize() const { return m_iBufferSize; }
	const char* GetFileName() const { return m_pcName; }
protected:
	int         m_iBufferSize;
	const char* m_pcName;
};

class FilePackage
{
public:
	FilePackage(void);
	~FilePackage(void);

	bool Initialize(const char* pcRootDir,bool bLocal = false);
	void Finialize(void);

	bool Load(const char* pcFileName,FileMemory& rkMemory);
	bool IsExistFile(const char* pcFileName);

protected:
	void* m_pkData;
};

class FileLoader
{
public:

	//	目前只处理xml的加载
	static bool LoadTableFile(const char* pcFileName, FileMemory& rkMemory);

	//	常规加载
	static bool LoadFile(const char* pcFileName, FileMemory& rkMemory);
};