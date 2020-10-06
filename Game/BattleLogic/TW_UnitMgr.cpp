/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_UnitMgr.h"

BeUnitMgr::BeUnitMgr(void)
{
}

BeUnitMgr::~BeUnitMgr(void)
{
	Clear();
}

bool BeUnitMgr::Initialize(void)
{
	Clear();

	return BeEntityMgr::Initialize();
}

void BeUnitMgr::Update(int iDeltaTime)
{
	BeEntityMgr::Update(iDeltaTime);

	if (!m_kID2SuspendUnit.IsEmpty())
	{
		for (std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2SuspendUnit.Begin(); itr != m_kID2SuspendUnit.End();)
		{
			std::unordered_map<int, BeUnit*>::Iterator itr1 = itr;
			BeUnit* pkUnit = itr->Value();
			++itr;
			if (pkUnit)
			{
				int iReliveTime = pkUnit->GetUnitReliveTime();
				int iCurTime = gTime;
				if (iReliveTime <= iCurTime)
				{
					m_kID2SuspendUnit.Erase(itr1);
					m_kID2Unit[pkUnit->GetID()] = pkUnit;
					pkUnit->OnResume();

					pkUnit->SetHP(pkUnit->GetMaxHP());
					pkUnit->SetMP(pkUnit->GetMaxMP());
					int iPlayer = pkUnit->GetPlayer();

					BePos2 kPos;
					float fFace = 0.0f;
					gMain.GetHeroOrgPos(iPlayer, kPos, fFace);
					pkUnit->SetAbsFace(fFace);
					float fPosX = pkUnit->GetUD_Float(UDK_UnitRelivePosX, pkUnit->GetPosX());
					float fPosY = pkUnit->GetUD_Float(UDK_UnitRelivePosY, pkUnit->GetPosY());
					pkUnit->PopUD_Float(UDK_UnitRelivePosX);
					pkUnit->PopUD_Float(UDK_UnitRelivePosY);

					gMap.SetUnitPosition(pkUnit, fPosX, fPosY, 0.0f, 600.0f, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);

					TePtParam kParamRelive;
					kParamRelive.SetParam(BTP_pkTrgUnit, pkUnit);

					gTrgMgr.FireTrigger(BTE_HERO_RELIVE, kParamRelive);

					BeTDEventArgs kArgs = BeTDEventArgs();
					kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITREBIRTH;
					kArgs.kUnitRebirth.iUnitID = pkUnit->GetID();
					gLevelMain.GetEventMgr()->FireEvent(kArgs);
					/////////////////////////////////////////////////////////////////////////

					float fHP = pkUnit->GetUD_Float(UDK_UnitReliveMaxHp, pkUnit->GetMaxHP());
					float fMP = pkUnit->GetUD_Float(UDK_UnitReliveMaxMp, pkUnit->GetMaxMP());
					pkUnit->PopUD_Float(UDK_UnitReliveMaxHp);
					pkUnit->PopUD_Float(UDK_UnitReliveMaxMp);
	
					pkUnit->SetHP(fHP);
					pkUnit->SetMP(fMP);
					pkUnit->ResetDeadCount();

					if (pkUnit->HasProperty(UNIT_PROPERTY_NUQIBAR))
					{
						pkUnit->SetMP(0.0f);
					}

					for (int i = 0; i < UNIT_MAX_ITEM; ++i)
					{
						BeItem* pkOrgItem = pkUnit->GetItemByPos(i);
						if (pkOrgItem)
						{
							pkUnit->OnItemUpDate(pkOrgItem->GetID());
						}
					}
					for (int i = 0; i < iMaxHeroSkillNum; i++)
					{
						BeSkill* pkSkill = pkUnit->GetSkillByPos(i);
						if (pkSkill)
						{
							TePtParam kParam;
							kParam.SetParam(BTP_pkTrgUnit, pkUnit);
							kParam.SetParam(BTP_pkSkill, pkSkill);

							gTrgMgr.FireTrigger(BTE_LEARN_SPELL, kParam);
						}
					}

					if (pkUnit->IsHero() && pkUnit->GetPlayer() < MAX_ACTIVEPLAYERS && pkUnit->GetPlayer() >= 0)
					{
						gMain.SetHeroFocusPlayerID(pkUnit->GetPlayer(), -1);
					}

					pkUnit->SetShareUnitChangeFlag(BSUDCF_ACTIONNAME);
				}

				for (int iPos = 0; iPos < UNIT_MAX_ITEM; ++iPos)
				{
					BeItem* pkItem = pkUnit->GetItemByPos(iPos);
					if (pkItem && pkItem->GetBuyInDead())
					{
						pkItem->SetBuyTime(gTime);
					}
				}
			}
			else
			{
				m_kID2SuspendUnit.Erase(itr1);
			}
		}
	}
	m_aiVecRandomHeroID.clear();
	for (std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Begin(); itr != m_kID2Unit.End();)
	{
		BeUnit* pkUnit = itr->Value();
		std::unordered_map<int, BeUnit*>::Iterator itr1 = itr;
		++itr;
		if (pkUnit->HasFlag(BUF_REMOVE))
		{

			if (pkUnit->HasFlag(BUF_RELIVE))
			{
				if (pkUnit->GetDeadCount() > 2)
				{
					int iPlayer = pkUnit->GetPlayer();
					m_kID2Unit.Erase(itr1);
					pkUnit->OnSuspend();
					m_kID2SuspendUnit[pkUnit->GetID()] = pkUnit;
				}
				else
				{
					pkUnit->AddDeadCount();
				}
			}
			else
			{
				if (pkUnit->GetDeadCount() > 2)
				{
					OnUnitDead(pkUnit);
					SafeDeleteUnit(pkUnit);
					BeUnit::DEL(pkUnit);
					pkUnit = NULL;
					m_kID2Unit.Erase(itr1);

				}
				else
				{
					pkUnit->AddDeadCount();
				}
			}
		}
		else if (!pkUnit->IsDead() && pkUnit->GetUnitAllLiveTime() != 0 && pkUnit->GetUnitCurLiveTime() < 0)
		{
			if (pkUnit->IsDividMan() || pkUnit->IsGhost())
			{
				pkUnit->SetFlag(BUF_DEAD | BUF_REMOVE, false);

				TePtParam kParamDeath;
				kParamDeath.SetParam(BTP_pkTrgUnit, pkUnit);

				gTrgMgr.FireTrigger(BTE_UNIT_TIMEDEATH, kParamDeath);
			}
			else if (pkUnit->IsSummonTemp())
			{
				pkUnit->OperateUnitDead(BAT_ABS, 0.0f, NULL, -1, 0, true);

				TePtParam kParamDeath;
				kParamDeath.SetParam(BTP_pkTrgUnit, pkUnit);

				gTrgMgr.FireTrigger(BTE_UNIT_TIMEDEATH, kParamDeath);
			}
			else
			{
				pkUnit->OperateUnitDead(BAT_ABS, 0.0f, NULL, -1, 0, true);
			}
		}
		else
		{
			pkUnit->Update(iDeltaTime);
		}
	}
}

