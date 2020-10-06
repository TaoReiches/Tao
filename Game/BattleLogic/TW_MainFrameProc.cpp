/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Main.h"

bool BeMain::ProcFrameCMD(const SeRaceCmd* pkRaceCmd)
{
	if (pkRaceCmd->dwCmd >= SRC_CMD_NUM)
	{
		return false;
	}
	switch (pkRaceCmd->dwCmd)
	{
	case SRC_FRAME:					return Proc_FRAME(pkRaceCmd);
	case SRC_PLAYER_INFO:			return Proc_PLAYER_INFO(pkRaceCmd);
	case SRC_LOAD:					return Proc_LOAD(pkRaceCmd);
	case SRC_START:					return Proc_START(pkRaceCmd);
	case SRC_END:					return Proc_END(pkRaceCmd);
	case SRC_CHART_PLAYER:			return Proc_CHART_PLAYER(pkRaceCmd);
	case SRC_PLAYER_LEAVE:			return Proc_PLAYER_LEAVE(pkRaceCmd);
	case SRC_SYSTEM_CHEAT:			return Proc_SYSTEM_CHEAT(pkRaceCmd);
	case SRC_SYSTEM_SURRENDER:		return Proc_SYSTEM_SURRENDER(pkRaceCmd);
	case SRC_HERO_MOVE:				return Proc_HERO_MOVE(pkRaceCmd);
	case SRC_UNIT_MOVE:				return Proc_UNIT_MOVE(pkRaceCmd);
	case SRC_HERO_STOP:				return Proc_HERO_STOP(pkRaceCmd);
	case SRC_UNIT_STOP:				return Proc_UNIT_STOP(pkRaceCmd);
	case SRC_HERO_HOLD:				return Proc_HERO_HOLD(pkRaceCmd);
	case SRC_UNIT_HOLD:				return Proc_UNIT_HOLD(pkRaceCmd);
	case SRC_HERO_ATTACK_POS:		return Proc_HERO_ATTACK_POS(pkRaceCmd);
	case SRC_UNIT_ATTACK_POS:		return Proc_UNIT_ATTACK_POS(pkRaceCmd);
	case SRC_HERO_ATTACK_UNIT:		return Proc_HERO_ATTACK_UNIT(pkRaceCmd);
	case SRC_UNIT_ATTACK_UNIT:		return Proc_UNIT_ATTACK_UNIT(pkRaceCmd);
	case SRC_UNIT_ATTACK:			return Proc_UNIT_ATTACK(pkRaceCmd);
	case SRC_UNIT_PATROL:			return Proc_UNIT_PATROL(pkRaceCmd);
	case SRC_UNIT_FOLLOW:			return Proc_UNIT_FOLLOW(pkRaceCmd);
	case SRC_UNIT_SPELL:			return Proc_UNIT_SPELL(pkRaceCmd);
	case SRC_UNIT_SPELLXULI:		return Proc_UNIT_SPELLXuLi(pkRaceCmd);
	case SRC_UNIT_USE_ITEM:			return Proc_UNIT_USE_ITEM(pkRaceCmd);
	case SRC_UNIT_DROP_ITEM:		return Proc_UNIT_DROP_ITEM(pkRaceCmd);
	case SRC_UNIT_PICK_ITEM:		return Proc_UNIT_PICK_ITEM(pkRaceCmd);
	case SRC_UNIT_PICK_ITEM_AUTO:	return Proc_UNIT_PICK_ITEM_AUTO(pkRaceCmd);
	case SRC_UNIT_BUY_ITEM:			return Proc_UNIT_BUY_ITEM(pkRaceCmd);
	case SRC_UNIT_DISPLACE_ITEM:	return Proc_UNIT_DISPLACE_ITEM(pkRaceCmd);
	case SRC_UNIT_GIVEUP_DISPLACE_ITEM:	return Proc_UNIT_GU_DISPLACE_ITEM(pkRaceCmd);
	case SRC_UNIT_DIRECTBUY_ITEM:	return Proc_UNIT_DIRECTBUY_ITEM(pkRaceCmd);
	case SRC_UNIT_DIRECTSELL_ITEM:	return Proc_UNIT_DIRECTSELL_ITEM(pkRaceCmd);
	case SRC_UNIT_SWAP_ITEM:		return Proc_UNIT_SWAP_ITEM(pkRaceCmd);
	case SRC_UNIT_LEARN_SKILL:		return Proc_UNIT_LEARN_SKILL(pkRaceCmd);
	case SRC_UNIT_ACTIVE_SKILL:		return Proc_UNIT_ACTIVE_SKILL(pkRaceCmd);
	case SRC_PLAYER_RELIVEHERO:		return Proc_PLAYER_RELIVEHERO(pkRaceCmd);
	case SRC_PLAYER_MAP_MESSAGE:	return Proc_PLAYER_MAP_MESSAGE(pkRaceCmd);
	case SRC_PLAYER_GUANQIA_CMD:	return Proc_PLAYER_GUANQIA_CMD(pkRaceCmd);
	case SRC_PLAYER_TOWNITEM:		return Proc_PLAYER_TOWNITEM(pkRaceCmd);
	case SRC_PLAYER_RECON_BEGIN:	return Proc_PLAYER_RECON_BEGIN(pkRaceCmd);
	case SRC_PLAYER_RECON_END:		return Proc_PLAYER_RECON_END(pkRaceCmd);
	case SRC_VIEW_SELHERO:			return Proc_VIEW_SELHERO(pkRaceCmd);
	case SRC_RACE_PAUSE:			return Proc_PAUSE_BEGIN(pkRaceCmd);
	case SRC_PAUSE_CANCEL:			return Proc_PAUSE_END(pkRaceCmd);
	case SRC_TRAN_LOADEND:			return Proc_PAUSE_BEGIN(pkRaceCmd);
	case SRC_SWITCH_MODE:			return Proc_PAUSE_END(pkRaceCmd);
	case SRC_LOCKUNIT:				return Proc_Race_LockUnit(pkRaceCmd);
	case SRC_ADJUSTFRAME:			return Proc_Race_AdjustFrame(pkRaceCmd);
	case SRC_RELINKBEGIN:			return Proc_Race_RelinkBegin(pkRaceCmd);
	case SRC_RELINKEND:				return Proc_Race_RelinkEnd(pkRaceCmd);
	case SRC_HERO_DIRECT_MOVE:		return Proc_HERO_DIRECT_MOVE(pkRaceCmd);
	case SRC_HERO_ATTACK_CANCEL:	return Proc_HERO_ATTACK_CANCEL(pkRaceCmd);
	case SRC_HERO_SELECT:			return Proc_HERO_SELECT(pkRaceCmd);
	case SRC_SWITCH_HEROFOCUS:		return Proc_SWITCH_HEROFOCUS(pkRaceCmd);
	case SRC_PING:					return Proc_Ping(pkRaceCmd);
	case SRC_SMALLMAP:				return Proc_SmallMapMessage(pkRaceCmd);
	case SRC_UNIT_CHANGE_ITEM:		return Proc_UNIT_CHANGE_ITEM(pkRaceCmd);
	case SRC_TRUN_TABLE_INFO:		return Proc_TURN_TABLE_INFO(pkRaceCmd);
	case SRC_COST_EXTERNAL_RESOURCE:return Proc_COST_EXTERNAL_RESOURCE(pkRaceCmd);
	case SRC_SHOW_ACTION:			return Proc_Show_Action(pkRaceCmd);
	case SRC_8V8_DEL_PLAYER:		return Proc_8V8_DEL_PLAYER(pkRaceCmd);
	case SRC_8V8_ADD_PLAYER:		return Proc_8V8_ADD_PLAYER(pkRaceCmd);
	default:
		return true;
	}
}

