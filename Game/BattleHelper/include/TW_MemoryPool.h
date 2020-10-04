#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Lock.h"

#ifdef _VISION_IOS
#if defined(__LP64__)
typedef unsigned long SIZE_T;
#else
typedef unsigned int SIZE_T;
#endif
#endif

#ifdef _VISION_ANDROID
#if defined(__LP64__)
typedef unsigned long SIZE_T;
#else
typedef unsigned int SIZE_T;
#endif
#endif

typedef void(*NotifyMempoolAlloc)(const char*, unsigned int);

class TeMemoryPool
{
public:
	TeMemoryPool(int iEleSize, int iBaseEles, int iAddEles);
	~TeMemoryPool(void);

	void*	NewEle(void);
	void	DelEle(void* pkEle);
	bool	IsValid(void* pkEle);

	int		GetAllocSize(void);
	int		GetTotalSize(void);
	int		GetAllocMaxEles(void);
	void	Clear(void);

	static void RegisterAllocCallBack(NotifyMempoolAlloc fCallBack);
private:
	bool	ExtendPool(void);
	void	Reset(void);

private:
	struct SeHeadList
	{
		SeHeadList* pkPrev;
		SeHeadList* pkNext;
		int			iSize;
		char		acData[8];
	};

	char		m_acName[64];
	int			m_iEleSize;
	int			m_iBaseEles;
	int			m_iAddEles;
	int			m_iTotalSize;

	int			m_iAllocEles;
	int			m_iAllocMaxEles;

	void*		m_pkFreeFirst;

	SeHeadList	m_kHeadList;

	SIZE_T		m_stMagicBegin;
	SIZE_T		m_stMagicEnd;

	NormalLock		m_kLock;

	static NotifyMempoolAlloc	m_fAllocCallBack;
};

enum PoolState
{
	PS_INTILISED = 0,
	PS_FINALISED,
};

#define DECLARE_POOL(ClassName)\
	private:\
		static TeMemoryPool s_kMemPool;\
		PoolState		 m_ePoolState; \
	public:\
		static ClassName*	NEW(void);\
		static void			DEL(ClassName *pkDel);\
		static void			CLR();\
		static int			GetAllocSize();\
		static int			GetAllocMaxEles(); \


