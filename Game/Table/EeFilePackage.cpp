#include "EeFilePackageDef.h"
#include "EeFilePackage.h"
//#include "TeCompress.h"
//#include "TeTool.h"

//#define PACKAGE_LICENSE_CHECK()	SYSTEMTIME kInitTime;\
//								GetLocalTime(&kInitTime);\
//								if(kInitTime.wYear > LICENSE_YEAR || (kInitTime.wYear == LICENSE_YEAR && kInitTime.wMonth > LICENSE_MONTH))\
//								{\
//									return false;\
//								}
//
#define DATA_CONFERENCE() FilePackageData& rkData = *((FilePackageData *)m_pkData)

#ifdef _WIN32
#include <Windows.h>
#include <stdlib.h>
#else
#include <stdio.h>
#include <string.h>

#ifndef __LINUX__
#include <Vision/Runtime/Engine/System/Vision.hpp>
#endif

#endif
//////////////////////////////////////////////////////////////////////////
FilePackageData::FilePackageData()
{
	bInitSuccess = false;
	bMustPackage = false;
	hPackageFile = NULL;
	iBufferSize = 0;
	pcBuffer = NULL;
}

bool FilePackageData::Read(void* pkVoid,int iSize)
{
#ifdef _WIN32
	if (hPackageFile == INVALID_HANDLE_VALUE)
#else
    if (hPackageFile == NULL)
#endif
	{
		return false;
	}

	int iReadSize = 0;
#ifdef _WIN32
	if(!ReadFile(hPackageFile,pkVoid,iSize,(LPDWORD)&iReadSize,NULL))
#else
    if ((iReadSize = fread(pkVoid, iSize, 1, (FILE*)hPackageFile)) <= 0)
#endif
	{
		return false;
	}

	if(iReadSize != iSize)
	{
		return false;
	}

	return true;
}

void FilePackageData::Seek(int iPos)
{
#ifdef _WIN32
	if(hPackageFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hPackageFile,iPos,NULL,FILE_BEGIN);
	}
#else
    if (hPackageFile != NULL)
    {
        fseek((FILE*)hPackageFile, iPos, SEEK_SET);
    }
#endif
}

void FilePackageData::GoPos(int iOff)
{
#ifdef _WIN32
	if(hPackageFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hPackageFile,iOff,NULL,FILE_CURRENT);
	}
#else
    if (hPackageFile != NULL)
    {
        fseek((FILE*)hPackageFile, iOff, SEEK_CUR);
    }
#endif
}