bool	BeMain::Proc_SmallMapMessage(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_SmallMap* pkCmd = (SeRaceCmd_SmallMap*)pkRaceCmd;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		BeShareSmallMapMessage		kData;
		kData.byMessageType = pkCmd->byMessageType;
		kData.usPosX = pkCmd->usPosX;
		kData.usPosY = pkCmd->usPosY;

		m_kSmallMapMessage.push_back(std::make_pair(pkHero->GetPlayer(), kData));
	}

	return true;
}

bool	BeMain::Proc_Ping(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_Ping* pkCmd = (SeRaceCmd_Ping*)pkRaceCmd;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		m_bSendPing[pkHero->GetPlayer()] = true;
	}

	return true;
}

bool BeMain::Proc_HERO_SELECT(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroSelect* pkCmd = (SeRaceCmd_HeroSelect*)pkRaceCmd;

	return true;
}

bool BeMain::Proc_HERO_ATTACK_CANCEL(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroAttackCancel* pkCmd = (SeRaceCmd_HeroAttackCancel*)pkRaceCmd;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
	}
	return true;
}

bool BeMain::Proc_HERO_DIRECT_MOVE(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroDirectMove* pkCmd = (SeRaceCmd_HeroDirectMove*)pkRaceCmd;

	float fMoveDirect = pkCmd->fMoveDirect;

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		if (fMoveDirect == 10086.0f)
		{
			BeCommand beCommand(BCT_STOP);
			pkHero->GiveCommand(beCommand);
		}
		else
		{
			float fTarX = pkHero->GetPosX() + 128.0f * cosf(fMoveDirect);
			float fTarY = pkHero->GetPosY() + 128.0f * sinf(fMoveDirect);

			BeCommand beCommand(BCT_MOVE, 0, fTarX, fTarY);
			pkHero->GiveCommand(beCommand, BCT_IMMEDIATE, true);
		}
	}

	return true;
}

