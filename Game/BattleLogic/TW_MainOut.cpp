/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <unordered_map>
#include "TW_Main.h"
#include "TW_ShareUnitData.h"
#include "TW_UnitMgr.h"
#include "TW_Unit.h"
#include "TW_EffectMgr.h"
#include "TW_Effect.h"

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
	std::vector<int>& rkDelEffectID = gEffectMgr.PureGetDelEffect();

	const	std::unordered_map<int, BeUnit*>& kAllLiveUnit = gUnitMgr.GetID2Unit();
	const	std::unordered_map<int, BeUnit*>& kAllSuspendUnit = gUnitMgr.GetID2SuspendUnit();
	std::vector<BeShareUnitData>	akShareData;

	for (int iIndex = 0; iIndex < iPureCampNum; iIndex++)
	{
		akShareData.clear();
		auto	Iter = kAllLiveUnit.begin();
		auto	IterEnd = kAllLiveUnit.end();
		while (Iter != IterEnd)
		{
			BeUnit* pkTempUnit = Iter->second;
			if (pkTempUnit)
			{
				if (pkTempUnit->GetUnitVisionForCamp(iIndex) || pkTempUnit->HasFlag(BUF_DEAD))
				{
				}
			}
			Iter++;
		}

		auto	IterSuspend = kAllSuspendUnit.begin();
		auto	IterSuspendEnd = kAllSuspendUnit.end();
		while (IterSuspend != IterSuspendEnd)
		{
			BeUnit* pkTempUnit = IterSuspend->second;
			if (pkTempUnit)
			{
				if (pkTempUnit->GetUnitVisionForCamp(iIndex))
				{
					//const BeShareUnitData& rkData = GetUnitShowData(pkTempUnit, pkTempUnit->IsNewUnit(iIndex));
					//if (rkData.iChangeFlag != 0)
					//{
					//	akShareData.push_back(rkData);
					//}
				}
			}
			IterSuspend++;
		}

		//PushRecordV(akShareData, SePureCmd_UnitShowData);
	}
	{
		akShareData.clear();
		auto	Iter = kAllLiveUnit.begin();
		auto	IterEnd = kAllLiveUnit.end();
		while (Iter != IterEnd)
		{
			BeUnit* pkTempUnit = Iter->second;
			if (pkTempUnit)
			{
				if (!pkTempUnit->HasProperty(UNIT_PROPERTY_HIDEEVER))
				{
					//const BeShareUnitData& rkData = GetUnitShowData(pkTempUnit);
					//if (rkData.iChangeFlag != 0)
					//{
					//	akShareData.push_back(rkData);
					//}
				}
			}
			Iter++;
		}
		//PushViewRecordV(akShareData, SePureCmd_UnitShowData);
	}

	ClrAllPureData();
}

int		BeMain::GetUnitRaceOut(int iSeatIndex, int iOffset, char* pcData, int iSize)
{
	//return m_akPureDataCmdRecord[iSeatIndex].GetRaceData(iOffset, pcData, iSize);
	return 0;
}

void BeMain::ClrAllPureData()
{
	gUnitMgr.ClrPureData();
	gEffectMgr.ClrAllPureEffectData();

	const std::unordered_map<int, BeUnit*>& kAllUnit1 = gUnitMgr.GetID2Unit();
	const std::unordered_map<int, BeUnit*>& kAllUnit2 = gUnitMgr.GetID2SuspendUnit();
	std::unordered_map<int, BeUnit*>::const_iterator itr = kAllUnit1.begin();
	for (; itr != kAllUnit1.end();)
	{
		BeUnit* pkUnit = itr->second;
		std::unordered_map<int, BeUnit*>::const_iterator itr1 = itr;
		++itr;

		if (pkUnit)
		{
			pkUnit->ClrAllPureData();
		}
	}
	std::unordered_map<int, BeUnit*>::const_iterator itrDel = kAllUnit2.begin();
	for (; itrDel != kAllUnit2.end();)
	{
		BeUnit* pkUnit = itrDel->second;
		std::unordered_map<int, BeUnit*>::const_iterator itr1 = itrDel;
		++itrDel;

		if (pkUnit)
		{
			pkUnit->ClrAllPureData();
		}
	}
}
