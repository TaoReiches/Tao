/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Main.h"

#define PushRecordV(Param,Type); if(!Param.empty())\
								{\
									Type* pkData = MNew(m_pcSend,Type);\
									pkData->dwFrame = GetFrameCount() - 1;\
									m_kNetStream.Attach(pkData->acData,SSD_SEND_SIZE);\
									int iSize = sizeof(Type) - SSD_ENDSTR_LEN;\
									m_kNetStream.WriteByte((int)Param.size());\
									for(int j = 0;j < (int)Param.size();++j)\
									{\
										Write(m_kNetStream,Param[j]);\
									}\
									int iDataSize = m_kNetStream.GetCurPos() + iSize;\
									pkData->iDataSize = iDataSize;\
									PushPureDataCmdRecord(eCamp,pkData,iDataSize);\
								}

#define PushRecord(Param,Type); {\
								Type* pkData = MNew(m_pcSend,Type);\
								pkData->dwFrame = GetFrameCount() - 1;\
								m_kNetStream.Attach(pkData->acData,SSD_SEND_SIZE);\
								int iSize = sizeof(Type) - SSD_ENDSTR_LEN;\
								Write(m_kNetStream,Param);\
								int iDataSize = m_kNetStream.GetCurPos() + iSize;\
								pkData->iDataSize = iDataSize;\
								PushPureDataCmdRecord(eCamp,pkData,iDataSize);\
								}

#define PushRecordPlayerV(Param,Type,iSeat); if(!Param.empty())\
											{\
											Type* pkData = MNew(m_pcSend,Type);\
											pkData->dwFrame = GetFrameCount() - 1;\
											m_kNetStream.Attach(pkData->acData,SSD_SEND_SIZE);\
											int iSize = sizeof(Type) - SSD_ENDSTR_LEN;\
											m_kNetStream.WriteByte((int)Param.size());\
											for(int j = 0;j < (int)Param.size();++j)\
											{\
												Write(m_kNetStream,Param[j]);\
											}\
											int iDataSize = m_kNetStream.GetCurPos() + iSize;\
											pkData->iDataSize = iDataSize;\
											m_akPureDataCmdRecord[iSeat].SetHostSvr(false);\
											m_akPureDataCmdRecord[iSeat].PushRaceData(pkData,iDataSize);\
											}

#define PushRecordPlayer(Param,Type,iSeat);	{\
											Type* pkData = MNew(m_pcSend,Type);\
											pkData->dwFrame = GetFrameCount() - 1;\
											m_kNetStream.Attach(pkData->acData,SSD_SEND_SIZE);\
											int iSize = sizeof(Type) - SSD_ENDSTR_LEN;\
											Write(m_kNetStream,Param);\
											int iDataSize = m_kNetStream.GetCurPos() + iSize;\
											pkData->iDataSize = iDataSize;\
											m_akPureDataCmdRecord[iSeat].SetHostSvr(false);\
											m_akPureDataCmdRecord[iSeat].PushRaceData(pkData,iDataSize);\
											}

#define PushViewRecordV(Param,Type);	if(!Param.empty())\
										{\
											Type* pkData = MNew(m_pcSend,Type);\
											pkData->dwFrame = GetFrameCount() - 1;\
											m_kNetStream.Attach(pkData->acData,SSD_SEND_SIZE);\
											int iSize = sizeof(Type) - SSD_ENDSTR_LEN;\
											m_kNetStream.WriteByte((int)Param.size());\
											for(int j = 0;j < (int)Param.size();++j)\
											{\
												Write(m_kNetStream,Param[j]);\
											}\
											int iDataSize = m_kNetStream.GetCurPos() + iSize;\
											pkData->iDataSize = iDataSize;\
											m_kViewData.SetHostSvr(false);\
											m_kViewData.PushRaceData(pkData,iDataSize);\
										}

#define PushViewRecord(Param,Type);	{\
											Type* pkData = MNew(m_pcSend,Type);\
											pkData->dwFrame = GetFrameCount() - 1;\
											m_kNetStream.Attach(pkData->acData,SSD_SEND_SIZE);\
											int iSize = sizeof(Type) - SSD_ENDSTR_LEN;\
											Write(m_kNetStream,Param);\
											int iDataSize = m_kNetStream.GetCurPos() + iSize;\
											pkData->iDataSize = iDataSize;\
											m_kViewData.SetHostSvr(false);\
											m_kViewData.PushRaceData(pkData,iDataSize);\
									}

#define CompareValue(St,Cur,Last,Flag) if(Cur != Last) {St.iChangeFlag |= Flag;} 

void BeMain::FinializeServerMode()
{
}

void BeMain::InitServerMode()
{
	FinializeServerMode();
}

