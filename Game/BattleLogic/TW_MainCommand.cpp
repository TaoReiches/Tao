/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Main.h"

bool BeMain::UnitSelectHero(const BeSelectHero& kCmd, bool bSetSel)
{
	int iTypeID = kCmd.iUnitTypeID;
	int iPlayIdx = kCmd.iPlayer;
	if (iPlayIdx < 0 || iPlayIdx >= MAX_PLAYERS)
	{
		return false;
	}

	BeUnit* pkUpHero = GetHeroBySeat(iPlayIdx);
	if (pkUpHero)
	{
		BeAttackingAttr kAttr;
		kAttr.kAttacker = pkUpHero->GetSharePtr();
		kAttr.fDamage = 99999999.0f;
		pkUpHero->OnBeDamaged(kAttr);

		pkUpHero->ClrFlag(BUF_RELIVE);
	}

	BeUnit* pkUnit = m_pkUnitMgr->AddUnit(iTypeID, -1, iPlayIdx);
	if (pkUnit)
	{
		pkUnit->AISetControlType(BACT_PLAYER);

		const MapInfoTable* pkMapInfo = GetMapInfoRes(m_iMapID);
		if (!pkMapInfo || !m_pkMap)
		{
			return false;
		}
		int iPlayer = pkUnit->GetPlayer();
		BePos2 kOrgPos;
		float fOrgFace = 0.0f;
		gMain.GetHeroOrgPos(iPlayIdx, kOrgPos, fOrgFace);
		m_pkMap->SetUnitPosition(pkUnit, kOrgPos.fX, kOrgPos.fY, 0.0f, 1000.0f, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);
		pkUnit->SetAbsFace(fOrgFace);
		pkUnit->SetLevel(1);
		SetHeroID(iPlayIdx, pkUnit->GetID());

		if (!m_pkDataMgr->GetIsMoreHeros())
		{
			m_pkDataMgr->SetHasCreateHero(iPlayIdx, true);
		}

		return true;
	}

	return false;
}

