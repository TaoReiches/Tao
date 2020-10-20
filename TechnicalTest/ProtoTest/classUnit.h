#pragma once

#include "classMainBase.h"

class UnitBase
{
public:
    int GetBaseId()
    {
        return base_Id;
    }

protected:
    UnitBase() {};

private:
    int base_Id = 99;
};

class Unit : public BeMainPtr, public UnitBase
{
public:
    Unit() = default;
    Unit(int id);
    ~Unit();

    inline int getId() { return m_Id; }

private:
    int m_Id = 10;
};
