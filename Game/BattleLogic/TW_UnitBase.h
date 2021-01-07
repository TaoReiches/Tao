#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <unordered_map>

#include "TW_LogicBase.h"
#include "TW_UserDataDefine.h"
#include "TW_UnitOutputFlag.h"
#include "TW_UnitFlag.h"

class TwUnitBase : public TwMainPtr, public TwUnitPtr
{
public:
    TwUnitBase(int iID);
    ~TwUnitBase();

public:
	bool        HasUnitImmunityFlag(int iFlag) const;
	void        SetUnitImmunityFlag(int iFlag);
	int         GetUnitImmunityFlag(void) const;
	void        ClrUnitImmunityFlag(int iFlag);
	void        SetOtherFlag(int iFlag);
	void        ClrOtherFlag(int iFlag);
	bool        HasOtherFlag(int iFalg) const;
	int         GetOtherFlag() const;
	int         GetUnitCarryFlag(void) const;
	void        SetUnitCarryFlag(int iFlag);
	bool        HasUnitCarryFlag(int iFlag) const;
	void        ClrUnitCarryFlag(int iFlag);
    void        SetOutputFlag(TwUnitOutputFlag iFlag);
    bool        HasOutputFlag(TwUnitOutputFlag iFlag);
    void        ClearOutputFlag(TwUnitOutputFlag iFlag);
    TwUnitOutputFlag    GetOutputFlag() const;
	void        SetUD_Int(UserDataKey eKey, int i);
	void        SetUD_Float(UserDataKey eKey, float f);
	int         GetUD_Int(UserDataKey eKey, int i = 0)const;
	float       GetUD_Float(UserDataKey eKey, float f = 0.f)const;
	int         PopUD_Int(UserDataKey eKey);
	float       PopUD_Float(UserDataKey eKey);
	void        ClearUserData(UserDataKey eKey);
	bool        HasUserData(UserDataKey eKey) const;
    int         GetID() const
    {
        return Id;
    }
    int GetTypeID() const
    {
        return m_iTypeID;
    }
    const bool HasFlag(BeUnitFlag flag) const
    {
        return (flag & UnitFlag) != 0;
    }
    const int GetBlockIndex() const
    {
        return BlockIndex;
    }
    void SetBlockIndex(int index)
    {
        BlockIndex = index;
    }

protected:
    int                                         m_iOtherFlag;
    int                                         m_iImmunityFlag;
    int                                         m_iCarryFlag;
    int                                         Id;
    int                                         m_iTypeID;
    int                                         BlockIndex;
    BeUnitFlag                                  UnitFlag;
    TwUnitOutputFlag                            OutputFlag;
	std::unordered_map<UserDataKey, int>        UserDataInt;
    std::unordered_map<UserDataKey, float>      UserDataFloat;
};

inline void TwUnitBase::SetUnitImmunityFlag(int iFlag)
{
	m_iImmunityFlag |= iFlag;
}

inline bool TwUnitBase::HasUnitImmunityFlag(int iFlag) const
{
	return (m_iImmunityFlag & iFlag) == iFlag;
}

inline int TwUnitBase::GetUnitImmunityFlag(void) const
{
	return m_iImmunityFlag;
}

inline void TwUnitBase::ClrUnitImmunityFlag(int iFlag)
{
	m_iImmunityFlag &= ~iFlag;
}
inline void TwUnitBase::SetOtherFlag(int iFlag)
{
	if (!HasOtherFlag(iFlag))
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_OTHERFLAG);
	}
	m_iOtherFlag |= iFlag;
}

inline void TwUnitBase::ClrOtherFlag(int iFlag)
{
	if (HasOtherFlag(iFlag))
	{
		SetOutputFlag(TwUnitOutputFlag::BSUDCF_OTHERFLAG);
	}
	m_iOtherFlag &= ~iFlag;
}

inline bool TwUnitBase::HasOtherFlag(int iFalg) const
{
	return (m_iOtherFlag & iFalg) == iFalg;
}

inline int TwUnitBase::GetOtherFlag() const
{
	return m_iOtherFlag;
}
inline int TwUnitBase::GetUnitCarryFlag(void) const
{
	return m_iCarryFlag;
}

inline void TwUnitBase::SetUnitCarryFlag(int iFlag)
{
	m_iCarryFlag |= iFlag;
}

inline bool TwUnitBase::HasUnitCarryFlag(int iFlag) const
{
	return (m_iCarryFlag & iFlag) != 0;
}

inline void TwUnitBase::ClrUnitCarryFlag(int iFlag)
{
	m_iCarryFlag &= ~iFlag;
}

inline void TwUnitBase::SetOutputFlag(TwUnitOutputFlag iFlag)
{
	OutputFlag |= iFlag;
}

inline bool TwUnitBase::HasOutputFlag(TwUnitOutputFlag iFlag)
{
	return (OutputFlag & iFlag) != TwUnitOutputFlag::BSUDCF_NULL;
}

inline void TwUnitBase::ClearOutputFlag(TwUnitOutputFlag iFlag)
{
	OutputFlag &= ~iFlag;
}

inline TwUnitOutputFlag TwUnitBase::GetOutputFlag() const
{
	return OutputFlag;
}

inline void TwUnitBase::SetUD_Int(UserDataKey eKey, int i)
{
	UserDataInt.emplace(eKey, i);
}

inline void TwUnitBase::SetUD_Float(UserDataKey eKey, float f)
{
	UserDataFloat.emplace(eKey, f);
}

inline int TwUnitBase::PopUD_Int(UserDataKey eKey)
{
	int iValue = 0;
	auto itr = UserDataInt.find(eKey);
	if (itr != UserDataInt.end())
	{
		iValue = itr->second;
		UserDataInt.erase(itr);
	}

	return iValue;
}

inline float TwUnitBase::PopUD_Float(UserDataKey eKey)
{
	float fValue = 0.f;
	auto itr = UserDataFloat.find(eKey);
	if (itr != UserDataFloat.end())
	{
		fValue = itr->second;
        UserDataFloat.erase(itr);
	}

	return fValue;
}

inline int TwUnitBase::GetUD_Int(UserDataKey eKey, int i) const
{
	auto itr = UserDataInt.find(eKey);
	if (itr != UserDataInt.end())
	{
		i = itr->second;
	}
	return i;
}

inline float TwUnitBase::GetUD_Float(UserDataKey eKey, float f) const
{
	auto itr = UserDataFloat.find(eKey);
	if (itr != UserDataFloat.end())
	{
		f = itr->second;
	}
	return f;
}

inline bool TwUnitBase::HasUserData(UserDataKey eKey) const
{
	auto itrInt = UserDataInt.find(eKey);
	if (itrInt != UserDataInt.end())
	{
		return true;
	}
    auto iterFloat = UserDataFloat.find(eKey);
    if (iterFloat != UserDataFloat.end())
    {
        return true;
    }
	return false;
}

inline void TwUnitBase::ClearUserData(UserDataKey eKey)
{
    UserDataInt.erase(eKey);
    UserDataFloat.erase(eKey);
}
