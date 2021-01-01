/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_CommandProc.h"
#include "TW_BattleCmd.h"
#include "TW_ShareUnitData.h"

TwCommandProc::TwCommandProc()
{

}

TwCommandProc::~TwCommandProc()
{

}

bool TwCommandProc::ProcFrameCMD(const SeRaceCmd* pkRaceCmd)
{
	if (pkRaceCmd->dwCmd >= SRC_CMD_NUM)
	{
		return false;
	}
	switch (pkRaceCmd->dwCmd)
	{
	case SRC_FRAME:					return Proc_FRAME(pkRaceCmd);
	case SRC_PLAYER_INFO:			return Proc_PLAYER_INFO(pkRaceCmd);
	case SRC_CHART_PLAYER:			return Proc_CHART_PLAYER(pkRaceCmd);
	case SRC_PLAYER_LEAVE:			return Proc_PLAYER_LEAVE(pkRaceCmd);
	case SRC_SYSTEM_CHEAT:			return Proc_SYSTEM_CHEAT(pkRaceCmd);
	case SRC_HERO_MOVE:				return Proc_HERO_MOVE(pkRaceCmd);
	case SRC_UNIT_MOVE:				return Proc_UNIT_MOVE(pkRaceCmd);
	case SRC_HERO_STOP:				return Proc_HERO_STOP(pkRaceCmd);
	case SRC_HERO_ATTACK_POS:		return Proc_HERO_ATTACK_POS(pkRaceCmd);
	case SRC_HERO_ATTACK_UNIT:		return Proc_HERO_ATTACK_UNIT(pkRaceCmd);
	case SRC_UNIT_SPELL:			return Proc_UNIT_SPELL(pkRaceCmd);
	case SRC_HERO_DIRECT_MOVE:		return Proc_HERO_DIRECT_MOVE(pkRaceCmd);
	case SRC_HERO_ATTACK_CANCEL:	return Proc_HERO_ATTACK_CANCEL(pkRaceCmd);
	case SRC_UNIT_CHANGE_ITEM:		return Proc_UNIT_CHANGE_ITEM(pkRaceCmd);
	case SRC_SHOW_ACTION:			return Proc_Show_Action(pkRaceCmd);
	default:
		return true;
	}
}

bool TwCommandProc::Proc_HERO_ATTACK_CANCEL(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroAttackCancel* pkCmd = (SeRaceCmd_HeroAttackCancel*)pkRaceCmd;
	//std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkHero)
	//{
	//}
	return true;
}

bool TwCommandProc::Proc_HERO_DIRECT_MOVE(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroDirectMove* pkCmd = (SeRaceCmd_HeroDirectMove*)pkRaceCmd;

	float fMoveDirect = pkCmd->fMoveDirect;

	//std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkHero)
	//{
	//	if (fMoveDirect == 10086.0f)
	//	{
	//		BeCommand beCommand(BCT_STOP);
	//		pkHero->GiveCommand(beCommand);
	//	}
	//	else
	//	{
	//		float fTarX = pkHero->GetPosX() + 128.0f * cosf(fMoveDirect);
	//		float fTarY = pkHero->GetPosY() + 128.0f * sinf(fMoveDirect);

	//		BeCommand beCommand(BCT_MOVE, 0, fTarX, fTarY);
	//		pkHero->GiveCommand(beCommand, BCT_IMMEDIATE, true);
	//	}
	//}

	return true;
}

