#pragma once

#include "classMainBase.h"

class Unit : public BeMainPtr
{
public:
    Unit() = default;
    ~Unit();

    inline int getId() { return m_Id; }

private:
    int m_Id = 10;
};