void BeUnitMgr::Finialize(void)
{
	Clear();

	BeEntityMgr::Finialize();
}

void BeUnitMgr::Clear()
{
	if (!m_kID2Unit.IsEmpty())
	{
		for (std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Begin(); itr != m_kID2Unit.End();)
		{
			SafeDeleteUnit(itr->Value());
			BeUnit::DEL(itr->Value());
			itr->Value() = NULL;
			itr = m_kID2Unit.Erase(itr);
		}
		m_kID2Unit.Clear();
	}

	if (!m_kID2SuspendUnit.IsEmpty())
	{
		for (std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2SuspendUnit.Begin(); itr != m_kID2SuspendUnit.End();)
		{
			SafeDeleteUnit(itr->Value());
			BeUnit::DEL(itr->Value());
			itr->Value() = NULL;
			itr = m_kID2SuspendUnit.Erase(itr);
		}
		m_kID2SuspendUnit.Clear();
	}

	m_kVPureNeedUpdateID.Clear();
	m_kPureDelID.Clear();
	m_kPureDelHeroBossBuildID.Clear();
	m_kPureNewBossID.Clear();
	m_kLoseVisionUnitGroup.clear();
}

BeUnit* BeUnitMgr::NewUnit(int iID)
{
	BeUnit* pkUnit = BeUnit::NEW(iID);
	pkUnit->AttachMain(pkAttachMain);
	return pkUnit;
}

