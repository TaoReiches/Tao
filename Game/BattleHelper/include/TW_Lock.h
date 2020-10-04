#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

class VirtualLock
{
public:
    VirtualLock() {}

    virtual ~VirtualLock() {}

    virtual void Lock(void) = 0;
    virtual bool TryLock(void) = 0;
    virtual void UnLock(void) = 0;
};

class AutoLock
{
public:
    AutoLock(VirtualLock* pkLock)
    {
        m_pkLock = pkLock;
        m_pkLock->Lock();
    }

    ~AutoLock()
    {
        m_pkLock->UnLock();
    }

protected:
    VirtualLock* m_pkLock;
};

#ifdef _VERSION_WINDOWS
#include <windows.h>
class NormalLock : public VirtualLock
{
public:
    NormalLock()
    {
        InitializeCriticalSection(&m_kSection);
    }

    virtual ~NormalLock()
    {
        DeleteCriticalSection(&m_kSection);
    }

    virtual void Lock(void)
    {
        EnterCriticalSection(&m_kSection);
    }

    virtual bool TryLock(void)
    {
        return (TryEnterCriticalSection(&m_kSection) > 0);
    }

    virtual void UnLock(void)
    {
        LeaveCriticalSection(&m_kSection);
    }

protected:
    CRITICAL_SECTION m_kSection;
};

#else 
#if defined( _VERSION_IOS) || defined(_VERSION_ANDROID)
class NormalLock : public VirtualLock
{
public:
    NormalLock()
    {
        pthread_mutexattr_t kAttr;
        pthread_mutexattr_init(&kAttr);
        pthread_mutexattr_settype(&kAttr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_kMutex, &kAttr);

    }

    virtual ~NormalLock()
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

class NormalLock : public VirtualLock
{
public:
    NormalLock()
    {
        pthread_mutexattr_t kAttr;
        pthread_mutexattr_init(&kAttr);
        pthread_mutexattr_settype(&kAttr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&m_kMutex, &kAttr);

    }

    virtual ~NormalLock()
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