void FilePackageData::FilterFileName(char* acFile)
{
	char* pcStr = acFile;
	while(*pcStr)
	{
		if(*pcStr == '/')
		{
			*pcStr = '\\';
		}
		else if(*pcStr >= 'A' && *pcStr <= 'Z')
		{
			*pcStr += 'a' - 'A';
		}
		pcStr++;
	}

	if(*acFile == '.')
	{
		memmove(acFile,acFile + 1,strlen(acFile));
	}
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
FilePackage::FilePackage(void)
{
	m_pkData = new FilePackageData;
	DATA_CONFERENCE();

	rkData.iBufferSize = 6 * 1024 * 1024;
	rkData.pcBuffer = new char[rkData.iBufferSize];
}

FilePackage::~FilePackage(void)
{
	Finialize();
}

bool FilePackage::Initialize(const char* pcRootDir,bool bLocal)
{
	DATA_CONFERENCE();

	bLocal = false;
//	PACKAGE_LICENSE_CHECK();

//	rkData.kAllFile.clear();

	if(pcRootDir)
	{
		rkData.kRootDir = pcRootDir;
	}
	else
	{
		rkData.kRootDir = ".\\";
	}

	char acPackageFile[512];
    
#ifdef _WIN32
    sprintf_s(acPackageFile, sizeof(acPackageFile), "%sData\\Data.dhp", rkData.kRootDir.c_str());
#else
    snprintf(acPackageFile, sizeof(acPackageFile),"%sData\\Data.dhp",rkData.kRootDir.c_str());
#endif

	//////////////////////////////////////////////////////////////////////////
	// 打开文件

	if(!bLocal)
	{
#ifdef _WIN32
		rkData.hPackageFile = CreateFile(acPackageFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
#elif __LINUX__
		rkData.hPackageFile = fopen(acPackageFile,"r");
#else
        VFileAccessManager::NativePathResult nativePathResult;
        if (VFileAccessManager::GetInstance()->MakePathNative(acPackageFile, nativePathResult, VFileSystemAccessMode::READ, VFileSystemElementType::FILE) == HKV_FAILURE)
        {
            rkData.hPackageFile = NULL;
        } 
		else
        {
            const char* absolutePath = nativePathResult.m_sNativePath.AsChar();
            FILE* hp = fopen(absolutePath, "rb");
            rkData.hPackageFile = hp;
        }
#endif
	}
	else
	{
#ifdef _WIN32
		rkData.hPackageFile = INVALID_HANDLE_VALUE;
#else
        rkData.hPackageFile = NULL;
#endif
	}

#ifdef _WIN32
	if(rkData.hPackageFile == INVALID_HANDLE_VALUE)
#else
    if (rkData.hPackageFile == NULL)
#endif
    {
		if(rkData.bMustPackage)
		{
			return false;
		}
		rkData.bInitSuccess = true;
		return true;
	}
    return true;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// DHP文件头
	//TeDhpHeader kDhpHeader;
	//memset(&kDhpHeader,0,sizeof(TeDhpHeader));
	//if(!rkData.Read(&kDhpHeader,sizeof(TeDhpHeader)))
	//{
	//	goto LOAD_FAILED;
	//}
	//if(kDhpHeader.iTagment != 'DHP1' || kDhpHeader.iMD5FileOff <= 0 || kDhpHeader.iFileInfoOff <= 0)
	//{
	//	goto LOAD_FAILED;
	//}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//// 读取MD5文件头
	//rkData.Seek(kDhpHeader.iMD5FileOff);
	//TeMD5FileHeader kMD5Header;
	//memset(&kMD5Header,0,sizeof(TeMD5FileHeader));
	//if(!rkData.Read(&kMD5Header,sizeof(TeMD5FileHeader)))
	//{
	//	goto LOAD_FAILED;
	//}
	//if(kMD5Header.iTagment != 'MD5H' || kMD5Header.iMD5FileNum <= 0 || kMD5Header.iMD5FileSize <= 0)
	//{
	//	goto LOAD_FAILED;
	//}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//// 读取文件信息头
	//rkData.Seek(kDhpHeader.iFileInfoOff);
	//TeFileInfoHeader kFileInfoHeader;
	//memset(&kFileInfoHeader,0,sizeof(TeFileInfoHeader));
	//if(!rkData.Read(&kFileInfoHeader,sizeof(TeFileInfoHeader)))
	//{
	//	goto LOAD_FAILED;
	//}
	//if(kFileInfoHeader.iTagment != 'FINF' || kFileInfoHeader.iInfoNum <= 0 || kFileInfoHeader.iInfoSize <= 0 || kFileInfoHeader.iCmprType != CL_DEFAULT_COMPRESS)
	//{
	//	goto LOAD_FAILED;
	//}

	//// 解压缩文件信息头
	//char* pcFileInfo = new char[kFileInfoHeader.iInfoSize];
	//if(!rkData.Read(pcFileInfo,kFileInfoHeader.iInfoSize))
	//{
	//	delete []pcFileInfo;
	//	goto LOAD_FAILED;
	//}

	//// 重新分配缓冲
	//int iDstSize = kFileInfoHeader.iInfoOrgSize + 256;
	//if(iDstSize > rkData.iBufferSize)
	//{
	//	delete []rkData.pcBuffer;
	//	rkData.iBufferSize = iDstSize;
	//	rkData.pcBuffer = new char[rkData.iBufferSize];
	//}

	//char* pcDstData = rkData.pcBuffer;
	//memset(pcDstData,0,iDstSize);
	//if(!Uncompress((unsigned short *)pcDstData,(unsigned int *)&iDstSize,(unsigned short *)pcFileInfo,(unsigned int)kFileInfoHeader.iInfoSize) || (kFileInfoHeader.iInfoOrgSize != iDstSize))
	//{
	//	delete []pcFileInfo;
	//	goto LOAD_FAILED;
	//}
	//delete []pcFileInfo;

	//// 解析文件信息头
	//int iSize = 0;
	//for(int i = 0;i < kFileInfoHeader.iInfoNum;i++)
	//{
	//	// 文件名长度
	//	if((iSize + 4) > iDstSize)
	//	{
	//		goto LOAD_FAILED;
	//	}
	//	int iLen;
	//	memcpy(&iLen,&pcDstData[iSize],4);
	//	iSize += 4;

	//	// 取得文件名
	//	char acFileName[512];
	//	if((iSize + iLen) > iDstSize || iLen >= sizeof(acFileName))
	//	{
	//		goto LOAD_FAILED;
	//	}
	//	memcpy(acFileName,&pcDstData[iSize],iLen);
	//	acFileName[iLen] = '\0';
	//	iSize += iLen;

	//	// 文件在整个包文件的偏移
	//	if((iSize + 4) > iDstSize)
	//	{
	//		goto LOAD_FAILED;
	//	}
	//	TeFileInfo kFileInfo;
	//	memcpy(&kFileInfo.iDataOff,&pcDstData[iSize],4);
	//	iSize += 4;

	//	// 文件压缩后的MD5码
	//	if((iSize + 16) > iDstSize)
	//	{
	//		goto LOAD_FAILED;
	//	}
	//	memcpy(kFileInfo.kCmprMD5.abyMD5,&pcDstData[iSize],16);
	//	iSize += 16;

	//	rkData.kAllFile[acFileName] = kFileInfo;
	//}
	//////////////////////////////////////////////////////////////////////////

	//rkData.bInitSuccess = true;
	//return true;

//LOAD_FAILED:
//	CloseHandle(rkData.hPackageFile);
//	rkData.bInitSuccess = false;
//	rkData.hPackageFile = INVALID_HANDLE_VALUE;
//	return false;
}

void FilePackage::Finialize(void)
{
	FilePackageData* pkData = (FilePackageData *)m_pkData;
	if(pkData)
	{
#ifdef _WIN32
		if(pkData->hPackageFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(pkData->hPackageFile);
		}
#else
        if (pkData->hPackageFile != NULL)
        {
            fclose((FILE*)pkData->hPackageFile);
        }
#endif
		delete []pkData->pcBuffer;

		delete pkData;
		m_pkData = NULL;
	}
}

bool FilePackage::Load(const char* pcFileName, FileMemory& rkMemory)
{
	DATA_CONFERENCE();

	// 是否已经初始化成功
	if (!rkData.bInitSuccess)
	{
		return false;
	}

	// 查看文件名是否合法
	if (!pcFileName || strlen(pcFileName) <= 4)
	{
		return false;
	}

	// 没有包文件就使用本地文件
#ifdef _WIN32
	if (rkData.hPackageFile == INVALID_HANDLE_VALUE)
#else
	if (rkData.hPackageFile == NULL)
#endif
	{
		if (rkData.bMustPackage)
		{
			return false;
		}
//#if defined(_WIN32) || defined(__LINUX__)
//#ifdef _WIN32
		if(*pcFileName == '.')
		{
			pcFileName++;
		}

		if(*pcFileName == '\\' || *pcFileName == '/')
		{
			pcFileName++;
		}

		char acFullName[512];
#ifdef _WIN32
		sprintf_s(acFullName, sizeof(acFullName), "%s%s", rkData.kRootDir.c_str(), pcFileName);
#else
		snprintf(acFullName, sizeof(acFullName), "%s%s", rkData.kRootDir.c_str(), pcFileName);
#endif
        
        FILE* fp = NULL;

#ifdef _WIN32
		fopen_s(&fp, acFullName, "rb");
#else
		fp = fopen(acFullName, "rb");
#endif      
        
        if (!fp)
        {
            return false;
        }
        
        if(fseek(fp,0,SEEK_END))
        {
            fclose(fp);
            return false;
        }
        
        int iSize = ftell(fp);
        try
        {
            if(!rkMemory.ReSize(iSize))
            {
                fclose(fp);
                return false;
            }
        }
        catch (const std::bad_alloc& ec)
        {
            fclose(fp);
            throw TeMemExcerption(pcFileName,iSize);
        }
        
        if(fseek(fp,0,SEEK_SET))
        {
            fclose(fp);
            return false;
        }
        fread(rkMemory.GetData(),iSize,1,fp);
        fclose(fp);
        return true;
//#else
//        if (!VFileAccessManager::GetInstance()->FileExists(pcFileName))
//        {
//            return false;
//        }
//        IVFileInStream* pStream = VFileAccessManager::GetInstance()->Open(pcFileName);
//        if (pStream == NULL)
//        {
//            return false;
//        }
//        unsigned int uiSize = pStream->GetSize();
//        hkvHybridArray<char, 4096 > tempBuffer;
//        tempBuffer.SetSize(uiSize + 1);
//        char* pBuffer = &tempBuffer[0];
//        
//        uiSize = static_cast<unsigned int>(pStream->Read(pBuffer, uiSize));
//        pBuffer[ uiSize ] = '\0';
//        pStream->Close();
//        
//        rkMemory.ReSize(uiSize + 1);
//        memcpy(rkMemory.GetData(), pBuffer, uiSize + 1);
//        return true;
//#endif
		return true;
	}

	//// 包文件是否有文件
	//if(rkData.kAllFile.size() <= 0)
	//{
	//	return false;
	//}

	//// 获取包裹中的文件名
	//char acFile[512];
	//strcpy(acFile,pcFileName);
	//rkData.FilterFileName(acFile);

	//// 查看是否存在包裹中
	//std::map<std::string,TeFileInfo>::iterator itr = rkData.kAllFile.find(acFile);
	//if(itr == rkData.kAllFile.end())
	//{
	//	return false;
	//}

	//TeFileInfo& rkFileInfo = itr->second;

	//TeMD5File kMD5File;
	//rkData.Seek(rkFileInfo.iDataOff);
	//if(!rkData.Read(&kMD5File,sizeof(TeMD5File)))
	//{
	//	return false;
	//}

	//if(kMD5File.iCmprType != CL_NONE_COMPRESS)
	//{
	//	bool bReAlloca = false;
	//	char* pcPrebuffer = rkData.pcBuffer; 
	//	int iPreSize = rkData.iBufferSize;
	//	if(kMD5File.iPkgSize > rkData.iBufferSize)
	//	{
	//		bReAlloca = true;
	//		rkData.iBufferSize = kMD5File.iPkgSize;
	//		try
	//		{
	//			rkData.pcBuffer = new char[rkData.iBufferSize];
	//		}
	//		catch (const std::bad_alloc& ec)
	//		{
	//			throw TeMemExcerption(pcFileName,rkData.iBufferSize);
	//		}

	//		if (rkData.pcBuffer == NULL)
	//		{
	//			MessageBox(NULL, "申请内存失败，请重新启动游戏!!!", NULL, 0);
	//			throw rkData.pcBuffer;
	//			exit(0);
	//		}
	//	}

	//	char* pcSrcData = rkData.pcBuffer;
	//	if(!rkData.Read(pcSrcData,kMD5File.iPkgSize))
	//	{
	//		return false;
	//	}

	//	int iDstSize = kMD5File.iOrgSize;
	//	
	//	try
	//	{
	//		if(!rkMemory.ReSize(iDstSize))
	//		{
	//			return false;
	//		}
	//	}
	//	catch (const std::bad_alloc& ec)
	//	{
	//		throw TeMemExcerption(pcFileName,iDstSize);
	//	}

	//	if(!Uncompress((unsigned short *)rkMemory.GetData(),(unsigned int *)&iDstSize,(unsigned short *)pcSrcData,(unsigned int)kMD5File.iPkgSize) || iDstSize != kMD5File.iOrgSize)
	//	{
	//		if (bReAlloca)
	//		{
	//			delete[] rkData.pcBuffer;
	//			rkData.pcBuffer = pcPrebuffer;
	//			rkData.iBufferSize = iPreSize;
	//		}
	//		rkMemory.Clear();
	//		return false;
	//	}

	//	if (bReAlloca)
	//	{
	//		delete[] rkData.pcBuffer;
	//		rkData.pcBuffer = pcPrebuffer;
	//		rkData.iBufferSize = iPreSize;
	//	}
	//}
	//else
	//{
	//	try
	//	{
	//		if(!rkMemory.ReSize(kMD5File.iPkgSize))
	//		{
	//			return false;
	//		}
	//	}
	//	catch (const std::bad_alloc& ec)
	//	{
	//		throw TeMemExcerption(pcFileName,kMD5File.iPkgSize);
	//	}

	//	if(!rkData.Read(rkMemory.GetData(),kMD5File.iPkgSize))
	//	{
	//		rkMemory.Clear();
	//		return false;
	//	}
	//}

	return true;
}

bool FilePackage::IsExistFile(const char* pcFileName)
{
	DATA_CONFERENCE();

	// 是否已经初始化成功
	if(!rkData.bInitSuccess)
	{
		return false;
	}

	// 查看文件名是否合法
	if(!pcFileName || strlen(pcFileName) <= 4)
	{
		return false;
	}

	// 没有包文件就使用本地文件
#if defined(_WIN32) || defined(__LINUX__)

#ifdef _WIN32
	if(rkData.hPackageFile == INVALID_HANDLE_VALUE)
#else
	if (rkData.hPackageFile == NULL)
#endif // _WIN32
    {
        if(*pcFileName == '.')
        {
            pcFileName++;
        }
        
        if(*pcFileName == '\\' || *pcFileName == '/')
        {
            pcFileName++;
        }
        
        char acFullName[512];
#ifdef _WIN32
		sprintf_s(acFullName, sizeof(acFullName), "%s%s", rkData.kRootDir.c_str(), pcFileName);
#else
		snprintf(acFullName, sizeof(acFullName), "%s%s", rkData.kRootDir.c_str(), pcFileName);
#endif
        
        FILE* fp = NULL;
#ifdef _WIN32
		fopen_s(&fp, acFullName, "rb");
#else
		fp = fopen(acFullName, "rb");
#endif
        if (!fp)
        {
            return false;
        }
        fclose(fp);
        
        return true;
    }
#else
    if (rkData.hPackageFile == NULL)
    {
        if(rkData.bMustPackage)
        {
            return false;
        }
        
        return VFileAccessManager::GetInstance()->FileExists(pcFileName);
    }
#endif

	//// 包文件是否已经打开
	//if(rkData.kAllFile.size() <= 0)
	//{
	//	return false;
	//}

	//// 获取包裹中的文件名
	//char acFile[512];
	//strcpy(acFile,pcFileName);
	//rkData.FilterFileName(acFile);

	//// 查看是否存在包裹中
	//std::map<std::string,TeFileInfo>::iterator itr = rkData.kAllFile.find(acFile);
	//if(itr == rkData.kAllFile.end())
	//{
	//	return false;
	//}
	return true;
}
//////////////////////////////////////////////////////////////////////////

//	资源加载句柄
static	FilePackage*	pkFilePkg = NULL;
//	表格资源
static	FileMemory*		pkTableFile = NULL;
static	std::map<std::string, FileMemory>		kTableData;

bool FileLoader::LoadFile(const char* pcFileName, FileMemory& rkMemory)
{
	if(pkFilePkg == NULL)
	{
		pkFilePkg = new FilePackage;
		pkFilePkg->Initialize("./", true);
	}

	return pkFilePkg->Load(pcFileName, rkMemory);
}

bool FileLoader::LoadTableFile(const char* pcFileName, FileMemory& rkMemory)
{
	//	win下方便测试 直接读取
//#ifdef _WIN32
	return LoadFile(pcFileName, rkMemory);
//#else
//	//	手机上直接读取打包资源
//	if(pkTableFile == NULL)
//	{
//		pkTableFile = new FileMemory;
//		bool bLoadXml = LoadFile("Data/Table/XmlBlock.data", *pkTableFile);
//		if(!bLoadXml)
//		{
//			return LoadFile(pcFileName, rkMemory);
//		}
//		//	解析数据段到每个表格
//		int size = pkTableFile->GetSize();
//		if(size > 0)
//		{
//			const char* content = pkTableFile->GetData();
//			int offset = 0;
//			unsigned char keyLength = 0;
//
//			do
//			{
//				keyLength = *(unsigned char*)content;
//
//				content += sizeof(unsigned char);
//				offset += sizeof(unsigned char);
//
//				char	acFileName[512] = {0};
//				memcpy(&acFileName, content, keyLength);
//
//				content += keyLength;
//				offset += keyLength;
//
//				int startX = *(int*)content;
//				if(startX < 0)
//				{
//					break;
//				}
//				content += sizeof(int);
//				int endX = *(int*)content;
//				int chunkSize = endX;
//				if(endX < 0)
//				{
//					chunkSize = size - startX;
//				}
//
//				FileMemory	kData;
//				kData.ReSize(chunkSize);
//				memcpy(kData.GetData(), pkTableFile->GetData() + startX, chunkSize);
//
//				kTableData[acFileName] = kData;
//
//				content += sizeof(int);
//				offset += sizeof(int);
//			} while(offset < size);
//		}
//	}
//	if(!pkTableFile)
//	{
//		return false;
//	}
//	
//	std::map<std::string, FileMemory>::iterator Iter = kTableData.find(pcFileName);
//	if(Iter != kTableData.end())
//	{
//		rkMemory = Iter->second;
//		return true;
//	}
//	
//	return false;
//#endif
}