BeUnit* BeUnitMgr::AddUnit(int iTypeID, int iSkillLevel, int iPlayer, int iUnitID, int iESLabel, int iSkinIdx)
{
	if (!iTypeID)
	{
		return NULL;
	}

	int iID = iUnitID;
	if (iID == -1)
	{
		iID = gMain.GenerateID(GIT_ENTITY);
	}
	else
	{
		gMain.SetGenerateID(GIT_ENTITY, iID);
	}

	BeUnit* pkUnit = NewUnit(iID);

	gMain.AddEntityPointer(GIT_ENTITY, iID, pkUnit);

	m_kID2Unit[iID] = pkUnit;
	pkUnit->SetOrgSkillLevel(iSkillLevel);
	if (iPlayer != -1)
	{
		pkUnit->SetPlayer(iPlayer);
	}
	if (pkUnit->Initialize(iTypeID))
	{
		pkUnit->UpdateAttribute();
		pkUnit->AppyUnitSkill();

		if (iPlayer != -1)
		{
			pkUnit->SetPlayer(iPlayer);
		}

		EsHandle rkHandle(BTOT_UNIT, pkUnit->GetID(), 0);
		pkUnit->SetHandle(rkHandle);

		if (pkUnit->IsHero())
		{
			if (iPlayer != -1 && gMain.GetHeroID(iPlayer) == 0)
			{
				m_kVHeroPtr.PushBack(pkUnit);
				//				pkUnit->SetClientFlag(BUCF_PLAYER_HERO);
			}

			m_kVAllHeroID.PushBack(pkUnit->GetID());
		}

		TePtParam kParam;
		kParam.SetParam(BTP_pkTrgUnit, pkUnit);

		gTrgMgr.FireTrigger(BTE_UNIT_ADDUNIT, kParam);

		BeLevelMain* pkTriggerMain = (BeLevelMain*)gMain.GetTaskHandle();
		EsAssert(pkTriggerMain);

		pkTriggerMain->GetLabelMgr()->AddObjLabel(pkUnit, iESLabel);

		pkTriggerMain->GetLabelMgr()->AutoLabelObj(pkUnit);

		BeTDEventArgs kArgs = BeTDEventArgs();
		kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITADD;
		kArgs.kUnitAdd.iUnitID = pkUnit->GetID();
		pkTriggerMain->GetEventMgr()->FireEvent(kArgs);

		return pkUnit;
	}
	else
	{
		std::unordered_map<int, BeUnit*>::Iterator it = m_kID2Unit.Find(iID);
		if (it != m_kID2Unit.End())
		{
			m_kID2Unit.Erase(it);
		}

		SafeDeleteUnit(pkUnit);
		BeUnit::DEL(pkUnit);
		pkUnit = NULL;
		return NULL;
	}

	return pkUnit;
}

void BeUnitMgr::OnDelUnit(BeUnit* pkUnit)
{
}

void BeUnitMgr::OnUnitDead(BeUnit* pkUnit)
{
}

void BeUnitMgr::DelUnit(int iID)
{
	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Find(iID);
	if (itr != m_kID2Unit.End())
	{
		BeUnit* pkUnit = itr->Value();
		if (pkUnit)
		{
			TePtParam kParamDel;
			kParamDel.SetParam(BTP_pkTrgUnit, pkUnit);

			gTrgMgr.FireTrigger(BTE_UNIT_DELUNIT, kParamDel);
			//////////////////////////////////////////////////////////////////////////
			OnDelUnit(pkUnit);
			pkUnit->SetFlag(BUF_REMOVE);
		}
		else
		{
			m_kID2Unit.Erase(itr);
		}
	}
	else
	{
		itr = m_kID2SuspendUnit.Find(iID);
		if (itr != m_kID2SuspendUnit.End())
		{
			BeUnit* pkUnit = itr->Value();
			if (pkUnit)
			{
				OnDelUnit(pkUnit);
				pkUnit->SetFlag(BUF_REMOVE);
			}
			else
			{
				m_kID2SuspendUnit.Erase(itr);
			}
		}
	}
}

void BeUnitMgr::DelUnitIterator(int iID)
{
	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Find(iID);
	if (itr != m_kID2Unit.End())
	{
		m_kID2Unit.Erase(itr);
	}
	else
	{
		itr = m_kID2SuspendUnit.Find(iID);
		if (itr != m_kID2SuspendUnit.End())
		{
			m_kID2SuspendUnit.Erase(itr);
		}
	}
}

void BeUnitMgr::SafeDeleteUnit(BeUnit*& pkUnit)
{
	if (!pkUnit)
	{
		return;
	}

	int iID = pkUnit->GetID();

	pkUnit->OnDelete();
}

BeUnit* BeUnitMgr::GetUnitByID(int iID, bool bSuspend, bool bSoul)
{
	if (iID == 0)
	{
		return NULL;
	}

	return (BeUnit*)(gMain.GetEntityPointer(GIT_ENTITY, iID));

	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Find(iID);
	if (itr == m_kID2Unit.End())
	{
		if (bSuspend)
		{
			std::unordered_map<int, BeUnit*>::Iterator itr2 = m_kID2SuspendUnit.Find(iID);
			if (itr2 == m_kID2SuspendUnit.End())
			{
				return NULL;
			}
			return itr2->Value();
		}
		return NULL;
	}

	if (bSoul)
	{
		return itr->Value();
	}
	else
	{
		BeUnit* pkUnit = itr->Value();
		return pkUnit;
	}
}