#if defined(_VERSION_WINDOWS)
#define IMPLEMENT_POOL(ClassName,BaseEles,AddEles)\
	TeMemoryPool	ClassName::s_kMemPool(sizeof(##ClassName),BaseEles,AddEles);\
	ClassName*	ClassName::NEW(void)\
					{\
		ClassName* pkNew = (ClassName *)(s_kMemPool.NewEle());\
		if(pkNew)\
										{\
			MNew(pkNew,ClassName);\
			pkNew->m_ePoolState = PS_INTILISED; \
				}\
		return pkNew;\
		}\
	void ClassName::DEL(ClassName* pkDel)\
		{\
		if(s_kMemPool.IsValid(pkDel))\
		{\
			pkDel->~ClassName();\
			pkDel->m_ePoolState = PS_FINALISED; \
			s_kMemPool.DelEle(pkDel);\
		}\
	}\
	void ClassName::CLR()\
	{\
		s_kMemPool.Clear();\
	}\
	int	ClassName::GetAllocSize()\
	{\
		return s_kMemPool.GetTotalSize();\
	}\
	int	ClassName::GetAllocMaxEles()\
	{\
		return s_kMemPool.GetAllocMaxEles();\
	}
#else
#define IMPLEMENT_POOL(ClassName,BaseEles,AddEles)\
    TeMemoryPool	ClassName::s_kMemPool(sizeof(ClassName),BaseEles,AddEles,(#ClassName)[0]);\
    ClassName*	ClassName::NEW(void)\
    {\
        ClassName* pkNew = (ClassName *)(s_kMemPool.NewEle());\
        if(pkNew)\
        {\
            MNew(pkNew,ClassName);\
            pkNew->m_ePoolState = PS_INTILISED; \
        }\
        return pkNew;\
    }\
    void ClassName::DEL(ClassName* pkDel)\
    {\
        if(s_kMemPool.IsValid(pkDel))\
        {\
			pkDel->~ClassName();\
            pkDel->m_ePoolState = PS_FINALISED; \
            s_kMemPool.DelEle(pkDel);\
        }\
    }\
    void ClassName::CLR()\
    {\
        s_kMemPool.Clear();\
    }\
    int	ClassName::GetAllocSize()\
    {\
        return s_kMemPool.GetTotalSize();\
    }\
    int	ClassName::GetAllocMaxEles()\
    {\
        return s_kMemPool.GetAllocMaxEles();\
    }
#endif

#define DECLARE_POOL1(ClassName)\
	private:\
		static TeMemoryPool s_kMemPool;\
		PoolState		 m_ePoolState; \
	public:\
		static ClassName*	NEW(unsigned int dwValue);\
		static void			DEL(ClassName *pkDel);\
		static void			CLR();\
		static int			GetAllocSize();\
		static int			GetAllocMaxEles();\
	public:\
		void				MemPool_Release(void);

#if defined(_VERSION_WINDOWS)
#define IMPLEMENT_POOL1(ClassName,BaseEles,AddEles)\
	TeMemoryPool	ClassName::s_kMemPool(sizeof(##ClassName),BaseEles,AddEles,#ClassName);\
	ClassName*	ClassName::NEW(unsigned int dwValue)\
	{\
		ClassName* pkNew = (ClassName *)(s_kMemPool.NewEle());\
		if(pkNew)\
		{\
			MNew1(pkNew,ClassName,dwValue);\
			pkNew->m_ePoolState = PS_INTILISED; \
		}\
		return pkNew;\
	}\
	void ClassName::DEL(ClassName* pkDel)\
	{\
		if(s_kMemPool.IsValid(pkDel))\
		{\
			pkDel->~ClassName();\
			pkDel->m_ePoolState = PS_FINALISED; \
			s_kMemPool.DelEle(pkDel);\
		}\
	}\
	void ClassName::CLR()\
	{\
		s_kMemPool.Clear();\
	}\
	int	ClassName::GetAllocSize()\
	{\
		return s_kMemPool.GetTotalSize();\
	}\
	int	ClassName::GetAllocMaxEles()\
	{\
		return s_kMemPool.GetAllocMaxEles();\
	}
#else
#define IMPLEMENT_POOL1(ClassName,BaseEles,AddEles)\
    TeMemoryPool	ClassName::s_kMemPool(sizeof(ClassName),BaseEles,AddEles,#ClassName);\
    ClassName*	ClassName::NEW(unsigned int dwValue)\
    {\
        ClassName* pkNew = (ClassName *)(s_kMemPool.NewEle());\
        if(pkNew)\
        {\
            MNew1(pkNew,ClassName,dwValue);\
            pkNew->m_ePoolState = PS_INTILISED; \
        }\
        return pkNew;\
    }\
    void ClassName::DEL(ClassName* pkDel)\
    {\
        if(s_kMemPool.IsValid(pkDel))\
        {\
			pkDel->~ClassName();\
            pkDel->m_ePoolState = PS_FINALISED; \
            s_kMemPool.DelEle(pkDel);\
        }\
    }\
    void ClassName::CLR()\
    {\
        s_kMemPool.Clear();\
    }\
    int	ClassName::GetAllocSize()\
    {\
        return s_kMemPool.GetTotalSize();\
    }\
    int	ClassName::GetAllocMaxEles()\
    {\
        return s_kMemPool.GetAllocMaxEles();\
    }
#endif

#define DECLARE_POOL_ARRAY(ClassName)\
	private:\
	static TeMemoryPool s_kMemPool;\
	PoolState		 m_ePoolState; \
	public:\
	static ClassName*	NEW(void);\
	static void			DEL(ClassName *pkDel);\
	static void			CLR();\
	static int			GetAllocSize();\
	static int			GetAllocMaxEles();\
	public:\
	void				MemPool_Release(void);
#if defined(_VERSION_WINDOWS)
#define IMPLEMENT_POOL_ARRAY(ClassName,ArraySize, BaseEles,AddEles)\
	TeMemoryPool	ClassName::s_kMemPool(sizeof(##ClassName) * ArraySize,BaseEles,AddEles,#ClassName);\
	ClassName*	ClassName::NEW(void)\
	{\
		ClassName* pkNew = (ClassName *)(s_kMemPool.NewEle());\
		if(pkNew)\
		{\
			MNew(pkNew,ClassName);\
			pkNew->m_ePoolState = PS_INTILISED; \
		}\
		return pkNew;\
	}\
	void ClassName::DEL(ClassName* pkDel)\
	{\
		if(s_kMemPool.IsValid(pkDel))\
		{\
			pkDel->~ClassName();\
			pkDel->m_ePoolState = PS_FINALISED; \
			s_kMemPool.DelEle(pkDel);\
		}\
	}\
	void ClassName::CLR()\
	{\
		s_kMemPool.Clear();\
	}\
	int	ClassName::GetAllocSize()\
	{\
		return s_kMemPool.GetTotalSize();\
	}\
	int	ClassName::GetAllocMaxEles()\
	{\
		return s_kMemPool.GetAllocMaxEles();\
	}
#else
#define IMPLEMENT_POOL_ARRAY(ClassName,ArraySize, BaseEles,AddEles)\
    TeMemoryPool	ClassName::s_kMemPool(sizeof(ClassName) * ArraySize,BaseEles,AddEles,#ClassName);\
    ClassName*	ClassName::NEW(void)\
    {\
        ClassName* pkNew = (ClassName *)(s_kMemPool.NewEle());\
        if(pkNew)\
        {\
            MNew(pkNew,ClassName);\
            pkNew->m_ePoolState = PS_INTILISED; \
        }\
        return pkNew;\
    }\
    void ClassName::DEL(ClassName* pkDel)\
    {\
        if(s_kMemPool.IsValid(pkDel))\
        {\
			pkDel->~ClassName();\
            pkDel->m_ePoolState = PS_FINALISED; \
            s_kMemPool.DelEle(pkDel);\
        }\
    }\
    void ClassName::CLR()\
    {\
        s_kMemPool.Clear();\
    }\
    int	ClassName::GetAllocSize()\
    {\
        return s_kMemPool.GetTotalSize();\
    }\
    int	ClassName::GetAllocMaxEles()\
    {\
        return s_kMemPool.GetAllocMaxEles();\
    }
#endif

#define CheckMemPool_Void()   \
	if (m_ePoolState != PS_INTILISED)   \
	{\
	assert(false);\
	return; }

#define CheckMemPool_Bool()   \
	if (m_ePoolState != PS_INTILISED)   \
	{\
	assert(false);\
	return false;}

#define CheckMemPool_Int()   \
	if (m_ePoolState != PS_INTILISED)   \
	{\
	assert(false);\
	return 0; }

#define CheckMemPool_Ptr()   \
	if (m_ePoolState != PS_INTILISED)   \
	{\
	assert(false);\
	return NULL; }