void BeMain::UpdateRaceOut()
{
	SeRoomPlayerCamp eCamp = SRPC_CAMPA;
	for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
	{
		if (GetPlayerType(i) == BPT_PLAYER)
		{
			const BeShareSelfData& kShareSelfData = GetSelfShowData(i);
			if (kShareSelfData.iChangeFlag != 0)
			{
				PushRecordPlayer(kShareSelfData, SePureCmd_SelfData, i);

				PushViewRecord(kShareSelfData, SePureCmd_SelfData);
			}
		}
	}

	const	std::vector<BeShareKillHeroData>& kKillHeroData = GetKillHeroData();
	const	std::vector<BeShareEffectData>& kShowEffectData = GetShareEffectData();
	const	std::vector<BeShareMapItemData>& kShowMapItemData = GetMapItemData();

	std::vector<int>& rkDelEffectID = gEffectMgr.PureGetDelEffect();

	const	std::vector<BeShareAttachData>& kShowAttachData = GetAttachData();
	const	std::vector<BeShareUIWindowData>& kAllWindowData = GetWindowData();

	const	Es::HashMap<int, BeUnit*>& kAllLiveUnit = gUnitMgr.GetID2Unit();
	const	Es::HashMap<int, BeUnit*>& kAllSuspendUnit = gUnitMgr.GetID2SuspendUnit();
	std::vector<BeShareUnitData>	akShareData;

	for (int iIndex = 0; iIndex < iPureCampNum; iIndex++)
	{
		akShareData.clear();
		eCamp = GetCamp(iIndex);
		auto	Iter = kAllLiveUnit.Begin();
		auto	IterEnd = kAllLiveUnit.End();
		while (Iter != IterEnd)
		{
			BeUnit* pkTempUnit = Iter->Value();
			if (pkTempUnit)
			{
				if (pkTempUnit->GetUnitVisionForCamp(iIndex) || pkTempUnit->HasFlag(BUF_DEAD))
				{
					const BeShareUnitData& rkData = GetUnitShowData(pkTempUnit, pkTempUnit->IsNewUnit(iIndex));
					if (rkData.iChangeFlag != 0)
					{
						akShareData.push_back(rkData);
					}
				}
			}
			Iter++;
		}

		auto	IterSuspend = kAllSuspendUnit.Begin();
		auto	IterSuspendEnd = kAllSuspendUnit.End();
		while (IterSuspend != IterSuspendEnd)
		{
			BeUnit* pkTempUnit = IterSuspend->Value();
			if (pkTempUnit)
			{
				if (pkTempUnit->GetUnitVisionForCamp(iIndex))
				{
					const BeShareUnitData& rkData = GetUnitShowData(pkTempUnit, pkTempUnit->IsNewUnit(iIndex));
					if (rkData.iChangeFlag != 0)
					{
						akShareData.push_back(rkData);
					}
				}
			}
			IterSuspend++;
		}

		PushRecordV(akShareData, SePureCmd_UnitShowData);
	}
	{
		akShareData.clear();
		auto	Iter = kAllLiveUnit.Begin();
		auto	IterEnd = kAllLiveUnit.End();
		while (Iter != IterEnd)
		{
			BeUnit* pkTempUnit = Iter->Value();
			if (pkTempUnit)
			{
				if (!pkTempUnit->HasProperty(UNIT_PROPERTY_HIDEEVER))
				{
					const BeShareUnitData& rkData = GetUnitShowData(pkTempUnit);
					if (rkData.iChangeFlag != 0)
					{
						akShareData.push_back(rkData);
					}
				}
			}
			Iter++;
		}
		PushViewRecordV(akShareData, SePureCmd_UnitShowData);
	}

	for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
	{
		if (GetPlayerType(i) == BPT_PLAYER)
		{
			UnitGroupID kGroupID;
			GenVisionUnit(i, kGroupID);

			const std::vector<BeShareDamageData>& kDamageData = GetVisionDamageData(i, kGroupID);

			if (kDamageData.size() > 0)
			{
				PushRecordPlayerV(kDamageData, SePureCmd_DamageData, i);
			}
			if (m_aiLockUnit[i] > 0)
			{
				BeShareLockUnitData& kLockUnitData = GetLockUnitData(i);
				if (kLockUnitData.iChangeFlag != 0)
				{
					PushRecordPlayer(kLockUnitData, SePureCmd_LockUnitData, i);
					memset(&m_kLockUnitData[i], 0, sizeof(m_kLockUnitData[i]));
				}
			}
		}
	}
	SePureCmd_PingData	kPingData;
	for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
	{
		if (GetPlayerType(i) == BPT_PLAYER)
		{
			if (NeedSendPing(i))
			{
				PushRecordPlayer(kPingData, SePureCmd_PingData, i);
			}
		}
	}
	if (!m_kSmallMapMessage.empty())
	{
		for (int iDataIndex = 0; iDataIndex < m_kSmallMapMessage.size(); iDataIndex++)
		{
			int		iDataPlayerIndex = m_kSmallMapMessage[iDataIndex].first;
			for (int iPlayerIndex = 0; iPlayerIndex < SERVERMODE_MAXPLAYER; iPlayerIndex++)
			{
				if (GetPlayerType(iPlayerIndex) == BPT_PLAYER)
				{
					if (GetPlayerCamp(iPlayerIndex) == GetPlayerCamp(iDataPlayerIndex))
					{
						PushRecordPlayer(m_kSmallMapMessage[iDataIndex].second, SePureCmd_SmallMap, iPlayerIndex);
					}
				}
			}
		}

		m_kSmallMapMessage.clear();
	}
	if (!m_kGameNoticeData.empty())
	{
		for (int iDataIndex = 0; iDataIndex < m_kGameNoticeData.size(); iDataIndex++)
		{
			for (int iPlayerIndex = 0; iPlayerIndex < SERVERMODE_MAXPLAYER; iPlayerIndex++)
			{
				if (GetPlayerType(iPlayerIndex) == BPT_PLAYER)
				{
					PushRecordPlayer(m_kGameNoticeData[iDataIndex], SePureCmd_GameNotice, iPlayerIndex);
				}
			}
		}

		m_kGameNoticeData.clear();
	}

	const std::vector<BeShareBufferData>& kShowBufferData = GetBufferShowData();
	const std::vector<BeShareChainEffectData>& kChainData = GetChainData();
	for (int iCampIndex = 0; iCampIndex < iPureCampNum; iCampIndex++)
	{
		eCamp = GetCamp(iCampIndex);

		std::vector<int> akDelUnit;
		const	UnitGroupID& rkDelUnit = gUnitMgr.PureGetDelUnitID(iCampIndex);
		for (int iDelUnitIndex = 0; iDelUnitIndex < (int)rkDelUnit.Size(); iDelUnitIndex++)
		{
			akDelUnit.push_back(rkDelUnit[iDelUnitIndex]);
		}

		PushRecordV(akDelUnit, SePureCmd_UnitDisappear);

		PushRecordV(kKillHeroData, SePureCmd_KillHeroData);
		PushRecordV(kShowMapItemData, SePureCmd_MapItemData);
		PushRecordV(kShowAttachData, SePureCmd_AttachData);
		PushRecordV(kShowEffectData, SePureCmd_EffectData);
		PushRecordV(rkDelEffectID, SePureCmd_EffectDel);
		PushRecordV(kAllWindowData, SePureCmd_UIWindowData);
		PushRecordV(kShowBufferData, SePureCmd_BufData);
		PushRecordV(kChainData, SePureCmd_ChainEffectData);
	}
	{
		PushViewRecordV(kKillHeroData, SePureCmd_KillHeroData);
		PushViewRecordV(kShowAttachData, SePureCmd_AttachData);
		PushViewRecordV(kShowEffectData, SePureCmd_EffectData);
		PushViewRecordV(rkDelEffectID, SePureCmd_EffectDel);
		PushViewRecordV(kShowBufferData, SePureCmd_BufData);
	}

	GetTabinfoData();

	const std::vector<BeShareUnitFlyData>& kUnitFlyData = GetUnitFlyData();
	if (kUnitFlyData.size() > 0)
	{
		for (int iCampIndex = 0; iCampIndex < iPureCampNum; iCampIndex++)
		{
			eCamp = GetCamp(iCampIndex);
			PushRecordV(kUnitFlyData, SePureCmd_UnitFly);
		}
		{
			PushViewRecordV(kUnitFlyData, SePureCmd_UnitFly);
		}
		ClearUnitFlyData();
	}

	if (IsZhanChangMap())
	{
		for (int iHeroIndex = 0; iHeroIndex < SERVERMODE_MAXPLAYER; iHeroIndex++)
		{
			if (GetPlayerType(iHeroIndex) == BPT_PLAYER)
			{
				BeUnit* pkHero = gMain.GetHeroBySeat(iHeroIndex);
				if (!pkHero)
				{
					continue;
				}
				unsigned	int		iTurnTableFlag = pkHero->GetTurnTableFlag();
				if (iTurnTableFlag != 0)
				{
					BeZhanChangTrunTable	kTurnTableData;
					kTurnTableData.byTurnTableFlag = iTurnTableFlag;
					kTurnTableData.uiNowResult = pkHero->GetNowTurnTableResult();
					kTurnTableData.uiReturnNum = pkHero->GetReTurnNum();
					kTurnTableData.uiConfirmResult = pkHero->GetTurnConfirmResult();
					kTurnTableData.uiTableBuffer[0] = pkHero->GetTurnTableInfoByPos(0);
					kTurnTableData.uiTableBuffer[1] = pkHero->GetTurnTableInfoByPos(1);
					kTurnTableData.uiTableBuffer[2] = pkHero->GetTurnTableInfoByPos(2);
					kTurnTableData.uiTableBuffer[3] = pkHero->GetTurnTableInfoByPos(3);
					kTurnTableData.uiTableBuffer[4] = pkHero->GetTurnTableInfoByPos(4);
					kTurnTableData.uiTableBuffer[5] = pkHero->GetTurnTableInfoByPos(5);
					kTurnTableData.uiResultArray[0] = pkHero->GetTurnResultArrayByPos(0);
					kTurnTableData.uiResultArray[1] = pkHero->GetTurnResultArrayByPos(1);
					kTurnTableData.uiResultArray[2] = pkHero->GetTurnResultArrayByPos(2);
					kTurnTableData.uiResultArray[3] = pkHero->GetTurnResultArrayByPos(3);
					kTurnTableData.uiResultArray[4] = pkHero->GetTurnResultArrayByPos(4);
					kTurnTableData.uiResultArray[5] = pkHero->GetTurnResultArrayByPos(5);
					kTurnTableData.uiAttrInfo[0] = pkHero->GetTurnTableAttrInfoByPos(0);
					kTurnTableData.uiAttrInfo[1] = pkHero->GetTurnTableAttrInfoByPos(1);
					kTurnTableData.uiAttrInfo[2] = pkHero->GetTurnTableAttrInfoByPos(2);
					kTurnTableData.uiAttrInfo[3] = pkHero->GetTurnTableAttrInfoByPos(3);
					kTurnTableData.uiAttrInfo[4] = pkHero->GetTurnTableAttrInfoByPos(4);
					kTurnTableData.uiAttrInfo[5] = pkHero->GetTurnTableAttrInfoByPos(5);
					kTurnTableData.usStoneNum = pkHero->GetHHunStoneNum();
					PushRecordPlayer(kTurnTableData, SeTurnTable_TabInfo, iHeroIndex);
					PushViewRecord(kTurnTableData, SeTurnTable_TabInfo);
					pkHero->ClearTurnTableFlag();
				}

			}
		}
		for (int iHeroIndex = 0; iHeroIndex < SERVERMODE_MAXPLAYER; iHeroIndex++)
		{
			if (GetPlayerType(iHeroIndex) != BPT_NULL)
			{
				BeUnit* pkHero = gMain.GetHeroBySeat(iHeroIndex);
				if (pkHero && pkHero->Get8v8PlayerState() == BPS_8V8_DEL)
				{
					Be8V8State kData;
					kData.iSeat = iHeroIndex;
					kData.uiLogicID = pkHero->GetID();
					for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
					{
						if (GetPlayerType(i) == BPT_PLAYER && i != iHeroIndex)
						{
							PushRecordPlayer(kData, SePureCmd_8V8PlayerState, i);
						}
					}
					DelOneHero(iHeroIndex);
				}
			}
		}
	}
	if (gMain.IsSecretArea()) {
		BeSecretAreaInfo kSecretArea;
		kSecretArea.bySecretAreaFlag = gData.GetSecretAreaFlag();
		for (int iHeroIndex = 0; iHeroIndex < SERVERMODE_MAXPLAYER; iHeroIndex++)
		{
			if (GetPlayerType(iHeroIndex) == BPT_PLAYER)
			{
				if (kSecretArea.bySecretAreaFlag != 0)
				{
					kSecretArea.uiEndTime = gData.GetSecretAreaEndTime();
					kSecretArea.uiLayer = gData.GetSecretAreaLayer();
					kSecretArea.byEnd = gData.GetSecretAreaFail();
					PushRecordPlayer(kSecretArea, SePureCmd_SecretArea, iHeroIndex);
				}
			}
		}
		gData.ClearSecretFlag();
	}

	if (GetState() == BMS_OVER)
	{
		for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
		{
			if (GetPlayerType(i) == BPT_PLAYER)
			{
				int		iResult = GetGameOver(i);
				Byte	bySendResult = 0;

				if ((iResult & BGI_FINISHED_SUCCES) != 0)
				{
					bySendResult = 1;
				}
				if ((iResult & BGI_SURRENDER_FAILED) != 0)
				{
					bySendResult = 2;
				}
				if ((iResult & BGI_SURRENDER_SUCCES) != 0)
				{
					bySendResult = 3;
				}
				PushRecordPlayer(bySendResult, SePureCmd_GameEnd, i);
			}
		}
		int	iResult = GetGameOver(0);
		Byte bySendResult = 0;
		if ((iResult & BGI_FINISHED_SUCCES) != 0)
		{
			bySendResult = 1;
		}

		if ((iResult & BGI_SURRENDER_FAILED) != 0)
		{
			bySendResult = 2;
		}
		if ((iResult & BGI_SURRENDER_SUCCES) != 0)
		{
			bySendResult = 3;
		}
		PushViewRecord(bySendResult, SePureCmd_GameEnd);
	}

	ClrAllPureData();
}