bool BeMain::CheatCommand(const BeCheatCommand& kCmd)
{
	std::string kCheatCommand = kCmd.acCheatCommand;
	int iPlayer = kCmd.iCheatPlayer;

	float fData = kCmd.fCheatData;
	int iData = kCmd.iCheatData;

	if (kCheatCommand == "H")
	{
		UnitSelectHero(BeSelectHero(iData, iPlayer));
	}
	else if (kCheatCommand.find("TUOGUAN") != std::string::npos)
	{
		if (gMain.GetPlayerType(iPlayer) == BPT_PLAYER)
		{
			gMain.SetPlayerType(iPlayer, BPT_ROBOT);
			BeUnit* pkHero = gUnitMgr.GetUnitByID(gMain.GetHeroID(iPlayer));
			if (pkHero)
			{
				switch (iPlayer)
				{
				case 0:
				case 5:
					pkHero->SetUD_Int(UDK_AI_LANE, (int)SL_MID);
					break;
				case 1:
				case 2:
				case 6:
				case 7:
					pkHero->SetUD_Int(UDK_AI_LANE, (int)SL_TOP);
					break;
				case 3:
				case 4:
				case 8:
				case 9:
					pkHero->SetUD_Int(UDK_AI_LANE, (int)SL_BOT);
					break;
				default:
					break;
				}
				int iRand = gRandNum.RandInt((int)BPC_Radical, (int)BPC_NULL);
				gMain.SetPlayerCharacter(iPlayer, (BePlayerCharacter)iRand);
			}
		}
		else
		{
			gMain.SetPlayerType(iPlayer, BPT_PLAYER);

			BeUnit* pkHero = gUnitMgr.GetUnitByID(gMain.GetHeroID(iPlayer));
			if (pkHero)
			{
				pkHero->PopUD_Int(UDK_AI_LANE);
			}
		}

	}
	else if (kCheatCommand.find("SETCHARACTER") != std::string::npos)
	{
		if (iData >= (int)BPC_Radical && iData <= BPC_NULL)
		{
			gMain.SetPlayerCharacter(iPlayer, (BePlayerCharacter)iData);
			if (iData == (int)BPC_NULL)
			{
				gMain.SetPlayerType(iPlayer, BPT_PLAYER);
				BeUnit* pkHero = gUnitMgr.GetUnitByID(gMain.GetHeroID(iPlayer));
				if (pkHero)
				{
					pkHero->PopUD_Int(UDK_AI_LANE);
				}
			}
			else
			{
				gMain.SetPlayerType(iPlayer, BPT_ROBOT);
				BeUnit* pkHero = gUnitMgr.GetUnitByID(gMain.GetHeroID(iPlayer));
				if (pkHero)
				{
					switch (iPlayer)
					{
					case 0:
					case 5:
						pkHero->SetUD_Int(UDK_AI_LANE, (int)SL_MID);
						break;
					case 1:
					case 2:
					case 6:
					case 7:
						pkHero->SetUD_Int(UDK_AI_LANE, (int)SL_TOP);
						break;
					case 3:
					case 4:
					case 8:
					case 9:
						pkHero->SetUD_Int(UDK_AI_LANE, (int)SL_BOT);
						break;
					default:
						break;
					}
				}
			}
		}
	}
	else if (kCheatCommand.find("ROBOT") != std::string::npos)
	{
		for (int i = 0; i < 10; i++)
		{
			if (gMain.GetHeroID(i) != 0)
			{
				continue;
			}

			CreateHero(i);

			BeUnit* pkUnit = m_pkUnitMgr->GetUnitByID(gMain.GetHeroID(i));
			if (pkUnit)
			{
				switch (i)
				{
				case 0:
				case 5:
					pkUnit->SetUD_Int(UDK_AI_LANE, (int)SL_MID);
					break;
				case 1:
				case 2:
				case 6:
				case 7:
					pkUnit->SetUD_Int(UDK_AI_LANE, (int)SL_TOP);
					break;
				case 3:
				case 4:
				case 8:
				case 9:
					pkUnit->SetUD_Int(UDK_AI_LANE, (int)SL_BOT);
					break;
				default:
					break;
				}
				int iRand = gRandNum.RandInt((int)BPC_Radical, (int)BPC_NULL);
				gMain.SetPlayerCharacter(i, (BePlayerCharacter)iRand);
			}
		}
	}
	else if (kCheatCommand == "ADDBUFFER")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (pkHero)
		{
			BeBuffer* pkBuffer = pkHero->AddBufferBegin(iData, -1);
			if (pkBuffer)
			{
				pkHero->AddBufferEnd(pkBuffer);
			}
		}
		return true;
	}
	else if (kCheatCommand == "HURTSHOW")
	{
		m_bShowAllDamage = !m_bShowAllDamage;
		return true;
	}
	else if (kCheatCommand == "ADDMONEY")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (pkHero)
		{
			gMain.AddPlayerGold(iPlayer, fData);
		}
		return true;
	}
	else if (kCheatCommand == "KILLSELF")
	{
		BeUnit* pkUpHero = GetHeroBySeat(iPlayer);
		if (pkUpHero)
		{
			BeAttackingAttr kAttr;
			kAttr.kAttacker = pkUpHero->GetSharePtr();
			kAttr.fDamage = 99999999.0f;

			pkUpHero->OnBeDamaged(kAttr);
		}

		return true;
	}
	else if (kCheatCommand == "DIE")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (!pkHero)
		{
			return false;
		}

		pkHero->OperateUnitDead();
		return true;
	}
	else if (kCheatCommand == "WTF")
	{
		if (m_pkDataMgr->IsWTF())
		{
			m_pkDataMgr->SetWTF(false);
		}
		else
		{
			m_pkDataMgr->SetWTF(true);
		}

		return true;
	}
	else if (kCheatCommand == "MANXUE")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (pkHero)
		{
			pkHero->SetHP(pkHero->GetMaxHP());
			pkHero->SetMP(pkHero->GetMaxMP());
		}

		return true;
	}
	else if (kCheatCommand == "LEVEL")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (pkHero)
		{
			pkHero->SetLevel(fData);
		}

		return true;
	}
	else if (kCheatCommand == "WUDI")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (pkHero)
		{
			BeBuffer* pkOldBuffer = pkHero->GetBuffer('BG05');
			if (pkOldBuffer)
			{
				pkOldBuffer->SetRemoveTime(m_uiGameTime, true);
			}
			else
			{
				BeBuffer* pkBuffer = pkHero->AddBufferBegin('BG05', -1);
				if (pkBuffer)
				{
					pkBuffer->SetNormalAttr(NAT_PER_MOVE_SPEED, 1000.0f);
					pkBuffer->SetNormalAttr(NAT_PER_ATTACK_SPEED, 1000.0f);
					pkBuffer->SetNormalAttr(NAT_PER_DAMAGE_BASE, 1000.0f);
					pkBuffer->SetNormalAttr(NAT_PER_ARMOR, 1000.0f);
					pkBuffer->SetNormalAttr(NAT_PER_REGEN_MP, 1000.0f);
					pkBuffer->SetNormalAttr(NAT_PER_REGEN_HP, 1000.0f);
					pkBuffer->SetNormalAttr(NAT_ABS_MAX_MP, 10000.0f);
					pkBuffer->SetNormalAttr(NAT_ABS_MAX_HP, 10000.0f);
					pkBuffer->SetNormalAttr(NAT_ABS_ALL, 1000.0f);

					pkBuffer->SetNormalAttr(NAT_PER_DEMAGE, 1000.0f);
					pkBuffer->SetNormalAttr(NAT_PER_BEDEMAGED, -0.99f);
					pkBuffer->SetCarryFlag(BUCF_CANNOTDEATH | BUCF_ISMAGICIMMUNITY);

					pkHero->AddBufferEnd(pkBuffer);
				}
			}
		}
		return true;
	}
	else if (kCheatCommand == "GOLD")
	{
		SetPlayerGold(iPlayer, (int)fData);
		return true;
	}
	else if (kCheatCommand == "ADDGOLD")
	{
		int iOriGold = GetPlayerGold(iPlayer);
		SetPlayerGold(iPlayer, (int)fData + iOriGold);
		return true;
	}
	else if (kCheatCommand == "LVLUPONE")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (!pkHero)
		{
			return false;
		}
		pkHero->SetLevel(pkHero->GetLevel() + 1);
		return true;
	}
	else if (kCheatCommand == "AIADDGOLD")
	{
		for (int i = 5; i < 10; i++)
		{
			SetPlayerGold(i, (int)fData);
		}
		return true;
	}
	else if (kCheatCommand == "LVLUP")
	{
		if (m_bAllZuoBi)
		{
			for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
			{
				BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(i), true);
				if (pkHero && gMain.GetPlayerType(i) != BPT_ROBOT)
				{
					pkHero->SetLevel((int)fData);
				}
			}
			return true;
		}
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (!pkHero)
		{
			return false;
		}
		pkHero->SetLevel((int)fData);
		return true;
	}
	else if (kCheatCommand == "ATK")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer), true);
		if (pkHero)
		{
			pkHero->SetUD_Float(UDK_CHEAT_ATK, fData);
			pkHero->SetCheatAttr();
		}
		return true;
	}
	else if (kCheatCommand == "MOVESPEED")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer), true);
		if (pkHero)
		{
			pkHero->SetUD_Float(UDK_CHEAT_MOVESPEED, fData);
			pkHero->SetCheatAttr();
		}
		return true;
	}
	else if (kCheatCommand == "ARMOR")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer), true);
		if (pkHero)
		{
			pkHero->SetUD_Float(UDK_CHEAT_ARMOR, fData);
			pkHero->SetCheatAttr();
		}
		return true;
	}
	else if (kCheatCommand == "MAGIC")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer), true);
		if (pkHero)
		{
			pkHero->SetUD_Float(UDK_CHEAT_MAGIC, fData);
			pkHero->SetCheatAttr();
		}
		return true;
	}
	else if (kCheatCommand == "CLEAR")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer), true);
		if (pkHero)
		{
			pkHero->SetUD_Float(UDK_CHEAT_MAXHP, 0.0f);

			pkHero->DelBuffer('B043');
		}

		return true;
	}
	else if (kCheatCommand == "ADDSKILL")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (pkHero)
		{
			int iCurLevel = pkHero->GetSkillLevel(iData);
			pkHero->AddSkill(iData, ++iCurLevel);
		}

		return true;
	}
	else if (kCheatCommand == "KILLMONSTERS")
	{
		BeUnit* pkHero = m_pkUnitMgr->GetUnitByID(GetHeroID(iPlayer));
		if (pkHero)
		{
			BeAttackingAttr	kAttr;
			kAttr.kAttacker = pkHero->GetSharePtr();
			kAttr.fDamage = 9999999;
			kAttr.eAttackType = BAT_ABS;
			UnitGroup	kGroup;
			gUnitMgr.GetAreaGroup(kGroup, pkHero->GetPosX(), pkHero->GetPosY(), 999999.0f, -1, 0, false);
			if (!kGroup.IsEmpty())
			{
				for (size_t iIndex = 0; iIndex < kGroup.Size(); iIndex++)
				{
					if (kGroup[iIndex] && !kGroup[iIndex]->IsHero())
					{
						kGroup[iIndex]->OnBeDamaged(kAttr);
					}
				}
			}
		}

		return true;
	}
	return false;
}

