#pragma once

#include "classMainBase.h"

class Unit : public BeMainPtr
{
public:
    Unit();
    ~Unit();

    int get() { return 10; }
};
