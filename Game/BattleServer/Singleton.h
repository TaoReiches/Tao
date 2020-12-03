#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

template <typename T>
class TwSingleton
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
    TwSingleton() { mpInstance = nullptr;}
private:
    static T* mpInstance;
};

template <typename T>
T* TwSingleton<T>::mpInstance = nullptr;
