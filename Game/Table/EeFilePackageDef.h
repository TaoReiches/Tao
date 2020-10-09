#pragma once
//#include "TeInclude.h"
//#include "TeMD5Check.h"
//
//#define MD5_FILE_TAG (0x13108019)
//
//// 包格式头信息
//struct TeDhpHeader
//{
//	int		iTagment;		// 包格式标识
//	int		iVersion;		// 版本号
//	TeMD5	kAllMD5;		// 所有文件数据MD5码
//	int		iMD5FileOff;	// MD5文件数据偏移
//	int		iFileInfoOff;	// 名字等信息偏移
//	char	acReserver[16];	// 保留
//};
//
//// MD5文件头信息
//struct TeMD5FileHeader
//{
//	int		iTagment;		// MD5文件标识
//	int		iMD5FileSize;	// MD5文件数据大小
//	int		iMD5FileNum;	// MD5文件个数
//};
//
//// 文件信息
//struct TeMD5File
//{
//	int		iTagment;		// 文件标识
//	TeMD5	kCmprMD5;		// 压缩后MD5码
//	int		iCmprType;		// 压缩类型
//	int		iOrgSize;		// 文件原始大小
//	int		iPkgSize;		// 文件打包后大小
//};
//
//// 文件信息头结构
//struct TeFileInfoHeader
//{
//	int		iTagment;		// 文件信息头标识
//	int		iInfoSize;		// 信息压缩后大小
//	int		iCmprType;		// 压缩类型
//	int		iInfoOrgSize;	// 信息原始大小
//	int		iInfoNum;		// 文件信息个数
//};
//
//// 包格式尾信息
//struct TeFileInfo
//{
//	int		iDataOff;		// 文件数据偏移
//	TeMD5	kCmprMD5;		// 文件数据压缩后MD5码
//};

#include <map>
#include <string>

struct FilePackageData
{
	FilePackageData();

	// 文件操作
	bool	Read(void* pkVoid,int iSize);
	void	Seek(int iPos);
	void	GoPos(int iOff);

	// 其他
	void	FilterFileName(char* acFile);	// 过滤文件，小写化替换'/'为'\\'

	// 数据
	bool		bInitSuccess;	// 是否初始化成功
	bool		bMustPackage;	// 包文件是否是必须的
	void*		hPackageFile;	// 包裹文件句柄
	std::string		kRootDir;		// 根目录

	// 缓冲
	int			iBufferSize;	// 当前缓冲大小
	char*		pcBuffer;		// 缓冲

	// 文件信息
	//std::map<std::string,TeFileInfo> kAllFile;	// 所有压缩文件的信息
};