bool TwCommandProc::Proc_FRAME(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_PLAYER_INFO(const SeRaceCmd* pkRaceCmd)
{
	//SeRacePlayerInfo* pkCmd = (SeRacePlayerInfo*)pkRaceCmd;

	//for (int i = 0; i < pkCmd->iNum; ++i)
	//{
	//	SePlayerInfoSeat& rkSeat = pkCmd->akSeat[i];
	//	const BePlayerInfo& rkPlayer = GetPlayerInfo(rkSeat.iSeatIndex);
	//	SetPlayerInfo(rkSeat.iSeatIndex, rkSeat.dwPlayerID, rkPlayer.iHeroUnitID, rkSeat.acName);
	//}
	return true;
}

bool TwCommandProc::Proc_START(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_END(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_CHART_PLAYER(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_PLAYER_LEAVE(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_PlayerLeave* pkCmd = (SeRaceCmd_PlayerLeave*)pkRaceCmd;

	int	iLeavePlayerIdx = pkCmd->dwSeat;

	//if (GetPlayerType(iLeavePlayerIdx) == BPT_PLAYER && !IsZhanChangMap())
	//{
	//	BeShareGameNotice	kData;
	//	kData.bySeat = iLeavePlayerIdx;
	//	kData.byType = BNT_Leave;

	//	AddGameNoticeData(kData);
	//}

	return true;
}

bool TwCommandProc::Proc_SYSTEM_CHEAT(const SeRaceCmd* pkRaceCmd)
{
	//SeRaceCmd_Cheat* pkCmd = (SeRaceCmd_Cheat*)pkRaceCmd;

	//BeCheatCommand kCheatCommand;
	//memcpy(kCheatCommand.acCheatCommand, pkCmd->acCommand, sizeof(pkCmd->acCommand));
	//kCheatCommand.fCheatData = pkCmd->fData;
	//kCheatCommand.iCheatData = pkCmd->iData;
	//kCheatCommand.iCheatPlayer = pkCmd->dwSeat;

	//CheatCommand(kCheatCommand);
	return true;
}

bool TwCommandProc::Proc_HERO_MOVE(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_UNIT_MOVE(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_HERO_STOP(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_HERO_ATTACK_POS(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroAttackPos* pkCmd = (SeRaceCmd_HeroAttackPos*)pkRaceCmd;

	//std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkHero)
	//{
	//	BeCommand beCommand(BCT_ATTACK, 0, pkCmd->sTargetX, pkCmd->sTargetY);

	//	pkHero->GiveCommand(beCommand, BCT_IMMEDIATE, true);
	//}
	return true;
}

bool TwCommandProc::Proc_HERO_ATTACK_UNIT(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_HeroAttackUnit* pkCmd = (SeRaceCmd_HeroAttackUnit*)pkRaceCmd;

	//std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkHero)
	//{
	//	BeCommand beCommand(BCT_ATTACK, pkCmd->iTargetID);

	//	pkHero->GiveCommand(beCommand, pkCmd->dwQueue ? BCT_PUSH_BACK : BCT_IMMEDIATE, true);
	//}
	return true;
}

bool TwCommandProc::Proc_UNIT_SPELL(const SeRaceCmd* pkRaceCmd)
{
	//SeRaceCmd_UnitSpell* pkCmd = (SeRaceCmd_UnitSpell*)pkRaceCmd;

	//std::shared_ptr<BeUnit> pkUnit = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkUnit)
	//{
	//	int iSpellPos = pkCmd->bySpellPos;

	//	BeSkill* pkSkill = pkUnit->GetSkillByPos(iSpellPos);
	//	if (!pkSkill)
	//	{
	//		return false;
	//	}

	//	if (pkSkill)
	//	{
	//		switch (pkCmd->byTargetType)
	//		{
	//		case BCTT_MAP:
	//		{
	//			BeCommand beCommand(BCT_SPELL, 0, pkCmd->sTargetX, pkCmd->sTargetY, pkSkill->GetTypeID(), false, pkCmd->dwSeat, (BeCommandTargetType)pkCmd->byTargetType, false);
	//			pkUnit->GiveCommand(beCommand, BCT_IMMEDIATE, true);
	//			break;
	//		}
	//		case BCTT_UNIT:
	//		{
	//			BeCommand beCommand(BCT_SPELL, pkCmd->iTargetID, 0, 0, pkSkill->GetTypeID(), false, pkCmd->byAutoSelect, (BeCommandTargetType)pkCmd->byTargetType);
	//			pkUnit->GiveCommand(beCommand, BCT_IMMEDIATE, true);
	//			break;
	//		}
	//		default:break;
	//		}
	//	}
	//}
	return true;
}

bool TwCommandProc::Proc_UNIT_DISPLACE_ITEM(const SeRaceCmd* pkRaceCmd)
{
	return true;
}

bool TwCommandProc::Proc_Show_Action(const SeRaceCmd* pkRaceCmd)
{
	SeRaceCmd_ShowAction* pkCmd = (SeRaceCmd_ShowAction*)pkRaceCmd;

	//if (pkCmd->usOpt == SAT_SHOW_PROFICIENCY_ACTION)
	//{
	//	std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkRaceCmd->dwSeat);
	//	if (pkHero)
	//	{
	//		int iEffectid = gMain->GetHeroProficiencyEffectID(pkHero->GetPlayer());
	//		pkHero->PlayAction(iEffectid, 1000);
	//	}
	//}
	//else if (pkCmd->usOpt == SAT_SHOW_EMOJI_ACTION)
	//{
	//	std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkRaceCmd->dwSeat);
	//	if (pkHero)
	//	{
	//		pkHero->PlayAction(m_akPlayer[pkRaceCmd->dwSeat].uiEmojiEffectID, 1000);
	//	}
	//}

	return true;
}

bool TwCommandProc::Proc_UNIT_CHANGE_ITEM(const SeRaceCmd* pkRaceCmd)
{
	//SeRaceCmd_UnitChangeItem* pkCmd = (SeRaceCmd_UnitChangeItem*)pkRaceCmd;
	//int iSeat = pkCmd->dwSeat;
	//unsigned int uiItemUniqueID = pkCmd->uiItemUniqueID;
	//unsigned int uiChangePos = pkCmd->uiChangePos;
	//std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkHero)
	//{
	//	if (0 == uiItemUniqueID)
	//	{
	//		pkHero->DelZItemByPos(uiChangePos);
	//	}
	//	else
	//	{
	//		if (!gMain->PlayerHasZItem(pkCmd->dwSeat, uiItemUniqueID))
	//		{
	//			return true;
	//		}
	//		const BeZItemInfo* pkItemInfo = gMain->GetPlayerZItemInfoByID(pkCmd->dwSeat, uiItemUniqueID);
	//		if (!pkItemInfo)
	//		{
	//			return true;
	//		}
	//		if (pkHero->GetZItemByPos(uiChangePos))
	//		{
	//			int iPerPos = pkHero->GetZItemPosByUniqueID(uiItemUniqueID);
	//			if (-1 == iPerPos)
	//			{
	//				pkHero->DelZItemByPos(uiChangePos);
	//				pkHero->AddZItem(pkItemInfo->iItemType, uiItemUniqueID, uiChangePos);
	//			}
	//			else
	//			{
	//				pkHero->SwapZItemPos(iPerPos, uiChangePos);
	//			}
	//		}
	//		else
	//		{
	//			int iPerPos = pkHero->GetZItemPosByUniqueID(uiItemUniqueID);
	//			if (-1 != iPerPos)
	//			{
	//				pkHero->SwapZItemPos(iPerPos, uiChangePos);
	//			}
	//			else
	//			{
	//				pkHero->AddZItem(pkItemInfo->iItemType, uiItemUniqueID, uiChangePos);
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	return false;
	//}

	//pkHero->UpdateItemPassiveSkill();
	return true;
}



bool TwCommandProc::Proc_UNIT_LEARN_SKILL(const SeRaceCmd* pkRaceCmd)
{
	//SeRaceCmd_UnitLearnSkill* pkCmd = (SeRaceCmd_UnitLearnSkill*)pkRaceCmd;

	//std::shared_ptr<BeUnit> pkUnit = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkUnit)
	//{
	//	return pkUnit->LearnSkill(pkCmd->byLearnPos);
	//}
	return true;
}

bool TwCommandProc::Proc_UNIT_ACTIVE_SKILL(const SeRaceCmd* pkRaceCmd)
{
	// SeRaceCmd_UnitActiveSkill* pkCmd = (SeRaceCmd_UnitActiveSkill*)pkRaceCmd;

	//std::shared_ptr<BeUnit> pkUnit = GetUnitControlBySeat(pkCmd->dwSeat, pkCmd->iUnitID);
	//if (pkUnit)
	//{
	//	BeSkill* pkSkill = pkUnit->GetSkillByPos(pkCmd->bySkillPos);
	//	if (pkSkill)
	//	{
	//		pkUnit->ActiveSkill(BeActiveSkill(pkSkill->GetTypeID(), pkCmd->byActive));
	//	}
	//}
	return true;
}

bool TwCommandProc::Proc_PLAYER_RELIVEHERO(const SeRaceCmd* pkRaceCmd)
{
	// SeRaceCmd_ReliveHero* pkCmd = (SeRaceCmd_ReliveHero*)pkRaceCmd;

	//std::shared_ptr<BeUnit> pkHero = GetHeroBySeat(pkCmd->dwSeat);
	//if (pkHero)
	//{
	//}
	return true;
}

bool TwCommandProc::Proc_PLAYER_MAP_MESSAGE(const SeRaceCmd* pkRaceCmd)
{
	return true;
}