bool BeMain::Proc_FRAME(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_PLAYER_INFO(const SeRaceCmd* pkRaceCmd)
{
	SeRacePlayerInfo* pkCmd = (SeRacePlayerInfo*)pkRaceCmd;

	for (int i = 0; i < pkCmd->iNum; ++i)
	{
		SePlayerInfoSeat& rkSeat = pkCmd->akSeat[i];
		const BePlayerInfo& rkPlayer = GetPlayerInfo(rkSeat.iSeatIndex);
		SetPlayerInfo(rkSeat.iSeatIndex, rkSeat.dwPlayerID, rkPlayer.iHeroUnitID, rkSeat.acName);
	}
	return true;
}

bool BeMain::Proc_LOAD(const SeRaceCmd* pkRaceCmd)
{
	SeRaceLoad* pkCmd = (SeRaceLoad*)pkRaceCmd;

	SeNetStream kNetStream;
	kNetStream.Attach(pkCmd->acData, pkCmd->iDataSize);

	std::vector<SeLoadSeat> akLoadSeat;
	while (true)
	{
		SeLoadSeat kLoadSeat;
		bool bResult = Read(kNetStream, kLoadSeat);
		if (!bResult)
		{
			break;
		}
		akLoadSeat.push_back(kLoadSeat);
	}

	int iNum = (int)akLoadSeat.size();
	if (iNum <= 0 || iNum > SSD_MAX_ROOM_PLAYER)
	{
		return false;
	}

	for (int i = 0; i < iNum; ++i)
	{
		SeLoadSeat& rkSeat = akLoadSeat[i];
		SetPlayerHeroTypeID(rkSeat.iSeatIndex, rkSeat.dwHeroTypeID);
		SetHeropProficiency(rkSeat.iSeatIndex, rkSeat.dwHeroProficiency);
		SetPlayerID(rkSeat.iSeatIndex, rkSeat.dwPlayerID);
		SetPlayerName(rkSeat.iSeatIndex, rkSeat.PlayerName);
		if (rkSeat.dwPlayerID == (DWord)-1)
		{
			SetPlayerType(rkSeat.iSeatIndex, BPT_ROBOT);
			SetPlayerCharacter(rkSeat.iSeatIndex, BPC_WEAK);
		}
		else
		{
			SetPlayerType(rkSeat.iSeatIndex, BPT_PLAYER);
		}
		m_akPlayer[rkSeat.iSeatIndex].iCamp = rkSeat.eCamp;

		int		iTempScore = GetCampMaxScore();
		if (rkSeat.uiScore > iTempScore)
		{
			SetCampMaxScore(rkSeat.uiScore);
		}

		m_akPlayer[rkSeat.iSeatIndex].fEquipMaxHp = rkSeat.fEquipMaxHp;
		m_akPlayer[rkSeat.iSeatIndex].fEquipMaxMp = rkSeat.fEquipMaxMp;
		m_akPlayer[rkSeat.iSeatIndex].fEquipRegenHp = rkSeat.fEquipRegenHp;
		m_akPlayer[rkSeat.iSeatIndex].fEquipRegenMp = rkSeat.fEquipRegenMp;
		m_akPlayer[rkSeat.iSeatIndex].fEquipDamage = rkSeat.fEquipDamage;
		m_akPlayer[rkSeat.iSeatIndex].fEquipAttackSpeed = rkSeat.fEquipAttackSpeed;
		m_akPlayer[rkSeat.iSeatIndex].fEquipMagicDamage = rkSeat.fEquipMagicDamage;
		m_akPlayer[rkSeat.iSeatIndex].fEquipMagicArmor = rkSeat.fEquipMagicArmor;
		m_akPlayer[rkSeat.iSeatIndex].fEquipArmor = rkSeat.fEquipArmor;
		m_akPlayer[rkSeat.iSeatIndex].fEquipDecCD = rkSeat.fEquipDecCD;

		m_akPlayer[rkSeat.iSeatIndex].fEquipCritical = rkSeat.fEquipCritical;
		m_akPlayer[rkSeat.iSeatIndex].iEquipSkill[0] = rkSeat.iEquipSkill[0];
		m_akPlayer[rkSeat.iSeatIndex].iEquipSkill[1] = rkSeat.iEquipSkill[1];
		m_akPlayer[rkSeat.iSeatIndex].iEquipSkill[2] = rkSeat.iEquipSkill[2];
		m_akPlayer[rkSeat.iSeatIndex].fLevelAddArmor = rkSeat.fLevelAddArmor;
		m_akPlayer[rkSeat.iSeatIndex].fLevelAddMagicDamage = rkSeat.fLevelAddMagicDamage;
		m_akPlayer[rkSeat.iSeatIndex].fLevelAddMagicArmor = rkSeat.fLevelAddMagicArmor;
		m_akPlayer[rkSeat.iSeatIndex].fPenetrateMagicArmor = rkSeat.fPenetrateMagicArmor;
		m_akPlayer[rkSeat.iSeatIndex].fPenetrateArmor = rkSeat.fPenetrateArmor;
		m_akPlayer[rkSeat.iSeatIndex].fPersentBaseMaxHp = rkSeat.fPersentBaseMaxHp;
		m_akPlayer[rkSeat.iSeatIndex].fBaseMoveSpeed = rkSeat.fBaseMoveSpeed;
		m_akPlayer[rkSeat.iSeatIndex].fPersentLifeSteal = rkSeat.fPersentLifeSteal;
		m_akPlayer[rkSeat.iSeatIndex].fPersentPenetrateMa = rkSeat.fPersentPenetrateMa;

		m_akPlayer[rkSeat.iSeatIndex].uiKillerEffectID = rkSeat.dwKillerEffect;
		m_akPlayer[rkSeat.iSeatIndex].uiEmojiEffectID = rkSeat.dwEmojiEffect;
		m_akPlayer[rkSeat.iSeatIndex].iZItenCount = rkSeat.iZItemCount;
		for (int item_idx = 0; item_idx < rkSeat.iZItemCount; item_idx++) {
			m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iUniqueID = rkSeat.kZItemInfo[item_idx].iUniqueID;
			m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iItemType = rkSeat.kZItemInfo[item_idx].iTypeID;
			m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iStrengLV = rkSeat.kZItemInfo[item_idx].iStrengLV;
			for (int iJewelIdx = 0; iJewelIdx < MAX_EQUIP_JEWEL_NUM; iJewelIdx++)
			{
				m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iJewelIds[iJewelIdx] = rkSeat.kZItemInfo[item_idx].iJewelIds[iJewelIdx];
			}
			for (int iEquipIdx = 0; iEquipIdx < MAX_EQUIP_ATTR_NUM; iEquipIdx++)
			{
				m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iAttrType[iEquipIdx] = rkSeat.kZItemInfo[item_idx].iAttrType[iEquipIdx];
				m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].fAttrValue[iEquipIdx] = rkSeat.kZItemInfo[item_idx].fAttrValue[iEquipIdx];
			}
		}
	}

	gMain.SetMapID(pkCmd->iMapID);

	return true;
}

bool BeMain::Proc_START(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_END(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_CHART_PLAYER(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_PLAYER_LEAVE(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_PlayerLeave* pkCmd = (SeRaceCmd_PlayerLeave*)pkRaceCmd;

	int	iLeavePlayerIdx = pkCmd->dwSeat;

	if (GetPlayerType(iLeavePlayerIdx) == BPT_PLAYER && !IsZhanChangMap())
	{
		BeShareGameNotice	kData;
		kData.bySeat = iLeavePlayerIdx;
		kData.byType = BNT_Leave;

		AddGameNoticeData(kData);
	}

	SetPlayerType(iLeavePlayerIdx, BPT_DIS_PLAYER);
	SetGameTime(iLeavePlayerIdx, (int)m_uiGameTime);

	BeUnit* pkHero = GetHeroBySeat(iLeavePlayerIdx);
	if (pkHero && GetPlayerType(iLeavePlayerIdx) != BPT_ROBOT)
	{
		BeBuffer* pkBuffer = pkHero->AddBufferBegin('BF30', pkHero->GetID());
		if (pkBuffer)
		{
			pkBuffer->SetCDTime(2000);
			pkHero->AddBufferEnd(pkBuffer);
		}
	}

	if (GetMapID() == 'M110')
	{
		OnRaceOver(2, 0);
	}

	return true;
}

bool BeMain::Proc_SYSTEM_CHEAT(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_Cheat* pkCmd = (SeRaceCmd_Cheat*)pkRaceCmd;

	BeCheatCommand kCheatCommand;
	memcpy(kCheatCommand.acCheatCommand, pkCmd->acCommand, sizeof(pkCmd->acCommand));
	kCheatCommand.fCheatData = pkCmd->fData;
	kCheatCommand.iCheatData = pkCmd->iData;
	kCheatCommand.iCheatPlayer = pkCmd->dwSeat;

	CheatCommand(kCheatCommand);
	return true;
}

bool BeMain::Proc_SYSTEM_SURRENDER(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_Surrender* pkCmd = (SeRaceCmd_Surrender*)pkRaceCmd;

	if (gTime < 12 * 60 * 1000)
	{
		return false;
	}
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (!pkHero)
	{
		return false;
	}
	Byte	byAgree = pkCmd->byAgree;
	if (byAgree == 1)
	{
		SetSurrenderTime(pkCmd->dwSeat, gTime);
		pkHero->SetTabInfoFlag(BTCF_SURRENDER);

		if (pkCmd->dwSeat < 5)
		{
			if (m_iLeftSurrenderBeginTime == 0)
			{
				m_iLeftSurrenderBeginTime = gTime;
			}
		}
		else
		{
			if (m_iRightSurrenderBeginTime == 0)
			{
				m_iRightSurrenderBeginTime = gTime;
			}
		}
	}
	else if (byAgree == 2)
	{
		SetSurrenderTime(pkCmd->dwSeat, -1);
		pkHero->SetTabInfoFlag(BTCF_SURRENDER);
	}

	return true;
}

bool BeMain::Proc_HERO_MOVE(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroMove* pkCmd = (SeRaceCmd_HeroMove*)pkRaceCmd;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		BeCommand beCommand(BCT_MOVE, 0, pkCmd->sTargetX, pkCmd->sTargetY);

		pkHero->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);

		SetNoPlayerCheckTime(pkHero->GetPlayer(), gTime);
	}
	return true;
}

bool BeMain::Proc_UNIT_MOVE(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_HERO_STOP(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroStop* pkCmd = (SeRaceCmd_HeroStop*)pkRaceCmd;

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		BeCommand beCommand(BCT_STOP);
		pkHero->GiveCommand(beCommand, BCT_IMMEDIATE, true);
	}
	return true;
}

bool BeMain::Proc_UNIT_STOP(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitStop* pkCmd = (SeRaceCmd_UnitStop*)pkRaceCmd;

	for (int i = 0; i < pkCmd->bySize; i++)
	{
		BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->aiIDs[i]);
		if (pkUnit)
		{
			BeCommand beCommand(BCT_STOP);

			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
		}
	}
	return true;
}

