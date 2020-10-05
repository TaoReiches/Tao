/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Item.h"

BeCarrySkill::BeCarrySkill(int iID) : BeCarry(iID)
{
}

BeCarrySkill::~BeCarrySkill()
{
	for (int i = 0; i < (int)m_akSkill.size(); ++i)
	{
		BeSkill* pkSkill = m_akSkill[i];
		if (pkSkill)
		{
			BeSkill::DEL(pkSkill);
		}
	}
	m_akSkill.clear();
}

const std::vector<BeSkill*>& BeCarrySkill::GetAllSkill(void) const
{
	return m_akSkill;
}

BeItem::BeItem(int iID) : BeCarrySkill(iID)
{
	m_eType = BCT_ITEM;
	m_pkRes = NULL;
	m_pkZRes = NULL;
	m_iSkillCDTime = 0;
	m_iSkillTypeID = 0;
	m_iUniqueID = 0;
	m_bComposeFlag = false;
}

BeItem::~BeItem(void)
{
}

bool BeItem::Initialize(int iTypeID)
{
	BeCarry::Initialize(iTypeID);
	m_pkRes = gMain.GetResItem(iTypeID);
	m_pkZRes = gMain.GetResZhanChangItem(iTypeID);

	return true;
}

void	BeItem::InitItemSkill(void)
{
	if (!m_pkZRes && gMain.IsPveMap())
	{
		return;
	}
	else if (!m_pkRes && !gMain.IsPveMap())
	{
		return;
	}

	ClearAttr();
	if (!m_kData.iItemTypeID)
	{
		if (gMain.IsPveMap())
		{
			AddProperty(m_pkZRes->uiItemProperty);
		}
		else
		{
			AddProperty(m_pkRes->uiItemProperty);
		}
		m_kData.iItemTypeID = m_iTypeID;
	}
	for (int i = 0; i < (int)m_akSkill.size(); ++i)
	{
		BeSkill* pkSkill = m_akSkill[i];
		if (pkSkill)
		{
			BeSkill::DEL(pkSkill);
		}
	}
	m_akSkill.clear();

	int		iLastAddSkill = 0;
	for (int i = 0; i < 6; i++)
	{
		int iTypeID = 0;
		if (gMain.IsPveMap())
		{
			iTypeID = m_pkZRes->iItemSkill[i];
		}
		else
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

		BeSkill* pkSkill = BeSkill::NEW(gMain.GenerateID(GIT_CARRY));
		pkSkill->AttachMain(pkAttachMain);
		pkSkill->AttachUnit(pkAttachUnit);
		if (pkSkill->Initialize(iTypeID))
		{
			m_akSkill.push_back(pkSkill);
			InitAttrFromSkill(iTypeID, pkSkill->GetLevel());

			m_iSkillCDTime = pkSkill->GetResPtr()->iCoolDown[0];

			if (pkSkill->GetOperateType() != SKILL_OPERATETYPE_BEIDONG)
			{
				float	fST15ForItem = gUnit.GetUD_Float(UDK_ST15_ItemTime);
				if (fST15ForItem > 0)
				{
					m_iSkillCDTime = m_iSkillCDTime * (1.0f - fST15ForItem);
				}
				m_iSkillTypeID = iTypeID;
			}
		}
		else
		{
			BeSkill::DEL(pkSkill);
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

BeSkill* BeItem::GetSkillByTypeID(int iSkillTypeID)
{
	for (int i = 0; i < (int)m_akSkill.size(); ++i)
	{
		BeSkill* pkSkill = m_akSkill[i];
		if (pkSkill && pkSkill->GetTypeID() == iSkillTypeID)
		{
			return pkSkill;
		}
	}

	return NULL;
}

void BeItem::Update(BeUnit* pkUnit, int iDeltaTime)
{
	if (!pkUnit)
	{
		return;
	}
	if (gMain.IsPveMap())
	{
		const ZhanChangItemTable* pkItemRes = GetZhanChangResPtr();
		if (!pkItemRes)
		{
			return;
		}

		for (int iSkill = 0; iSkill < (int)m_akSkill.size(); ++iSkill)
		{
			BeSkill* pkSkill = m_akSkill[iSkill];
			if (!pkSkill)
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
	else
	{
		const ItemTable* pkItemRes = GetResPtr();
		if (!pkItemRes)
		{
			return;
		}

		for (int iSkill = 0; iSkill < (int)m_akSkill.size(); ++iSkill)
		{
			BeSkill* pkSkill = m_akSkill[iSkill];
			if (!pkSkill)
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
	if (gData.IsWTF())
	{
		return true;
	}
	if (!m_kData.iLastUseTime || (UInt)(m_kData.iLastUseTime + iCoolTime) < gTime)
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

int BeItem::GetUseSkillTypeID(void) const
{
	int iSkillTypeID(0);

	return iSkillTypeID;
}

IMPLEMENT_POOL1(BeEquip, 256, 64);
BeEquip::BeEquip(int iID) : BeCarrySkill(iID)
{
	m_eType = BCT_EQUIP;
	m_iSuitNumber = 0;
	m_iRandomID = 0;
}

BeEquip::~BeEquip(void)
{
}

void BeEquip::SetSuitNumber(int iNumber)
{
	m_iSuitNumber = iNumber;
}

void BeEquip::SetRandomID(int iID)
{
	m_iRandomID = iID;
}

bool BeEquip::Initialize(int iTypeID)
{
	m_iTypeID = iTypeID;

	return false;
}

bool BeEquip::Strengthen(int iCardID, int iStrengthenID)
{
	return true;
}

bool BeItem::IsUnitZhuanShu(BeUnit* pkUnit)
{
	if (!pkUnit)
	{
		return false;
	}

	return false;
}

void BeItem::SetItemLeveUpState(bool bLevelUp)
{
	m_kData.bLevelUp = bLevelUp;
}

bool BeItem::IsItemLevelUp()
{
	return m_kData.bLevelUp;
}

void BeItem::CheckItemSkillCD(BeUnit* pkUnit)
{
	if (!pkUnit)
	{
		return;
	}

	for (int i = 0; i < (int)m_akSkill.size(); ++i)
	{
		BeSkill* pkSkill = m_akSkill[i];
		if (pkSkill)
		{
		}
	}
}