void	BeMain::GetTabinfoData()
{
	for (int iCampIndex = 0; iCampIndex < iPureCampNum; iCampIndex++)
	{
		SeRoomPlayerCamp eCamp = GetCamp(iCampIndex);
		for (int iHeroIndex = 0; iHeroIndex < SERVERMODE_MAXPLAYER; iHeroIndex++)
		{
			if (GetPlayerType(iHeroIndex) != BPT_NULL)
			{
				BeUnit* pkHero = gMain.GetHeroBySeat(iHeroIndex);
				if (pkHero)
				{

					if (IsZhanChangMap() && pkHero->Get8v8PlayerState() != BPS_8V8_DEL)
					{
						unsigned	int		iTabInfoChangeFlag = pkHero->GetTabInfoFlag(eCamp);
						if (iTabInfoChangeFlag > 0)
						{
							BeShareTabInfo	kTabInfoData;
							kTabInfoData.uiChangeFlag = iTabInfoChangeFlag;
							kTabInfoData.bySeat = pkHero->GetPlayer();
							kTabInfoData.byKill = gMain.GetPlayerKillCount(iHeroIndex);
							kTabInfoData.byHelp = gMain.GetHelpCount(iHeroIndex);
							kTabInfoData.byDead = gMain.GetPlayerBeKilledCount(iHeroIndex);
							kTabInfoData.byLevel = pkHero->GetLevel();
							kTabInfoData.sUnitID = pkHero->GetID();
							kTabInfoData.uiReliveTime = pkHero->GetUnitReliveTime();
							kTabInfoData.uiSurrenderTime = gMain.GetSurrenderTime(iHeroIndex);
							kTabInfoData.uiFeatsValue = gMain.GetPlayerFeatsValue(iHeroIndex);
							kTabInfoData.uiMoraleValue = gMain.GetPlayerMoraleValue(iHeroIndex);
							kTabInfoData.uiTeamScore = gMain.GetCampTeamScore(pkHero->GetCamp());
							for (int iItemIndex = 0; iItemIndex < 6; iItemIndex++)
							{
								if (gMain.IsPveMap())
								{
									kTabInfoData.uiItem[iItemIndex] = pkHero->GetZItemUniqueIDByPos(iItemIndex);
								}
								else
								{
									kTabInfoData.uiItem[iItemIndex] = pkHero->GetItemTypeIDByPos(iItemIndex);
								}

							}
							kTabInfoData.usKillSolider = gMain.GetKillSoildiers(iHeroIndex);

							if (iTabInfoChangeFlag & BTCF_BIGSKILL)
							{
								BeSkill* pkBigSkill = pkHero->GetSkillByPos(3);
								if (pkBigSkill)
								{
									SeCalSkillLvlData	rkData;
									if (pkHero->GetSkillLvlData(rkData, pkBigSkill->GetTypeID()))
									{
										int		iCDTime = rkData.GetCoolDown();
										int		iLastTime = pkHero->GetCommonCDLastUseTime(pkBigSkill->GetTypeID());
										kTabInfoData.uiBigSkillTime = iLastTime + iCDTime;
									}
								}
							}
							for (int iSendIndex = 0; iSendIndex < SERVERMODE_MAXPLAYER; iSendIndex++)
							{
								if (GetPlayerType(iSendIndex) != BPT_NULL)
								{
									if (GetPlayerCamp(iSendIndex) == eCamp)
									{
										PushRecordPlayer(kTabInfoData, SePureCmd_TabInfo, iSendIndex);
										PushViewRecord(kTabInfoData, SePureCmd_TabInfo);
									}
								}
							}

							pkHero->ClrTabInfoFlag(eCamp);
						}

					}
					else
					{
						if (pkHero->GetUnitVisionForCamp(iCampIndex))
						{
							unsigned	int		iTabInfoChangeFlag = pkHero->GetTabInfoFlag(eCamp);
							if (iTabInfoChangeFlag > 0)
							{
								BeShareTabInfo	kTabInfoData;
								kTabInfoData.uiChangeFlag = iTabInfoChangeFlag;
								kTabInfoData.bySeat = pkHero->GetPlayer();
								kTabInfoData.byKill = gMain.GetPlayerKillCount(iHeroIndex);
								kTabInfoData.byHelp = gMain.GetHelpCount(iHeroIndex);
								kTabInfoData.byDead = gMain.GetPlayerBeKilledCount(iHeroIndex);
								kTabInfoData.byLevel = pkHero->GetLevel();
								kTabInfoData.sUnitID = pkHero->GetID();
								kTabInfoData.uiReliveTime = pkHero->GetUnitReliveTime();
								kTabInfoData.uiSurrenderTime = gMain.GetSurrenderTime(iHeroIndex);
								kTabInfoData.uiFeatsValue = gMain.GetPlayerFeatsValue(iHeroIndex);
								kTabInfoData.uiMoraleValue = gMain.GetPlayerMoraleValue(iHeroIndex);
								kTabInfoData.uiTeamScore = gMain.GetCampTeamScore(pkHero->GetCamp());
								for (int iItemIndex = 0; iItemIndex < 6; iItemIndex++)
								{
									if (gMain.IsPveMap())
									{
										kTabInfoData.uiItem[iItemIndex] = pkHero->GetZItemUniqueIDByPos(iItemIndex);
									}
									else
									{
										kTabInfoData.uiItem[iItemIndex] = pkHero->GetItemTypeIDByPos(iItemIndex);
									}

								}
								kTabInfoData.usKillSolider = gMain.GetKillSoildiers(iHeroIndex);

								if (iTabInfoChangeFlag & BTCF_BIGSKILL)
								{
									BeSkill* pkBigSkill = pkHero->GetSkillByPos(3);
									if (pkBigSkill)
									{
										SeCalSkillLvlData	rkData;
										if (pkHero->GetSkillLvlData(rkData, pkBigSkill->GetTypeID()))
										{
											int		iCDTime = rkData.GetCoolDown();
											int		iLastTime = pkHero->GetCommonCDLastUseTime(pkBigSkill->GetTypeID());
											kTabInfoData.uiBigSkillTime = iLastTime + iCDTime;
										}
									}
								}
								for (int iSendIndex = 0; iSendIndex < SERVERMODE_MAXPLAYER; iSendIndex++)
								{
									if (GetPlayerType(iSendIndex) != BPT_NULL)
									{
										if (GetPlayerCamp(iSendIndex) == eCamp)
										{
											PushRecordPlayer(kTabInfoData, SePureCmd_TabInfo, iSendIndex);
											PushViewRecord(kTabInfoData, SePureCmd_TabInfo);
										}
									}
								}

								pkHero->ClrTabInfoFlag(eCamp);
							}
						}
					}

				}
			}
		}
	}
}