void BeUnitMgr::GetBlockAreaGroup(UnitGroup& kGroup, float fX1, float fY1, float fX2, float fY2, int iPlayerIdx, int iFlag)
{
	kGroup.Clear();

	int iPlayerGroup = -1;
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	float fX = (fX1 + fX2) / 2.0f;
	float fY = (fY1 + fY2) / 2.0f;
	float fDX = fabs(fX - fX1);
	float fDY = fabs(fY - fY1);
	float fRadius = max(fDX, fDY);
	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	for (int y = iBY; y <= iEY; y++)
	{
		for (int x = iBX; x <= iEX; x++)
		{
			BeElement* pkHead = &m_akBlock[m_iBlocksW * y + x];
			BeUnit* pkUnit = (BeUnit*)pkHead->pkNext;
			if (!pkUnit)
			{
				continue;
			}
			while (pkUnit != pkHead)
			{
				while (true)
				{
					if (fabs(pkUnit->GetPosX() - fX) > fDX || fabs(pkUnit->GetPosY() - fY) > fDY)
					{
						break;
					}

					if (!IsPassUnit(pkUnit, iPlayerGroup, iFlag))
					{
						break;
					}

					kGroup.PushBack(pkUnit);
					break;
				}
				pkUnit = (BeUnit*)pkUnit->pkNext;
			}
		}
	}
}

void BeUnitMgr::GetFanAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fB, float fE, int iPlayerIdx, int iFlag)
{
	kGroup.Clear();

	if (fE > 2.0f * D3DX_PI)
	{
		fB -= 2.0f * D3DX_PI;
		fE -= 2.0f * D3DX_PI;
	}

	int iPlayerGroup = -1;
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	fRadius *= fRadius;
	for (int y = iBY; y <= iEY; y++)
	{
		for (int x = iBX; x <= iEX; x++)
		{
			BeElement* pkHead = &m_akBlock[m_iBlocksW * y + x];
			BeUnit* pkUnit = (BeUnit*)pkHead->pkNext;
			if (!pkUnit)
			{
				continue;
			}
			while (pkUnit != pkHead)
			{
				while (true)
				{
					if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius)
					{
						if (!IsPassUnit(pkUnit, iPlayerGroup, iFlag))
						{
							break;
						}

						float fAngle = atan2f(pkUnit->GetPosY() - fY, pkUnit->GetPosX() - fX);
						if (fAngle >= fB && fAngle <= fE)
						{
							kGroup.PushBack(pkUnit);
							break;
						}

						fAngle -= 2 * D3DX_PI;
						if (fAngle >= fB && fAngle <= fE)
						{
							kGroup.PushBack(pkUnit);
							break;
						}
					}
					break;
				}

				pkUnit = (BeUnit*)pkUnit->pkNext;
			}
		}
	}
}

int	BeUnitMgr::GetUnitNums()
{
	return (int)m_kID2Unit.Size();
}

void BeUnitMgr::GetAllMapGroup(UnitGroup& kGroup, int iPlayerIdx, int iFlag)
{
	kGroup.Clear();

	int iPlayerGroup = -1;
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Begin();
	for (; itr != m_kID2Unit.End(); ++itr)
	{
		BeUnit* pkUnit = itr->Value();
		if (!pkUnit || !IsPassUnit(pkUnit, iPlayerGroup, iFlag))
		{
			continue;
		}

		kGroup.PushBack(pkUnit);
	}
}

void BeUnitMgr::GetAllMapGroup(UnitGroup& kGroup, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData)
{
	if (!pkSrcUnit)
	{
		return;
	}
	kGroup.Clear();

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	const SkillTable* pkRes = NULL;
	if (eCommand == BCT_SPELL)
	{
		pkRes = gMain.GetResSkill(iData);
	}
	else if (eCommand == BCT_USE_ITEM)
	{
	}
	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Begin();
	for (; itr != m_kID2Unit.End(); ++itr)
	{
		BeUnit* pkUnit = itr->Value();
		if (!pkUnit)
		{
			continue;
		}
		bool bPass = false;
		switch (eCommand)
		{
		case BCT_STOP:
		case BCT_HOLD:
		case BCT_PATROL:
		case BCT_ATTACK:
			if (pkSrcUnit->GetCamp() != pkUnit->GetCamp())
			{
				bPass = pkSrcUnit->UnitCanAttack(pkUnit, true);
			}
			break;
		case BCT_SPELL:
			bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
			break;
		case BCT_USE_ITEM:
			bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
			break;
		default:
			break;
		}
		if (bPass)
		{
			kGroup.PushBack(pkUnit);
		}
	}
}