bool BeMain::GuanQiaCommand(const BeGuanQiaCommand& kCmd)
{
	char acTemp[256] = { 0 };
	strncpy(acTemp, kCmd.acGuanQiaCommand, sizeof(kCmd.acGuanQiaCommand));
	std::string kGuanQiaCommand = acTemp;
	int iPlayer = kCmd.iGuanQiaPlayer;
	int iData = (int)kCmd.dwGuanQiaCmd;
	float fData1 = kCmd.fData1;
	float fData2 = kCmd.fData2;


	return false;
}

bool BeMain::GetHeroOrgPos(int iSeat, BePos2& rkPos, float& rfFace) const
{
	const MapInfoTable* pkMapInfo = MapInfoTableMgr::Get()->GetMapInfoTable(m_iMapID);
	if (!pkMapInfo)
	{
		return false;
	}

	SeRoomPlayerCamp eGroup = (SeRoomPlayerCamp)GetPlayerCamp(iSeat);
	if (eGroup == SRPC_INVALID)
	{
		return false;
	}
	int iMaxPlayer = 0;
	int iPlayerIndex = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		int iGroup = GetPlayerCamp(i);
		if (iGroup == eGroup)
		{
			if (i == iSeat)
			{
				iPlayerIndex = iMaxPlayer;
			}
			iMaxPlayer++;
		}
	}
	if (iMaxPlayer <= 0)
	{
		iMaxPlayer = 1;
	}

	int iCampIndex = (eGroup - 1) * 2;
	rkPos.fX = pkMapInfo->iCampPos[iCampIndex];
	rkPos.fY = pkMapInfo->iCampPos[iCampIndex + 1];

	float fFace = 0.0f;

	float fDelAngle = 2.0f * D3DX_PI / 10;
	fFace = -0.5f * D3DX_PI + iPlayerIndex * fDelAngle;

	rkPos.fX += 150.0f * cos(fFace);
	rkPos.fY += 150.0f * sin(fFace);

	rfFace = fFace + D3DX_PI;
	return true;
}