void BeMain::OnPlayerRelink(int iSeat)
{
	m_akPureDataCmdRecord[iSeat].ResetSM();
	SeRoomPlayerCamp eCamp = (SeRoomPlayerCamp)gMain.GetPlayerCamp(iSeat);

	const BeShareSelfData& kShareSelfData = GetSelfShowData(iSeat, true);
	if (kShareSelfData.iChangeFlag != 0)
	{
		PushRecordPlayer(kShareSelfData, SePureCmd_SelfData, iSeat);
	}

	const	Es::HashMap<int, BeUnit*>& kAllLiveUnit = gUnitMgr.GetID2Unit();
	std::vector<BeShareUnitData> akShareData;
	auto	Iter = kAllLiveUnit.Begin();
	while (Iter != kAllLiveUnit.End())
	{
		BeUnit* pkTempUnit = Iter->Value();
		if (pkTempUnit)
		{
			if (pkTempUnit->GetUnitVisionForCamp(eCamp - 1) || pkTempUnit->HasFlag(BUF_DEAD))
			{
				const BeShareUnitData& rkData = GetUnitShowData(pkTempUnit, true);
				if (rkData.iChangeFlag != 0)
				{
					akShareData.push_back(rkData);
				}
			}
		}
		Iter++;
	}

	const Es::HashMap<int, BeUnit*>& kAllDeadUnit = gUnitMgr.GetID2SuspendUnit();
	auto	IterDeadUnit = kAllDeadUnit.Begin();
	while (IterDeadUnit != kAllDeadUnit.End())
	{
		BeUnit* pkTempUnit = IterDeadUnit->Value();
		if (pkTempUnit)
		{
			const BeShareUnitData& rkData = GetUnitShowData(pkTempUnit, true);
			if (rkData.iChangeFlag != 0)
			{
				akShareData.push_back(rkData);
			}
		}
		IterDeadUnit++;
	}

	PushRecordPlayerV(akShareData, SePureCmd_UnitShowData, iSeat);

	for (int iHeroIndex = 0; iHeroIndex < SERVERMODE_MAXPLAYER; iHeroIndex++)
	{
		if (GetPlayerType(iHeroIndex) != BPT_NULL)
		{
			BeUnit* pkHero = gMain.GetHeroBySeat(iHeroIndex);
			if (pkHero && pkHero->Get8v8PlayerState() != BPS_8V8_DEL)
			{
				unsigned	int		iTabInfoChangeFlag = uiAllFilg;

				BeShareTabInfo	kTabInfoData;
				kTabInfoData.uiChangeFlag = iTabInfoChangeFlag;
				kTabInfoData.bySeat = pkHero->GetPlayer();
				kTabInfoData.byKill = gMain.GetPlayerKillCount(iHeroIndex);
				kTabInfoData.byHelp = gMain.GetHelpCount(iHeroIndex);
				kTabInfoData.byDead = gMain.GetPlayerBeKilledCount(iHeroIndex);
				kTabInfoData.byLevel = pkHero->GetLevel();
				kTabInfoData.sUnitID = pkHero->GetID();
				kTabInfoData.uiReliveTime = pkHero->GetUnitReliveTime();
				kTabInfoData.usKillSolider = gMain.GetKillSoildiers(iHeroIndex);
				kTabInfoData.uiFeatsValue = gMain.GetPlayerFeatsValue(iHeroIndex);
				kTabInfoData.uiMoraleValue = gMain.GetPlayerMoraleValue(iHeroIndex);
				kTabInfoData.uiTeamScore = gMain.GetCampTeamScore(pkHero->GetCamp());
				for (int iItemIndex = 0; iItemIndex < 6; iItemIndex++)
				{
					if (gMain.IsPveMap())
					{
						kTabInfoData.uiItem[iItemIndex] = pkHero->GetZItemUniqueIDByPos(iItemIndex);
					}
					else
					{
						kTabInfoData.uiItem[iItemIndex] = pkHero->GetItemTypeIDByPos(iItemIndex);
					}
				}

				PushRecordPlayer(kTabInfoData, SePureCmd_TabInfo, iSeat);
			}
		}
	}
	if (gMain.IsSecretArea()) {
		BeSecretAreaInfo kSecretArea;
		kSecretArea.bySecretAreaFlag = BSAF_END_TIME | BSAF_FAIL | BSAF_LAYER;
		kSecretArea.uiEndTime = gData.GetSecretAreaEndTime();
		kSecretArea.uiLayer = gData.GetSecretAreaLayer();
		kSecretArea.byEnd = gData.GetSecretAreaFail();
		PushRecordPlayer(kSecretArea, SePureCmd_SecretArea, iSeat);
	}
}