bool BeMain::Proc_HERO_HOLD(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroHold* pkCmd = (SeRaceCmd_HeroHold*)pkRaceCmd;

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		BeCommand beCommand(BCT_HOLD);

		pkHero->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
	}
	return true;
}

bool BeMain::Proc_UNIT_HOLD(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitHold* pkCmd = (SeRaceCmd_UnitHold*)pkRaceCmd;

	for (int i = 0; i < pkCmd->bySize; i++)
	{
		BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->aiIDs[i]);
		if (pkUnit)
		{
			BeCommand beCommand(BCT_HOLD);

			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
		}
	}
	return true;
}

bool BeMain::Proc_HERO_ATTACK_POS(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroAttackPos* pkCmd = (SeRaceCmd_HeroAttackPos*)pkRaceCmd;

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		BeCommand beCommand(BCT_ATTACK, 0, pkCmd->sTargetX, pkCmd->sTargetY);

		pkHero->GiveCommand(beCommand, BCT_IMMEDIATE, true);
	}
	return true;
}

bool BeMain::Proc_UNIT_ATTACK_POS(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitAttackPos* pkCmd = (SeRaceCmd_UnitAttackPos*)pkRaceCmd;

	for (int i = 0; i < pkCmd->bySize; i++)
	{
		BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->aiIDs[i]);
		if (pkUnit)
		{
			BeCommand beCommand(BCT_ATTACK, 0, pkCmd->sTargetX, pkCmd->sTargetY);

			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
		}
	}
	return true;
}

bool BeMain::Proc_HERO_ATTACK_UNIT(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroAttackUnit* pkCmd = (SeRaceCmd_HeroAttackUnit*)pkRaceCmd;

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		BeCommand beCommand(BCT_ATTACK, pkCmd->iTargetID);

		pkHero->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
	}
	return true;
}