const UnitGroup& BeUnitMgr::GetAllHeroPtr()
{
	return m_kVHeroPtr;
}

const UnitGroupID& BeUnitMgr::GetAllHeroID()
{
	return m_kVAllHeroID;
}

void BeUnitMgr::GetUnitGroupByID(UnitGroup& kGroup, int iID)
{
	kGroup.Clear();

	BeUnit* pkUnit = NULL;
	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Begin();
	for (; itr != m_kID2Unit.End(); ++itr)
	{
		pkUnit = itr->Value();
		if (!pkUnit)
		{
			continue;
		}
		if (pkUnit->IsDead())
		{
			continue;
		}

		if (iID != pkUnit->GetTypeID())
		{
			continue;
		}
		kGroup.PushBack(pkUnit);
	}
}

void BeUnitMgr::GetUnitGroupByControl(UnitGroup& kGroup, int iIdx, bool bSuspend, bool bBPTPlayer, bool bSpecial)
{
	if (iIdx > MAX_ACTIVEPLAYERS || iIdx < 0)
	{
		return;
	}

	kGroup.Clear();

	BeUnit* pkUnit = NULL;
	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2Unit.Begin();
	for (; itr != m_kID2Unit.End(); ++itr)
	{
		pkUnit = itr->Value();
		if (!pkUnit)
		{
			continue;
		}
		if (pkUnit->IsDead() && !bSuspend)
		{
			continue;
		}

		if (pkUnit->UnitCanBeControl(iIdx) && pkUnit->GetPlayer() != -1)
		{
			if (!bBPTPlayer && pkUnit->GetPlayer() != iIdx && pkUnit->GetPlayer() < MAX_ACTIVEPLAYERS)
			{
				continue;
			}

			kGroup.PushBack(pkUnit);
		}
	}

	if (bSuspend)
	{
		pkUnit = NULL;
		std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2SuspendUnit.Begin();
		for (; itr != m_kID2SuspendUnit.End(); ++itr)
		{
			pkUnit = itr->Value();
			if (!pkUnit)
			{
				continue;
			}

			bool bHas = false;
			for (int i = 0; i < (int)kGroup.Size(); i++)
			{
				BeUnit* pkUnitInGroup = kGroup[i];
				if (pkUnitInGroup && pkUnitInGroup->GetID() == pkUnit->GetID())
				{
					bHas = true;
					break;
				}
			}

			if (bHas)
			{
				continue;
			}

			if (pkUnit->UnitCanBeControl(iIdx) && pkUnit->GetPlayer() != -1)
			{
				if (!bBPTPlayer && pkUnit->GetPlayer() != iIdx && pkUnit->GetPlayer() < MAX_ACTIVEPLAYERS)
				{
					continue;
				}

				kGroup.PushBack(pkUnit);
			}
		}
	}
}

bool BeUnitMgr::IsPassUnit(BeUnit* pkUnit, int iPlayerGroup, int iFlag, bool bDead) const
{
	if (!pkUnit || (pkUnit->IsDead() && !bDead))
	{
		return false;
	}

	int iUnitFlag = 0;

	if ((iUnitFlag & iFlag) == iUnitFlag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BeUnitMgr::GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, int iPlayerIdx, int iFlag, bool bDead) const
{
	kGroup.Clear();

	int iPlayerGroup = -1;
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	for (int y = iBY; y <= iEY; y++)
	{
		for (int x = iBX; x <= iEX; x++)
		{
			BeElement* pkHead = &m_akBlock[m_iBlocksW * y + x];
			BeUnit* pkUnit = (BeUnit*)pkHead->pkNext;
			if (!pkUnit)
			{
				continue;
			}
			while (pkUnit != pkHead)
			{
				float fRadius2 = (fRadius) * (fRadius);
				if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
				{
					if (IsPassUnit(pkUnit, iPlayerGroup, iFlag, bDead))
					{
						kGroup.PushBack(pkUnit);
					}
				}
				pkUnit = (BeUnit*)pkUnit->pkNext;
			}
		}
	}
}

void BeUnitMgr::GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, int iDynamicProperty, int iStaticProperty) const
{
	kGroup.Clear();

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	for (int y = iBY; y <= iEY; y++)
	{
		for (int x = iBX; x <= iEX; x++)
		{
			BeElement* pkHead = &m_akBlock[m_iBlocksW * y + x];
			BeUnit* pkUnit = (BeUnit*)pkHead->pkNext;
			if (!pkUnit)
			{
				continue;
			}
			while (pkUnit != pkHead)
			{
				float fRadius2 = (fRadius) * (fRadius);
				if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
				{
					bool bPass = pkSrcUnit->IsTargetUnit(pkUnit, iStaticProperty, iDynamicProperty);
					if (bPass)
					{
						kGroup.PushBack(pkUnit);
					}
				}
				pkUnit = (BeUnit*)pkUnit->pkNext;
			}
		}
	}
}