BeShareLockUnitData& BeMain::GetLockUnitData(int iPlayer)
{
	BeUnit* pkLockUnit = gUnitMgr.GetUnitByID(m_aiLockUnit[iPlayer]);
	if (!pkLockUnit)
	{
		m_aiLockUnit[iPlayer] = -1;
		m_aiLastLockUnit[iPlayer] = -1;

		return m_kLockUnitData[iPlayer];
	}

	if (m_aiLockUnit[iPlayer] != m_aiLastLockUnit[iPlayer])
	{
		m_kLockUnitData[iPlayer].iChangeFlag = 0xFFFFFFFF;
		m_kLockUnitData[iPlayer].fArmor = pkLockUnit->GetArmor();
		m_kLockUnitData[iPlayer].fAttack = pkLockUnit->GetDamageNum();
		m_kLockUnitData[iPlayer].fAttackSpeed = pkLockUnit->GetAttackCD();
		m_kLockUnitData[iPlayer].fCdDec = pkLockUnit->GetDecCDTime() * 100;
		m_kLockUnitData[iPlayer].fCritical = pkLockUnit->GetBaoJiRate() * 100;
		m_kLockUnitData[iPlayer].fMagic = pkLockUnit->GetMagicDamage();
		m_kLockUnitData[iPlayer].fMagicArmor = pkLockUnit->GetMagicArmor();

		m_aiLastLockUnit[iPlayer] = m_aiLockUnit[iPlayer];
		m_kLastLockUnitData[iPlayer] = m_kLockUnitData[iPlayer];
	}
	else
	{
		m_kLockUnitData[iPlayer].fArmor = pkLockUnit->GetArmor();
		m_kLockUnitData[iPlayer].fAttack = pkLockUnit->GetDamageNum();
		m_kLockUnitData[iPlayer].fAttackSpeed = pkLockUnit->GetAttackCD();
		m_kLockUnitData[iPlayer].fCdDec = pkLockUnit->GetDecCDTime() * 100;
		m_kLockUnitData[iPlayer].fCritical = pkLockUnit->GetBaoJiRate() * 100;
		m_kLockUnitData[iPlayer].fMagic = pkLockUnit->GetMagicDamage();
		m_kLockUnitData[iPlayer].fMagicArmor = pkLockUnit->GetMagicArmor();

		CompareValue(m_kLockUnitData[iPlayer], m_kLockUnitData[iPlayer].fArmor, m_kLastLockUnitData[iPlayer].fArmor, BLUF_ARMOR);
		CompareValue(m_kLockUnitData[iPlayer], m_kLockUnitData[iPlayer].fAttack, m_kLastLockUnitData[iPlayer].fAttack, BLUF_ATTACK);
		CompareValue(m_kLockUnitData[iPlayer], m_kLockUnitData[iPlayer].fAttackSpeed, m_kLastLockUnitData[iPlayer].fAttackSpeed, BLUF_ATTACKSPEED);
		CompareValue(m_kLockUnitData[iPlayer], m_kLockUnitData[iPlayer].fCdDec, m_kLastLockUnitData[iPlayer].fCdDec, BLUF_CD_DEC);
		CompareValue(m_kLockUnitData[iPlayer], m_kLockUnitData[iPlayer].fCritical, m_kLastLockUnitData[iPlayer].fCritical, BLUF_CRITICAL);
		CompareValue(m_kLockUnitData[iPlayer], m_kLockUnitData[iPlayer].fMagic, m_kLastLockUnitData[iPlayer].fMagic, BLUF_MAGIC);
		CompareValue(m_kLockUnitData[iPlayer], m_kLockUnitData[iPlayer].fMagicArmor, m_kLastLockUnitData[iPlayer].fMagicArmor, BLUF_MAGIC_ARMOR);

		if (m_kLockUnitData[iPlayer].iChangeFlag > 0)
		{
			m_kLastLockUnitData[iPlayer] = m_kLockUnitData[iPlayer];
		}
	}

	return m_kLockUnitData[iPlayer];
}

void BeMain::PushPureDataCmdRecord(SeRoomPlayerCamp eCamp, const void* pkData, int iSize)
{
	for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
	{
		if (GetPlayerCamp(i) == eCamp && GetPlayerType(i) == BPT_PLAYER)
		{
			m_akPureDataCmdRecord[i].SetHostSvr(false);
			m_akPureDataCmdRecord[i].PushRaceData(pkData, iSize);
		}
	}
	return;
}

int		BeMain::GetUnitRaceOut(int iSeatIndex, int iOffset, char* pcData, int iSize)
{
	return m_akPureDataCmdRecord[iSeatIndex].GetRaceData(iOffset, pcData, iSize);
}

int		BeMain::GetViewRaceOut(int iOffset, char* pcData, int iSize)
{
	return m_kViewData.GetRaceData(iOffset, pcData, iSize);
}

inline SeRoomPlayerCamp BeMain::GetCamp(int i)
{
	switch (i)
	{
	case 0:
		return SRPC_CAMPA;
	case 1:
		return SRPC_CAMPB;
	case 2:
		return SRPC_CAMPC;
	case 3:
		return SRPC_CAMPD;
	default:
		return SRPC_INVALID;
	}
}
//

