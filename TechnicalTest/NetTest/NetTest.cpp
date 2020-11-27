// NetTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "NetWork.h"

SeNetMgr	kNet;

int main()
{
    std::cout << "Hello World!\n";

    kNet.BeginGame();

    getchar();
}
