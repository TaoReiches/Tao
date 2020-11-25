#ifndef __SELOCK_H__
#define __SELOCK_H__

#include "TeInclude.h"

// 锁定义
class SeVirtualLock
{
	public:
	SeVirtualLock(){}

	virtual ~SeVirtualLock(){}

	virtual void Lock(void) = 0;
	virtual bool TryLock(void) = 0;
	virtual void UnLock(void) = 0;
};

// 自动锁
class SeAutoLock
{
public:
	SeAutoLock(SeVirtualLock *pkLock)
	{
		m_pkLock = pkLock;
		m_pkLock->Lock();
	}

	~SeAutoLock()
	{
		m_pkLock->UnLock();
	}

protected:
	SeVirtualLock*	m_pkLock;
};

#ifdef WIN32
// 手动锁
class SeLock : public SeVirtualLock
{
public:
	SeLock()
	{
		InitializeCriticalSection(&m_kSection);
	}

	virtual ~SeLock()
	{
		DeleteCriticalSection(&m_kSection);
	}

	virtual void Lock(void)
	{
		EnterCriticalSection(&m_kSection);
	}

	virtual bool TryLock(void)
	{
		return TryEnterCriticalSection(&m_kSection);
	}

	virtual void UnLock(void)
	{
		LeaveCriticalSection(&m_kSection);
	}

protected:
	CRITICAL_SECTION m_kSection;
};
#elif defined __LINUX__
class SeLock  : public SeVirtualLock
{
public:
	SeLock()
	{
		pthread_mutexattr_t kAttr;
		pthread_mutexattr_init(&kAttr);
		pthread_mutexattr_settype(&kAttr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&m_kMutex,&kAttr);
	}

	virtual ~SeLock()
	{
		pthread_mutex_destroy(&m_kMutex);

	}

	virtual void Lock(void)
	{
		pthread_mutex_lock(&m_kMutex);
	}

	virtual void UnLock(void)
	{
		pthread_mutex_unlock(&m_kMutex);
	}

	virtual bool TryLock(void)
	{
		return (pthread_mutex_trylock(&m_kMutex) != 0);
	}

protected:
	pthread_mutex_t  m_kMutex;
};
#else
class SeLock  : public SeVirtualLock
{
public:
	SeLock()
	{
		pthread_mutexattr_t kAttr;
		pthread_mutexattr_init(&kAttr);
		pthread_mutexattr_settype(&kAttr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_kMutex,&kAttr);
	}

	virtual ~SeLock()
	{
		pthread_mutex_destroy(&m_kMutex);

	}

	virtual void Lock(void)
	{
		pthread_mutex_lock(&m_kMutex);
	}

	virtual void UnLock(void)
	{
		pthread_mutex_unlock(&m_kMutex);
	}

	virtual bool TryLock(void)
	{
        return (pthread_mutex_trylock(&m_kMutex) != 0);
	}

protected:
	pthread_mutex_t  m_kMutex;
};

#endif

#endif