bool BeMain::Proc_UNIT_ATTACK_UNIT(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitAttackUnit* pkCmd = (SeRaceCmd_UnitAttackUnit*)pkRaceCmd;

	for (int i = 0; i < pkCmd->bySize; i++)
	{
		BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->aiIDs[i]);
		if (pkUnit)
		{
			BeCommand beCommand(BCT_ATTACK, pkCmd->iTargetID);

			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);

			TePtParam kParamUnitCmd;
			kParamUnitCmd.SetParam(BTP_pkTrgUnit, pkUnit);
			gTrgMgr.FireTrigger(BTE_UNIT_PLAYERCMD, kParamUnitCmd);
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_ATTACK(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitAttack* pkCmd = (SeRaceCmd_UnitAttack*)pkRaceCmd;

	for (int i = 0; i < pkCmd->bySize; i++)
	{
		BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->aiIDs[i]);
		if (pkUnit)
		{
			BeCommand beCommand(BCT_ATTACK, pkCmd->iTargetID, 0, 0, pkCmd->byTargetType);

			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);

			TePtParam kParamUnitCmd;
			kParamUnitCmd.SetParam(BTP_pkTrgUnit, pkUnit);
			gTrgMgr.FireTrigger(BTE_UNIT_PLAYERCMD, kParamUnitCmd);
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_PATROL(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitPatrol* pkCmd = (SeRaceCmd_UnitPatrol*)pkRaceCmd;

	for (int i = 0; i < pkCmd->bySize; i++)
	{
		BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->aiIDs[i]);
		if (pkUnit)
		{
			BeCommand beCommand(BCT_PATROL, 0, pkCmd->sTargetX, pkCmd->sTargetY);

			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_FOLLOW(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitFollow* pkCmd = (SeRaceCmd_UnitFollow*)pkRaceCmd;

	for (int i = 0; i < pkCmd->bySize; i++)
	{
		BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->aiIDs[i]);
		if (pkUnit)
		{
			BeCommand beCommand(BCT_MOVE, pkCmd->iTargetID);

			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_SPELL(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitSpell* pkCmd = (SeRaceCmd_UnitSpell*)pkRaceCmd;

	BeUnit* pkUnit = GetHeroBySeat(pkCmd->dwSeat);
	if (pkUnit)
	{
		int iSpellPos = pkCmd->bySpellPos;

		BeSkill* pkSkill = pkUnit->GetSkillByPos(iSpellPos);
		if (!pkSkill)
		{
			return false;
		}

		if (pkSkill)
		{
			switch (pkCmd->byTargetType)
			{
			case BCTT_MAP:
			{
				BeCommand beCommand(BCT_SPELL, 0, pkCmd->sTargetX, pkCmd->sTargetY, pkSkill->GetTypeID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType, false);
				pkUnit->GiveCommand(beCommand, BCT_IMMEDIATE, true);
				break;
			}
			case BCTT_UNIT:
			{
				BeCommand beCommand(BCT_SPELL, pkCmd->iTargetID, 0, 0, pkSkill->GetTypeID(), false, pkCmd->byAutoSelect, (BeCommandTargetType)pkCmd->byTargetType);
				pkUnit->GiveCommand(beCommand, BCT_IMMEDIATE, true);
				break;
			}
			default:break;
			}
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_SPELLXuLi(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitSpellXuLi* pkCmd = (SeRaceCmd_UnitSpellXuLi*)pkRaceCmd;

	BeUnit* pkUnit = GetHeroBySeat(pkCmd->dwSeat);
	if (pkUnit)
	{
		int iSpellPos = pkCmd->bySpellPos;

		BeSkill* pkSkill = pkUnit->GetSkillByPos(iSpellPos);
		if (!pkSkill)
		{
			const UnitTable* pkUnitRes = UnitTableMgr::Get()->GetUnitTable(pkUnit->GetTypeID());
			unsigned int iSkillTypeID = 0;
			int iSpellPos = pkCmd->bySpellPos;
			if (iSpellPos >= 0 && iSpellPos < 4)
			{
				iSkillTypeID = pkUnitRes->iSkillList[pkCmd->bySpellPos];
			}
		}

		if (pkSkill)
		{
			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, pkUnit);
			kParam.SetParam(BTP_pkSkill, pkSkill);
			kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
			kParam.SetParam(BTP_iSkillUsePlayer, pkUnit->GetPlayer());

			gTrgMgr.FireTrigger(BTE_SPELL_XULI, kParam);
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_USE_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitUseItem* pkCmd = (SeRaceCmd_UnitUseItem*)pkRaceCmd;

	BeUnit* pkUnit = GetHeroBySeat(pkCmd->dwSeat);
	if (pkUnit)
	{
		BeItem* pkItem = pkUnit->GetItemByPos(pkCmd->byItemPos);
		if (!pkItem)
		{
			return false;
		}

		switch (pkCmd->byTargetType)
		{
		case BCTT_MAP:
		{
			BeCommand beCommand(BCT_USE_ITEM, 0, pkCmd->sTargetX, pkCmd->sTargetY, pkItem->GetID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType);
			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
			break;
		}
		case BCTT_UNIT:
		{
			BeCommand beCommand(BCT_USE_ITEM, pkCmd->iTargetID, 0, 0, pkItem->GetID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType);
			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
			break;
		}
		case BCTT_MAPITEM:
		{
			BeCommand beCommand(BCT_USE_ITEM, pkCmd->iTargetID, 0, 0, pkItem->GetID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType);
			pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
			break;
		}
		default:break;
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_DROP_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitDropItem* pkCmd = (SeRaceCmd_UnitDropItem*)pkRaceCmd;

	BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->iUnitID);
	if (pkUnit)
	{
		BeItem* pkItem = NULL;
		{
			pkItem = pkUnit->GetItemByPos(pkCmd->byItemPos);
		}
		if (pkItem)
		{
			switch (pkCmd->byTargetType)
			{
			case BCTT_MAP:
			{
				BeCommand beCommand(BCT_DROP_ITEM, 0, pkCmd->sTargetX, pkCmd->sTargetY, pkItem->GetID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType);
				pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
				break;
			}
			case BCTT_UNIT:
			{
				BeCommand beCommand(BCT_DROP_ITEM, pkCmd->iTargetID, 0, 0, pkItem->GetID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType);
				pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
				break;
			}
			case BCTT_MAPITEM:
			{
				BeCommand beCommand(BCT_DROP_ITEM, pkCmd->iTargetID, 0, 0, pkItem->GetID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType);
				pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
				break;
			}
			default:break;
			}
		}
	}
	return true;
}

bool BeMain::Proc_UNIT_PICK_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitPickItem* pkCmd = (SeRaceCmd_UnitPickItem*)pkRaceCmd;

	BeMapItem* pkMapItem = gMapItemMgr.GetMapItemByID(pkCmd->dwMapItemID);
	if (!pkMapItem)
	{
		return false;
	}

	BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->iUnitID);
	if (pkUnit)
	{
		BeCommand beCommand(BCT_PICK_ITEM, 0, 0, 0, pkCmd->dwMapItemID, false, pkCmd->dwSeat);
		pkUnit->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
	}
	return true;
}

bool BeMain::Proc_UNIT_PICK_ITEM_AUTO(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitPickItemAuto* pkCmd = (SeRaceCmd_UnitPickItemAuto*)pkRaceCmd;

	BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->iUnitID);
	if (pkUnit)
	{
		pkUnit->PickAroundItem();
	}
	return true;
}

bool BeMain::Proc_UNIT_GU_DISPLACE_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitGiveUpDisplaceItem* pkCmd = (SeRaceCmd_UnitGiveUpDisplaceItem*)pkRaceCmd;
	BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->iUnitID);
	if (pkUnit)
	{
		pkUnit->SetUD_Int(UDK_NONE, 1);
		{
			DisplaceItemAdd(pkUnit, 4000);
		}
	}
	return true;
}

void BeMain::DisplaceItemAdd(BeUnit* pkUnit, int iItemPrice)
{
}

bool BeMain::Proc_UNIT_DISPLACE_ITEM(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_UNIT_BUY_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitBuyItem* pkCmd = (SeRaceCmd_UnitBuyItem*)pkRaceCmd;

	int iSeat = pkCmd->dwSeat;
	unsigned	int iItemTypeID = pkCmd->uiItemTypeID;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		pkHero->BuyItem(iItemTypeID);
	}

	return true;
}
bool BeMain::Proc_Show_Action(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_ShowAction* pkCmd = (SeRaceCmd_ShowAction*)pkRaceCmd;

	if (pkCmd->usOpt == SAT_SHOW_PROFICIENCY_ACTION)
	{
		BeUnit* pkHero = GetHeroBySeat(pkRaceCmd->dwSeat);
		if (pkHero)
		{
			int iEffectid = gMain.GetHeroProficiencyEffectID(pkHero->GetPlayer());
			pkHero->PlayAction(iEffectid, 1000);
		}
	}
	else if (pkCmd->usOpt == SAT_SHOW_EMOJI_ACTION)
	{
		BeUnit* pkHero = GetHeroBySeat(pkRaceCmd->dwSeat);
		if (pkHero)
		{
			pkHero->PlayAction(m_akPlayer[pkRaceCmd->dwSeat].uiEmojiEffectID, 1000);
		}
	}

	return true;
}

bool BeMain::Proc_UNIT_CHANGE_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitChangeItem* pkCmd = (SeRaceCmd_UnitChangeItem*)pkRaceCmd;
	int iSeat = pkCmd->dwSeat;
	unsigned int uiItemUniqueID = pkCmd->uiItemUniqueID;
	unsigned int uiChangePos = pkCmd->uiChangePos;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		if (0 == uiItemUniqueID)
		{
			pkHero->DelZItemByPos(uiChangePos);
		}
		else
		{
			if (!gMain.PlayerHasZItem(pkCmd->dwSeat, uiItemUniqueID))
			{
				return true;
			}
			const BeZItemInfo* pkItemInfo = gMain.GetPlayerZItemInfoByID(pkCmd->dwSeat, uiItemUniqueID);
			if (!pkItemInfo)
			{
				return true;
			}
			if (pkHero->GetZItemByPos(uiChangePos))
			{
				int iPerPos = pkHero->GetZItemPosByUniqueID(uiItemUniqueID);
				if (-1 == iPerPos)
				{
					pkHero->DelZItemByPos(uiChangePos);
					pkHero->AddZItem(pkItemInfo->iItemType, uiItemUniqueID, uiChangePos);
				}
				else
				{
					pkHero->SwapZItemPos(iPerPos, uiChangePos);
				}
			}
			else
			{
				int iPerPos = pkHero->GetZItemPosByUniqueID(uiItemUniqueID);
				if (-1 != iPerPos)
				{
					pkHero->SwapZItemPos(iPerPos, uiChangePos);
				}
				else
				{
					pkHero->AddZItem(pkItemInfo->iItemType, uiItemUniqueID, uiChangePos);
				}
			}
		}
	}
	else
	{
		return false;
	}

	pkHero->UpdateItemPassiveSkill();
	return true;
}

bool BeMain::Proc_UNIT_DIRECTBUY_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitDirectBuyItem* pkCmd = (SeRaceCmd_UnitDirectBuyItem*)pkRaceCmd;
	std::vector<int> akShopID;
	for (int i = 0; i < pkCmd->bySize; i++)
	{
		akShopID.push_back(pkCmd->aiShopID[i]);
	}

	if (pkCmd->iBuyState == 10086)
	{
	}
	else
	{
	}
	return true;
}

bool BeMain::Proc_UNIT_DIRECTSELL_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitDirectSellItem* pkCmd = (SeRaceCmd_UnitDirectSellItem*)pkRaceCmd;

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		pkHero->SellItem(pkCmd->iPos);
	}

	return true;
}

