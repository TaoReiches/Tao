/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Item.h"
#include "TW_Skill.h"
#include "TW_Main.h"
#include "Item_table.hpp"
#include "TW_MemoryObject.h"

BeItem::BeItem(int iID) : BeCarrySkill(iID)
{
	m_eType = BCT_ITEM;
	m_pkRes.reset();
	m_iSkillCDTime = 0;
	m_iSkillTypeID = 0;
	m_iUniqueID = 0;
	m_bComposeFlag = false;
}

BeItem::~BeItem()
{
}

bool BeItem::Initialize(int iTypeID)
{
	BeCarry::Initialize(iTypeID);
	m_pkRes = ItemTableMgr::Get()->GetItemTable(iTypeID);

	return true;
}

void	BeItem::InitItemSkill(void)
{
	ClearAttr();
	if (!m_kData.iItemTypeID)
	{
		{
			AddProperty(m_pkRes->uiItemProperty);
		}
		m_kData.iItemTypeID = m_iTypeID;
	}
	for (auto skillIter = m_akSkill.begin(); skillIter != m_akSkill.end(); ++skillIter)
	{
		auto& pkSkill = *skillIter;
		if (pkSkill.get())
		{
			mpSkill.free(pkSkill.get());
			pkSkill.release();
		}
	}
	m_akSkill.clear();

	int		iLastAddSkill = 0;
	for (int i = 0; i < 6; i++)
	{
		int iTypeID = 0;
		{
			iTypeID = m_pkRes->iItemSkill[i];
		}

		if (!iTypeID)
		{
			break;
		}
		if (iLastAddSkill == iTypeID)
		{
			continue;
		}

		if (IsForbidSkill(iTypeID))
		{
			continue;
		}
		iLastAddSkill = iTypeID;

		auto pkSkill = std::unique_ptr<BeSkill>(mpSkill.alloc(gMain->GenerateID(BeGenIDType::GIT_CARRY)));
		pkSkill->AttachMain(pkAttachMain);
		pkSkill->AttachUnit(pkAttachUnit);
		if (pkSkill->Initialize(iTypeID))
		{
			m_akSkill.push_back(std::move(pkSkill));
			InitAttrFromSkill(iTypeID, pkSkill->GetLevel());
			m_iSkillCDTime = pkSkill->GetSkillRes()->iCoolDown[0];
			if (pkSkill->GetOperateType() != SKILL_OPERATETYPE_PASSIVE)
			{
				m_iSkillTypeID = iTypeID;
			}
		}
		else
		{
			mpSkill.free(pkSkill.get());
			pkSkill.release();
		}
	}
}

bool BeItem::GetValid(void)
{
	if (!m_kData.bValid)
	{
		return false;
	}

	if (GetValidTime() != 0 && (int)gTime < GetValidTime())
	{
		return false;
	}
	return true;
}

bool BeItem::GetIsUsableJudge(void)
{
	return true;
}

std::unique_ptr<BeSkill>& BeItem::GetSkillByTypeID(int iSkillTypeID)
{
	for (auto skillIter = m_akSkill.begin(); skillIter != m_akSkill.end(); ++skillIter)
	{
		auto& pkSkill = *skillIter;
		if (pkSkill.get() && pkSkill->GetTypeID() == iSkillTypeID)
		{
			return pkSkill;
		}
	}
	
	static std::unique_ptr<BeSkill> errorSkill;
	return errorSkill;
}

void BeItem::Update(std::shared_ptr<TwUnit> pkUnit, int iDeltaTime)
{
	if (!pkUnit)
	{
		return;
	}
	{
		auto& pkItemRes = GetResPtr();
		if (!pkItemRes)
		{
			return;
		}

		for (auto skillIter = m_akSkill.begin(); skillIter != m_akSkill.end(); ++skillIter)
		{
			auto& pkSkill = *skillIter;
			if (!pkSkill.get())
			{
				continue;
			}

			if (IsForbidSkill(pkSkill->GetTypeID()))
			{
				continue;
			}
			pkSkill->Update(pkUnit, m_kData.iPackagePos);
		}
	}
}

bool BeItem::CDComplete(int iCoolTime) const
{
	if (!m_kData.iLastUseTime || (unsigned int)(m_kData.iLastUseTime + iCoolTime) < gTime)
	{
		return true;
	}

	return false;
}

bool BeItem::GetCanSell(void) const
{
	if (gTime - m_kData.iBuyTime < 15 * 1000)
	{
		return true;
	}

	if (!m_kData.bCanSell)
	{
		return false;
	}

	return true;
}

void BeItem::SetItemLeveUpState(bool bLevelUp)
{
	m_kData.bLevelUp = bLevelUp;
}

bool BeItem::IsItemLevelUp()
{
	return m_kData.bLevelUp;
}
