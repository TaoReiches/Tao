// NetTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "NetWork.h"
#include <chrono>
#include <thread>

SeNetMgr	kNet;

int main()
{
    std::cout << "Hello World!\n";

    kNet.BeginGame();

    while (true)
    {
        kNet.UpdateNet();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    getchar();
}