void BeMain::ClrAllPureData()
{
	for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
	{
		m_aiHeroDataChangeFlag[i] = 0;
	}
	gUnitMgr.ClrPureData();
	gEffectMgr.ClrAllPureEffectData();

	const Es::HashMap<int, BeUnit*>& kAllUnit1 = gUnitMgr.GetID2Unit();
	const Es::HashMap<int, BeUnit*>& kAllUnit2 = gUnitMgr.GetID2SuspendUnit();
	Es::HashMap<int, BeUnit*>::ConstIterator itr = kAllUnit1.Begin();
	for (; itr != kAllUnit1.End();)
	{
		BeUnit* pkUnit = itr->Value();
		Es::HashMap<int, BeUnit*>::ConstIterator itr1 = itr;
		++itr;

		if (pkUnit)
		{
			pkUnit->ClrAllPureData();
		}
	}
	Es::HashMap<int, BeUnit*>::ConstIterator itrDel = kAllUnit2.Begin();
	for (; itrDel != kAllUnit2.End();)
	{
		BeUnit* pkUnit = itrDel->Value();
		Es::HashMap<int, BeUnit*>::ConstIterator itr1 = itrDel;
		++itrDel;

		if (pkUnit)
		{
			pkUnit->ClrAllPureData();
		}
	}

	ClearDamageData();
	ClearKillHeroData();
	ClearBufferShowData();
	ClearMapItem();
	ClearAttachData();
	ClearWindowData();
	ClearChainData();

	for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
	{
		m_bSendPing[i] = false;
	}
}

void BeMain::ClearRaceRecord()
{
	for (int i = 0; i < SERVERMODE_MAXPLAYER; i++)
	{
		m_akPureDataCmdRecord[i].ResetSM();
	}
	m_kViewData.ResetSM();
}

const BeShareUnitData& BeMain::GetUnitShowData(BeUnit* pkUnit, bool bNew)
{
	if (!pkUnit)
	{
		return m_kShareUnitData;
	}
	if (pkUnit->HasTableProperty(UNIT_PROPERTY_HIDEEVER))
	{
		m_kShareUnitData.iChangeFlag = 0;
		return m_kShareUnitData;
	}
	m_kShareUnitData.iChangeFlag = pkUnit->GetShareUnitChangeFlag();
	if (bNew)
	{
		m_kShareUnitData.iChangeFlag = BSUDCF_NEW_UNIT;
	}

	m_kShareUnitData.iUnitLogicID = pkUnit->GetID();
	m_kShareUnitData.iUnitTypeID = pkUnit->GetCurrentTypeID();
	m_kShareUnitData.fPosX = pkUnit->GetPosX();
	m_kShareUnitData.fPosY = pkUnit->GetPosY();
	m_kShareUnitData.fPosZ = pkUnit->GetPosZ();
	m_kShareUnitData.eActionName = pkUnit->GetActionName();
	m_kShareUnitData.iActionStartTime = pkUnit->GetActionSartTime();
	m_kShareUnitData.fFace = pkUnit->GetFace();
	m_kShareUnitData.fCurHp = round(pkUnit->GetHP());
	m_kShareUnitData.fMaxHp = pkUnit->GetMaxHP();
	m_kShareUnitData.fRegenHP = pkUnit->GetRegenHP() * 100;
	m_kShareUnitData.iUpdateTime = GetGameTime();
	m_kShareUnitData.iAttackCD = pkUnit->GetAttackCD();
	m_kShareUnitData.iUnitLevel = pkUnit->GetLevel();
	m_kShareUnitData.fPreAttack = (float)pkUnit->GetAttackDamagePt() / (float)pkUnit->GetRealDamagePt();
	m_kShareUnitData.fCurMp = pkUnit->GetMP();
	m_kShareUnitData.fMaxMp = pkUnit->GetMaxMP();
	m_kShareUnitData.fRegenMp = pkUnit->GetRegenMP() * 100;
	m_kShareUnitData.iCamp = pkUnit->GetCamp();
	m_kShareUnitData.fScale = pkUnit->GetScale();

	m_kShareUnitData.fMoveSpeed = pkUnit->GetMoveSpeed();
	m_kShareUnitData.iGuankaFlag = pkUnit->GetUnitGuankaFlag();
	m_kShareUnitData.iFlag = pkUnit->GetFlag();
	m_kShareUnitData.iOtherFlag = pkUnit->GetOtherFlag();
	m_kShareUnitData.iUnitCarryFlag = pkUnit->GetUnitCarryFlag();
	m_kShareUnitData.iExp = pkUnit->GetCurExp();
	m_kShareUnitData.fShild = pkUnit->GetTotalShield();
	m_kShareUnitData.iGrassIndex = pkUnit->GetGrassIndex();
	m_kShareUnitData.byPlayer = pkUnit->GetPlayer();

	if (pkUnit->HasShareUnitChangeFlag(BSUDCF_PATHFINDER) || bNew)
	{
		if (bNew)
		{
			m_kShareUnitData.iChangeFlag &= ~BSUDCF_PATHFINDERSUCESS;
		}
		if (pkUnit->GetActionState() == 1)
		{
			if (!bNew)
			{
				m_kShareUnitData.iChangeFlag &= ~(BSUDCF_POSX);
			}
			m_kShareUnitData.iChangeFlag &= ~BSUDCF_STOPPATHFIND;
			m_kShareUnitData.fTargetX = pkUnit->GetTarPosX();
			m_kShareUnitData.fTargetY = pkUnit->GetTarPosY();
		}
		else
		{
			m_kShareUnitData.iChangeFlag |= BSUDCF_STOPPATHFIND;
			if (pkUnit->HasShareUnitChangeFlag(BSUDCF_PATHFINDERSUCESS) && !bNew)
			{
				m_kShareUnitData.iPathSucessTime = pkUnit->m_iPathFindSucessTime;
			}
			m_kShareUnitData.fTargetX = pkUnit->GetPosX();
			m_kShareUnitData.fTargetY = pkUnit->GetPosY();
		}
	}

	return m_kShareUnitData;
}