void BeUnitMgr::GetAreaGroupByAttackedType(UnitGroup& kGroup, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, int iAttackedType) const
{
	kGroup.Clear();

	int iSkillTargetFlag = iAttackedType;
	int iStaticProcFlag = 0;
	int iDynaPropFlag = 0;
	pkSrcUnit->TranslateSkillTargetType(iSkillTargetFlag, iStaticProcFlag, iDynaPropFlag);

	GetAreaGroup(kGroup, fX, fY, fRadius, pkSrcUnit, iDynaPropFlag, iStaticProcFlag);
}

void BeUnitMgr::GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, int iDynamicProperty, int iStaticProperty) const
{
	rkGroupID.Clear();

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	for (int y = iBY; y <= iEY; y++)
	{
		for (int x = iBX; x <= iEX; x++)
		{
			BeElement* pkHead = &m_akBlock[m_iBlocksW * y + x];
			BeUnit* pkUnit = (BeUnit*)pkHead->pkNext;
			if (!pkUnit)
			{
				continue;
			}
			while (pkUnit != pkHead)
			{
				float fRadius2 = (fRadius) * (fRadius);
				if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
				{
					bool bPass = pkSrcUnit->IsTargetUnit(pkUnit, iStaticProperty, iDynamicProperty);
					if (bPass)
					{
						rkGroupID.PushBack(pkUnit->GetID());
					}
				}
				pkUnit = (BeUnit*)pkUnit->pkNext;
			}
		}
	}
}

void BeUnitMgr::GetAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData) const
{
	kGroup.Clear();
	if (!pkSrcUnit)
	{
		return;
	}

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}
	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	const SkillTable* pkRes = NULL;

	if (eCommand == BCT_SPELL)
	{
		pkRes = gMain.GetResSkill(iData);
	}
	else if (eCommand == BCT_USE_ITEM)
	{
		const ItemTable* pkItemRes = gMain.GetResItem(iData);
		if (!pkItemRes)
		{
			return;
		}

		int iSkillTypeID = 0;
	}

	for (int y = iBY; y <= iEY; y++)
	{
		for (int x = iBX; x <= iEX; x++)
		{
			BeElement* pkHead = &m_akBlock[m_iBlocksW * y + x];
			BeUnit* pkUnit = (BeUnit*)pkHead->pkNext;
			if (!pkUnit)
			{
				continue;
			}
			while (pkUnit != pkHead)
			{
				float fRadius2 = (fRadius + pkUnit->GetCollision()) * (fRadius + pkUnit->GetCollision());
				float fDis2 = GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY);
				if (fDis2 <= fRadius2)
				{
					bool bPass = false;
					switch (eCommand)
					{
					case BCT_STOP:
					case BCT_HOLD:
						// case BCT_MOVE:
					case BCT_PATROL:
					case BCT_ATTACK:
						if (pkSrcUnit->GetCamp() != pkUnit->GetCamp() && pkUnit->GetCamp() != SRPC_NEUTRAL_ALLY)
						{
							bPass = pkSrcUnit->UnitCanAttack(pkUnit, true);
						}
						break;
					case BCT_SPELL:
						bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
						break;
					case BCT_USE_ITEM:
						bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
						break;
					default:
						break;
					}
					if (bPass)
					{
						kGroup.PushBack(pkUnit);
					}
				}
				pkUnit = (BeUnit*)pkUnit->pkNext;
			}
		}
	}
}

void	BeUnitMgr::GetRectangleAreaGroup(UnitGroup& kGroup, float fX, float fY, float fRadius, float fDis, float fFace, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData) const
{
	kGroup.Clear();
	UnitGroup kFirstGroup;
	GetAreaGroup(kFirstGroup, fX, fY, fDis, pkSrcUnit, eCommand, iData);

	float	fHalfPI = D3DX_PI / 2.0f;
	float	fDeltaX = fRadius * cosf(fHalfPI - fFace);
	float	fDeltaY = fRadius * sinf(fHalfPI - fFace);
	float	fX0 = fX - fDeltaX;
	float	fY0 = fY + fDeltaY;
	float	fX1 = fX0 + fDis * cosf(fFace);
	float	fY1 = fY0 + fDis * sinf(fFace);
	float	fX3 = fX + fDeltaX;
	float	fY3 = fY - fDeltaY;
	float	fX2 = fX3 + fDis * cosf(fFace);
	float	fY2 = fY3 + fDis * sinf(fFace);

	for (int i = 0; i < kFirstGroup.Size(); i++)
	{
		BeUnit* pkTarget = kFirstGroup[i];
		if (pkTarget)
		{
			if (IsInQuadrangle(fX0, fY0, fX1, fY1, fX2, fY2, fX3, fY3, pkTarget->GetPosX(), pkTarget->GetPosY()))
			{
				kGroup.PushBack(pkTarget);
			}
		}
	}
}