bool BeMain::Proc_UNIT_SWAP_ITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitSwapItem* pkCmd = (SeRaceCmd_UnitSwapItem*)pkRaceCmd;

	BeUnit* pkUnit = GetHeroBySeat(pkCmd->dwSeat);
	if (pkUnit)
	{
		pkUnit->SwapItemPos(pkCmd->bySrcPos, pkCmd->byTarPos);
	}
	return true;
}

bool BeMain::Proc_UNIT_LEARN_SKILL(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitLearnSkill* pkCmd = (SeRaceCmd_UnitLearnSkill*)pkRaceCmd;

	BeUnit* pkUnit = GetHeroBySeat(pkCmd->dwSeat);
	if (pkUnit)
	{
		return pkUnit->LearnSkill(pkCmd->byLearnPos);
	}
	return true;
}

bool BeMain::Proc_UNIT_ACTIVE_SKILL(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_UnitActiveSkill* pkCmd = (SeRaceCmd_UnitActiveSkill*)pkRaceCmd;

	BeUnit* pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->iUnitID);
	if (pkUnit)
	{
		BeSkill* pkSkill = pkUnit->GetSkillByPos(pkCmd->bySkillPos);
		if (pkSkill)
		{
			pkUnit->ActiveSkill(BeActiveSkill(pkSkill->GetTypeID(), pkCmd->byActive));
		}
	}
	return true;
}

bool BeMain::Proc_PLAYER_RELIVEHERO(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_ReliveHero* pkCmd = (SeRaceCmd_ReliveHero*)pkRaceCmd;

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
	}
	return true;
}

bool BeMain::Proc_PLAYER_MAP_MESSAGE(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_PLAYER_GUANQIA_CMD(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_GuanQia* pkCmd = (SeRaceCmd_GuanQia*)pkRaceCmd;

	BeGuanQiaCommand kGuanQiaCommand;
	memcpy(kGuanQiaCommand.acGuanQiaCommand, pkCmd->acCommand, sizeof(pkCmd->acCommand));
	kGuanQiaCommand.dwGuanQiaCmd = pkCmd->dwCmd;
	kGuanQiaCommand.iGuanQiaPlayer = pkCmd->dwSeat;
	kGuanQiaCommand.fData1 = pkCmd->fData1;
	kGuanQiaCommand.fData2 = pkCmd->fData2;

	GuanQiaCommand(kGuanQiaCommand);
	return true;
}

bool BeMain::Proc_PLAYER_TOWNITEM(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_TownItem* pkCmd = (SeRaceCmd_TownItem*)pkRaceCmd;

	BeTownItemCommand kTownItemCommand(pkCmd->dwSeat, pkCmd->dwTownItemTypeID, pkCmd->dwTargetUnitID, pkCmd->dwData);
	kTownItemCommand.bSuccess = pkCmd->bSuccess;
	return true;
}

bool BeMain::Proc_PLAYER_RECON_BEGIN(const SeRaceCmd* pkRaceCmd)
{
	SePalyerReconBegin* pkCmd = (SePalyerReconBegin*)pkRaceCmd;

	ExcuPlayerReconBegin(pkCmd->dwSeat, pkCmd->dwPlayerID);

	return true;
}

bool BeMain::Proc_PLAYER_RECON_END(const SeRaceCmd* pkRaceCmd)
{
	SePlayerReconEnd* pkCmd = (SePlayerReconEnd*)pkRaceCmd;

	SetPlayerType(pkCmd->dwSeat, BPT_PLAYER);

	OnPlayerRelink(pkCmd->dwSeat);

	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		BePos2 kPos;
		float fFace = 0.0f;
		gMain.GetHeroOrgPos(pkHero->GetPlayer(), kPos, fFace);
		BeCommand	kComm(BCT_MOVE, 0, kPos.fX, kPos.fY);
		pkHero->GiveCommand(kComm, BCT_DEATH);

		BeBuffer* pkBuffer = pkHero->GetBuffer('BF30');
		if (pkBuffer) {

			pkBuffer->SetRemoveTime(gTime);
		}
	}
	if (!IsZhanChangMap())
	{
		BeShareGameNotice	kData;
		kData.bySeat = pkCmd->dwSeat;
		kData.byType = BNT_Relink;
		AddGameNoticeData(kData);
	}
	else
	{
		if (gMain.PlayerHasQualityZItem(pkCmd->dwSeat, 5))
		{
			BeShareGameNotice	kData;
			kData.bySeat = pkCmd->dwSeat;
			kData.byType = BNT_New_Add;
			AddGameNoticeData(kData);
		}
	}
	return true;
}

