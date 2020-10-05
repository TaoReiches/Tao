#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

struct BeFlagObj
{
public:
	BeFlagObj(void) :m_iFlag(0)
	{
	}

	inline bool HasFlag(int iFlag) const
	{
		return (bool)((m_iFlag & iFlag) == iFlag);
	}

	inline void SetFlag(int iFlag)
	{
		m_iFlag |= iFlag;
	}

	inline void ClrFlag(int iFlag)
	{
		m_iFlag &= ~iFlag;
	}

	inline int GetFlag(void) const
	{
		return m_iFlag;
	}

protected:
	int m_iFlag;
};