void BeUnitMgr::GetAreaGroupID(UnitGroupID& rkGroupID, float fX, float fY, float fRadius, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData) const
{
	rkGroupID.Clear();
	if (!pkSrcUnit)
	{
		return;
	}

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	int iBX, iBY, iEX, iEY;
	GetBlockArea(fX, fY, fRadius, iBX, iBY, iEX, iEY);

	const SkillTable* pkRes = 0;

	if (eCommand == BCT_SPELL)
	{
		pkRes = gMain.GetResSkill(iData);
	}
	else if (eCommand == BCT_USE_ITEM)
	{
		const ItemTable* pkItemRes = gMain.GetResItem(iData);
		if (!pkItemRes)
		{
			return;
		}

		int iSkillTypeID = 0;
	}

	for (int y = iBY; y <= iEY; y++)
	{
		for (int x = iBX; x <= iEX; x++)
		{
			BeElement* pkHead = &m_akBlock[m_iBlocksW * y + x];
			BeUnit* pkUnit = (BeUnit*)pkHead->pkNext;
			if (!pkUnit)
			{
				continue;
			}
			while (pkUnit != pkHead)
			{
				float fRadius2 = (fRadius) * (fRadius);
				if (GetDistance2(pkUnit->GetPosX(), pkUnit->GetPosY(), fX, fY) <= fRadius2)
				{
					bool bPass = false;
					switch (eCommand)
					{
					case BCT_STOP:
					case BCT_HOLD:
						// case BCT_MOVE:
					case BCT_PATROL:
					case BCT_ATTACK:
						if (pkSrcUnit->GetCamp() != pkUnit->GetCamp() && pkUnit->GetCamp() != SRPC_NEUTRAL_ALLY)
						{
							bPass = pkSrcUnit->UnitCanAttack(pkUnit, true);
						}
						break;
					case BCT_SPELL:
						bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
						break;
					case BCT_USE_ITEM:
						bPass = pkSrcUnit->IsSkillTargetType(pkRes, pkUnit);
						break;
					default:
						break;
					}
					if (bPass)
					{
						rkGroupID.PushBack(pkUnit->GetID());
					}
				}
				pkUnit = (BeUnit*)pkUnit->pkNext;
			}
		}
	}
}

BeUnit* BeUnitMgr::GetGroupFarestUnit(const UnitGroup& kGroup, float fPosX, float fPosY) const
{
	BeUnit* pkFarest = NULL;
	float fFarestDis = 0.0f;
	if (!kGroup.IsEmpty())
	{
		for (int i = 0; i < (int)kGroup.Size(); ++i)
		{
			BeUnit* pkNearUnit = kGroup[i];
			if (pkNearUnit && !pkNearUnit->IsDead())
			{
				float fDis = GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fPosX, fPosY);
				if (fFarestDis == 0.0f)
				{
					fFarestDis = fDis;
					pkFarest = pkNearUnit;
				}
				else
				{
					if (fFarestDis < fDis)
					{
						fFarestDis = fDis;
						pkFarest = pkNearUnit;
					}
				}
			}
		}
		return pkFarest;
	}
	else
	{
		return NULL;
	}
}

BeUnit* BeUnitMgr::GetGroupNearestUnit(const UnitGroup& kGroup, float fPosX, float fPosY, float fDistance, float fRefX, float fRefY) const
{
	BeUnit* pkNearestUnit = NULL;
	float fNearestDis = 0.0f;
	if (!kGroup.IsEmpty())
	{
		for (int i = 0; i < (int)kGroup.Size(); ++i)
		{
			BeUnit* pkNearUnit = kGroup[i];
			if (pkNearUnit && !pkNearUnit->IsDead())
			{
				if (fDistance > 0 && GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fRefX, fRefY) > fDistance * fDistance)
				{
					continue;
				}

				float fDis = GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fPosX, fPosY);
				if (fNearestDis == 0.0f)
				{
					fNearestDis = fDis;
					pkNearestUnit = pkNearUnit;
				}
				else
				{
					if (fNearestDis > fDis)
					{
						fNearestDis = fDis;
						pkNearestUnit = pkNearUnit;
					}
				}
			}
		}
		return pkNearestUnit;
	}
	else
	{
		return NULL;
	}
}