bool BeMain::CreateHero(int iPlayer)
{
	bool bHasCreateHeroSuc = false;

	const MapInfoTable* pkMapInfo = MapInfoTableMgr::Get()->GetMapInfoTable(m_iMapID);
	if (!pkMapInfo)
	{
		return false;
	}
	int iMaxActivePlayers = pkMapInfo->iMaxPlayer;
	if (iMaxActivePlayers < 1)
	{
		iMaxActivePlayers = 1;
	}
	int	iMaxGroup = pkMapInfo->iCampNum;
	if (iMaxGroup < 1)
	{
		iMaxGroup = 1;
	}
	int iGroupPlayer = iMaxActivePlayers / iMaxGroup;
	for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
	{
		int iGroup = i / iGroupPlayer + 1;

		if (i >= MAX_ACTIVEPLAYERS)
		{
			break;
		}
		if (i < MAX_ACTIVEPLAYERS / 2)
		{
			iGroup = SRPC_CAMPA;
		}
		else
		{
			iGroup = SRPC_CAMPB;
		}

		if (m_akPlayer[i].iCamp != 0)
		{
			iGroup = m_akPlayer[i].iCamp;
		}
		if (m_akPlayer[i].iTypeID == 0)
		{
			continue;
		}

		BeUnit* pkUnit = m_pkUnitMgr->AddUnit(m_akPlayer[i].iTypeID, -1, i);
		if (pkUnit)
		{
			gLabelMgr.AddObjLabel(pkUnit, TDLABEL_HERO);

			pkUnit->SetPlayer(i);
			m_pkDataMgr->SetHasCreateHero(i, true);

			pkUnit->SetLevel(1);

			SetHeroID(i, pkUnit->GetID());

			gMain.SetPlayerGroup(i, iGroup);
			pkUnit->SetCamp(iGroup);
			pkUnit->SetPlayer(i);

			gMain.SetPlayerHeroID(i, pkUnit->GetID());

			gMain.AddPlayerGold(pkUnit->GetPlayer(), pkMapInfo->iStartGold);

			if (GetPlayerType(i) == BPT_ROBOT)
			{
				SetPlayerCharacter(i, BPC_WEAK);

				if (i == 11 || i == 12 || i == 1 || i == 2)
				{
					pkUnit->SetUD_Int(UDK_AI_LANE, SL_TOP);
				}
				else if (i == 10)
				{
					pkUnit->SetUD_Int(UDK_AI_LANE, SL_MID);
				}
				else
				{
					pkUnit->SetUD_Int(UDK_AI_LANE, SL_BOT);
				}
			}

			bHasCreateHeroSuc = true;

			int iZhuanShuSkillTypeID = pkUnit->GetZhuanShuSkillTypeID();

			if (gMain.IsPveMap())
			{
				pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(0), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(0))->iSkillMaxLevel);
				pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(1), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(1))->iSkillMaxLevel);
				pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(2), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(2))->iSkillMaxLevel);
				pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(3), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(3))->iSkillMaxLevel);
				pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(4), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(4))->iSkillMaxLevel);
			}
			if (gMain.IsSecretArea()) {
				BeBuffer* pkBuffer = pkUnit->AddBufferBegin('B571', pkUnit->GetID());
				if (pkBuffer)
				{
					pkBuffer->SetCarryFlag(BUCF_CANNOTMOVE | BUCF_CANNOTATTACK);
					pkBuffer->SetRemoveTime(gTime + 3 * 1000);
					pkUnit->AddBufferEnd(pkBuffer);
				}
			}
			if (gMain.IsZhanChangMap())
			{
				pkUnit->ResetTurnTableInfo();
			}
			pkUnit->AddSkill(iZhuanShuSkillTypeID);

			pkUnit->AddSkill('SA02');
			pkUnit->AddSkill('SA03');
			pkUnit->AddSkill('SA04');
			pkUnit->AddSkill('SA06');

			if (pkUnit->GetTypeID() == 'UT51')
			{
				pkUnit->AddSkill('S172');
			}
			if (pkUnit->GetTypeID() == 'UT55')
			{
				pkUnit->AddSkill('S192');
			}

			BeBuffer* pkFuWenBuffer = pkUnit->AddBufferBegin('B336', -1);
			if (pkFuWenBuffer)
			{
				pkFuWenBuffer->SetNormalAttr(NAT_ABS_MAX_HP, m_akPlayer[i].fEquipMaxHp);
				pkFuWenBuffer->SetNormalAttr(NAT_ABS_MAX_MP, m_akPlayer[i].fEquipMaxMp);

				pkFuWenBuffer->SetNormalAttr(NAT_ABS_REGEN_HP, m_akPlayer[i].fEquipRegenHp);
				pkFuWenBuffer->SetNormalAttr(NAT_ABS_REGEN_MP, m_akPlayer[i].fEquipRegenMp);
				pkFuWenBuffer->SetNormalAttr(NAT_ABS_DAMAGE, m_akPlayer[i].fEquipDamage);
				pkFuWenBuffer->SetNormalAttr(NAT_PER_ATTACK_SPEED, m_akPlayer[i].fEquipAttackSpeed);
				pkFuWenBuffer->SetNormalAttr(NAT_ABS_MAGIC, m_akPlayer[i].fEquipMagicDamage);
				pkFuWenBuffer->SetNormalAttr(NAT_MAGIC_ARMOR, m_akPlayer[i].fEquipMagicArmor);
				pkFuWenBuffer->SetNormalAttr(NAT_ABS_ARMOR, m_akPlayer[i].fEquipArmor);
				pkFuWenBuffer->SetNormalAttr(NAT_DEC_CDTIME, m_akPlayer[i].fEquipDecCD);
				pkFuWenBuffer->SetNormalAttr(NAT_BAOJI, m_akPlayer[i].fEquipCritical);
				pkFuWenBuffer->SetNormalAttr(NAT_DEC_MAGIC_RESISTANCE, m_akPlayer[i].fPenetrateMagicArmor);
				pkFuWenBuffer->SetNormalAttr(NAT_DEC_ARMOR, m_akPlayer[i].fPenetrateArmor);
				pkFuWenBuffer->SetNormalAttr(NAT_PER_MOVE_SPEED, m_akPlayer[i].fBaseMoveSpeed);
				pkFuWenBuffer->SetNormalAttr(NAT_PER_BASE_MAXHP, m_akPlayer[i].fPersentBaseMaxHp);
				pkFuWenBuffer->SetNormalAttr(NAT_LEECH, m_akPlayer[i].fPersentLifeSteal);
				pkFuWenBuffer->SetNormalAttr(NAT_PER_FASHUCHUANTOU, m_akPlayer[i].fPersentPenetrateMa);
				pkFuWenBuffer->SetDeadNoRemove(true);
				pkUnit->AddBufferEnd(pkFuWenBuffer);
			}

			if (m_akPlayer[i].iEquipSkill[0] > 0)
			{
				pkUnit->AddSkill(m_akPlayer[i].iEquipSkill[0]);
			}
			if (m_akPlayer[i].iEquipSkill[1] > 0)
			{
				pkUnit->AddSkill(m_akPlayer[i].iEquipSkill[1]);
			}
			if (m_akPlayer[i].iEquipSkill[2] > 0)
			{
				pkUnit->AddSkill(m_akPlayer[i].iEquipSkill[2]);
			}

			BePos2 kPos;
			float fFace = 0.0f;
			GetHeroOrgPos(i, kPos, fFace);
			pkUnit->SetAbsFace(fFace);
			m_pkMap->SetUnitPosition(pkUnit, kPos.fX, kPos.fY, 0.0f, 1000.0f, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);

			BeLevelMain* pkTriggerMain = (BeLevelMain*)gMain.GetTaskHandle();
			EsAssert(pkTriggerMain);
			pkTriggerMain->GetLabelMgr()->AutoLabelObj(pkUnit);
		}
	}
	if (!bHasCreateHeroSuc)
	{
		return false;
	}

	return true;
}