const BeShareEffectData& BeMain::GetEffectShowData(BeEffect* pkEffect, bool bRecon)
{
	if (!pkEffect)
	{
		return m_kShareEffectData;
	}

	m_kShareEffectData.iChangeFlag = pkEffect->GetChangeFlag();

	bool bNew = (pkEffect->GetCreateTime() + GAME_FRAME_TIME == gTime);
	if (bNew)
	{
		m_kShareEffectData.iChangeFlag = pkEffect->GetBackChangeFlag();
	}

	if (pkEffect->GetModelFile() <= 0)
	{
		m_kShareEffectData.iChangeFlag = 0;
	}
	if (bRecon)
	{
		m_kShareEffectData.iChangeFlag = uiAllFilg;
	}
	if (m_kShareEffectData.iChangeFlag == 0)
	{
		return m_kShareEffectData;
	}

	m_kShareEffectData.iEffectLogicID = pkEffect->GetID();
	m_kShareEffectData.fPosX = pkEffect->GetPosX();
	m_kShareEffectData.fPosY = pkEffect->GetPosY();
	m_kShareEffectData.fPosZ = pkEffect->GetPosZ();
	m_kShareEffectData.iModleName = pkEffect->GetModelFile();
	m_kShareEffectData.fFace = pkEffect->GetEffectFace();
	m_kShareEffectData.iOwnUnitID = pkEffect->GetOwnerID();
	m_kShareEffectData.iTarUnitID = pkEffect->GetTargetID();
	m_kShareEffectData.fMoveSpeed = pkEffect->GetMoveSpeed();
	//	m_kShareEffectData.fScale = pkEffect->GetScale();
	m_kShareEffectData.fMissileArc = pkEffect->GetMissileArc();
	//	m_kShareEffectData.eAttachNode = pkEffect->GetAttachNode();
	m_kShareEffectData.fTarX = pkEffect->GetTarPosX();
	m_kShareEffectData.fTarY = pkEffect->GetTarPosY();
	m_kShareEffectData.fCycleAngel = pkEffect->GetCycleAngle();
	m_kShareEffectData.iProperty = pkEffect->GetProperty();
	m_kShareEffectData.fVisionRadius = pkEffect->GetVisionRadius();
	m_kShareEffectData.eRace = (BeEffectRace)pkEffect->GetRace();
	m_kShareEffectData.iSkillTypeID = pkEffect->GetAttachSkill();
	return m_kShareEffectData;
}