int BeUnitMgr::GetGroupNearestUnit(const UnitGroupID& kGroupID, float fPosX, float fPosY) const
{
	int iNearestUnitID = 0;
	float fNearestDis = 0.0f;
	if (!kGroupID.IsEmpty())
	{
		for (int i = 0; i < (int)kGroupID.Size(); ++i)
		{
			BeUnit* pkNearUnit = gUnitMgr.GetUnitByID(kGroupID[i]);
			if (pkNearUnit && !pkNearUnit->IsDead())
			{
				float fDis = GetDistance2(pkNearUnit->GetPosX(), pkNearUnit->GetPosY(), fPosX, fPosY);
				if (fNearestDis == 0.0f)
				{
					fNearestDis = fDis;
					iNearestUnitID = pkNearUnit->GetID();
				}
				else
				{
					if (fNearestDis > fDis)
					{
						fNearestDis = fDis;
						iNearestUnitID = pkNearUnit->GetID();
					}
				}
			}
		}
		return iNearestUnitID;
	}
	else
	{
		return 0;
	}
}

int	BeUnitMgr::GetUnitIDFromSuspend(int ITypeID)
{
	if (m_kID2SuspendUnit.IsEmpty())
	{
		return 0;
	}

	std::unordered_map<int, BeUnit*>::Iterator itr = m_kID2SuspendUnit.Begin();
	for (; itr != m_kID2SuspendUnit.End(); ++itr)
	{
		BeUnit* pkUnit = itr->Value();
		if (pkUnit && pkUnit->GetTypeID() == ITypeID)
		{
			return pkUnit->GetID();
		}
	}

	return 0;
}

const std::unordered_map<int, BeUnit*>& BeUnitMgr::GetID2Unit() const
{
	return m_kID2Unit;
}

const std::unordered_map<int, BeUnit*>& BeUnitMgr::GetID2SuspendUnit() const
{
	return m_kID2SuspendUnit;
}

bool BeUnitMgr::GetUnitByTypeID(UnitGroup& kGroup, int iTypeID) const
{
	for (std::unordered_map<int, BeUnit*>::ConstIterator itr = m_kID2Unit.Begin(); itr != m_kID2Unit.End(); ++itr)
	{
		BeUnit* pkUnit = itr->Value();
		if (pkUnit && !pkUnit->IsDead())
		{
			if (iTypeID == pkUnit->GetTypeID())
			{
				kGroup.PushBack(pkUnit);
			}
		}
	}
	return true;
}
bool BeUnitMgr::GetUnitByPlayer(UnitGroup& kGroup, const std::string& kPlayerName) const
{
	return true;
}

void BeUnitMgr::GetUnitByCommand(UnitGroup& kGroup, const BeUnit* pkSrcUnit, BeCommandType eCommand, int iData) const
{
	kGroup.Clear();
	if (!pkSrcUnit)
	{
		return;
	}

	int iPlayerGroup = -1;
	int iPlayerIdx = pkSrcUnit->GetPlayer();
	if (iPlayerIdx != -1)
	{
		iPlayerGroup = gMain.GetPlayerCamp(iPlayerIdx);
	}

	if (eCommand == BCT_SPELL)
	{
	}
}

void BeUnitMgr::PushNeedUpdateUnitID(int iID)
{
}

void BeUnitMgr::ClrPureData()
{
	m_kVPureNeedUpdateID.Clear();
	m_kPureDelID.Clear();
	m_kPureDelHeroBossBuildID.Clear();
	m_kPureNewBossID.Clear();
	m_kLoseVisionUnitGroup.clear();
}

const UnitGroupID& BeUnitMgr::PureGetAllNeedUpdateUnitID()
{
	return m_kVPureNeedUpdateID;
}

const UnitGroupID& BeUnitMgr::PureGetDelUnitID(int iCamp)
{
	return m_kLoseVisionUnitGroup[iCamp];
}

void BeUnitMgr::PushNeedDelNormalUnit(int iCamp, int iID)
{
	m_kLoseVisionUnitGroup[iCamp].PushBack(iID);
}

void BeUnitMgr::PushNeedDelHeroBossBuilding(int iID)
{
	{
		m_kPureDelHeroBossBuildID.PushBack(iID);
	}
}

const UnitGroupID& BeUnitMgr::PureGetDelBoosBuildingID()
{
	return m_kPureDelHeroBossBuildID;
}

const UnitGroupID& BeUnitMgr::PureGetNewBossID()
{
	return m_kPureNewBossID;
}

void BeUnitMgr::PushNewBossID(int iID)
{
	{
		m_kPureNewBossID.PushBack(iID);
	}
}
