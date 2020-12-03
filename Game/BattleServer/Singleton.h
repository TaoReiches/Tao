#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

template <typename T>
class Singleton
{
public:
    static T* Get()
    {
        if (mpInstance == nullptr)
        {
            mpInstance = new T();
        }
        return mpInstance;
    }

protected:
    Singleton() { mpInstance = nullptr;}
private:
    static T* mpInstance;
};

template <typename T>
T* Singleton<T>::mpInstance = nullptr;