bool BeMain::Proc_VIEW_SELHERO(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool BeMain::Proc_Race_LockUnit(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_LockUnit* pkLockUnit = (SeRaceCmd_LockUnit*)pkRaceCmd;
	int iPlayer = pkLockUnit->dwSeat;
	int iID = pkLockUnit->iUnitID;
	if (iPlayer > -1 && iPlayer < SSD_MAX_ROOM_PLAYER)
	{
		m_aiLockUnit[iPlayer] = iID;
	}
	return true;
}

bool  BeMain::Proc_PAUSE_BEGIN(const SeRaceCmd* pkRaceCmd)
{
	m_bRacePause = true;
	return true;
}

bool BeMain::Proc_PAUSE_END(const SeRaceCmd* pkRaceCmd)
{
	m_bRacePause = false;
	return true;
}

bool BeMain::IsRacePause()
{
	return m_bRacePause;
}

bool BeMain::Proc_Race_AdjustFrame(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_AdjustFrame* pkCmd = (SeRaceCmd_AdjustFrame*)pkRaceCmd;
	SetGameFrame(pkCmd->dwAdjustFrame);

	return true;
}

bool BeMain::Proc_Race_RelinkBegin(const SeRaceCmd* pkRaceCmd)
{
	m_bReLink = true;

	return true;
}

bool BeMain::Proc_Race_RelinkEnd(const SeRaceCmd* pkRaceCmd)
{
	m_bReLink = false;

	return true;
}

bool BeMain::Proc_SWITCH_HEROFOCUS(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_SwitchHeroFocus* pkCmd = (SeRaceCmd_SwitchHeroFocus*)pkRaceCmd;
	SetHeroFocusPlayerID(pkCmd->dwSeat, pkCmd->iFocusID);

	return true;
}

bool BeMain::Proc_TURN_TABLE_INFO(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_TurnTable* pkCmd = (SeRaceCmd_TurnTable*)pkRaceCmd;
	int iSeat = pkCmd->dwSeat;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		if (1 == pkCmd->usOpt)
		{
			if (pkHero->GetTurnTableInfoByPos(0) == 0 || pkHero->GetHHunStoneNum() == 0 || pkHero->GetTurnResultArrayByPos(0) != 0)
			{
				return true;
			}
			pkHero->RollTurnTableBuffer();
		}
		else if (2 == pkCmd->usOpt)
		{
			bool bCanAdd = false;
			for (int i = 0; i < 6; i++)
			{
				if (pkCmd->uiPos == pkHero->GetTurnResultArrayByPos(i))
				{
					bCanAdd = true;
					break;
				}
			}
			if (bCanAdd && pkHero->GetTurnConfirmResult() == 0 && pkCmd->uiPos != 0)
			{
				pkHero->SetNowTurnTableResult(pkCmd->uiPos);
				pkHero->AddTurnTableAttrInfo(pkCmd->uiPos);
				pkHero->AddTurnTableBuffer();
				pkHero->ReduceHunStoneNum(1);
				pkHero->ResetTurnTableInfo();
			}
		}
		else if (3 == pkCmd->usOpt)
		{
			if ((pkCmd->uiPos == 'HQ01' || pkCmd->uiPos == 'HQ02' || pkCmd->uiPos == 'HQ03') && pkHero->GetHunQiSkillID() == 0)
			{
				pkHero->AddSkill(pkCmd->uiPos);
				pkHero->SetHunQiSkillID(pkCmd->uiPos);
			}
		}
		else if (4 == pkCmd->usOpt)
		{
			if (pkCmd->uiPos < 6 && pkHero->GetTurnTableAttrInfoByPos(pkCmd->uiPos) != 0)
			{
				pkHero->AddHunStoneNum(1);
				pkHero->SetTurnTableAttrInfoByPos(pkCmd->uiPos, 0);
				pkHero->AddTurnTableBuffer();
				pkHero->ResetTurnTableInfo();
			}
		}
	}

	return true;
}

bool BeMain::Proc_COST_EXTERNAL_RESOURCE(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_CostExternalResourceResult* pkCmd = (SeRaceCmd_CostExternalResourceResult*)pkRaceCmd;
	if (pkCmd->iRet != 0)
	{
		return 0;
	}
	int iSeat = pkCmd->dwSeat;
	BeUnit* pkHero = GetHeroBySeat(pkCmd->dwSeat);
	if (pkHero)
	{
		if (pkCmd->iOpt == 1 || pkCmd->iOpt == 2)
		{
			if (pkHero->GetTurnTableInfoByPos(0) == 0)
			{
				return true;
			}
			pkHero->RollTurnTableBuffer();
			pkHero->AddReTurnNum();
		}
		if (pkCmd->iOpt == 3 || pkCmd->iOpt == 4)
		{
			if (pkCmd->iRNum < 6 && pkHero->GetTurnTableAttrInfoByPos(pkCmd->iRNum) != 0)
			{
				pkHero->AddHunStoneNum(1);
				pkHero->SetTurnTableAttrInfoByPos(pkCmd->iRNum, 0);
				pkHero->AddTurnTableBuffer();
				pkHero->ResetTurnTableInfo();
			}
		}
	}

	return true;
}

