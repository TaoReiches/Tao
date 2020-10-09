#include "TableMgr.h"
#include <set>

TableMgr* TableMgr::m_pkAllTableMgr = NULL;
TableMgr* TableMgr::Get()
{
	if (m_pkAllTableMgr == NULL)
	{
		m_pkAllTableMgr = new TableMgr();
	}
	return m_pkAllTableMgr;
}

int TableMgr::GetHeroIDByPosIndex(int iPosIndex)
{
	std::map<int, int>::iterator itr = m_akPosIndex2HeroID.find(iPosIndex);
	if (itr != m_akPosIndex2HeroID.end())
	{
		return itr->second;
	}

	return 0;
}

int TableMgr::GetPosIndexByHeroID(int iHeroID)
{
	std::map<int, int>::iterator itr = m_akHeroID2PosIndex.find(iHeroID);
	if (itr != m_akHeroID2PosIndex.end())
	{
		return itr->second;
	}

	return 0;
}
