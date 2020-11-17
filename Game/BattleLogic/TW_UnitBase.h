#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <unordered_map>

#include "TW_Entity.h"
#include "TW_UserDataDefine.h"
#include <TW_PointerType.h>
#include "TW_ShareUnitData.h"

class TwUnitBase : public BeSingleLinkEntity, public BeEntity
{
protected:
    TwUnitBase();
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
	void        SetShareUnitChangeFlag(int iFlag);
	bool        HasShareUnitChangeFlag(int iFlag);
	void        ClrShareUnitChangeFlag(int iFlag);
	int         GetShareUnitChangeFlag() const;
	void        SetUD_Int(UserDataKey eKey, int i);
	void        SetUD_Float(UserDataKey eKey, float f);
	int         GetUD_Int(UserDataKey eKey, int i = 0)const;
	float       GetUD_Float(UserDataKey eKey, float f = 0.f)const;
	int         PopUD_Int(UserDataKey eKey);
	float       PopUD_Float(UserDataKey eKey);
	void        ClearUserData(UserDataKey eKey);
	bool        HasUserData(UserDataKey eKey) const;

protected:
    int                                         m_iOtherFlag;
    int                                         m_iImmunityFlag;
	int                                         m_iCarryFlag;
	int                                         m_iShareUnitDataChangeFlag;
	std::unordered_map<UserDataKey, TwPointerType>      m_akUserData;
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
		SetShareUnitChangeFlag(BSUDCF_OTHERFLAG);
	}
	m_iOtherFlag |= iFlag;
}

inline void TwUnitBase::ClrOtherFlag(int iFlag)
{
	if (HasOtherFlag(iFlag))
	{
		SetShareUnitChangeFlag(BSUDCF_OTHERFLAG);
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

inline void TwUnitBase::SetShareUnitChangeFlag(int iFlag)
{
	m_iShareUnitDataChangeFlag |= iFlag;
}

inline bool TwUnitBase::HasShareUnitChangeFlag(int iFlag)
{
	return (m_iShareUnitDataChangeFlag & iFlag) != 0;
}

inline void TwUnitBase::ClrShareUnitChangeFlag(int iFlag)
{
	m_iShareUnitDataChangeFlag &= ~iFlag;
}

inline int TwUnitBase::GetShareUnitChangeFlag() const
{
	return m_iShareUnitDataChangeFlag;
}

inline void TwUnitBase::SetUD_Int(UserDataKey eKey, int i)
{
	m_akUserData[eKey] = TwPointerType(i);
}

inline void TwUnitBase::SetUD_Float(UserDataKey eKey, float f)
{
	m_akUserData[eKey] = TwPointerType(f);
}

inline int TwUnitBase::PopUD_Int(UserDataKey eKey)
{
	int iValue = 0;
	auto itr = m_akUserData.find(eKey);
	if (itr != m_akUserData.end())
	{
		iValue = itr->second.v.iValue;
		m_akUserData.erase(itr);
	}

	return iValue;
}

inline float TwUnitBase::PopUD_Float(UserDataKey eKey)
{
	float fValue = 0.f;
	auto itr = m_akUserData.find(eKey);
	if (itr != m_akUserData.end())
	{
		fValue = itr->second.v.fValue;
		m_akUserData.erase(itr);
	}

	return fValue;
}

inline int TwUnitBase::GetUD_Int(UserDataKey eKey, int i) const
{
	auto itr = m_akUserData.find(eKey);
	if (itr != m_akUserData.end())
	{
		i = itr->second.v.iValue;
	}

	return i;
}

inline float TwUnitBase::GetUD_Float(UserDataKey eKey, float f) const
{
	auto itr = m_akUserData.find(eKey);
	if (itr != m_akUserData.end())
	{
		f = itr->second.v.fValue;
	}

	return f;
}

inline bool TwUnitBase::HasUserData(UserDataKey eKey) const
{
	auto itr = m_akUserData.find(eKey);
	if (itr != m_akUserData.end())
	{
		return true;
	}
	return false;
}

inline void TwUnitBase::ClearUserData(UserDataKey eKey)
{
	m_akUserData.erase(eKey);
}