void BeMain::ExcuPlayerReconBegin(int iSeat, unsigned int dwPlayerID)
{
	CHECK_IDX(iSeat);

	const BePlayerInfo& rkPlayer = GetPlayerInfo(iSeat);

	BeUnit* pkReconUnit = GetHeroBySeat(iSeat);
	if (!pkReconUnit)
	{
		return;
	}
}

bool BeMain::CreateOneHero(int iPlayer)
{
	const MapInfoTable* pkMapInfo = MapInfoTableMgr::Get()->GetMapInfoTable(m_iMapID);
	if (!pkMapInfo)
	{
		return false;
	}

	int iGroup = SRPC_CAMPB;
	if (iPlayer < MAX_ACTIVEPLAYERS / 2)
	{
		iGroup = SRPC_CAMPA;
	}

	if (m_akPlayer[iPlayer].iTypeID == 0)
	{
		return false;
	}

	BeUnit* pkUnit = m_pkUnitMgr->AddUnit(m_akPlayer[iPlayer].iTypeID, -1, iPlayer);
	if (pkUnit)
	{
		gLabelMgr.AddObjLabel(pkUnit, TDLABEL_HERO);

		pkUnit->SetPlayer(iPlayer);
		m_pkDataMgr->SetHasCreateHero(iPlayer, true);

		pkUnit->SetLevel(18);

		SetHeroID(iPlayer, pkUnit->GetID());

		gMain.SetPlayerGroup(iPlayer, iGroup);
		pkUnit->SetCamp(iGroup);
		pkUnit->SetPlayer(iPlayer);

		gMain.SetPlayerHeroID(iPlayer, pkUnit->GetID());

		gMain.AddPlayerGold(pkUnit->GetPlayer(), pkMapInfo->iStartGold);

		if (GetPlayerType(iPlayer) == BPT_ROBOT)
		{
			SetPlayerCharacter(iPlayer, BPC_WEAK);

			if (iPlayer == 11 || iPlayer == 12 || iPlayer == 1 || iPlayer == 2)
			{
				pkUnit->SetUD_Int(UDK_AI_LANE, SL_TOP);
			}
			else if (iPlayer == 10)
			{
				pkUnit->SetUD_Int(UDK_AI_LANE, SL_MID);
			}
			else
			{
				pkUnit->SetUD_Int(UDK_AI_LANE, SL_BOT);
			}
		}

		int iZhuanShuSkillTypeID = pkUnit->GetZhuanShuSkillTypeID();

		if (gMain.IsPveMap()) //PVE 
		{
			pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(0), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(0))->iSkillMaxLevel);
			pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(1), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(1))->iSkillMaxLevel);
			pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(2), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(2))->iSkillMaxLevel);
			pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(3), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(3))->iSkillMaxLevel);
			pkUnit->AddSkill(pkUnit->GetSkillTypeIDByPos(4), SkillTableMgr::Get()->GetSkillTable(pkUnit->GetSkillTypeIDByPos(4))->iSkillMaxLevel);
		}
		if (gMain.IsZhanChangMap())
		{
			pkUnit->ResetTurnTableInfo();
		}

		pkUnit->AddSkill(iZhuanShuSkillTypeID);
		pkUnit->AddSkill('SA02');
		pkUnit->AddSkill('SA03');
		pkUnit->AddSkill('SA04');
		pkUnit->AddSkill('SA06');
		if (pkUnit->GetTypeID() == 'UT51')
		{
			pkUnit->AddSkill('S172');
		}
		if (pkUnit->GetTypeID() == 'UT55')
		{
			pkUnit->AddSkill('S192');
		}

		BeBuffer* pkFuWenBuffer = pkUnit->AddBufferBegin('B336', -1);
		if (pkFuWenBuffer)
		{
			pkFuWenBuffer->SetNormalAttr(NAT_ABS_MAX_HP, m_akPlayer[iPlayer].fEquipMaxHp);
			pkFuWenBuffer->SetNormalAttr(NAT_ABS_MAX_MP, m_akPlayer[iPlayer].fEquipMaxMp);
			pkFuWenBuffer->SetNormalAttr(NAT_ABS_REGEN_HP, m_akPlayer[iPlayer].fEquipRegenHp);
			pkFuWenBuffer->SetNormalAttr(NAT_ABS_REGEN_MP, m_akPlayer[iPlayer].fEquipRegenMp);
			pkFuWenBuffer->SetNormalAttr(NAT_ABS_DAMAGE, m_akPlayer[iPlayer].fEquipDamage);
			pkFuWenBuffer->SetNormalAttr(NAT_PER_ATTACK_SPEED, m_akPlayer[iPlayer].fEquipAttackSpeed);
			pkFuWenBuffer->SetNormalAttr(NAT_ABS_MAGIC, m_akPlayer[iPlayer].fEquipMagicDamage);
			pkFuWenBuffer->SetNormalAttr(NAT_MAGIC_ARMOR, m_akPlayer[iPlayer].fEquipMagicArmor);
			pkFuWenBuffer->SetNormalAttr(NAT_ABS_ARMOR, m_akPlayer[iPlayer].fEquipArmor);
			pkFuWenBuffer->SetNormalAttr(NAT_DEC_CDTIME, m_akPlayer[iPlayer].fEquipDecCD);
			pkFuWenBuffer->SetNormalAttr(NAT_BAOJI, m_akPlayer[iPlayer].fEquipCritical);
			pkFuWenBuffer->SetNormalAttr(NAT_DEC_MAGIC_RESISTANCE, m_akPlayer[iPlayer].fPenetrateMagicArmor);
			pkFuWenBuffer->SetNormalAttr(NAT_DEC_ARMOR, m_akPlayer[iPlayer].fPenetrateArmor);
			pkFuWenBuffer->SetNormalAttr(NAT_PER_MOVE_SPEED, m_akPlayer[iPlayer].fBaseMoveSpeed);
			pkFuWenBuffer->SetNormalAttr(NAT_PER_BASE_MAXHP, m_akPlayer[iPlayer].fPersentBaseMaxHp);

			pkFuWenBuffer->SetDeadNoRemove(true);
			pkUnit->AddBufferEnd(pkFuWenBuffer);
		}

		if (m_akPlayer[iPlayer].iEquipSkill[0] > 0)
		{
			pkUnit->AddSkill(m_akPlayer[iPlayer].iEquipSkill[0]);
		}
		if (m_akPlayer[iPlayer].iEquipSkill[1] > 0)
		{
			pkUnit->AddSkill(m_akPlayer[iPlayer].iEquipSkill[1]);
		}
		if (m_akPlayer[iPlayer].iEquipSkill[2] > 0)
		{
			pkUnit->AddSkill(m_akPlayer[iPlayer].iEquipSkill[2]);
		}

		BePos2 kPos;
		float fFace = 0.0f;
		GetHeroOrgPos(iPlayer, kPos, fFace);
		pkUnit->SetAbsFace(fFace);
		m_pkMap->SetUnitPosition(pkUnit, kPos.fX, kPos.fY, 0.0f, 1000.0f, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);

		BeLevelMain* pkTriggerMain = (BeLevelMain*)gMain.GetTaskHandle();
		EsAssert(pkTriggerMain);
		pkTriggerMain->GetLabelMgr()->AutoLabelObj(pkUnit);
	}
	else 
	{
		return false;
	}

	return true;
}

bool BeMain::DelOneHero(int iSeat)
{
	BeUnit* pkOwner = GetHeroBySeat(iSeat);
	if (pkOwner)
	{
		pkOwner->ClrFlag(BUF_RELIVE);
		ClearPlayer(iSeat);
		m_pkUnitMgr->DelUnit(pkOwner->GetID());
	}
	return true;
}
