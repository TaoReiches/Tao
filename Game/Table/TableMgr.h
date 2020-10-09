#pragma once
#pragma execution_character_set("utf-8")

#include "Unit_table.hpp"
#include "Skill_table.hpp"
#include "Buffer_table.hpp"
#include "MapInfo_table.hpp"
#include "Item_table.hpp"

#include <vector>

class TableMgr
{
public: 
	static	void		LoadTable()
	{
		UnitTableMgr::Get();
		SkillTableMgr::Get();
		BufferTableMgr::Get();
		MapInfoTableMgr::Get();
		ItemTableMgr::Get();
	}

	static TableMgr*	Get();

	int					GetHeroIDByPosIndex(int iPosIndex);
	int					GetPosIndexByHeroID(int iHeroID);

private:
	static TableMgr*			m_pkAllTableMgr;

	// 英雄相关
	std::map<int, int>	m_akHeroID2PosIndex;
	std::map<int, int>	m_akPosIndex2HeroID;
};