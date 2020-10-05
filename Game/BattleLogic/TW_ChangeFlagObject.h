#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

struct BeChangeFlagObj
{
public:
	BeChangeFlagObj(void) :m_iChangeFlag(0), m_iBackChangeFlag(0)
	{
	}

	inline bool HasChangeFlag(int iFlag) const
	{
		return (bool)((m_iChangeFlag & iFlag) == iFlag);
	}

	inline void SetChangeFlag(int iFlag)
	{
		m_iChangeFlag |= iFlag;

		m_iBackChangeFlag |= iFlag;
	}

	inline void ClrChangeFlag(int iFlag)
	{
		m_iChangeFlag &= ~iFlag;
	}
	inline void ClrBackChangeFlag(int iFlag)
	{
		m_iBackChangeFlag &= ~iFlag;
	}

	inline int GetChangeFlag(void) const
	{
		return m_iChangeFlag;
	}

	inline bool HasBackChangeFlag(int iFlag) const
	{
		return (bool)((m_iBackChangeFlag & iFlag) == iFlag);
	}

	inline int GetBackChangeFlag(void) const
	{
		return m_iBackChangeFlag;
	}

protected:
	int m_iChangeFlag;
	int m_iBackChangeFlag;
};