const BeShareSelfData& BeMain::GetSelfShowData(int iSeat, bool bRecon)
{
	BeUnit* pkSelf = gUnitMgr.GetUnitByID(gMain.GetHeroID(iSeat), true);
	if (!pkSelf)
	{
		const static BeShareSelfData s_kSelfData;
		return s_kSelfData;
	}
	BeShareSelfData& rkCurSelfData = pkSelf->GetCurShareSelfData();
	rkCurSelfData.iChangeFlag = 0;

	if (bRecon)
	{
		rkCurSelfData.iChangeFlag = uiAllFilg;
	}
	rkCurSelfData.iUnitID = pkSelf->GetID();
	rkCurSelfData.iSelfMoney = gMain.GetPlayerGold(iSeat);
	rkCurSelfData.iSeat = pkSelf->GetPlayer();

	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		BeSkill* pkSkill = pkSelf->GetSkillByPos(i);
		if (pkSkill)
		{
			int iEnterCDTime = pkSelf->GetCommonCDLastUseTime(pkSkill->GetTypeID());
			if (!pkSkill->HasProperty(SKILL_SKILLPROPERTY_NOTSHOWCD))
			{
				rkCurSelfData.iSkillEnterCdTime[i] = iEnterCDTime;
			}

			rkCurSelfData.iSkillAllCdTime[i] = pkSkill->GetCDTime(pkSelf);
			rkCurSelfData.iSkillTypeID[i] = pkSkill->GetTypeID();
			rkCurSelfData.bySkillLevel[i] = pkSkill->GetLevel();
			rkCurSelfData.bySkillCounter[i] = pkSkill->GetUICounter();
		}
		else
		{
			rkCurSelfData.iSkillTypeID[i] = pkSelf->GetAbilSkillID(i);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (gMain.IsPveMap())
		{
			BeItem* pkItem = pkSelf->GetZItemByPos(i);
			if (pkItem)
			{
				rkCurSelfData.iItemInfo[i] = pkItem->GetUniqueID();
				rkCurSelfData.iItemEnterCDTime[i] = pkItem->GetLastUseTime();
				rkCurSelfData.iItemAllCDTime[i] = pkItem->GetItemAllCDTime();
				rkCurSelfData.byItemPipleNum[i] = pkItem->GetPileCount();
			}
			else
			{
				rkCurSelfData.iItemInfo[i] = 0;
			}
		}
		else
		{
			BeItem* pkItem = pkSelf->GetItemByPos(i);
			if (pkItem)
			{
				rkCurSelfData.iItemInfo[i] = pkItem->GetTypeID();
				rkCurSelfData.iItemEnterCDTime[i] = pkItem->GetLastUseTime();
				rkCurSelfData.iItemAllCDTime[i] = pkItem->GetItemAllCDTime();
				rkCurSelfData.byItemPipleNum[i] = pkItem->GetPileCount();
			}
			else
			{
				rkCurSelfData.iItemInfo[i] = 0;
			}
		}
	}

	const BeShareSelfData& rkLastSelfData = pkSelf->GetLastShareSelfData();

	CompareValue(rkCurSelfData, rkCurSelfData.iUnitID, rkLastSelfData.iUnitID, BSDCF_SELFID);

	if (rkCurSelfData.iChangeFlag & BSDCF_SELFID)
	{
		rkCurSelfData.iChangeFlag = uiAllFilg;
	}

	CompareValue(rkCurSelfData, rkCurSelfData.iSelfMoney, rkLastSelfData.iSelfMoney, BSDCF_MONEY);

	rkCurSelfData.ClearVec();
	for (int i = 0; i < iMaxHeroSkillNum; i++)
	{
		if (rkCurSelfData.iSkillEnterCdTime[i] != rkLastSelfData.iSkillEnterCdTime[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ENTERCDTIME;
			rkCurSelfData.bEnterCdTimePos[i] = true;
		}
		if (rkCurSelfData.iSkillAllCdTime[i] != rkLastSelfData.iSkillAllCdTime[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ALLCDTIME;
			rkCurSelfData.bAllCdTimePos[i] = true;
		}
		if (rkCurSelfData.iSkillTypeID[i] != rkLastSelfData.iSkillTypeID[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_SKILLTYPEID;
			rkCurSelfData.bSkillTypeIDPos[i] = true;
		}
		if (rkCurSelfData.bySkillLevel[i] != rkLastSelfData.bySkillLevel[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_SKILLLEVEL;
			rkCurSelfData.bSkillLevelPos[i] = true;
		}
		if (rkCurSelfData.bySkillCounter[i] != rkLastSelfData.bySkillCounter[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_SKILLCOUNTER;
			rkCurSelfData.bSkillCounterPos[i] = true;
		}
		if (pkSelf->IsChangeSkill() || bRecon)
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ENTERCDTIME;
			rkCurSelfData.bEnterCdTimePos[i] = true;
			rkCurSelfData.iChangeFlag |= BSDCF_ALLCDTIME;
			rkCurSelfData.bAllCdTimePos[i] = true;
			rkCurSelfData.iChangeFlag |= BSDCF_SKILLTYPEID;
			rkCurSelfData.bSkillTypeIDPos[i] = true;
			rkCurSelfData.iChangeFlag |= BSDCF_SKILLLEVEL;
			rkCurSelfData.bSkillLevelPos[i] = true;
			rkCurSelfData.iChangeFlag |= BSDCF_SKILLCOUNTER;
			rkCurSelfData.bSkillCounterPos[i] = true;
		}
	}
	pkSelf->ClearnChangeSkill();
	for (int i = 0; i < iMaxHeroItem; i++)
	{
		if (rkCurSelfData.iItemInfo[i] != rkLastSelfData.iItemInfo[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ITEMINFO;
			rkCurSelfData.bItemInfoPos[i] = true;

			rkCurSelfData.iChangeFlag |= BSDCF_ITEMPIPLENUM;
			rkCurSelfData.bItemPiplePos[i] = true;
		}
		if (rkCurSelfData.iItemEnterCDTime[i] != rkLastSelfData.iItemEnterCDTime[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ITEMENTERCDTIME;
			rkCurSelfData.bItemEnterCDPos[i] = true;
		}
		if (rkCurSelfData.iItemAllCDTime[i] != rkLastSelfData.iItemAllCDTime[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ITEMALLCDTIME;
			rkCurSelfData.bItemAllCDTimePos[i] = true;
		}
		if (rkCurSelfData.byItemPipleNum[i] != rkLastSelfData.byItemPipleNum[i])
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ITEMPIPLENUM;
			rkCurSelfData.bItemPiplePos[i] = true;
		}
		if (bRecon)
		{
			rkCurSelfData.iChangeFlag |= BSDCF_ITEMINFO;
			rkCurSelfData.bItemInfoPos[i] = true;
		}
	}

	rkCurSelfData.sAttackNum = pkSelf->GetDamageNum();
	rkCurSelfData.sMagicNum = pkSelf->GetMagicDamage();
	rkCurSelfData.sArmor = pkSelf->GetArmor();
	rkCurSelfData.sMagicArmor = pkSelf->GetMagicArmor();
	rkCurSelfData.by100DecCD = pkSelf->GetDecCDTime() * 100;
	rkCurSelfData.s100BaoJi = pkSelf->GetBaoJiRate() * 100;

	CompareValue(rkCurSelfData, rkCurSelfData.sAttackNum, rkLastSelfData.sAttackNum, BSDCF_ATTACKNUM);
	CompareValue(rkCurSelfData, rkCurSelfData.sMagicNum, rkLastSelfData.sMagicNum, BSDCF_MAGICNUM);
	CompareValue(rkCurSelfData, rkCurSelfData.sArmor, rkLastSelfData.sArmor, BSDCF_ARMOR);
	CompareValue(rkCurSelfData, rkCurSelfData.sMagicArmor, rkLastSelfData.sMagicArmor, BSDCF_MAGICARMOR);
	CompareValue(rkCurSelfData, rkCurSelfData.by100DecCD, rkLastSelfData.by100DecCD, BSDCF_DECCD);
	CompareValue(rkCurSelfData, rkCurSelfData.s100BaoJi, rkLastSelfData.s100BaoJi, BSDCF_BAOJI);

	rkCurSelfData.sPassArmor = pkSelf->GetDecArmor();
	rkCurSelfData.sPassMagicArmor = pkSelf->GetDecMagicArmor();
	rkCurSelfData.sLeech = pkSelf->GetLeech() * 100;
	rkCurSelfData.sMagicLeech = pkSelf->GetMagicLeech() * 100;
	rkCurSelfData.sAttackRange = pkSelf->GetAttackRange(pkSelf);
	rkCurSelfData.sToughness = round(pkSelf->GetToughness() * 100);
	rkCurSelfData.by100PerPassArmor = pkSelf->GetPerDecArmor() * 100;
	rkCurSelfData.by100PerPassMagicArmor = pkSelf->GetPerDecMagicArmor() * 100;

	CompareValue(rkCurSelfData, rkCurSelfData.sPassArmor, rkLastSelfData.sPassArmor, BSDCF_PassArmor);
	CompareValue(rkCurSelfData, rkCurSelfData.sPassMagicArmor, rkLastSelfData.sPassMagicArmor, BSDCF_PassMagicArmor);
	CompareValue(rkCurSelfData, rkCurSelfData.sLeech, rkLastSelfData.sLeech, BSDCF_Leech);
	CompareValue(rkCurSelfData, rkCurSelfData.sMagicLeech, rkLastSelfData.sMagicLeech, BSDCF_MagicLeech);
	CompareValue(rkCurSelfData, rkCurSelfData.sAttackRange, rkLastSelfData.sAttackRange, BSDCF_AttackRange);
	CompareValue(rkCurSelfData, rkCurSelfData.sToughness, rkLastSelfData.sToughness, BSDCF_Toughness);
	CompareValue(rkCurSelfData, rkCurSelfData.by100PerPassArmor, rkLastSelfData.by100PerPassArmor, BSDCF_PerPassArmor);
	CompareValue(rkCurSelfData, rkCurSelfData.by100PerPassMagicArmor, rkLastSelfData.by100PerPassMagicArmor, BSDCF_PerPassMagicArmor);

	if (rkCurSelfData.iChangeFlag != 0)
	{
		rkCurSelfData.iChangeFlag |= BSDCF_CAMP;
	}

	return rkCurSelfData;
}

void BeMain::GenVisionUnit(int iHeroSeat, UnitGroupID& rkGroupID)
{
	rkGroupID.Clear();
	if (!IsPlayerActive(iHeroSeat))
	{
		return;
	}
	BeUnit* pkHero = GetHeroBySeat(iHeroSeat);
	if (!pkHero)
	{
		return;
	}

	int aiHeroPlayerSeat[MAX_ACTIVEPLAYERS] = { 0 };
	UnitGroup kGroup;

	gUnitMgr.GetAreaGroup(kGroup, pkHero->GetPosX(), pkHero->GetPosY(), 1200.f, pkHero->GetPlayer(), 0xFFFFFFFF, true);
	for (int j = 0; j < (int)kGroup.Size(); j++)
	{
		BeUnit* pkUnit = kGroup[j];
		if (!pkUnit)
		{
			continue;
		}

		if (pkUnit->GetPlayer() >= 0 && pkUnit->GetPlayer() < MAX_ACTIVEPLAYERS)
		{
			aiHeroPlayerSeat[pkUnit->GetPlayer()] = 1;
		}
		rkGroupID.PushBack(pkUnit->GetID());
		pkUnit->SetIsInPlayerVision(iHeroSeat);
	}
	for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
	{
		if (aiHeroPlayerSeat[i] == 0 && IsPlayerActive(i))
		{
			rkGroupID.PushBack(GetHeroID(i));
		}
	}
}

const std::vector<BeShareEffectData>& BeMain::GetShareEffectData(bool bRecon)
{
	m_akEffectData.clear();

	Es::HashMap<int, BeEffect*>& kAllEffect = gEffectMgr.GetID2Effect();
	Es::HashMap<int, BeEffect*>::ConstIterator itr = kAllEffect.Begin();
	for (; itr != kAllEffect.End();)
	{
		BeEffect* pkEffect = itr->Value();
		Es::HashMap<int, BeEffect*>::ConstIterator itr1 = itr;
		++itr;

		if (pkEffect)
		{
			const BeShareEffectData& rkEffectData = GetEffectShowData(pkEffect, bRecon);
			if (rkEffectData.iChangeFlag != 0)
			{
				m_akEffectData.push_back(rkEffectData);
			}
		}
		pkEffect->ClrChangeFlag(uiAllFilg);
	}

	return m_akEffectData;
}