bool BeMain::Proc_8V8_DEL_PLAYER(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_8V8DelPlayerResult* pkCmd = (SeRaceCmd_8V8DelPlayerResult*)pkRaceCmd;
	int iSeat = pkCmd->dwSeat;
	BeUnit* pkOwner = GetHeroBySeat(iSeat);
	if (pkOwner)
	{
		pkOwner->Set8v8PlayerState(BPS_8V8_DEL);
	}
	return 0;
}


bool BeMain::Proc_8V8_ADD_PLAYER(const SeRaceCmd* pkRaceCmd)
{
	SeAddPlayerRaceLoad* pkCmd = (SeAddPlayerRaceLoad*)pkRaceCmd;

	SeNetStream kNetStream;
	kNetStream.Attach(pkCmd->acData, pkCmd->iDataSize);

	SeLoadSeat kLoadSeat;
	bool bResult = Read(kNetStream, kLoadSeat);
	if (!bResult)
	{
		return false;
	}

	SeLoadSeat& rkSeat = kLoadSeat;
	if (GetHeroBySeat(rkSeat.iSeatIndex))
	{
		return false;
	}

	SetPlayerHeroTypeID(rkSeat.iSeatIndex, rkSeat.dwHeroTypeID);
	SetHeropProficiency(rkSeat.iSeatIndex, rkSeat.dwHeroProficiency);
	SetPlayerID(rkSeat.iSeatIndex, rkSeat.dwPlayerID);
	SetPlayerName(rkSeat.iSeatIndex, rkSeat.PlayerName);
	if (rkSeat.dwPlayerID == (DWord)-1)
	{
		SetPlayerType(rkSeat.iSeatIndex, BPT_ROBOT);
		SetPlayerCharacter(rkSeat.iSeatIndex, BPC_WEAK);
	}
	else
	{
		SetPlayerType(rkSeat.iSeatIndex, BPT_DIS_PLAYER);
	}
	m_akPlayer[rkSeat.iSeatIndex].iCamp = rkSeat.eCamp;

	int		iTempScore = GetCampMaxScore();
	if (rkSeat.uiScore > iTempScore)
	{
		SetCampMaxScore(rkSeat.uiScore);
	}

	m_akPlayer[rkSeat.iSeatIndex].fEquipMaxHp = rkSeat.fEquipMaxHp;
	m_akPlayer[rkSeat.iSeatIndex].fEquipMaxMp = rkSeat.fEquipMaxMp;
	m_akPlayer[rkSeat.iSeatIndex].fEquipRegenHp = rkSeat.fEquipRegenHp;
	m_akPlayer[rkSeat.iSeatIndex].fEquipRegenMp = rkSeat.fEquipRegenMp;
	m_akPlayer[rkSeat.iSeatIndex].fEquipDamage = rkSeat.fEquipDamage;
	m_akPlayer[rkSeat.iSeatIndex].fEquipAttackSpeed = rkSeat.fEquipAttackSpeed;
	m_akPlayer[rkSeat.iSeatIndex].fEquipMagicDamage = rkSeat.fEquipMagicDamage;
	m_akPlayer[rkSeat.iSeatIndex].fEquipMagicArmor = rkSeat.fEquipMagicArmor;
	m_akPlayer[rkSeat.iSeatIndex].fEquipArmor = rkSeat.fEquipArmor;
	m_akPlayer[rkSeat.iSeatIndex].fEquipDecCD = rkSeat.fEquipDecCD;
	m_akPlayer[rkSeat.iSeatIndex].fEquipCritical = rkSeat.fEquipCritical;
	m_akPlayer[rkSeat.iSeatIndex].iEquipSkill[0] = rkSeat.iEquipSkill[0];
	m_akPlayer[rkSeat.iSeatIndex].iEquipSkill[1] = rkSeat.iEquipSkill[1];
	m_akPlayer[rkSeat.iSeatIndex].iEquipSkill[2] = rkSeat.iEquipSkill[2];
	m_akPlayer[rkSeat.iSeatIndex].fLevelAddArmor = rkSeat.fLevelAddArmor;
	m_akPlayer[rkSeat.iSeatIndex].fLevelAddMagicDamage = rkSeat.fLevelAddMagicDamage;
	m_akPlayer[rkSeat.iSeatIndex].fLevelAddMagicArmor = rkSeat.fLevelAddMagicArmor;
	m_akPlayer[rkSeat.iSeatIndex].fPenetrateMagicArmor = rkSeat.fPenetrateMagicArmor;
	m_akPlayer[rkSeat.iSeatIndex].fPenetrateArmor = rkSeat.fPenetrateArmor;
	m_akPlayer[rkSeat.iSeatIndex].fPersentBaseMaxHp = rkSeat.fPersentBaseMaxHp;
	m_akPlayer[rkSeat.iSeatIndex].fBaseMoveSpeed = rkSeat.fBaseMoveSpeed;
	m_akPlayer[rkSeat.iSeatIndex].fPersentLifeSteal = rkSeat.fPersentLifeSteal;
	m_akPlayer[rkSeat.iSeatIndex].fPersentPenetrateMa = rkSeat.fPersentPenetrateMa;
	m_akPlayer[rkSeat.iSeatIndex].iZItenCount = rkSeat.iZItemCount;
	for (int item_idx = 0; item_idx < rkSeat.iZItemCount; item_idx++) {
		m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iUniqueID = rkSeat.kZItemInfo[item_idx].iUniqueID;
		m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iItemType = rkSeat.kZItemInfo[item_idx].iTypeID;
		m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iStrengLV = rkSeat.kZItemInfo[item_idx].iStrengLV;
		for (int iJewelNum = 0; iJewelNum < MAX_EQUIP_JEWEL_NUM; iJewelNum++)
		{
			m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iJewelIds[iJewelNum] = rkSeat.kZItemInfo[item_idx].iJewelIds[iJewelNum];
		}
		for (int iAttrNum = 0; iAttrNum < MAX_EQUIP_ATTR_NUM; iAttrNum++)
		{
			m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].iAttrType[iAttrNum] = rkSeat.kZItemInfo[item_idx].iAttrType[iAttrNum];
			m_akPlayer[rkSeat.iSeatIndex].akZItemsInfo[item_idx].fAttrValue[iAttrNum] = rkSeat.kZItemInfo[item_idx].fAttrValue[iAttrNum];
		}
	}
	CreateOneHero(rkSeat.iSeatIndex);

	return true;
}
