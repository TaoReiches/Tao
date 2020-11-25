#ifndef __SEINCLUDE_H__
#define __SEINCLUDE_H__

#if defined WIN32

#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)
#pragma warning(disable:4819)
#pragma warning(disable:4717)
#pragma warning(disable:4102)

#pragma warning(disable:4018)
#pragma warning(disable:4065)
#pragma warning(disable:4267)
#pragma warning(disable:4244)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500
#endif

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

#endif

// STL
#include <stdarg.h>
#include <cctype>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <algorithm>

#include <locale>

#include <string>
#include <sstream>
#include <fstream>

#include <limits>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <new>

#if defined WIN32
// windows
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include <wincon.h>
#include <conio.h>
#include <xutility>
#include <process.h>
#include <mswsock.h>
#include <dbghelp.h>
#include <Tlhelp32.h>

typedef		short		MQ_Key;
typedef		unsigned	TID ;

#elif defined __LINUX__
#include "Include/Linux.h"
#endif

// 宏定义
#ifndef SAFE_DELETE
#define SAFE_DELETE(P)				if(P)	{ delete (P);         (P) = NULL; }
#define SAFE_ARRAY_DELETE(P)		if(P)	{ delete[] (P);       (P) = NULL; }
#define SAFE_RELEASE(P)				if(P)	{ (P)->Release();     (P) = NULL; }
#endif

#ifndef SAFECHECK_INDEX
#define SAFECHECK_INDEX(i,m)			{if(i < 0 || i >= m) {return;}}
#endif

#ifndef SE_D3DX_PI
#define SE_D3DX_PI    ((FLOAT)3.141592654f)
#define	SE_D3DX_2PI	(SE_D3DX_PI * 2.0f)		// 2PI
#endif

#ifdef WIN32
#ifndef MNew
#define MNew(MemAddr,ClassName)			(((ClassName *)MemAddr)->ClassName::ClassName(),((ClassName *)MemAddr))
#endif

#ifndef MNew1
#define MNew1(MemAddr,ClassName,Value)	(((ClassName *)MemAddr)->ClassName::ClassName(Value),((ClassName *)MemAddr))
#endif
#else
#ifndef MNew
#define MNew(MemAddr,ClassName)			(new((ClassName*)MemAddr)ClassName)
#endif

#ifndef MNew1
#define MNew1(MemAddr,ClassName,Value)	(new((ClassName*)MemAddr)ClassName(Value))
#endif
#endif
#define _USE_MATH_DEFINES

#define __TO_STRING(x) #x
#define MACRO_STR(macro) __TO_STRING(macro)

#endif
