/*
 * =====================================================================================
 *
 *       Filename:  SocketDefines.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012-10-01
 *       Revision:  none
 *       Compiler:  cmake & gcc
 *
 *         Author:  ankjt
 *
 * =====================================================================================
 */

#ifndef __LINUX_H__
#define __LINUX_H__

#include <errno.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <signal.h>
#include <netdb.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <list>
#include <queue>
#include <map>
#include <fstream>
#include <algorithm>
#include <ext/hash_map>
#include <cstring>
#include <semaphore.h>
#include <dlfcn.h>

#define SOCKET int
#define SD_BOTH SHUT_RDWR

#define __declspec __attribute__
#define ZTTIME_USEC_PER_SEC 1000000
#define SOCKET_DATE_SIZE 65536
#define TRUE             1
#define FALSE            0
#define WINAPI
#define MB_OK            0x00000000L
#define MAX_PATH         260
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define sprintf_s(buffer,size,format,...)  snprintf(buffer,size,format,__VA_ARGS__)
#define sscanf_s(buffer,format,...) sscanf(buffer,format,__VA_ARGS__)
#define	_stricmp	strcasecmp      
#define FLT_MAX         3.402823466e+38F        /* max value */
#define MEM_COMMIT           0x1000     
#define MEM_RESERVE          0x2000  
#define PAGE_EXECUTE_READWRITE 0x40   
#define MEM_RELEASE          0x8000
#define RTL_FIELD_SIZE(type, field) (sizeof(((type *)0)->field))
/*

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

*/

#define FILE_MAP_ALL_ACCESS IPC_CREAT|0666
#define FILE_MAP_READ       SHM_R

typedef     unsigned char  BYTE;
typedef     unsigned int   DWORD;
typedef     unsigned short WORD;
typedef     unsigned char  uint8;
typedef     unsigned short uint16;
typedef     unsigned long  ulong;
typedef     int            BOOL;
typedef     pthread_t	   TID ;
typedef     unsigned int   UINT;
typedef     uint64_t       UINT64;
typedef     int64_t        __int64;
typedef     char           CHAR;
typedef     float          FLOAT;
typedef     int            HANDLE;
typedef     long           SIZE_T;
typedef		long		   LONG;
typedef		int            INT;
typedef		key_t		   MQ_Key;
typedef		pthread_t	   TID;
typedef		BYTE*		   PBYTE;
      

#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#define _THROW(x, y)	throw x(y)

typedef void *PVOID;
typedef void *LPVOID;

typedef struct _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} 	SYSTEMTIME;
typedef struct _SYSTEMTIME *PSYSTEMTIME;
typedef struct _SYSTEMTIME *LPSYSTEMTIME;

typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _WIN32_FIND_DATAA {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	CHAR   cFileName[ MAX_PATH ];
	CHAR   cAlternateFileName[ 14 ];
#ifdef _MAC
	DWORD dwFileType;
	DWORD dwCreatorType;
	WORD  wFinderFlags;
#endif
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
typedef WIN32_FIND_DATAA WIN32_FIND_DATA;

inline int  _mkdir(const char * _Path)
{
	mkdir(_Path,S_IRWXU);
}


inline void GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
	tm kTime;
	__int64 iTime;

	time(&iTime);
	kTime = *localtime(&iTime);
    lpSystemTime->wYear = kTime.tm_year + 1900;
    lpSystemTime->wMonth = kTime.tm_mon + 1;
    lpSystemTime->wDay = kTime.tm_mday;
    lpSystemTime->wHour = kTime.tm_hour;
    lpSystemTime->wMinute = kTime.tm_min;
    lpSystemTime->wSecond = kTime.tm_sec;
}
inline BOOL QueryPerformanceCounter(UINT64* lpCounter)
{
    struct timespec tp;
	if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp) == 0)
	{
       *lpCounter = (UINT64)(tp.tv_sec * (UINT64)1000000000) + (UINT64)tp.tv_nsec;
	   return TRUE;
	}
	return FALSE;
}


inline BOOL QueryPerformanceFrequency(UINT64 *lpFrequency)
{
	struct timespec tp;
	if(clock_getres(CLOCK_PROCESS_CPUTIME_ID,&tp) == 0)
	{
		*lpFrequency = (UINT64)(tp.tv_sec * (UINT64)1000000000) + (UINT64)tp.tv_nsec;
		return TRUE;
	}
	return FALSE;
}

inline DWORD GetTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec/(1000 * 1000));
}

inline void Sleep(DWORD dwMilliseconds)
{
	long long lTime = dwMilliseconds * 1000;
	/*
	struct timespec req={0};
	req.tv_sec=0;
	req.tv_nsec=lTime * 1000;

	if(nanosleep(&req,NULL) == -1)
	{
		usleep(lTime);
	}
	*/
	usleep(lTime);
}


inline DWORD WINAPI GetCurrentThreadId()
{
    return (DWORD)pthread_self();
}

inline DWORD GetLastError(void)
{
	return 0;
}

inline HANDLE OpenFileMapping(DWORD dwDesiredAccess,BOOL bInheritHandle,std::string lpName,long lSize)
{
	char acFileName[64] = {};
	sprintf(acFileName,"./%s",lpName.c_str());
	_mkdir(acFileName);	   
	HANDLE hMapping = shmget(ftok(acFileName, 1), lSize + sizeof(long), FILE_MAP_ALL_ACCESS);   
    if( -1 == hMapping )
	{
		return 0; 
	}

	void* pkBaseAddr = (void*)shmat(hMapping,0,0);
	if(!pkBaseAddr)
	{
		return 0;
	}

//	*((SIZE_T*)pkBaseAddr) = lSize;

	if(-1 == shmdt(pkBaseAddr))
	{
		return 0;
	}

	return hMapping;
}

inline void* MapViewOfFile(HANDLE hFileMappingObject,DWORD dwDesiredAccess,DWORD dwFileOffsetHigh,DWORD dwFileOffsetLow,SIZE_T dwNumberOfBytesToMap)
{
	return (void*)shmat(hFileMappingObject,0,0);
}

inline BOOL UnmapViewOfFile(void* lpBaseAddress)
{
	if(-1 == shmdt(lpBaseAddress))
	{
		return FALSE;
	}
	return TRUE;
}

inline BOOL CloseHandle(HANDLE hObject)
{
	if(-1 == shmctl(hObject,IPC_RMID,0))
	{
		return FALSE;
	}
	return TRUE;
}


inline void* VirtualAlloc(void* pkVoid,int iSize,DWORD dwAllocType,DWORD dwProtect)
{
	return mmap(pkVoid,iSize,dwProtect,dwAllocType,-1,0);
}

inline int VirtualFree(void* pkVoid,int iSize,DWORD dwFreeType)
{
	return munmap(pkVoid,iSize);
}

#endif

