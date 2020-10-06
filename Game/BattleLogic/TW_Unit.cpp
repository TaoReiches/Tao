/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_Unit.h"

template<class T>
void BeUnit::TrgOnPreDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr, int iItemPos)
{
	for (int i = 0; i < iCount; ++i)
	{
		BeSkill* pkSkill = kSkill[i];
		if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_SHANGHAICHUFA))
		{
			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, this);
			kParam.SetParam(BTP_pkTarget, pkTarget);
			kParam.SetParam(BTP_pkAttacker, this);
			kParam.SetParam(BTP_pkSkill, pkSkill);
			kParam.SetParam(BTP_iItemPos, iItemPos + 1);
			kParam.SetParam(BTP_pfDamage, &fDamage);
			kParam.SetParam(BTP_iFlag, iFlag);
			kParam.SetParam(BTP_iAttackType, eAttackType);
			kParam.SetParam(BTP_pkAttackAttr, &kAttr);

			gTrgMgr.FireTrigger(BTE_SKILL_PREONDAMAGE, kParam);
		}
	}
}

template<class T>
void BeUnit::TrgOnBeDamaged_T(T& kSkill, int iCount, int eAttackType, float& fDamage, float fRawDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos)
{
	for (int i = 0; i < iCount; ++i)
	{
		BeSkill* pkSkill = kSkill[i];
		if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_BEIJICHUFA))
		{
			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, this);
			kParam.SetParam(BTP_pkAttacker, pkAttacker);
			kParam.SetParam(BTP_pkTarget, this);
			kParam.SetParam(BTP_pkSkill, pkSkill);
			kParam.SetParam(BTP_iItemPos, iItemPos + 1);
			kParam.SetParam(BTP_pfDamage, &fDamage);
			kParam.SetParam(BTP_fRawDamage, fRawDamage);
			kParam.SetParam(BTP_iFlag, iFlag);
			kParam.SetParam(BTP_iAttackType, eAttackType);
			kParam.SetParam(BTP_iSkillTypeID, iAttackSkillTypeID);

			gTrgMgr.FireTrigger(BTE_SKILL_ONBEDAMAGE, kParam);
		}
	}
}

template<class T>
void BeUnit::TrgOnDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos, int iItemID)
{
	for (int i = 0; i < iCount; ++i)
	{
		BeSkill* pkSkill = kSkill[i];
		if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_SHANGHAICHUFA))
		{
			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, this);
			kParam.SetParam(BTP_pkTarget, pkTarget);
			kParam.SetParam(BTP_pkAttacker, this);
			kParam.SetParam(BTP_pkSkill, pkSkill);
			kParam.SetParam(BTP_iItemPos, iItemPos + 1);
			kParam.SetParam(BTP_iItemID, iItemID);
			kParam.SetParam(BTP_pfDamage, &fDamage);
			kParam.SetParam(BTP_iFlag, iFlag);
			kParam.SetParam(BTP_iAttackType, eAttackType);
			kParam.SetParam(BTP_iSkillTypeID, iAttackSkillTypeID);
			kParam.SetParam(BTP_iSkillOrgTypeID, iAttackSkillTypeID);

			gTrgMgr.FireTrigger(BTE_SKILL_ONDAMAGE, kParam);
		}
	}
}

template<class T>
void BeUnit::TrgOnAttack_T(T& kSkill, int iCount, BeUnit* pkTarget, BeAttackingAttr* pkAttackAttr, int iItemPos)
{
	for (int i = 0; i < iCount; ++i)
	{
		BeSkill* pkSkill = kSkill[i];
		if (pkSkill && (pkSkill->HasProperty(SKILL_SKILLPROPERTY_GONGJICHUFA)))
		{
			if ((IsDividMan() || IsGhost()))
			{
				continue;
			}

			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, this);
			kParam.SetParam(BTP_pkTarget, pkTarget);
			kParam.SetParam(BTP_pkSkill, pkSkill);
			kParam.SetParam(BTP_pkAttackAttr, pkAttackAttr);
			kParam.SetParam(BTP_iItemPos, iItemPos + 1);
			if (pkSkill->HasProperty(SKILL_SKILLPROPERTY_GONGJICHUFA))
			{
				kParam.SetParam(BTP_iTrgEventBy, BTE_SKILL_ONATTACK);
				gTrgMgr.FireTrigger(BTE_SKILL_ONATTACK, kParam);
			}
		}
	}
}

BeUnit::BeUnit(int iID) : BeSingleLinkEntity(iID)
{
	m_iDeadCount = 0;


	m_iOtherFlag = 0;
	m_iCarryFlag = 0;
	m_iImmunityFlag = 0;
	m_iNotInvisByGroup = 0;
	m_iGuankaFlag = 0;

	m_fPreMaxHP = 0.0f;
	m_fPreMaxMP = 0.0f;
	m_pkAI = NULL;

	m_eActionName = BAN_stand;
	m_iActionStartTime = 0;

	m_eCamp = SRPC_INVALID;
	m_bChangeSkill = false;
	m_iMoveToUnitID = 0;
	m_fMoveTarPosX = 0.0f;
	m_fMoveTarPosY = 0.0f;

	m_iMoveAllTime = 0;
	m_fMinMoveDistance = 0.0f;

	m_iGiveAttackCmdTime = 0;

	m_iOrgSkillLevel = -1;

	BeUnitData* pkData = BeUnitData::NEW();
	m_akUnitData.push_back(pkData);
	m_pkBackData = pkData;
	m_pkCurData = pkData;

	memset(m_apkItem, 0, sizeof(m_apkItem));

	m_iActionStayTime = 0;

	m_kCommander.AttachUnit(this);
	m_eActorTransType = UATT_NONE_TRANS;
	m_iTransTime = 0;
	m_bNeedTrans = false;
	m_iAddTransTime = 0;

	m_iTabInfoFlagForCampA = 0;
	m_iTabInfoFlagForCampB = 0;


	m_iSummonSkillOrItem = 0;


	m_iOutBattleTime = 0;
	m_bOutOfBattle = false;

	m_iDeathRecordLastTime = 0;
	m_fDeathRecordTotalDamage = 0.0f;


	m_iHPLock = 0;
	m_fSpeedFixup = -1.0f;
	m_bSpeedLimit = true;


	m_bIsUpdate = false;

	m_bPureNeedUpdae = false;


	m_iActionState = 0;
	m_fWalkTargetX = 0.f;
	m_fWalkTargetY = 0.f;
	m_iPathFindSucessTime = 0;


	m_iWalkLineIdx = -1;

	m_iShareUnitDataChangeFlag = 0;


	m_dwTransAlphaDec = 0;
	m_iAlphaTransTime = 0;

	m_dwStartAlpha = 0xFF;

	m_iGrassIndex = 0;
	m_fTotalShield = 0.0f;
	m_iCastVisionTime = 0;

	memset(m_abInPlayerVision, 0, sizeof(m_abInPlayerVision));
	memset(m_abEverInPlayerVision, 0, sizeof(m_abEverInPlayerVision));
	memset(m_bVisionForCamp, 0, sizeof(m_bVisionForCamp));
	memset(m_bVisionForCampLast, 0, sizeof(m_bVisionForCampLast));
	memset(m_bGrassVisionForCamp, 0, sizeof(m_bGrassVisionForCamp));
	memset(m_bGrassVisionForCampLast, 0, sizeof(m_bGrassVisionForCampLast));


	m_bNeedUpdateObstacle = true;
	m_bSetObstacle = false;
	m_spSharePtr.Bind(this);
	m_spSharePtr.SetOutDestroy(true);
	m_iLastAttackTime = 0;
	m_iLastAttackHeroTime = 0;

	m_iTurnTableFlag = 0;
	m_uiConfirmResult = 0;
	memset(m_iTurnTableInfo, 0, sizeof(m_iTurnTableInfo));
	m_uiNowTurnResult = 0;
	m_uiReTurnNum = 0;
	memset(m_uiTurnResult, 0, sizeof(m_uiTurnResult));

	m_uiHunQiSkillID = 0;
	m_usHunStoneNum = 0;
	m_usHunStoneTotalNum = 0;
	memset(m_uiTurnTableAttrInfo, 0, sizeof(m_uiTurnTableAttrInfo));

	m_e8v8PlayerState = BPS_8V8_NULL;
	m_bNewUnit[0] = true;
	m_bNewUnit[1] = true;
	m_pkVisionData = nullptr;
}

void BeUnit::OnDelete(void)
{
	m_kCommander.OnDelete();
	DelAllBuffer();

	gMap.DelUnitObstacle(this, true, true);
	gMap.ReleaseUnitVision(this);

	gUnitMgr.Unlink(this);

	m_apkNormalSkill.clear();

	BeLearnSkillData::DEL(m_pkBackData->akLearnSkill);
	BeUnitData::DEL(m_pkBackData);
	m_pkBackData = NULL;
	m_pkCurData = NULL;

	for (int i = 0; i < (int)m_akConnectUnits.size(); i++)
	{
		if (m_akConnectUnits[i].IsNull())
		{
			continue;
		}

		BeUnit* pkUnit = m_akConnectUnits[i].GetPointer();//gUnitMgr.GetUnitByID(iConnectID,true);
		if (pkUnit)
		{
			pkUnit->DelConnectUnit(this);
			if (pkUnit->HasOtherFlag(BUOF_FELLOW))
			{
				gUnitMgr.DelUnit(pkUnit->GetID());
			}
		}
	}
}

BeUnit::~BeUnit(void)
{
	gTrgMgr.OnUnitDelete(this);

	DelAllBufferFinal();
	if (m_pkAI)
	{
		gAIMgr.SafeDelUnitAI(m_pkAI);
	}
	m_spSharePtr.SetNull();
	gMain.DelEntityPointer(GIT_ENTITY, m_iID);
}

bool BeUnit::Initialize(int iTypeID)
{
	m_iTypeID = iTypeID;
	const UnitTable* pkRes = UnitTableMgr::Get()->GetUnitTable(iTypeID);
	if (!pkRes)
	{
		return false;
	}

	m_akCommonCD.clear();
	m_pkBackData->pkRes = pkRes;
	m_iFlag = 0;
	SetCurrentTypeID(iTypeID);
	m_pkBackData->iTypeID = iTypeID;
	m_pkBackData->iUnitProperty = m_pkBackData->pkRes->uiProperty;
	OnInitAttribute(true, false);
	SetLevel(1, false);

	m_kCommander.AttachMain(pkAttachMain);

	if (pkRes->uiClassType == UNIT_CLASSTYPE_HERO)
	{
		AISetType(BUAT_HERO, false);
	}
	else if (pkRes->uiSecClassType == UNIT_SECCLASSTYPE_GUARD)
	{
		AISetType(BUAT_GUARD, false);
	}
	else if (pkRes->uiClassType == UNIT_CLASSTYPE_BUILDING)
	{
		AISetType(BUAT_TOWER, false);
	}
	else if (pkRes->uiClassType == UNIT_CLASSTYPE_SMALLMONST || pkRes->uiClassType == UNIT_CLASSTYPE_BIGMONST)
	{
		AISetType(BUAT_MONSTER, false);
	}
	else if (pkRes->uiClassType == UNIT_CLASSTYPE_BOSS)
	{
		AISetType(BUAT_MONSTER, false);
	}
	else if (pkRes->uiClassType == UNIT_CLASSTYPE_PET)
	{
		AISetType(BUAT_SUMMON, false);
	}
	else if (pkRes->uiClassType == UNIT_CLASSTYPE_MIJINGMONSTER || pkRes->uiClassType == UNIT_CLASSTYPE_MIJINGBOSS)
	{
		AISetType(BUAT_MIJING, false);
	}
	else
	{
		AISetType(BUAT_SOLDIER, false);
	}

	DelAllBuffer();
	ClearAllCommonCD();
	DelAllSkill();
	DelAllBufferFinal();
	BeLearnSkillData::DEL(m_pkBackData->akLearnSkill);

	BeCommand	kComm(BCT_STOP);
	m_kCommander.GiveCommand(kComm);
	OnPlayerChanged();

	SetTabInfoFlag(BTCF_UNITID);
	SetShareUnitChangeFlag(BSUDCF_NEW_UNIT);

	if (pkRes->uiClassType == UNIT_CLASSTYPE_BOSS || pkRes->uiClassType == UNIT_CLASSTYPE_BIGMONST || pkRes->uiClassType == UNIT_CLASSTYPE_SMALLMONST) {

		m_iCastVisionTime = gTime + 50;
	}

	return true;
}

void BeUnit::AppyUnitSkill(void)
{
	if (GetClass() == UNIT_CLASSTYPE_HERO && GetPlayer() < MAX_ACTIVEPLAYERS)
	{
		(m_pkBackData->akLearnSkill) = BeLearnSkillData::NEW();

		for (int i = 0; i < MAX_LEARN_SKILLS - 1; ++i)
		{
			BeLearnSkillData& rkData = (m_pkBackData->akLearnSkill)[i];
			rkData.iTypeID = GetAbilSkillID(i);

			const SkillTable* pkSkillRes = SkillTableMgr::Get()->GetSkillTable(rkData.iTypeID);
			if (!pkSkillRes)
			{
				continue;
			}

			rkData.iLearnLevel = 1;
			rkData.iPos = i;
			SetSkillPointSum(m_pkBackData->iSkillPointSum + pkSkillRes->iSkillMaxLevel);

		}
	}
	else
	{
		(m_pkBackData->akLearnSkill) = BeLearnSkillData::NEW();

		int	iLastSkillTypeID = 0;
		for (int iSkillIndex = 0; iSkillIndex < iMaxHeroSkillNum; iSkillIndex++)
		{
			int		iSkillTypeID = GetAbilSkillID(iSkillIndex);
			if (iSkillTypeID > 0 && iSkillTypeID != iLastSkillTypeID)
			{
				iLastSkillTypeID = iSkillTypeID;
				AddSkill(iSkillTypeID);
			}
		}
	}
}

void BeUnit::UpdateState(int iDeltaTime)
{
	if (!IsDead() && !m_akShield.empty())
	{
		std::map<int, float>::iterator iter = m_akShield.begin();
		while (iter != m_akShield.end())
		{
			int iBufferID = iter->first;
			float fShield = iter->second;

			BeBuffer* pkBuffer = GetBufferByInnerID(iBufferID);
			if (!pkBuffer || fShield <= 0)
			{
				SetShareUnitChangeFlag(BSUDCF_SHELD);
				m_akShield.erase(iter++);
				m_fTotalShield -= fShield;
			}
			else
			{
				iter++;
			}
		}
	}

	if (!IsDead())
	{
		AIOnUpdate(gTime);
		UpdateHP(iDeltaTime);
		UpdateMP(iDeltaTime);
	}

	if (m_iMoveAllTime > 0)
	{
		m_iMoveAllTime -= iDeltaTime;

		int iMoveAllTime = ((m_iMoveAllTime > 0) ? m_iMoveAllTime : 1);
		if (m_iMoveToUnitID > 0)
		{
			BeUnit* pkMUnit = gUnitMgr.GetUnitByID(m_iMoveToUnitID);
			if (pkMUnit)
			{
				m_fMoveTarPosX = pkMUnit->GetPosX();
				m_fMoveTarPosY = pkMUnit->GetPosY();
			}
			else
			{
				m_iMoveToUnitID = 0;
				m_iMoveAllTime = 0;
			}
		}

		float fMFace = atan2f(m_fMoveTarPosY - GetPosY(), m_fMoveTarPosX - GetPosX());
		float fMDis = GetDistance(GetPosX(), GetPosY(), m_fMoveTarPosX, m_fMoveTarPosY);
		float fMMinDis = (fMDis / iMoveAllTime) * iDeltaTime;

		if (fMMinDis < fMDis && fMDis > 16.0f)
		{
			float fPosX = GetPosX() + fMMinDis * cosf(fMFace);
			float fPosY = GetPosY() + fMMinDis * sinf(fMFace);
			if (m_iMoveToUnitID == 0 && gMap.IsObstacle(fPosX, fPosY, BGF_GUTDOODAD, GetOBSize())) {

				m_iMoveAllTime = 0;
			}
			else {
				SetPosition(fPosX, fPosY, 0.0f, true);
			}
		}
		else
		{
			if (gMap.IsObstacle(GetPosX(), GetPosY(), BGF_GUTDOODAD, GetOBSize()) || gMap.IsObstacle(m_fMoveTarPosX, m_fMoveTarPosY, BGF_GUTDOODAD, GetOBSize())) {

			}
			else {
				SetPosition(m_fMoveTarPosX, m_fMoveTarPosY, 0.0f, true);
			}
			m_iMoveToUnitID = 0;
			m_iMoveAllTime = 0;
		}

		if (m_iMoveAllTime <= 0)
		{
			m_iMoveAllTime = 0;
			DelBuffer('BA19');

			ClrFlag(BUF_IGNOREUNITOBS);
			ClrOtherFlag(BUOF_WEIYI);

			gMap.SetUnitPosition(this, GetPosX(), GetPosY(), 0.0f, 1000.0f, false, BGF_FIXED_OBS | BGF_UNIT, 0, true);
			SetShareUnitChangeFlag(BSUDCF_RESET_POX);
		}
	}
}

void BeUnit::Update(int iDeltaTime)
{
	m_iOneFrameDamage = 0;

	BeEntity::Update(iDeltaTime);

	TrgOnUpdate(iDeltaTime);

	gMap.DelUnitObstacle(this, false, false);

	m_kCommander.ExecuteCmd(iDeltaTime);



	UpdateState(iDeltaTime);

	if (IsDead())
	{
	}
	else
	{
		gMap.AddUnitObstacle(this, false, false);
	}

	float fPerCDTime = GetPerCDTime();
	if (fPerCDTime > 0.0f)
	{
		float fAddTime = (float)iDeltaTime / (1.0f - fPerCDTime);
		int iDecTime = (int)fAddTime - iDeltaTime;
		for (int i = 0; i < iMaxHeroSkillNum; i++)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill)
			{
				if (!pkSkill->CDComplete(this))
				{
					int iLastTime = pkSkill->GetLastUseTime(this);
					pkSkill->SetLastUseTime(this, iLastTime - iDecTime);
				}
			}
		}
	}

	UpdateBattleState(false);

	UpdateBuffer(iDeltaTime);

	UpdateLiveTime(iDeltaTime);

	gMap.UpdateUnitVision(this);

	gUnitMgr.Link(GetPosX(), GetPosY(), this);

	UpdateTransrate(iDeltaTime);
	UpdateSummonUnit();

	int iTempIndex = gMap.GetGrassIndex(GetPosX(), GetPosY());
	if (m_iGrassIndex != iTempIndex)
	{
		SetShareUnitChangeFlag(BSUDCF_GRASS);
		if (m_iGrassIndex > 0)
		{
			gMain.RemoveGrassUnit(GetID(), m_iGrassIndex);
		}
		if (iTempIndex > 0)
		{
			gMain.SetGrassUnit(GetID(), iTempIndex);
		}
	}
	m_iGrassIndex = iTempIndex;

	if (m_iGrassIndex > 0)
	{
		for (int iIndex = 0; iIndex < iPureCampNum; iIndex++)
		{
			int	iDstCamp = iIndex + SRPC_CAMPA;
			if (iDstCamp == GetCamp())
			{
				m_bGrassVisionForCamp[iIndex] = true;
			}
			else
			{
				if (gMain.HasGrassVisionForCamp(m_iGrassIndex, GetCamp(), iDstCamp))
				{
					m_bGrassVisionForCamp[iIndex] = true;
				}
				else
				{
					m_bGrassVisionForCamp[iIndex] = false;
				}
			}
		}
	}
	else
	{
		for (int iIndex = 0; iIndex < iPureCampNum; iIndex++)
		{
			m_bGrassVisionForCamp[iIndex] = true;
		}
	}

	for (int iIndex = 0; iIndex < iPureCampNum; iIndex++)
	{
		if (HasProperty(UNIT_PROPERTY_HIDEEVER))
		{
			m_bVisionForCamp[iIndex] = false;
		}
		else if (GetClass() == UNIT_CLASSTYPE_BUILDING)
		{
			m_bVisionForCamp[iIndex] = true;
			if (!m_bVisionForCampLast[iIndex] && m_bVisionForCamp[iIndex])
			{
				m_bNewUnit[iIndex] = true;
			}
			m_bVisionForCampLast[iIndex] = m_bVisionForCamp[iIndex];
		}
		else
		{
			m_bVisionForCamp[iIndex] = gMap.IsInSight(SRPC_CAMPA + iIndex, this);
			if (gTime < m_iCastVisionTime)
			{
				m_bVisionForCamp[iIndex] = true;
				m_bGrassVisionForCamp[iIndex] = true;
			}
			if (!m_bVisionForCamp[iIndex])
			{
				if (!HasFlag(BUF_HASINVISIBLE))
				{
					if (gMain.HasGrassVisionForCamp(m_iGrassIndex, GetCamp(), SRPC_CAMPA + iIndex))
					{
						m_bVisionForCamp[iIndex] = true;
						m_bGrassVisionForCamp[iIndex] = true;
					}
				}
			}

			if (GetClass() == UNIT_CLASSTYPE_SMALLMONST || GetClass() == UNIT_CLASSTYPE_BIGMONST)
			{
				m_bGrassVisionForCamp[iIndex] = true;
			}
			if (!m_bVisionForCampLast[iIndex] && m_bVisionForCamp[iIndex])
			{
				m_bNewUnit[iIndex] = true;
			}
			else if (m_bVisionForCampLast[iIndex] && !m_bVisionForCamp[iIndex])
			{
				gUnitMgr.PushNeedDelNormalUnit(iIndex, m_iID);
			}
			m_bVisionForCampLast[iIndex] = m_bVisionForCamp[iIndex];

			if (m_bVisionForCamp[iIndex] && GetCamp() != iIndex + 1)
			{
				if (m_bGrassVisionForCampLast[iIndex] && !m_bGrassVisionForCamp[iIndex])
				{
					gUnitMgr.PushNeedDelNormalUnit(iIndex, m_iID);
				}

				if (!m_bGrassVisionForCampLast[iIndex] && m_bGrassVisionForCamp[iIndex])
				{
					m_bNewUnit[iIndex] = true;
				}
			}

			m_bGrassVisionForCampLast[iIndex] = m_bGrassVisionForCamp[iIndex];
		}
	}
}

void BeUnit::SetPlayer(int iPlayerIdx)
{
	if (iPlayerIdx < 0 || iPlayerIdx >= MAX_PLAYERS)
	{
		return;
	}

	if (iPlayerIdx < MAX_ACTIVEPLAYERS)
	{
		for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
		{
			m_abInPlayerVision[i] = true;
		}
	}

	int iOldPlayer = GetPlayer();
	SeRoomPlayerCamp eOldCamp = SeRoomPlayerCamp(m_eCamp);
	gMap.ReleaseUnitVision(this);
	if (m_pkBackData->iOrgPlayer == -1)
	{
		if (m_pkBackData->iPlayer == -1)
		{
			m_pkBackData->iOrgPlayer = iPlayerIdx;
		}
	}

	m_pkBackData->iPlayer = iPlayerIdx;
	SetShareUnitChangeFlag(BSUDCF_CAMP);
	m_eCamp = gMain.GetPlayerCamp(iPlayerIdx);
	SetControl(iPlayerIdx, true, false);

	if (iPlayerIdx != iOldPlayer)
	{
		(gLevelMain.GetLabelMgr())->AutoLabelObj(this, eOldCamp);
	}

	OnPlayerChanged();
}

void BeUnit::OnPlayerChanged(void)
{
}

void BeUnit::OnSuspend(void)
{
	ClrFlag(BUF_REMOVE);
	SetDead();
	gMap.ReleaseUnitVision(this);
	DelAllBuffer(HasFlag(BUF_RELIVE));
	SetUnitvisible(false);


}

void BeUnit::OnResume(void)
{
	ClrFlag(BUF_RELIVE | BUF_BUYLIVE | BUF_REMOVE | BUF_DEAD);
	ClrOtherFlag(BUOF_SPECIAL_RELIVE);
	BeCommand beCommand(BCT_STOP);
	GiveCommand(beCommand, BCT_DEATH);
	SetUnitvisible(true);
	ClrOtherFlag(BUOF_WEIYI);
	m_iMoveAllTime = 0;
}

void BeUnit::SetUnitReliveTime(UInt uiUnitReliveTime)
{
	if (m_pkBackData->uiUnitReliveTime < gTime)
	{
		m_pkBackData->uiUnitReliveTime = uiUnitReliveTime;
	}
	else
	{
		m_pkBackData->uiUnitReliveTime = m_pkBackData->uiUnitReliveTime > uiUnitReliveTime ? uiUnitReliveTime : m_pkBackData->uiUnitReliveTime;
	}
}

bool BeUnit::IsIdle(void) const
{
	return m_kCommander.IsIdle();
}

bool BeUnit::GiveCommand(BeCommand& kCmd, BeGiveCmdType eType, bool bPlayerControl, bool bNeedHangCurrent, bool bCheckChaoFeng)
{
	if (eType != BCT_PLAYER_SHIFT && HasUnitImmunityFlag(BUIF_BATI) && kCmd.eCmdType == BCT_STOP)
	{
		return false;
	}

	switch (kCmd.eCmdType)
	{
	case BCT_ATTACK:
	{
		if ((GetBuffer('B538') || GetBuffer('B611')) && bCheckChaoFeng) {

			return false;
		}

		if (kCmd.iUnitID && kCmd.iUnitID == GetID())
		{
			return false;
		}
		if (GetResUnitAttackType() == UNIT_ATTACKTYPE_NONE)
		{
			return false;
		}

		BeUnit* pkUnit = gUnitMgr.GetUnitByID(kCmd.iUnitID);
		if (pkUnit)
		{
			if (eType == BCT_SYSTEM_SHIFT)
			{
				break;
			}
			if (pkUnit->IsDead())
			{
				return false;
			}
		}
		else
		{
			if (kCmd.iUnitID != 0)
			{
				return false;
			}
		}
		m_iGiveAttackCmdTime = gTime;
		break;
	}
	case BCT_STOP:
	{
		if ((GetBuffer('B538') || GetBuffer('B611')) && bCheckChaoFeng) {

			return false;
		}
		if (HasFlag(BUF_ISPERSISTSKILL) && eType != BCT_DEATH)
		{
			return false;
		}
		break;
	}
	case BCT_MOVE:
	case BCT_MOVE_DIRECT:
	{
		if ((GetBuffer('B538') || GetBuffer('B611')) && bCheckChaoFeng) {

			return false;
		}

		if (m_iGiveAttackCmdTime == gTime)
		{
			return false;
		}
		if (GetClass() == UNIT_CLASSTYPE_BUILDING || GetClass() == UNIT_CLASSTYPE_SHOP)
		{
			return false;
		}
		if (!UnitCanMove(true))
		{
			return false;
		}
		break;
	}
	case BCT_SPELL:
	{
		if ((GetBuffer('B538') || GetBuffer('B611')) && bCheckChaoFeng) {

			return false;
		}

		BeSkill* pkSkill = GetSkill(kCmd.iData);
		if (pkSkill)
		{
			const	SkillTable* pkSkillRes = pkSkill->GetResPtr();
			if (pkSkillRes)
			{
				if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_GODUSE)
				{
					if (!pkSkill->CDComplete(this))
					{
						return false;
					}

					pkSkill->SetLastUseTime(this, gTime);

					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
					kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
					gTrgMgr.FireTrigger(BTE_SPELL_EFFECT, kParam);

					return true;
				}

				if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_TAKEDRUGS || pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_XIADI)
				{
					bool bXiaDiResult = true;
					if ((pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_XIADI) && kCmd.eTargetType == BCTT_UNIT && kCmd.iUnitID && kCmd.iUnitID != GetID()) {

						bXiaDiResult = false;
					}

					if ((pkSkillRes->uiSkillProperty & SKILL_SKILLPROPERTY_XIADI) && kCmd.eTargetType == BCTT_MAP) {

						if (pkSkillRes->iSkillDistance[pkSkill->GetLevel()] < GetDistance(GetPosX(), GetPosY(), kCmd.kPos.fX, kCmd.kPos.fY)) {

							bXiaDiResult = false;
						}
					}

					if (bXiaDiResult) {

						if (!pkSkill->CDComplete(this))
						{
							return false;
						}

						if (pkSkillRes->uiOperateType != SKILL_OPERATETYPE_TAKEDRUGS) {

							if (kCmd.eTargetType == BCTT_UNIT) {

								BeUnit* pkTarget = gUnitMgr.GetUnitByID(kCmd.iUnitID);

								if (!pkTarget) {
									return false;
								}

								if (GetDistance(GetPosX(), GetPosY(), pkTarget->GetPosX(), pkTarget->GetPosY()) > (pkSkillRes->iSkillDistance[pkSkill->GetLevel()] + 1)) {

									return false;
								}
							}
						}

						int iNeedMana = 0;
						const SkillTable* pkSkillRes = SkillTableMgr::Get()->GetSkillTable(pkSkill->GetTypeID());
						if (pkSkillRes)
						{
							iNeedMana = SkillTableMgr::Get()->GetSkillTable(pkSkill->GetTypeID())->fManaSpend[pkSkill->GetLevel() - 1];
						}

						if (GetMP() < iNeedMana)
						{
							return false;
						}

						if (HasUnitCarryFlag(BUCF_DIZZY) || HasUnitCarryFlag(BUCF_ISFORBIDSKILL))
						{
							return false;
						}

						SetMP(GetMP() - iNeedMana);
						pkSkill->SetLastUseTime(this, gTime);

						TePtParam kParam;
						kParam.SetParam(BTP_pkTrgUnit, this);
						kParam.SetParam(BTP_pkSkill, pkSkill);
						kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
						kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
						kParam.SetParam(BTP_iSpellTargetID, kCmd.iUnitID);
						kParam.SetParam(BTP_fSpellTargetPosX, kCmd.kPos.fX);
						kParam.SetParam(BTP_fSpellTargetPosY, kCmd.kPos.fY);
						gTrgMgr.FireTrigger(BTE_SKILL_ONSPELL, kParam);
						gTrgMgr.FireTrigger(BTE_SPELL_EFFECT, kParam);

						if (IsHero() && pkSkill->GetTypeID() != 'SA06')
						{
							for (int i = 0; i < UNIT_MAX_ITEM; ++i)
							{
								BeItem* pkItem = m_apkItem[i];
								if (!pkItem || !pkItem->GetOrgValid())
								{
									continue;
								}
								for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
								{
									BeSkill* pkItemSkill = pkItem->m_akSkill[iSkill];
									if (pkItemSkill && pkItemSkill->HasProperty(SKILL_SKILLPROPERTY_ONSPELLFIRE))
									{
										if ((IsDividMan() || IsGhost()))
										{
											continue;
										}

										TePtParam kParam;
										kParam.SetParam(BTP_pkTrgUnit, this);
										kParam.SetParam(BTP_pkSkill, pkItemSkill);
										kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
										kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
										kParam.SetParam(BTP_iItemID, pkItem->GetID());
										kParam.SetParam(BTP_iItemPos, i + 1);
										kParam.SetParam(BTP_iItemTypeID, pkItem->GetTypeID());
										kParam.SetParam(BTP_iTriggerItemID, pkItem->GetID());

										gTrgMgr.FireTrigger(BTE_SKILL_ONSPELL, kParam);
										break;
									}
								}
							}
						}

						if (!GetBuffer('BA28')) {
							return true;
						}
					}
				}

				if (pkSkillRes->uiTypeID == 'SA03')
				{
					if (!pkSkill->CDComplete(this))
					{
						return false;
					}

					if (HasUnitCarryFlag(BUCF_DIZZY) || HasUnitCarryFlag(BUCF_ISFORBIDSKILL) || HasUnitCarryFlag(BUCF_CANNOTMOVE) || HasUnitCarryFlag(BUCF_ISMOVING))
					{
						return false;
					}

					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
					kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
					SetUD_Float(UDK_Flash_X, kCmd.kPos.fX);
					SetUD_Float(UDK_Flash_Y, kCmd.kPos.fY);
					gTrgMgr.FireTrigger(BTE_SPELL_EFFECT, kParam);

					pkSkill->SetLastUseTime(this, gTime);
					return true;
				}
			}
		}

		if (HasUnitCarryFlag(BUCF_ISFORBIDSKILL))
		{
			return false;
		}

		if (!pkSkill || IsForbidSkill(pkSkill))
		{
			return false;
		}

		if (!pkSkill->CDComplete(this))
		{
			return false;
		}

		if (pkSkill->GetOperateType() == SKILL_OPERATETYPE_BEIDONG)
		{
			return false;
		}
		if (UnitCanUseSkill(kCmd.iData, gUnitMgr.GetUnitByID(kCmd.iUnitID), false, false, kCmd.kPos.fX, kCmd.kPos.fY) != UUSRT_OK)
		{
			return false;
		}

		if (kCmd.eTargetType == BCTT_UNIT)
		{
			BeUnit* pkUnit = gUnitMgr.GetUnitByID(kCmd.iUnitID);
			if (pkUnit)
			{
				UnitUseSkillResultType eResult = UnitCanUseSkill(pkSkill->GetTypeID(), pkUnit, true);
				if (eResult != UUSRT_OK)
				{
					return false;
				}
				if (!IsSkillTargetType(pkSkill->GetResPtr(), pkUnit))
				{
					return false;
				}

				if (pkUnit->IsDead())
				{
					return false;
				}

				if (pkUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER || pkUnit->GetClass() == UNIT_CLASSTYPE_HERO)
				{
					if (GetCamp() != pkUnit->GetCamp())
					{
						SetCastVisionTime(gTime + 3000);
					}
				}
			}
		}

		if (m_kLastCommand.iData == kCmd.iData && kCmd.iData == 'SA02')
		{
			return false;
		}
		break;
	}
	case BCT_USE_ITEM:
	{
		BeItem* pkItem = GetItemByID(kCmd.iData);
		if (!pkItem)
		{
			return false;
		}

		int iSkillTypeID = GetItemSkillTypeID(kCmd.iData);
		BeSkill* pkSkill = pkItem->GetSkillByTypeID(iSkillTypeID);
		if (!pkSkill)
		{
			return false;
		}

		if (!pkSkill->CDComplete(this))
		{
			return false;
		}

		const	SkillTable* pkSkillRes = pkSkill->GetResPtr();
		if (pkSkillRes)
		{
			if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_GODUSE)
			{
				if (!pkSkill->CDComplete(this))
				{
					return false;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
				kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
				gTrgMgr.FireTrigger(BTE_SPELL_EFFECT, kParam);

				pkSkill->SetLastUseTime(this, gTime);
				pkItem->SetLastUseTime(gTime);
				return true;
			}
			else {

				if ((GetBuffer('B538') || GetBuffer('B611')) && bCheckChaoFeng) {

					return false;
				}
			}
			if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_TAKEDRUGS)
			{
				if (!pkSkill->CDComplete(this))
				{
					return false;
				}
				if (HasUnitCarryFlag(BUCF_DIZZY) || HasUnitCarryFlag(BUCF_ISFORBIDSKILL) || HasUnitCarryFlag(BUCF_ISTETANIC))
				{
					return false;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_iSkillTypeID, pkSkill->GetTypeID());
				kParam.SetParam(BTP_iSkillLevel, pkSkill->GetLevel());
				gTrgMgr.FireTrigger(BTE_SKILL_ONSPELL, kParam);
				gTrgMgr.FireTrigger(BTE_SPELL_EFFECT, kParam);

				pkSkill->SetLastUseTime(this, gTime);
				return true;
			}
		}
		break;
	}
	}

	if (GetTypeID() == 'UT32' || GetTypeID() == 'UT29' || GetTypeID() == 'UT46')
	{
		if (kCmd.eCmdType != BCT_USE_ITEM)
		{
			SetUD_Int(UDK_BreakSkill, 200);
		}
	}
	m_kCommander.GiveCommand(kCmd, eType, GetID(), bNeedHangCurrent);
	m_kLastCommand = kCmd;

	return true;
}

int	BeUnit::GetCommandCount() const
{
	return m_kCommander.GetCommandCount();
}

void BeUnit::SetPosition(float fX, float fY, float fZ, bool bNoRecordChange)
{
	if (HasOtherFlag(BUOF_CANNOT_WEIYI))
	{
		return;
	}

	if (!bNoRecordChange)
	{
		if (fabs(m_pkCurData->fPosX - fX) > 1.f)
		{
			SetShareUnitChangeFlag(BSUDCF_POSX);
		}
		if (fabs(m_pkCurData->fPosY - fY) > 1.f)
		{
			SetShareUnitChangeFlag(BSUDCF_POSX);
		}
	}

	m_pkCurData->fPosX = fX;
	m_pkCurData->fPosY = fY;
}

void BeUnit::SetUnitAction(BeUnitAction eAction, int eActionName, int iAllTime, bool bSetShow, bool bForceRefresh)
{
	if (HasFlag(BUF_ISPERSIST))
	{
		return;
	}

	int iTime = gTime;

	if (m_iActionStayTime > iTime && eAction != BUA_DEATH)
	{
		return;
	}

	switch (eAction)
	{
	case BUA_BIRTH:
	{
		if (m_pkCurData->eActionType != eAction)
		{
			m_pkCurData->eActionType = eAction;
			m_pkCurData->iActionCurTime = 0;
			SetActionAllTime(iAllTime);
			SetActionName(BAN_birth, gTime);
		}
		break;
	}
	case BUA_STAND:
	{
		if (m_pkCurData->eActionType != eAction || bForceRefresh)
		{
			if (m_pkCurData->eActionType != BUA_WALK)
			{
				SetActionName(BAN_stand, gTime);
			}
			m_pkCurData->eActionType = eAction;
			m_pkCurData->iActionCurTime = 0;
		}
		break;
	}
	case BUA_WALK:
	{
		if (m_pkCurData->eActionType != eAction || bForceRefresh)
		{
			m_pkCurData->eActionType = eAction;
			m_pkCurData->iActionCurTime = 0;

			SetActionName(BAN_walk, gTime);

			if (HasUnitCarryFlag(BUCF_NEW_WALK))
			{
				SetActionName(BAN_run, gTime);
			}
		}
		break;
	}
	case BUA_ATTACK:
	{
		if (HasProperty(UNIT_PROPERTY_NOATTACKACTION))
		{
			return;
		}
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(GetAttackCD());
		break;
	}
	case BUA_SPELL:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(iAllTime);
		break;
	}
	case BUA_DEATH:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;

		if (m_pkCurData->iActionAllTime <= 0)
		{
			SetActionAllTime(10);
		}
		break;
	}
	case BUA_DISSIPATE:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(2000);
		break;
	}
	case BUA_DECAY:
	{
		m_pkCurData->eActionType = eAction;
		m_pkCurData->iActionCurTime = 0;
		SetActionAllTime(12000);
		break;
	}
	default:
	{
		break;
	}
	}

	if (eActionName != BAN_Null)
	{
		SetActionName(eActionName, gTime);

		if (IsHero() && (eAction == BUA_SPELL || eAction == BUA_ATTACK))
		{
			if (HasUnitCarryFlag(BUCF_ISINVISIBLE))
			{
				BeBuffer* pkYinShen = GetBuffer('BA15');
				if (pkYinShen)
				{
					pkYinShen->ClrCarryFlag(BUCF_ISINVISIBLE);
					ClrFlag(BUF_HIDE);
					ClrUnitCarryFlag(BUCF_ISINVISIBLE);
				}
			}
		}
	}
}

void BeUnit::DamagedByAbsNum(BeAttackType eAttackType, float fDamage, float fRawDamage, const SeSharedPtr<BeUnit>& kAttacker, int iPlayer, int iFlag, bool bCanDead, bool bFirstAttack, int iSkillTypeID, int iSkillLevel, float fLeech, float fMultiDamage, int iBeDamagedEffect)
{
	if (IsDead())
	{
		return;
	}

	fDamage *= 1.0f - GetPerDamageReduce();

	BeUnit* pkAttacker = kAttacker.Get();
	int iAttacker = 0;
	if (!kAttacker.IsNull())
	{
		iAttacker = kAttacker.GetPointer()->GetID();
	}

	if (IsHero())
	{
		pkAttacker->SetUD_Int(UDK_AttackHeroEpTime, gTime);
	}

	if (fMultiDamage > 0.0f)
	{
		iFlag |= BAF_HAS_BAOJI;
	}

	AIOnBeDamaged(pkAttacker, gTime, iSkillTypeID);
	TrgOnBeDamaged(eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iSkillTypeID);

	if (bFirstAttack)
	{
		TePtParam kParamAttackPre;
		kParamAttackPre.SetParam(BTP_iAttackType, (int)eAttackType);
		kParamAttackPre.SetParam(BTP_iFlag, iFlag);
		kParamAttackPre.SetParam(BTP_iAttackerPlayer, iPlayer);
		kParamAttackPre.SetParam(BTP_pkTrgUnit, this);
		kParamAttackPre.SetParam(BTP_pkAttacker, pkAttacker);
		kParamAttackPre.SetParam(BTP_pfDamage, &fDamage);
		kParamAttackPre.SetParam(BTP_iSkillTypeID, iSkillTypeID);
		kParamAttackPre.SetParam(BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(BTE_UNIT_ATTACKED_PRE, kParamAttackPre);

		TePtParam kParamAttack;
		kParamAttack.SetParam(BTP_iAttackType, (int)eAttackType);
		kParamAttack.SetParam(BTP_iFlag, iFlag);
		kParamAttack.SetParam(BTP_iAttackerPlayer, iPlayer);
		kParamAttack.SetParam(BTP_pkTrgUnit, this);
		kParamAttack.SetParam(BTP_pkAttacker, pkAttacker);
		kParamAttack.SetParam(BTP_pfDamage, &fDamage);
		kParamAttack.SetParam(BTP_iSkillTypeID, iSkillTypeID);
		kParamAttack.SetParam(BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(BTE_UNIT_ATTACKED, kParamAttack);
	}
	else
	{
		TePtParam kParamAttack;
		kParamAttack.SetParam(BTP_iAttackType, (int)eAttackType);
		kParamAttack.SetParam(BTP_iFlag, iFlag);
		kParamAttack.SetParam(BTP_iAttackerPlayer, iPlayer);
		kParamAttack.SetParam(BTP_pkTrgUnit, this);
		kParamAttack.SetParam(BTP_pkAttacker, pkAttacker);
		kParamAttack.SetParam(BTP_pfDamage, &fDamage);
		kParamAttack.SetParam(BTP_iSkillTypeID, iSkillTypeID);
		kParamAttack.SetParam(BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(BTE_UNIT_ATTACKED_NOTFIRST, kParamAttack);
	}

	BeTDEventArgs kArgs = BeTDEventArgs();
	kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITONBEDAMAGED;
	kArgs.kUnitOnBeDamaged.iAttackFlag = iFlag;
	kArgs.kUnitOnBeDamaged.iSkillTypeID = iSkillTypeID;
	kArgs.kUnitOnBeDamaged.fDamage = fDamage;
	kArgs.kUnitOnBeDamaged.iTargetID = m_iID;
	if (pkAttacker)
	{
		kArgs.kUnitOnBeDamaged.iAttackerID = pkAttacker->GetID();
	}
	gLevelMain.GetEventMgr()->FireEvent(kArgs);

	if ((iFlag & BAF_NORMAL))
	{
		BeUnit* pkAttacked = this;
		if (fLeech > 0.0f)
		{
			if (pkAttacker->GetCamp() != pkAttacked->GetCamp() && GetClass() != UNIT_CLASSTYPE_BUILDING)
			{
				float fTempDam = fDamage;
				float fStealHP = 0.0f;
				fStealHP = fTempDam * fLeech;
				if (fStealHP < 0)
				{
					fStealHP = 0;
				}
				if (pkAttacker->IsDividMan() || pkAttacker->IsGhost())
				{
					fStealHP = 0;
				}

				pkAttacker->SetHP(pkAttacker->GetHP() + fStealHP);
			}
		}
	}

	std::map<int, float>::iterator iter = m_akShield.begin();
	for (; iter != m_akShield.end(); iter++)
	{
		int iBuffer = iter->first;
		float fShield = iter->second;
		if (fShield <= 0)
		{
			continue;
		}

		BeBuffer* pkBuffer = GetBufferByInnerID(iBuffer);
		if (!pkBuffer)
		{
			continue;
		}
		if (!pkBuffer->HasShieldDefendType(eAttackType))
		{
			continue;
		}

		TePtParam kParam;
		kParam.SetParam(BTP_pkTrgUnit, this);
		kParam.SetParam(BTP_pkAttacker, pkAttacker);
		kParam.SetParam(BTP_pkTarget, this);
		kParam.SetParam(BTP_pkBuffer, pkBuffer);
		kParam.SetParam(BTP_iFlag, iFlag);
		kParam.SetParam(BTP_iAttackType, eAttackType);
		kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);

		gTrgMgr.FireTrigger(BTE_SHIELD_BEDAMAGE, kParam);

		if (fDamage >= fShield)
		{
			fDamage -= fShield;
			m_fTotalShield -= fShield;
			iter->second = 0;

			SetShareUnitChangeFlag(BSUDCF_SHELD);
			gTrgMgr.FireTrigger(BTE_SHIELD_DELBUFFER, kParam);
			OnDelBuffer(pkBuffer);
		}
		else
		{
			SetShareUnitChangeFlag(BSUDCF_SHELD);
			iter->second -= fDamage;
			m_fTotalShield -= fDamage;
			fDamage = 0.0f;
			break;
		}
	}

	BeShareDamageData	kData;
	kData.fValue = fDamage;
	kData.iAttackerID = iAttacker;
	kData.iUnitID = GetID();
	kData.iDamageType = eAttackType;
	if (iFlag & BAF_HAS_BAOJI)
	{
		kData.eType = BSDT_CRITILE;
	}

	gMain.AddDamageData(kData);

	int iPreHP = (int)ceil(m_pkCurData->fHP);

	if (fDamage >= m_pkCurData->fHP)
	{
		fDamage = m_pkCurData->fHP;
	}
	else if (fDamage < 0.0f)
	{
		fDamage = 0.0f;
	}

	if ((iFlag & BAF_NODAMAGE) != BAF_NODAMAGE)
	{
		SetHP(GetHP() - fDamage);
		if (m_iHPLock > 0 && GetHP() < float(m_iHPLock))
		{
			SetHP(float(m_iHPLock));
		}

		if (IsHero() && pkAttacker->IsHero())
		{
			gMain.SetPlayerAttackTime(GetPlayer(), pkAttacker->GetPlayer());
		}
	}

	if (bFirstAttack)
	{
		TePtParam kParamAttack;
		kParamAttack.SetParam(BTP_iAttackType, (int)eAttackType);
		kParamAttack.SetParam(BTP_iFlag, iFlag);
		kParamAttack.SetParam(BTP_iAttackerPlayer, iPlayer);
		kParamAttack.SetParam(BTP_pkTrgUnit, this);
		kParamAttack.SetParam(BTP_pkAttacker, pkAttacker);
		kParamAttack.SetParam(BTP_pfDamage, &fDamage);
		kParamAttack.SetParam(BTP_iSkillTypeID, iSkillTypeID);
		kParamAttack.SetParam(BTP_iSkillLevel, iSkillLevel);

		gTrgMgr.FireTrigger(BTE_UNIT_AFTERATTACKED, kParamAttack);
	}

	if (pkAttacker->GetTypeID() != 'LT05' && pkAttacker->GetTypeID() != 'HT05')
	{
		if (!bCanDead && m_pkCurData->fHP <= 0.0f)
		{
			SetHP(1.0f);
		}
		if (HasOtherFlag(BUOF_NEVEDEATH) && m_pkCurData->fHP <= 0.0f)
		{
			SetHP(1.0f);
		}
		if (HasUnitCarryFlag(BUCF_CANNOTDEATH) && m_pkCurData->fHP <= 0.0f)
		{
			SetHP(1.0f);
		}
	}
	if (iBeDamagedEffect != 0)
	{
		AddAttach(iBeDamagedEffect, 100, iAttacker);
	}
	if (m_pkCurData->fHP <= 0.0f)
	{
		OperateUnitDead(eAttackType, fDamage, pkAttacker, iPlayer, iFlag, false, iSkillTypeID);
	}

	if (pkAttacker && pkAttacker->IsHero() && IsHero()) 
	{
		BeSkill* pkSkill = pkAttacker->GetSkill(iSkillTypeID);

		if (pkSkill) {

			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DamagedVision)) {
				pkAttacker->SetCastVisionTime(gTime + 4000);
			}
		}
	}
}

void BeUnit::OperateUnitDead(BeAttackType eAttackType, float fDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, bool bIsTimeOver, int iSkillTypeID)
{
	if (IsDead())
	{
		return;
	}
	if (HasOtherFlag(BUOF_NEVEDEATH))
	{
		SetHP(1.0f);
		return;
	}

	SetHP(0.0f);

	if (!bIsTimeOver && IsHero())
	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = GetItemByPos(i);
			if (!pkItem)
			{
				continue;
			}
			const std::vector<BeSkill*>& rkItemSkill = pkItem->GetAllSkill();
			for (int iSkillIndex = 0; iSkillIndex < rkItemSkill.size(); iSkillIndex++)
			{
				BeSkill* pkSkill = rkItemSkill[iSkillIndex];
				if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEATHRELIVE) && pkSkill->CDComplete(this))
				{
					if ((IsDividMan() || IsGhost()))
					{
						continue;
					}
					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					if (pkAttacker)
					{
						kParam.SetParam(BTP_pkAttacker, pkAttacker);
					}

					kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
					kParam.SetParam(BTP_pkTarget, this);
					kParam.SetParam(BTP_pkSkill, pkSkill);
					kParam.SetParam(BTP_pfDamage, &fDamage);
					kParam.SetParam(BTP_iFlag, iFlag);
					kParam.SetParam(BTP_iAttackType, eAttackType);

					gTrgMgr.FireTrigger(BTE_SKILL_SKIP_DEATH, kParam);

					pkSkill->SetLastUseTime(this, (int)gTime);
					pkItem->SetLastUseTime(gTime);
					pkItem->SetItemCDTime(pkSkill->GetCDTime(this));
					return;
				}
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEATHRELIVE) && pkSkill->CDComplete(this))
			{
				if ((IsDividMan() || IsGhost()))
				{
					continue;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				if (pkAttacker)
				{
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
				}

				kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
				kParam.SetParam(BTP_pkTarget, this);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(BTE_SKILL_SKIP_DEATH, kParam);

				pkSkill->SetLastUseTime(this, (int)gTime);
				return;
			}
		}
	}

	SetDead();

	if (!bIsTimeOver)
	{
		TrgOnDead(eAttackType, fDamage, pkAttacker, iPlayer, iFlag, iSkillTypeID);
	}

	DelAllBuffer(HasFlag(BUF_RELIVE));

	m_kCommander.ClearAllCommands();

	SetFlag(BUF_REMOVE);
}

void BeUnit::OperateUnitRemove()
{
	if (IsDead())
	{
		return;
	}

	SetHP(0.0f);
	SetDead();
	DelAllBuffer(HasFlag(BUF_RELIVE));
	SetFlag(BUF_REMOVE);
}

float BeUnit::GetDamagedByFormula(const BeUnit* pkAttacker, const BeAttackingAttr& rkAttackingAttr, float fDamage) const
{
	fDamage -= m_pkCurData->fDecDamage;
	if (fDamage < 0.0f)
	{
		return 0.0f;
	}
	float	fAttackerDamagePer = pkAttacker->GetDamagePer();
	fDamage *= fAttackerDamagePer;
	float	fSelfBeDamagePer = GetBeDamagePer();
	fDamage *= fSelfBeDamagePer;
	if (rkAttackingAttr.iSkillTypeID != 0)
	{
		float	fSkillDamagePer = pkAttacker->GetSkillDamagePer();
		fDamage *= fSkillDamagePer;
	}

	if (pkAttacker->GetClass() == UNIT_CLASSTYPE_SOLIDER)
	{
		if (GetClass() == UNIT_CLASSTYPE_BUILDING)
		{
			if (pkAttacker->HasProperty(UNIT_PROPERTY_DECPOWDAMAGE))
			{
				fDamage *= 1.8f;
			}
			else
			{
				fDamage *= 1.0f;
			}
		}
		else if (GetClass() == UNIT_CLASSTYPE_HERO)
		{
			fDamage *= 0.6f;
		}
	}
	else if (pkAttacker->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		if (HasProperty(UNIT_PROPERTY_DECPOWDAMAGE))
		{
			fDamage *= 0.7f;
		}
		if (!IsHero() && GetSecClass() != UNIT_SECCLASSTYPE_GUARD)
		{
			if (GetTypeID() == 'HB01' || GetTypeID() == 'LB01')
			{
				fDamage = GetMaxHP() * 0.45f;
			}
			else if (GetTypeID() == 'HB02' || GetTypeID() == 'LB02')
			{
				fDamage = GetMaxHP() * 0.75f;
			}
			else if (GetTypeID() == 'HB03' || GetTypeID() == 'LB03')
			{
				fDamage = GetMaxHP() * 0.11f;
			}
			else if (GetTypeID() == 'HB04' || GetTypeID() == 'LB04')
			{
				fDamage = GetMaxHP() * 0.11f;
			}
			return fDamage;
		}
	}

	if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_MAGIC)
	{
		float	fAntiMagic = 0.0f;
		GetAttackedMagicAttr(fAntiMagic, pkAttacker, pkAttacker->GetDecMagicArmor(), pkAttacker->GetPerDecMagicArmor());
		fDamage *= fAntiMagic;
	}
	else if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_PHYSICS)
	{
		float fAntiPhysic = 1.0f;
		GetAttackedPhysicAttr(fAntiPhysic, pkAttacker, pkAttacker->GetDecArmor(), pkAttacker->GetPerDecArmor());
		fDamage *= fAntiPhysic;
	}
	else if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_ABSOLUTE)
	{
	}
	else
	{
	}
	return fDamage;
}
void BeUnit::SetDead(void)
{
	BeCommand	kComm(BCT_STOP);
	GiveCommand(kComm, BCT_DEATH);

	SetFlag(BUF_DEAD);
	gMap.DelUnitObstacle(this, true);
	gUnitMgr.OnUnitDead(this);
}

void	BeUnit::OnBeDamaged(BeAttackingAttr& rkAttackingAttr, bool bCanDead, bool bIgnoreInvincible, float fMaxHeroAbsDamage)
{
	//	gRandNum.RandInteger(RANDINFO);
	if (IsDead())
	{
		return;
	}

	if (!bIgnoreInvincible && IsInvincible())
	{
		return;
	}

	BeUnit* pkAttacker = rkAttackingAttr.kAttacker.Get();
	if (!pkAttacker)
	{
		return;
	}
	if (!pkAttacker->m_pkCurData)
	{
		return;
	}

	if (rkAttackingAttr.fDamage <= 0)
	{
		return;
	}

	float fMultiDamage = 0.f;
	if (rkAttackingAttr.eAttackType == BAT_NORMAL && GetClass() != UNIT_CLASSTYPE_BUILDING)
	{
		if (rkAttackingAttr.GetFlag() & BAF_NOT_BAOJI)
		{
		}
		else if (rkAttackingAttr.GetFlag() & BAF_BAOJI)
		{
			float	fBaoJiPerDamage = 0;
			if (m_pkCurData->fBaoJiDecDamage > 0.1 && m_pkCurData->fBaoJiDecDamage < 1.0) {
				fBaoJiPerDamage = 2.0f * pkAttacker->GetBaoJiDamagePer() * m_pkCurData->fBaoJiDecDamage;
			}
			else {
				fBaoJiPerDamage = 2.0f * pkAttacker->GetBaoJiDamagePer();
			}
			fMultiDamage = rkAttackingAttr.fDamage;
			rkAttackingAttr.fDamage *= fBaoJiPerDamage;

			rkAttackingAttr.SetFlag(BAF_NOT_BAOJI);
		}
	}

	if (rkAttackingAttr.HasFlag(BAF_FIRST) && rkAttackingAttr.HasFlag(BAF_AVOID) && !rkAttackingAttr.HasFlag(BAF_CANTAVOID))
	{
		if (!IsGhost() && !IsDividMan())
		{
			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, this);
			kParam.SetParam(BTP_pkAttacker, pkAttacker);
			kParam.SetParam(BTP_pkTarget, this);

			gTrgMgr.FireTrigger(BTE_ATTACK_AVOIDED, kParam);

			return;
		}
	}

	TrgOnPreBeDamaged(rkAttackingAttr.eAttackType, rkAttackingAttr.fDamage, rkAttackingAttr.fDamage, pkAttacker, rkAttackingAttr.iPlayer, rkAttackingAttr.GetFlag(), bCanDead, rkAttackingAttr.iSkillTypeID, rkAttackingAttr);

	float fDamage = rkAttackingAttr.fDamage;
	float fExtDamage = 0.0f;

	if (rkAttackingAttr.eAttackType == SKILL_ATTACKTYPE_ABSOLUTE)
	{
		float fDamageAbs = fDamage;
		DamagedByAbsNum(rkAttackingAttr.eAttackType, fDamageAbs, fDamageAbs, rkAttackingAttr.kAttacker, rkAttackingAttr.iPlayer, rkAttackingAttr.GetFlag(), bCanDead, rkAttackingAttr.bFirstAttack, rkAttackingAttr.iSkillTypeID, rkAttackingAttr.iSkillLevel, 0.f, 0.f, rkAttackingAttr.iBeDamagedEffect);
		return;
	}

	fDamage = GetDamagedByFormula(pkAttacker, rkAttackingAttr, fDamage);

	int		iAttackFlag = rkAttackingAttr.GetFlag();
	if ((iAttackFlag & BAF_FIRST) && (iAttackFlag & BAF_NORMAL))
	{
		if (pkAttacker->IsHero())
		{
			if (GetClass() == UNIT_CLASSTYPE_BUILDING)
			{
				float	fAp = pkAttacker->GetMagicDamage() * 0.5f;
				float	fAddDamage = pkAttacker->GetAddDamage();
				if (fAp > fAddDamage)
				{
					BeAttackingAttr		kAPDamageAttr;
					kAPDamageAttr.fDamage = fAp;
					kAPDamageAttr.eAttackType = BAT_SKILL;
					kAPDamageAttr.kAttacker = pkAttacker->GetSharePtr();

					OnBeDamaged(kAPDamageAttr);
				}
			}
		}
	}

	DamagedByAbsNum(rkAttackingAttr.eAttackType, fDamage, 1, rkAttackingAttr.kAttacker, rkAttackingAttr.iPlayer, rkAttackingAttr.GetFlag(), bCanDead, rkAttackingAttr.bFirstAttack, rkAttackingAttr.iSkillTypeID, rkAttackingAttr.iSkillLevel, rkAttackingAttr.fLeech, fMultiDamage, rkAttackingAttr.iBeDamagedEffect);
}

BeUnit* BeUnit::CopyUnitToOther(BeUnitFlag eFlag, int iSkillID)
{
	if (eFlag != BUF_ISGHOST && eFlag != BUF_ISDIVIDMAN)
	{
		return NULL;
	}

	int iTypeID = GetCurrentTypeID();
	bool bLiDian = false;
	bool bHuanZhuang = false;
	if (iTypeID == 'U671')
	{
		bLiDian = true;
		iTypeID = GetBackTypeID();
	}

	int iSkinIdx = -1;
	BeUnit* pkUnit = gUnitMgr.AddUnit(iTypeID, -1, GetPlayer(), -1, 0, iSkinIdx);
	if (pkUnit)
	{
		pkUnit->SetOrgPlayer(GetOrgPlayer());
		pkUnit->SetFlag(eFlag);
		pkUnit->SetConnectUnit(this);
		pkUnit->SetPlayer(GetPlayer());
		SetConnectUnit(pkUnit);
		pkUnit->SetScale(GetScale());
		if (HasOtherFlag(BUOF_AREA))
		{
			pkUnit->SetOtherFlag(BUOF_AREA);
		}
		if (HasOtherFlag(BUOF_AREA_SELF))
		{
			pkUnit->SetOtherFlag(BUOF_AREA_SELF);
		}

		pkUnit->CopyAllItemAndSkill(this);

		pkUnit->CopyAttribute(this);

		return pkUnit;
	}
	return NULL;
}

void BeUnit::CopyAllItemAndSkill(BeUnit* pkUnit)
{
	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	{
		BeItem* pkOrgItem = pkUnit->GetItemByPos(i);
		if (pkOrgItem)
		{
			BeItem* pkItem = AddItem(pkOrgItem->GetTypeID(), pkOrgItem->GetPackagePos());
			if (pkItem)
			{
				pkItem->SetItemData(pkOrgItem->GetItemData());
			}
		}
	}

	for (int i = 0; i < iMaxHeroSkillNum; ++i)
	{
		BeSkill* pkSkill = pkUnit->GetSkillByPos(i);
		if (pkSkill)
		{
			BeSkill* pkNewSkill = AddSkill(pkSkill->GetTypeID(), pkSkill->GetLevel());
			if (pkNewSkill)
			{
				pkNewSkill->CopySkillAttribute(pkSkill);
			}
		}
	}

	const std::vector<BeSkill*>& akNormal = pkUnit->GetNormalSkill();
	for (std::vector<BeSkill*>::const_iterator itr = akNormal.begin(); itr != akNormal.end(); ++itr)
	{
		BeSkill* pkSkill = *itr;
		if (pkSkill)
		{
			BeSkill* pkNewSkill = AddSkill(pkSkill->GetTypeID(), pkSkill->GetLevel());
			if (pkNewSkill)
			{
				pkNewSkill->CopySkillAttribute(pkSkill);
			}
		}
	}
}


bool BeUnit::UnitCanUseActiveSkill(int iAttackedID, int& iSkillTypeID, int& iSkillLevel)
{
	BeUnit* pkAttacked = gUnitMgr.GetUnitByID(iAttackedID);
	if (!pkAttacked)
	{
		return false;
	}

	return false;
}

void BeUnit::AddLevel(int iAddLevel)
{
	if (iAddLevel <= 0)
	{
		return;
	}
	int iPreLevel = m_pkBackData->iLevel;
	int iMaxLevel = iMonsterMaxLevel;
	if (AIGetType() == BUAT_HERO)
	{
		iMaxLevel = iHeroMaxLevel;
	}

	if (iAddLevel + m_pkBackData->iLevel > iMaxLevel)
	{
		iAddLevel = iMaxLevel - m_pkBackData->iLevel;
	}
	m_pkBackData->iLevel += iAddLevel;

	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_iIntData, m_pkBackData->iLevel);

	gTrgMgr.FireTrigger(BTE_UNIT_LEVELUP, kParam);

	BeTDEventArgs kArgs = BeTDEventArgs();
	kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNIT_LEVELUP;
	kArgs.kUnitOnLevelUp.iUnitID = GetID();
	gLevelMain.GetEventMgr()->FireEvent(kArgs);

	OnPropertyUpdate(m_pkBackData->iLevel);

	if (iPreLevel != m_pkBackData->iLevel)
	{
		SetShareUnitChangeFlag(BSUDCF_UNITLEVEL);

		SetTabInfoFlag(BTCF_LEVEL);
	}

	if (IsHero())
	{
		SetShareUnitChangeFlag(BSUDCF_HEROLVLUP);
	}
}

void BeUnit::ResetSkill(void)
{
	for (int iPos = 0; iPos < iMaxHeroSkillNum; iPos++)
	{
		BeSkill* pkSkill = GetSkillByPos(iPos, true);
		if (pkSkill)
		{
			pkSkill->SetLastUseTime(this, 0);
		}
	}
	DelBufferByClean();
	DelAllSkill();
	BeLearnSkillData::DEL(m_pkBackData->akLearnSkill);
}

void BeUnit::AISetType(BeUnitAIType eType, bool bNeedRecordChange)
{
	if (m_pkAI)
	{
		if (m_pkAI->GetAIType() == eType)
		{
			return;
		}

		gAIMgr.SafeDelUnitAI(m_pkAI);
	}

	m_pkAI = gAIMgr.CreateUnitAI(m_iID, eType);
	if (m_pkAI)
	{
		m_pkAI->AttachUnit(this);
		m_pkAI->AttachMain(pkAttachMain);
	}
}

BeUnitAIType BeUnit::AIGetType() const
{
	if (!m_pkAI)
	{
		return BUAT_NOAI;
	}
	return m_pkAI->GetAIType();
}

void BeUnit::AISetControlType(BeAIControlType eType)
{
	if (!m_pkAI)
	{
		return;
	}

	m_pkAI->SetAIControlType(eType);
}

BeAIControlType BeUnit::AIGetControlType() const
{
	if (!m_pkAI)
	{
		return BACT_BOT;
	}

	return m_pkAI->GetAIControlType();
}

void BeUnit::AIOnBeDamaged(BeUnit* pkAttacker, unsigned int dwTime, int iSkillTypeID)
{
	if (!m_pkAI)
	{
		return;
	}
	m_pkAI->OnBeDamaged(pkAttacker, dwTime, iSkillTypeID);
}
void BeUnit::AIOnUpdate(unsigned int dwTime)
{
	if (IsDead())
	{
		return;
	}
	if (!m_pkAI)
	{
		return;
	}
	if (gTime % 200 != 0)
	{
		return;
	}
	m_pkAI->OnTimeUpdate(dwTime);
}

void BeUnit::AISetFirstChoiceTarge(int iTarget)
{
	if (m_pkAI)
	{
		m_pkAI->SetFirstChoiceTarget(iTarget);
	}
}

void BeUnit::AIOnTargetChange(int iTargetID, bool bIsOrb, bool bAttackPos)
{
	int iID = GetAttackingUnitID();
	BeUnit* pkTarget = gUnitMgr.GetUnitByID(iID);
	if (pkTarget)
	{
		pkTarget->AIOnBeTargeted(this, gTime, bIsOrb, iTargetID, bAttackPos);

		if (GetClass() == UNIT_CLASSTYPE_HERO)
		{
			const std::vector<SeSharedPtr<BeUnit>>& rkVConnectUnits = GetConnectUnits();
			if (rkVConnectUnits.empty())
			{
				return;
			}

			for (int i = 0; i < (int)rkVConnectUnits.size(); ++i)
			{
				BeUnit* pkConnectUnit = rkVConnectUnits[i].GetPointer();
				if (pkConnectUnit)
				{
					BeBuffer* pkPetBuffer = pkConnectUnit->GetBuffer('BG04');
					if (pkPetBuffer)
					{
						BeCommand	kComm(BCT_ATTACK, pkTarget->GetID());
						pkConnectUnit->GiveCommand(kComm, BCT_IMMEDIATE);
					}

					if (pkConnectUnit->HasOtherFlag(BUOF_SHOUWEI))
					{
						if (pkConnectUnit->UnitCanAttack(pkTarget, false, false))
						{
							BeCommand	kComm(BCT_ATTACK, pkTarget->GetID());
							pkConnectUnit->GiveCommand(kComm, BCT_IMMEDIATE);
						}
					}
				}
			}
		}
	}
}
void  BeUnit::AIOnBeTargeted(BeUnit* pkAttacker, unsigned int dwCurrentTime, bool bIsOrb, int iTargetID, bool bAttackPos)
{
	if (!m_pkAI)
	{
		return;
	}
	m_pkAI->OnBeTargeted(pkAttacker, dwCurrentTime, bIsOrb, iTargetID, bAttackPos);
}

void  BeUnit::AIOnBeSkillTargeted(BeUnit* pkAttacker, int iSkillID, unsigned int dwCurrentTime)
{
	if (!m_pkAI)
	{
		return;
	}
	m_pkAI->OnBeSkillTargeted(pkAttacker, iSkillID, dwCurrentTime);
}

void BeUnit::AIOnHold(bool bConnect)
{
	if (!m_pkAI)
	{
		return;
	}
	m_pkAI->OnHold(bConnect);
}

void BeUnit::AIOnStop(bool bConnect)
{
	if (!m_pkAI)
	{
		return;
	}
	m_pkAI->OnStop(bConnect);
}

bool BeUnit::InBossArea(const BeUnit* pkBoss) const
{
	if (pkBoss && pkBoss->GetBackTypeID() == 'D017')
	{
		return true;
	}

	return true;
}
const BeUnit* BeUnit::AIFindTarget(const BeUnit* pkCurrentTarget, bool bCanMove, bool bSkill)
{
	if (IsDead() || !m_pkAI || HasUnitCarryFlag(BUCF_NOTFINDTARGET))
	{
		return NULL;
	}

	float fRadius = GetAttackRange(this);
	switch (AIGetType())
	{
	case BUAT_HERO:			fRadius = max(fRadius, 800.0f);		break;
	case BUAT_MONSTER:		fRadius = 250.0f;					break;
	case BUAT_GUARD:		fRadius = 800.0f;					break;
	case BUAT_MIJING:		fRadius = 800.0f;					break;
	default:				fRadius = max(fRadius, 500.0f);		break;
	}

	if (gMain.IsZhanChangMap())
	{
		BeUnit* pkTarget = m_pkAI->AttackFindTarget(fRadius);
		if (pkTarget && this->GetSecClass() == UNIT_SECCLASSTYPE_GUARD)
		{
			if (pkTarget->GetSecClass() == UNIT_SECCLASSTYPE_POST)
			{
				pkTarget = NULL;
			}
		}
		return pkTarget;
	}
	if (gMain.IsSecretArea())
	{
		return m_pkAI->MiJingAttackFindTarget(fRadius);
	}
	else
	{
		return m_pkAI->AttackFindTarget(fRadius);
	}
}

bool BeUnit::AIGetNeedRefreshTarget()
{
	return m_pkAI->GetNeedUpdateTarget();
}

void BeUnit::AISetNeedRefreshTarget(bool bNeed)
{
	if (!m_pkAI)
	{
		return;
	}

	m_pkAI->SetNeedUpdateTarget(bNeed);
}

bool BeUnit::UnitCanBeControl(int iPlayer) const
{
	if (GetControl(iPlayer))
	{
		return true;
	}
	return false;
}
bool BeUnit::UnitCanMove(bool bTemp) const
{
	if (bTemp && HasUnitCarryFlag(BUCF_CANNOTMOVE))
	{
		return false;
	}
	if (AIGetType() == BUAT_TOWER)
	{
		return false;
	}

	return true;
}
bool BeUnit::UnitCanAttack(const BeUnit* pkTarget, bool bTemp, bool bForceAttackSameGroup) const
{
	if (bTemp && HasUnitCarryFlag(BUCF_CANNOTATTACK))
	{
		return false;
	}
	else if (pkTarget)
	{
		if (GetID() == pkTarget->GetID())
		{
			return false;
		}
		if (!pkTarget->GetUnitVisionForCamp(GetCamp() - 1) && GetCamp() < SRPC_OTHER_SIDE)
		{
			return false;
		}
		if (HasOtherFlag(BUOF_ONLYATTACKBUILD) && pkTarget->GetClass() != UNIT_CLASSTYPE_BUILDING)
		{
			return false;
		}
		if (pkTarget->IsInvincible() || pkTarget->HasFlag(BUF_ISINVINCIBLE))
		{
			return false;
		}
		else if (pkTarget->HasUnitCarryFlag(BUCF_ISPHYSICIMMUNITY) && !HasUnitCarryFlag(BUCF_IGNORE_PHYSICIMMUNITY))
		{
			return false;
		}
		else if (pkTarget->IsDead())
		{
			return false;
		}

		if (GetClass() == UNIT_CLASSTYPE_BUILDING)
		{
			unsigned	int		iTargetClass = pkTarget->GetClass();
			if (iTargetClass == UNIT_CLASSTYPE_SMALLMONST || iTargetClass == UNIT_CLASSTYPE_BIGMONST || iTargetClass == UNIT_CLASSTYPE_BOSS)
			{
				return false;
			}
		}

		if (GetCamp() == pkTarget->GetCamp())
		{
			if (bForceAttackSameGroup)
			{
				return true;
			}

			return false;
		}
		else if (pkTarget->GetCamp() == SRPC_NEUTRAL_ALLY)
		{
			return true;
		}
	}
	return true;
}

UnitUseSkillResultType BeUnit::UnitCanUseSkill(int iSkillTypeID, const BeUnit* pkTarget, bool bTemp, bool bCheckPos, float fTargetPosX, float fTargetPosY, bool bArea, BeUnit** ppkTarget)
{
	BeSkill* pkSkill = GetSkill(iSkillTypeID);
	if (!pkSkill)
	{
		if (!IsHero())
		{
			return UUSRT_EER_COMMON;
		}
	}

	if (!pkSkill)
	{
		return UUSRT_EER_COMMON;
	}

	const SkillTable* pkRes = pkSkill->GetResPtr();
	if (!pkRes)
	{
		return UUSRT_EER_COMMON;
	}

	SeCalSkillLvlData kData;
	if (!GetSkillLvlData(kData, iSkillTypeID))
	{
		return UUSRT_EER_COMMON;
	}

	int iSkillOrgID = pkSkill->GetTypeID();
	switch (iSkillOrgID)
	{
	case 'S004':
	{
		if (GetUD_Int(UDK_S003) == 200)
		{
			SetUD_Int(UDK_S003, 100);
			DelBuffer('BA22');
			ClearActionStayTime();
		}
		break;
	}
	case 'S001':
	case 'S034':
	case 'S078':
	case 'S086':
	case 'S119':
	{
		int		iBufferForbid = GetUD_Int(UDK_Buffer_Forbid);
		if (iBufferForbid == 100)
		{
			return UUSRT_EER_ADDSKILL;
		}
		break;
	}
	case 'SA06':
	{
		if (pkSkill->GetUICounter() == 234)
		{
			return UUSRT_EER_ADDSKILL;
		}
		break;
	}
	case 'S107':
	{
		if (pkTarget)
		{
			if (pkTarget->GetBuffer('B375'))
			{
				return UUSRT_EER_ADDSKILL;
			}
		}
		break;
	}
	case 'S108':
	{
		bool	bCanUse = false;
		UnitGroup	kGroup;
		gUnitMgr.GetAreaGroup(kGroup, GetPosX(), GetPosY(), kData.GetSpellDistance(), this, BCT_SPELL, iSkillTypeID);
		if (!kGroup.IsEmpty())
		{
			for (int iIndex = 0; iIndex < kGroup.Size(); iIndex++)
			{
				BeUnit* pkFindUnit = kGroup[iIndex];
				if (pkFindUnit)
				{
					if (pkFindUnit->GetBuffer('BA04'))
					{
						bCanUse = true;
						break;
					}
				}
			}
		}
		if (!bCanUse)
		{
			return UUSRT_EER_ADDSKILL;
		}
		break;
	}
	default:
		break;
	}

	if (pkSkill->IsSwitch() && pkSkill->IsActive())
	{
		return UUSRT_OK;
	}

	int iNeedMana = 0;
	const SkillTable* pkSkillRes = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID);
	if (pkSkillRes)
	{
		iNeedMana = SkillTableMgr::Get()->GetSkillTable(iSkillTypeID)->fManaSpend[pkSkill->GetLevel() - 1];
	}

	if (GetMP() < iNeedMana)
	{
		return UUSRT_EER_MANA_NOT_ENOUGH;
	}

	if (!pkSkill->CDComplete(this))
	{
		return UUSRT_EER_CD;
	}

	if (IsDead() && pkSkill->GetTypeID() != 'SN04')
	{
		return UUSRT_EER_COMMON;
	}

	if (IsForbidSkill(pkSkill))
	{
		return UUSRT_EER_FORBID_SKILL;
	}

	return UUSRT_OK;
}

bool BeUnit::IsSkillTargetType(const SkillTable* pkRes, const BeUnit* pkTarget) const
{
	if (!pkRes)
	{
		return false;
	}

	if (pkTarget->IsDead())
	{
		return false;
	}

	if (pkTarget->GetClass() == UNIT_CLASSTYPE_EYE)
	{
		if (pkRes->uiTypeID == 'S059' || pkRes->uiTypeID == 'SA01' || pkRes->uiTypeID == 'S206' || pkRes->uiTypeID == 'S151')
		{
		}
		else
		{
			return false;
		}
	}

	int iSkillTargetFlag = pkRes->uiTargetType;
	int iStaticProcFlag = 0;
	int iDynaPropFlag = 0;
	TranslateSkillTargetType(iSkillTargetFlag, iStaticProcFlag, iDynaPropFlag);

	return IsTargetUnit(pkTarget, iStaticProcFlag, iDynaPropFlag);
}

void BeUnit::TranslateSkillTargetType(int iSkillTargetFlag, int& iStaticProcFlag, int& iDynaPropFlag) const
{
	iStaticProcFlag = 0;

	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_HERO ? BUSP_CLASS_HERO : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_BUILDING ? BUSP_CLASS_BUILDING : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_YUANGU ? BUSP_PROP_ANCIENT : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_SOLIDER ? BUSP_CLASS_SOLIDER : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_YEGUAI ? BUSP_CLASS_YEGUAI : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_BOSS ? BUSP_CLASS_BOSS : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_TOUSHI ? BUSP_CLASS_TOUSHI : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_TOTEM ? BUSP_CLASS_TOTEM : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_BRAND ? BUSP_CLASS_BRAND : 0;
	iStaticProcFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_MJ ? BUSP_CLASS_MIJING : 0;

	iDynaPropFlag = 0;
	iDynaPropFlag |= BUDP_SUMMON | BUDP_NOTSUMMON;

	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_DIJUN ? BUDP_CAMP_ENEMY : 0;
	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_SELF ? BUDP_CAMP_SELF : 0;
	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_YOUJUN ? BUDP_CAMP_ALLY : 0;
	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_MOMIAN ? BUDP_MAGIC_IMMUNITY : 0;
	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_WUDI ? BUDP_INVINCIBLE : 0;
	iDynaPropFlag |= iSkillTargetFlag & SKILL_TARGETTYPE_ZHONGLI ? BUDP_CAMP_NATURE : 0;
}

int BeUnit::GetAttackCD(void) const
{
	return m_pkCurData->iAttackCD;
}

bool BeUnit::IsTargetUnit(const BeUnit* pkUnit, int iStaticProcFlag, int iDynaPropFlag) const
{
	if (!pkUnit)
	{
		return false;
	}

	int iStaticFlag = 0;
	if ((iStaticFlag & iStaticProcFlag) != iStaticFlag)
	{
		return false;
	}

	if (pkUnit->GetClass() == UNIT_CLASSTYPE_HERO)
	{
		iStaticFlag |= BUSP_CLASS_HERO;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_BUILDING)
	{
		iStaticFlag |= BUSP_CLASS_BUILDING;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_SOLIDER)
	{
		iStaticFlag |= BUSP_CLASS_SOLIDER;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_SMALLMONST)
	{
		iStaticFlag |= BUSP_CLASS_YEGUAI;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_MACHINE)
	{
		iStaticFlag |= BUSP_CLASS_TOUSHI;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_BOSS)
	{
		iStaticFlag |= BUSP_CLASS_BOSS;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_TOTEM)
	{
		iStaticFlag |= BUSP_CLASS_TOTEM;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_BRAND)
	{
		iStaticFlag |= BUSP_CLASS_BRAND;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_BIGMONST)
	{
		iStaticFlag |= BUSP_PROP_ANCIENT;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_MIJINGMONSTER)
	{
		iStaticFlag |= BUSP_CLASS_MIJING;
	}
	if (pkUnit->GetClass() == UNIT_CLASSTYPE_MIJINGBOSS)
	{
		iStaticFlag |= BUSP_CLASS_MIJING;
	}

	if ((iStaticFlag & iStaticProcFlag) != iStaticFlag)
	{
		return false;
	}

	if (pkUnit->HasProperty(UNIT_PROPERTY_NONE))
	{
		iStaticFlag |= BUSP_PROP_NONE;
	}

	if (pkUnit->HasProperty(UNIT_PROPERTY_YUANGU))
	{
		iStaticFlag |= BUSP_PROP_ANCIENT;
	}

	if ((iStaticFlag & iStaticProcFlag) != iStaticFlag)
	{
		return false;
	}

	int iDynaFlag = 0;
	if (pkUnit->IsSummomPer() || pkUnit->IsSummonTemp())
	{
		iDynaFlag |= BUDP_SUMMON;
	}
	else
	{
		iDynaFlag |= BUDP_NOTSUMMON;
	}

	if (pkAttachMain)
	{
		int iPlayerGroup = gMain.GetPlayerCamp(GetPlayer());
		if (iPlayerGroup != -1)
		{
			int iUnitGroup = gMain.GetPlayerCamp(pkUnit->GetPlayer());
			if (GetPlayer() == pkUnit->GetPlayer() && pkUnit->IsHero())
			{
				iDynaFlag |= BUDP_CAMP_SELF;
			}
			else if (iUnitGroup == iPlayerGroup)
			{
				iDynaFlag |= BUDP_CAMP_ALLY;
			}
			else if (iUnitGroup == SRPC_NEUTRAL_PASSIVE)
			{
				iDynaFlag |= BUDP_CAMP_NATURE;
			}
			else
			{
				iDynaFlag |= BUDP_CAMP_ENEMY;
			}
		}
		else
		{
		}
	}

	if (pkUnit->IsInvincible() || pkUnit->HasFlag(BUF_ISINVINCIBLE))
	{
		iDynaFlag |= BUDP_INVINCIBLE;
	}

	if (pkUnit->HasUnitCarryFlag(BUCF_ISMAGICIMMUNITY))
	{
		iDynaFlag |= BUDP_MAGIC_IMMUNITY;
	}

	if (pkUnit->HasUnitCarryFlag(BUCF_ISPHYSICIMMUNITY) && !HasUnitCarryFlag(BUCF_IGNORE_PHYSICIMMUNITY))
	{
		iDynaFlag |= BUDP_PHYSIC_IMMUNITY;
	}

	if ((iDynaFlag & iDynaPropFlag) != iDynaFlag)
	{
		return false;
	}
	return true;
}

void BeUnit::TrgOnPreBeDamaged(int eAttackType, float& fDamage, float fRawDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, BeAttackingAttr& kAttr)
{
	if (pkAttacker && pkAttacker->m_pkCurData)
	{
		pkAttacker->TrgOnPreDamage(eAttackType, fDamage, this, iPlayer, iFlag, bCanDead, kAttr);
	}
}

void BeUnit::TrgOnPreDamage(int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr)
{
	{
		TrgOnPreDamage_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, kAttr);
	}

	TrgOnPreDamage_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, kAttr);

	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (pkItem && pkItem->GetOrgValid())
			{
				TrgOnPreDamage_T(pkItem->GetAllSkill(), (int)pkItem->GetAllSkill().size(), eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, kAttr, i);
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{
		/////////////////////////////////////////////////////////
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_SHANGHAI))
			{
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkBuffer, pkBuffer);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONPREDAMAGE, kParam);
			}
		}
	}
}

void BeUnit::TrgOnBeDamaged(int eAttackType, float& fDamage, float fRawDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID)
{
	{
		TrgOnBeDamaged_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);
	}

	TrgOnBeDamaged_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);

	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (pkItem && pkItem->GetOrgValid())
			{
				TrgOnBeDamaged_T(pkItem->GetAllSkill(), (int)pkItem->GetAllSkill().size(), eAttackType, fDamage, fRawDamage, pkAttacker, iPlayer, iFlag, bCanDead, iAttackSkillTypeID, i);
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{

		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_BEIJI))
			{

				TePtParam kParam;

				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkAttacker, pkAttacker);
				kParam.SetParam(BTP_pkTarget, this);
				kParam.SetParam(BTP_pkBuffer, pkBuffer);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_fRawDamage, fRawDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);
				kParam.SetParam(BTP_iSkillTypeID, iAttackSkillTypeID);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONBEDAMAGE, kParam);

				if (gLevelMain.GetEventMgr()->CanFireEvent(BeTDEventArgs::BeTDEVENT_BUFFER_ONBEDAMAGE))
				{
					BeTDEventArgs kArgs = BeTDEventArgs();
					kArgs.iEventType = BeTDEventArgs::BeTDEVENT_BUFFER_ONBEDAMAGE;
					kArgs.kUnitPreDamagedByBuffer.iAttackFlag = iFlag;
					kArgs.kUnitPreDamagedByBuffer.iBufferTypeID = pkBuffer->GetTypeID();
					kArgs.kUnitPreDamagedByBuffer.iSkillTypeID = iAttackSkillTypeID;
					kArgs.kUnitPreDamagedByBuffer.fDamage = fDamage;
					kArgs.kUnitPreDamagedByBuffer.iTargetID = m_iID;
					if (pkAttacker)
					{
						kArgs.kUnitPreDamagedByBuffer.iAttackerID = pkAttacker->GetID();
					}

					gLevelMain.GetEventMgr()->FireEvent(kArgs);
				}
			}
		}
	}
	if (pkAttacker)
	{
		pkAttacker->TrgOnDamage(eAttackType, fDamage, this, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);
	}
}

void BeUnit::TrgOnDamage(int eAttackType, float& fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID)
{
	{
		TrgOnDamage_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);
	}

	TrgOnDamage_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, iAttackSkillTypeID);

	if (IsHero())
	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (pkItem)
			{
				TrgOnDamage_T(pkItem->GetAllSkill(), (int)pkItem->GetAllSkill().size(), eAttackType, fDamage, pkTarget, iPlayer, iFlag, bCanDead, iAttackSkillTypeID, i, pkItem->GetID());
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_SHANGHAI))
			{
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkBuffer, pkBuffer);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iFlagPtr, &iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);
				kParam.SetParam(BTP_iSkillTypeID, iAttackSkillTypeID);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONDAMAGE, kParam);
			}
		}
	}
}

void BeUnit::TrgOnDead(int eAttackType, float fDamage, BeUnit* pkAttacker, int iPlayer, int iFlag, int iSkillTypeID)
{
	if (m_pkBackData->akLearnSkill && !(IsDividMan() || IsGhost()))
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (!HasFlag(BUF_RELIVE) && pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEADCHUFA))
			{
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				if (pkAttacker)
				{
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
				}

				kParam.SetParam(BTP_pkTarget, this);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(BTE_SKILL_ONDEAD, kParam);
			}
		}
	}

	if (!(IsDividMan() || IsGhost()) && !m_apkNormalSkill.empty())
	{
		for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
		{
			BeSkill* pkSkill = m_apkNormalSkill[i];
			if (!HasFlag(BUF_RELIVE) && pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEADCHUFA))
			{
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				if (pkAttacker)
				{
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
				}

				kParam.SetParam(BTP_pkTarget, this);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(BTE_SKILL_ONDEAD, kParam);
			}
		}
	}

	if (!HasFlag(BUF_RELIVE))
	{
		if (!m_apkBuffer.IsEmpty())
		{
			/////////////////////////////////////////////////////////
			for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
			{
				BeBuffer* pkBuffer = m_apkBuffer[i];
				if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_SIWANG))
				{
					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					if (pkAttacker)
					{
						kParam.SetParam(BTP_pkAttacker, pkAttacker);
					}

					kParam.SetParam(BTP_pkTarget, this);
					kParam.SetParam(BTP_pkBuffer, pkBuffer);
					kParam.SetParam(BTP_pfDamage, &fDamage);
					kParam.SetParam(BTP_iFlag, iFlag);
					kParam.SetParam(BTP_iAttackType, eAttackType);

					gTrgMgr.FireTrigger(BTE_BUFFER_ONDEAD_PRE, kParam);
				}
			}
		}
	}

	if (!HasFlag(BUF_RELIVE))
	{
		TePtParam kParamDeath;
		kParamDeath.SetParam(BTP_pkTrgUnit, this);
		if (pkAttacker)
		{
			kParamDeath.SetParam(BTP_pkAttacker, pkAttacker);
		}
		kParamDeath.SetParam(BTP_iAttackerPlayer, iPlayer);
		kParamDeath.SetParam(BTP_iSkillTypeID, iSkillTypeID);
		kParamDeath.SetParam(BTP_pfDamage, &fDamage);
		kParamDeath.SetParam(BTP_fDamageFace, pkAttacker->GetFace());
		kParamDeath.SetParam(BTP_iFlag, iFlag);

		gTrgMgr.FireTrigger(BTE_UNIT_DEATH, kParamDeath);
		//////////////////////////////////////////////////////////////////////////
		BeTDEventArgs kArgs = BeTDEventArgs();
		kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITDEATH;
		kArgs.kUnitDeath.iAttackFlag = iFlag;
		kArgs.kUnitDeath.iSkillTypeID = iSkillTypeID;
		kArgs.kUnitDeath.iUnitID = m_iID;
		if (pkAttacker)
		{
			kArgs.kUnitDeath.iUnitKillerID = pkAttacker->GetID();
		}
		gLevelMain.GetEventMgr()->FireEvent(kArgs);

		if (pkAttacker)
		{
			pkAttacker->TrgOnKill(eAttackType, fDamage, this, GetPlayer(), iFlag, iSkillTypeID);
		}
		if (!m_apkBuffer.IsEmpty())
		{
			/////////////////////////////////////////////////////////
			for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
			{
				BeBuffer* pkBuffer = m_apkBuffer[i];
				if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_SIWANG))
				{
					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					if (pkAttacker)
					{
						kParam.SetParam(BTP_pkAttacker, pkAttacker);
					}

					kParam.SetParam(BTP_pkTarget, this);
					kParam.SetParam(BTP_pkBuffer, pkBuffer);
					kParam.SetParam(BTP_pfDamage, &fDamage);
					kParam.SetParam(BTP_iFlag, iFlag);
					kParam.SetParam(BTP_iAttackType, eAttackType);

					gTrgMgr.FireTrigger(BTE_BUFFER_ONDEAD, kParam);
					///////////////////////////////////////////////////////////////////
				}
			}
		}
	}

	if (IsHero())
	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (!pkItem)
			{
				continue;
			}
			if (!pkItem->GetValid())
			{
				continue;
			}
			for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
			{
				BeSkill* pkSkill = pkItem->m_akSkill[iSkill];
				if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_DEADCHUFA))
				{
					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					if (pkAttacker)
					{
						kParam.SetParam(BTP_pkAttacker, pkAttacker);
					}
					kParam.SetParam(BTP_pkTarget, this);
					kParam.SetParam(BTP_pkSkill, pkSkill);
					kParam.SetParam(BTP_iItemPos, i + 1);
					kParam.SetParam(BTP_iTriggerItemID, pkItem->GetID());
					kParam.SetParam(BTP_pfDamage, &fDamage);
					kParam.SetParam(BTP_iFlag, iFlag);
					kParam.SetParam(BTP_iAttackType, eAttackType);

					gTrgMgr.FireTrigger(BTE_SKILL_ONDEAD, kParam);
					break;
				}
			}
		}
	}
}


void BeUnit::TrgOnPreAttack(BeUnit* pkTarget, BeAttackingAttr* pkAttackAttr)
{
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_GONGJICHUFA))
			{
				if ((IsDividMan() || IsGhost()))
				{
					continue;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_pkAttackAttr, pkAttackAttr);

				gTrgMgr.FireTrigger(BTE_SKILL_ONPREATTACK, kParam);

				UpdateAttribute(true);
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{
		/////////////////////////////////////////////////////////
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_GONGJI))
			{
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkAttackAttr, pkAttackAttr);
				kParam.SetParam(BTP_pkBuffer, pkBuffer);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONPREATTACK, kParam);
				///////////////////////////////////////////////////////////////////
			}
		}
	}
}

void BeUnit::TrgOnAttack(int iTargetID, BeAttackingAttr* pkAttackAttr)
{
	BeUnit* pkTarget = gUnitMgr.GetUnitByID(iTargetID);

	{
		TrgOnAttack_T(m_pkCurData->apkUISkill, iMaxHeroSkillNum, pkTarget, pkAttackAttr);
	}

	TrgOnAttack_T(m_apkNormalSkill, (int)m_apkNormalSkill.size(), pkTarget, pkAttackAttr);

	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (pkItem && pkItem->GetOrgValid())
			{
				TrgOnAttack_T(pkItem->GetAllSkill(), (int)pkItem->GetAllSkill().size(), pkTarget, pkAttackAttr, i);
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{
		/////////////////////////////////////////////////////////
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_GONGJI))
			{
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkAttackAttr, pkAttackAttr);
				kParam.SetParam(BTP_pkBuffer, pkBuffer);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONATTACK, kParam);
				///////////////////////////////////////////////////////////////////
			}
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_YINSHENTUICHU))
			{
				if (pkBuffer->HasCarryFlag(BUCF_ISINVISIBLE) && HasFlag(BUF_HASINVISIBLE))
				{
					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					kParam.SetParam(BTP_pkTarget, pkTarget);
					kParam.SetParam(BTP_pkAttackAttr, pkAttackAttr);
					kParam.SetParam(BTP_pkBuffer, pkBuffer);

					gTrgMgr.FireTrigger(BTE_BUFFER_INVISIBLEBREAK, kParam);
				}
			}
		}
	}

	if (HasFlag(BUF_HASINVISIBLE))
	{
		ClrFlag(BUF_HASINVISIBLE);
	}
}

void BeUnit::TrgOnSpell(int iSkillTypeID, int iSkillLevel, int iItemID, int iTargetID, float fTargetPosX, float fTargetPosY)
{
	const SkillTable* pkSkillRes = GetResSkill(iSkillTypeID);
	if (pkSkillRes->uiOperateType == SKILL_OPERATETYPE_BEIDONG)
	{
		return;
	}

	if (pkSkillRes->iUIShowPos < 1 || pkSkillRes->iUIShowPos > 4)
	{
		return;
	}

	if (HasSkill())
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && (pkSkill->HasProperty(SKILL_SKILLPROPERTY_ONSPELLFIRE)))
			{
				if ((IsDividMan() || IsGhost()))
				{
					continue;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
				kParam.SetParam(BTP_iSkillLevel, iSkillLevel);
				kParam.SetParam(BTP_iItemID, iItemID);
				kParam.SetParam(BTP_iSpellTargetID, iTargetID);
				kParam.SetParam(BTP_fSpellTargetPosX, fTargetPosX);
				kParam.SetParam(BTP_fSpellTargetPosY, fTargetPosY);

				if (pkSkill->HasProperty(SKILL_SKILLPROPERTY_ONSPELLFIRE))
				{
					gTrgMgr.FireTrigger(BTE_SKILL_ONSPELL, kParam);
				}
			}
		}
	}

	if (!m_apkNormalSkill.empty())
	{
		for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
		{
			BeSkill* pkSkill = m_apkNormalSkill[i];
			if (pkSkill && (pkSkill->HasProperty(SKILL_SKILLPROPERTY_ONSPELLFIRE)))
			{
				if ((IsDividMan() || IsGhost()))
				{
					continue;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
				kParam.SetParam(BTP_iSkillLevel, iSkillLevel);
				kParam.SetParam(BTP_iItemID, iItemID);
				kParam.SetParam(BTP_iSpellTargetID, iTargetID);
				kParam.SetParam(BTP_fSpellTargetPosX, fTargetPosX);
				kParam.SetParam(BTP_fSpellTargetPosY, fTargetPosY);
				if (pkSkill->HasProperty(SKILL_SKILLPROPERTY_ONSPELLFIRE))
				{
					gTrgMgr.FireTrigger(BTE_SKILL_ONSPELL, kParam);
				}
			}
		}
	}

	if (IsHero())
	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (!pkItem || !pkItem->GetOrgValid())
			{
				continue;
			}
			for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
			{
				BeSkill* pkSkill = pkItem->m_akSkill[iSkill];
				if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_ONSPELLFIRE))
				{
					if ((IsDividMan() || IsGhost()))
					{
						continue;
					}

					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					kParam.SetParam(BTP_pkSkill, pkSkill);
					kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
					kParam.SetParam(BTP_iSkillLevel, iSkillLevel);
					kParam.SetParam(BTP_iItemID, iItemID);
					kParam.SetParam(BTP_iItemPos, i + 1);
					kParam.SetParam(BTP_iTriggerItemID, pkItem->GetID());
					kParam.SetParam(BTP_iSpellTargetID, iTargetID);
					kParam.SetParam(BTP_fSpellTargetPosX, fTargetPosX);
					kParam.SetParam(BTP_fSpellTargetPosY, fTargetPosY);

					gTrgMgr.FireTrigger(BTE_SKILL_ONSPELL, kParam);
					break;
				}
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{
		/////////////////////////////////////////////////////////
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_SHIFA))
			{
				BeUnit* pkAttacker = pkBuffer->GetUnit();
				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkAttacker, pkAttacker);
				kParam.SetParam(BTP_pkBuffer, pkBuffer);
				kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
				kParam.SetParam(BTP_iSkillLevel, iSkillLevel);
				kParam.SetParam(BTP_iItemID, iItemID);
				kParam.SetParam(BTP_iItemPos, i + 1);
				kParam.SetParam(BTP_iSpellTargetID, iTargetID);
				kParam.SetParam(BTP_fSpellTargetPosX, fTargetPosX);
				kParam.SetParam(BTP_fSpellTargetPosY, fTargetPosY);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONSPELL, kParam);
				///////////////////////////////////////////////////////////////////
			}
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_YINSHENTUICHU))
			{
				if (pkBuffer->HasCarryFlag(BUCF_ISINVISIBLE) && HasFlag(BUF_HASINVISIBLE))
				{
					BeUnit* pkAttacker = pkBuffer->GetUnit();
					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					kParam.SetParam(BTP_pkAttacker, pkAttacker);
					kParam.SetParam(BTP_pkBuffer, pkBuffer);

					gTrgMgr.FireTrigger(BTE_BUFFER_INVISIBLEBREAK, kParam);
				}
			}
		}
	}

	if (HasFlag(BUF_HASINVISIBLE))
	{
		ClrFlag(BUF_HASINVISIBLE);
	}
}

void BeUnit::TrgOnUpdate(int iDeltaTime)
{
	if (HasFlag(BUF_NEEDUPDATESKILL) && (gTime % 100 == 0))
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			if (m_pkCurData)
			{
				BeSkill* pkSkill = (m_pkCurData->apkUISkill)[i];
				if (pkSkill)
				{
					pkSkill->Update(this);
				}
			}
		}

		if (!m_apkNormalSkill.empty())
		{
			for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
			{
				BeSkill* pkSkill = m_apkNormalSkill[i];
				if (pkSkill)
				{
					pkSkill->Update(this);
				}
			}
		}
	}

	if (HasFlag(BUF_NEEDUPDATEITEMSKILL))
	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (pkItem)
			{
				pkItem->Update(this, iDeltaTime);
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			int iBufferCount = (int)m_apkBuffer.Size();
			if (pkBuffer)
			{
				pkBuffer->Update(this, iDeltaTime);
				if (iBufferCount != m_apkBuffer.Size())
				{
					break;
				}
			}
		}
	}
}

void BeUnit::TrgOnKill(int eAttackType, float fDamage, BeUnit* pkTarget, int iPlayer, int iFlag, int iSkillTypeID)
{
	if (HasSkill())
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = GetSkillByPos(i);
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_KILLCHUFA))
			{
				if ((IsDividMan() || IsGhost()))
				{
					continue;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkAttacker, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(BTE_SKILL_ONKILL, kParam);
			}
		}
	}

	if (m_apkNormalSkill.size())
	{
		for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
		{
			BeSkill* pkSkill = m_apkNormalSkill[i];
			if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_KILLCHUFA))
			{
				if ((IsDividMan() || IsGhost()))
				{
					continue;
				}

				TePtParam kParam;
				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkAttacker, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkSkill, pkSkill);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);
				kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);

				gTrgMgr.FireTrigger(BTE_SKILL_ONKILL, kParam);
			}
		}
	}

	if (IsHero())
	{
		for (int i = 0; i < UNIT_MAX_ITEM; ++i)
		{
			BeItem* pkItem = m_apkItem[i];
			if (!pkItem || !pkItem->GetOrgValid())
			{
				continue;
			}
			for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
			{
				BeSkill* pkSkill = pkItem->m_akSkill[iSkill];
				if (pkSkill && pkSkill->HasProperty(SKILL_SKILLPROPERTY_KILLCHUFA))
				{
					if ((IsDividMan() || IsGhost()))
					{
						continue;
					}

					TePtParam kParam;
					kParam.SetParam(BTP_pkTrgUnit, this);
					kParam.SetParam(BTP_pkAttacker, this);
					kParam.SetParam(BTP_pkTarget, pkTarget);
					kParam.SetParam(BTP_pkSkill, pkSkill);
					kParam.SetParam(BTP_iItemPos, i + 1);
					kParam.SetParam(BTP_iTriggerItemID, pkItem->GetID());
					kParam.SetParam(BTP_pfDamage, &fDamage);
					kParam.SetParam(BTP_iFlag, iFlag);
					kParam.SetParam(BTP_iAttackType, eAttackType);

					gTrgMgr.FireTrigger(BTE_SKILL_ONKILL, kParam);
				}
			}
		}
	}

	if (!m_apkBuffer.IsEmpty())
	{
		/////////////////////////////////////////////////////////
		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer->GetHasDel() && pkBuffer->HasProperty(BUFFER_PROPERTY_JISHA))
			{

				TePtParam kParam;

				kParam.SetParam(BTP_pkTrgUnit, this);
				kParam.SetParam(BTP_pkAttacker, this);
				kParam.SetParam(BTP_pkTarget, pkTarget);
				kParam.SetParam(BTP_pkBuffer, pkBuffer);
				kParam.SetParam(BTP_pfDamage, &fDamage);
				kParam.SetParam(BTP_iFlag, iFlag);
				kParam.SetParam(BTP_iAttackType, eAttackType);

				gTrgMgr.FireTrigger(BTE_BUFFER_ONKILL, kParam);
			}
		}
	}
}

int BeUnit::AIGetEP(const BeUnit* pkTarget) const
{
	if (!m_pkAI)
	{
		return 0;
	}
	return m_pkAI->CalculateEnmityPoint(pkTarget);
}

void BeUnit::SetCommonCDLastUseTime(int iTypeID, int iTime)
{
	m_akCommonCD[iTypeID] = iTime;
}

int BeUnit::GetCommonCDLastUseTime(int iTypeID) const
{
	if (gData.IsWTF())
	{
		return 0;
	}
	std::map<int, int>::const_iterator it = m_akCommonCD.find(iTypeID);
	if (it != m_akCommonCD.end())
	{
		return it->second;
	}
	return 0;
}

bool BeUnit::CommonCDComplete(int iTypeID, int iCDTime) const
{
	int iLastUseTime = GetCommonCDLastUseTime(iTypeID);
	if (iLastUseTime == 0)
	{
		return true;
	}

	if ((int)gTime < iLastUseTime + iCDTime)
	{
		return false;
	}

	return true;
}

void  BeUnit::ClearAllCommonCD(void)
{
	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		if (m_apkItem[i])
		{
			m_apkItem[i]->SetLastUseTime(0);
			break;
		}
	}
	m_akCommonCD.clear();
}

std::map<int, int>& BeUnit::GetAllCD(void)
{
	return m_akCommonCD;
}

void BeUnit::SetTowerLastHasUnitTime(int iTime)
{
	if (!m_pkAI)
	{
		return;
	}
	m_pkAI->SetTowerTimeLastHasUnit(iTime);
}

bool BeUnit::PickMapItem(BeMapItem* pkMapItem)
{
	if (!pkMapItem)
	{
		return false;
	}

	if (HasUnitCarryFlag(BUCF_ONLYCANUSEITEM))
	{
		return false;
	}

	int iItemTypeID = pkMapItem->GetTypeID();
	int iSkillTypeID = 0;
	const ItemTable* pkItemRes = gMain.GetResItem(iItemTypeID);
	if (pkItemRes)
	{
		for (int i = 0; i < ITEM_MAX_SKILL; ++i)
		{
			int iID = pkItemRes->iItemSkill[i];
			const SkillTable* pkSkillRes = gMain.GetResSkill(iID);
			if (pkSkillRes)
			{
				if (pkSkillRes->uiOperateType != SKILL_OPERATETYPE_BEIDONG)
				{
					iSkillTypeID = iID;
					break;
				}
			}
		}
	}

	gMain.InitItemEventTriggerT(iItemTypeID);
	gMain.InitItemEventTriggerZhanChang(iItemTypeID);
	TePtParam kParam;
	kParam.SetParam(BTP_pkTrgUnit, this);
	kParam.SetParam(BTP_iItemTypeID, iItemTypeID);
	kParam.SetParam(BTP_iSkillTypeID, iSkillTypeID);
	kParam.SetParam(BTP_pkMapItem, pkMapItem);

	gTrgMgr.FireTrigger(BTE_UNIT_PICKUP_ITEM, kParam);

	if (pkItemRes)
	{
		if (pkItemRes->uiItemProperty & ITEM_ITEMPROPERTY_PICKUSE)
		{
			BeSkill* pkSkill = GetSkill(iSkillTypeID);
			if (!pkSkill)
			{
				AddSkill(iSkillTypeID);
				gMain.InitItemEventTriggerT(iSkillTypeID);
				gMain.InitItemEventTriggerZhanChang(iItemTypeID);
			}

			BeCommand	kComm(BCT_SPELL, GetID(), 0.0f, 0.0f, iSkillTypeID);
			GiveCommand(kComm);
		}
	}

	BeItemData kData = pkMapItem->GetItemData();
	if (pkItemRes && (pkItemRes->uiItemProperty & ITEM_ITEMPROPERTY_PICKUSE))
	{
		gMapItemMgr.DelMapItem(pkMapItem->GetID());

		BeLevelMain* pkTriggerMain = (BeLevelMain*)gMain.GetTaskHandle();
		EsAssert(pkTriggerMain);
		BeTDEventArgs kArgs = BeTDEventArgs();
		kArgs.iEventType = BeTDEventArgs::BeTDEVENT_UNITUSEITEM;
		kArgs.kUnitUseItem.iPlayer = GetPlayer();
		kArgs.kUnitUseItem.iItemTypeID = iItemTypeID;
		kArgs.kUnitUseItem.iTrgSkillTypeID = iSkillTypeID;
		pkTriggerMain->GetEventMgr()->FireEvent(kArgs);

		return BER_EXE_END;
	}
	else
	{
		gMapItemMgr.DelMapItem(pkMapItem->GetID());
	}

	return true;
}

void BeUnit::GuanQiaAddExp(int iExp)
{
	SetCurExp(GetCurExp() + iExp);
}

void BeUnit::AddIgnoreUnit(int iUnitID, int iTime)
{
	if (!m_pkAI)
	{
		return;
	}
	m_pkAI->AddIgnoreUnit(iUnitID, iTime);
}

bool BeUnit::IsIgnoreUnit(int iUnitID) const
{
	if (!m_pkAI)
	{
		return false;
	}
	return m_pkAI->IsIgnoreUnit(iUnitID);
}

void BeUnit::SetCheatAttr(void)
{
	BeBuffer* pkBuffer = this->AddBufferBegin('BG02', -1);
	if (pkBuffer)
	{
		pkBuffer->SetNormalAttr(NAT_ABS_MAX_HP, this->GetUD_Float(UDK_CHEAT_MAXHP), true);
		pkBuffer->SetNormalAttr(NAT_ABS_DAMAGE, this->GetUD_Float(UDK_CHEAT_ATK), true);
		pkBuffer->SetNormalAttr(NAT_ABS_MOVE_SPEED, this->GetUD_Float(UDK_CHEAT_MOVESPEED), true);
		pkBuffer->SetNormalAttr(NAT_ABS_ARMOR, this->GetUD_Float(UDK_CHEAT_ARMOR), true);
		pkBuffer->SetNormalAttr(NAT_ABS_MAGIC, this->GetUD_Float(UDK_CHEAT_MAGIC), true);

		pkBuffer->SetRemoveTime(-1);
		pkBuffer->SetNeedUpdate(true);
	}

	UpdateAttribute(true);
}

int	 BeUnit::AIUnitRandSpell(BeUnit* pkTarget)
{
	if (!pkTarget)
	{
		return 0;
	}

	if (HasFlag(BUF_ISPERSISTSKILL))
	{
		return 0;
	}

	std::vector<int> aiSkillPos;
	for (int iPos = 0; iPos < MAX_ACTIVE_SKILLS; ++iPos)
	{
		aiSkillPos.push_back(iPos);
	}
	if (GetTypeID() == 'UT15')
	{
		aiSkillPos.push_back(4);
	}

	return 0;
}

void BeUnit::SetAllLiveTime(int iAllTime)
{
	SetUnitAllLiveTime(iAllTime);
	SetUnitCurLiveTime(iAllTime);
	SetUnitCreateTime(gTime);
}

void BeUnit::AutoSpell(UnitAutoSpellType eType, BeUnit* pkEnemy)
{
	if (HasAutoSpellFlag(eType))
	{
		for (int i = 0; i < iMaxHeroSkillNum; ++i)
		{
			BeSkill* pkSkill = (m_pkCurData->apkUISkill)[i];
			if (!pkSkill)
			{
				continue;
			}

			if (pkSkill->HasAutoSpellFlag(eType) && UnitCanUseSkill(pkSkill->GetTypeID(), NULL, true))
			{
				return;
				break;
			}
		}

		for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
		{
			BeSkill* pkSkill = m_apkNormalSkill[i];
			if (!pkSkill)
			{
				continue;
			}
			if (pkSkill->HasAutoSpellFlag(eType) && UnitCanUseSkill(pkSkill->GetTypeID(), NULL, true))
			{
				return;
				break;
			}
		}
	}
}

void BeUnit::AddNewShield(int iBufferID, float fShield)
{
	auto	IterShild = m_akShield.find(iBufferID);
	if (IterShild != m_akShield.end())
	{
		float	fDeltaShild = fShield - IterShild->second;
		m_fTotalShield += fDeltaShild;
	}
	else
	{
		m_fTotalShield += fShield;
	}

	SetShareUnitChangeFlag(BSUDCF_SHELD);
	m_akShield[iBufferID] = fShield;
}

float BeUnit::GetShieldByBuffer(int iBuffer)
{
	std::map<int, float>::iterator iter = m_akShield.find(iBuffer);
	if (iter != m_akShield.end())
	{
		return iter->second;
	}

	return 0.0f;
}

void		BeUnit::CheckDeathRecord(void)
{
	std::map<int, std::vector<BeDeathRecordData>>::iterator itr = m_akDeathRecord.begin();
	while (itr != m_akDeathRecord.end())
	{
		std::vector<BeDeathRecordData>& rkVecData = itr->second;
		std::vector<BeDeathRecordData>::iterator itrVec = rkVecData.begin();
		while (itrVec != rkVecData.end())
		{
			BeDeathRecordData& rkData = *itrVec;
			if (gTime - rkData.iHitTime > DEATH_RECORD_TIME)
			{
				itrVec = rkVecData.erase(itrVec);
			}
			else
			{
				itrVec++;
			}
		}

		if (rkVecData.empty())
		{
			m_akDeathRecord.erase(itr++);
		}
		else
		{
			itr++;
		}
	}
}

void		BeUnit::CalFinalDeathRecord(int iKiller)
{
	std::map<int, float> akTempMap;

	SetDeathRecordLastTime(gTime);
	m_fDeathRecordTotalDamage = 0.0f;

	std::map<int, std::vector<BeDeathRecordData>>::iterator itr = m_akDeathRecord.begin();
	for (; itr != m_akDeathRecord.end(); itr++)
	{
		int iID = itr->first;
		std::vector<BeDeathRecordData>& rkVec = itr->second;
		for (int i = 0; i < (int)rkVec.size(); i++)
		{
			akTempMap[iID] += rkVec[i].fDamage;
			m_fDeathRecordTotalDamage += rkVec[i].fDamage;
			if (rkVec[i].iHitTime < m_iDeathRecordLastTime)
			{
				SetDeathRecordLastTime(rkVec[i].iHitTime);
			}
		}
	}

	SetDeathRecordLastTime(gTime - m_iDeathRecordLastTime);

	float fMaxDamage[2] = { 0 };
	int iMaxID[2] = { 0 };
	for (int i = 0; i < 2; i++)
	{
		if (akTempMap.empty())
		{
			break;
		}

		std::map<int, float>::iterator itr = akTempMap.begin();
		for (; itr != akTempMap.end(); itr++)
		{
			if (itr->first == iKiller)
			{
				continue;
			}

			if (itr->second > fMaxDamage[i])
			{
				fMaxDamage[i] = itr->second;
				iMaxID[i] = itr->first;
			}
		}

		itr = akTempMap.find(iMaxID[i]);
		if (itr != akTempMap.end())
		{
			akTempMap.erase(itr);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_akFinalDeathRecord[i].iUnitID = 0;
		m_akFinalDeathRecord[i].kMapDamage.clear();
	}
	itr = m_akDeathRecord.begin();
	for (; itr != m_akDeathRecord.end(); itr++)
	{
		int iID = itr->first;

		if (iID == iKiller)
		{
			m_akFinalDeathRecord[0].iUnitID = iID;
			std::vector<BeDeathRecordData>& rkVec = itr->second;
			for (int i = 0; i < (int)rkVec.size(); i++)
			{
				BeDeathRecordData& rkData = rkVec[i];
				m_akFinalDeathRecord[0].kMapDamage[rkData.iSkillTypeID] += rkData.fDamage;
				//m_akFinalDeathRecord[0].iUnitID = iUnitID;
			}
		}
		else if (iID == iMaxID[0])
		{
			m_akFinalDeathRecord[1].iUnitID = iID;
			std::vector<BeDeathRecordData>& rkVec = itr->second;
			for (int i = 0; i < (int)rkVec.size(); i++)
			{
				BeDeathRecordData& rkData = rkVec[i];
				m_akFinalDeathRecord[1].kMapDamage[rkData.iSkillTypeID] += rkData.fDamage;
			}
		}
		else if (iID == iMaxID[1])
		{
			m_akFinalDeathRecord[2].iUnitID = iID;
			std::vector<BeDeathRecordData>& rkVec = itr->second;
			for (int i = 0; i < (int)rkVec.size(); i++)
			{
				BeDeathRecordData& rkData = rkVec[i];
				m_akFinalDeathRecord[2].kMapDamage[rkData.iSkillTypeID] += rkData.fDamage;
			}
		}
	}

	m_akDeathRecord.clear();
}

float BeUnit::GetExtraBossDamageTotal(int iBossID)
{
	float fResult = 0.0f;
	for (int i = 0; i < (int)m_apkCarry.size(); i++)
	{
		BeCarry* pkCarry = m_apkCarry[i];
		if (pkCarry && pkCarry->GetBossExtraDamage(iBossID) != 0)
		{
			fResult += pkCarry->GetBossExtraDamage(iBossID);
		}
	}

	return fResult;
}

void BeUnit::SetChangeScale(int iBufferID, float fChangeScale, int iChangeTime /*= 0*/)
{
	BufChangeScaleData kData;
	kData.fScale = fChangeScale;
	kData.iAllTime = iChangeTime;
	kData.iCurTime = iChangeTime;

	//	m_kMapChangeScale[iBufferID] = kData;
}

void BeUnit::SetColorWithBuf(int iBufferID, unsigned int dwColor)
{

}

void BeUnit::UpdateTransrate(int iDeltaTime)
{
	if (HasUnitCarryFlag(BUCF_ISINVISIBLE))
	{
		m_eActorTransType = UATT_WHOLE_TRANS;
		m_bNeedTrans = true;
	}
	else
	{
		m_eActorTransType = UATT_NONE_TRANS;
	}

	if (m_bNeedTrans)
	{
		switch (m_eActorTransType)
		{
		case UATT_NONE_TRANS:
		{
			if (m_iAddTransTime >= m_iTransTime)
			{
				ClrFlag(BUF_HASINVISIBLE);
				m_iAddTransTime = 0;
				m_iTransTime = 0;
			}
			else
			{
				m_iAddTransTime += iDeltaTime;
			}

			break;
		}
		case UATT_WHOLE_TRANS:
		{
			if (HasFlag(BUF_HASINVISIBLE))
			{
				break;
			}
			if (m_iAddTransTime >= m_iTransTime)
			{
				SetFlag(BUF_HASINVISIBLE);
				//m_iTransTime = 0;
				m_iAddTransTime = 0;
			}
			else
			{
				m_iAddTransTime += iDeltaTime;
			}
			break;
		}
		default:
		{
			ClrFlag(BUF_HASINVISIBLE);
			m_iTransTime = 0;
			m_iAddTransTime = 0;
			break;
		}
		}
	}
}

void BeUnit::SetUnitvisible(bool bVisible)
{
	if (!bVisible)
	{
		m_eActorTransType = UATT_WHOLE_TRANS;
	}
	else
	{
		m_eActorTransType = UATT_NONE_TRANS;
	}
}

void	BeUnit::AddAttach(int pcMdlFile, int iRemoveTime, int iAttackerID, bool bChangeFace)
{
	BeShareAttachData kData;
	kData.iRemoveTime = gTime + iRemoveTime;
	kData.iUnitLogicID = GetID();
	kData.iModleName = pcMdlFile;
	kData.bChangeFace = bChangeFace;
	kData.iAttackerID = iAttackerID;

	gMain.AddAttachData(kData);
}

bool BeUnit::DelAttachByName(int iModelID)
{
	BeShareAttachData kData;
	kData.iRemoveTime = gTime;
	kData.iUnitLogicID = GetID();
	kData.iModleName = iModelID;
	kData.bRemove = true;

	gMain.AddAttachData(kData);

	return true;
}

void BeUnit::AddControlSummonUnitID(int iUnitID)
{
	//	m_akControlSummonID.PushBack(iUnitID);
}

void BeUnit::SafeDelBuf(BeBuffer* pkBuffer, bool bNeedRecordChange)
{
	if (!pkBuffer)
	{
		return;
	}

	int iID = pkBuffer->GetID();
	BeBuffer::DEL(pkBuffer);
}

void BeUnit::ClrAllPureData()
{
	// 
	for (int i = 0; i < MAX_ACTIVEPLAYERS; i++)
	{
		m_abEverInPlayerVision[i] = m_abInPlayerVision[i];
	}

	m_kShareSelfDataLast = m_kShareSelfDataCur;
	m_bNewUnit[0] = false;
	m_bNewUnit[1] = false;
	m_iShareUnitDataChangeFlag = 0;

	m_iPathFindSucessTime = 0;

	for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
	{
		BeBuffer* pkBuffer = m_apkBuffer[i];
		if (!pkBuffer)
		{
			continue;
		}
	}

	for (int i = 0; i < UNIT_MAX_ITEM; i++)
	{
		BeItem* pkItem = m_apkItem[i];
		if (!pkItem)
		{
			continue;
		}
	}
}

#define CLIENTNONEEDFLAG (BUF_IGNOREDOODADOBS|BUF_NOOBSTACLE|BUF_IGNOREUNITOBS|BUF_IGNOREFIXEDOBS|BUF_HASHALOSKILL|BUF_OBSTACLESET|BUF_MOVING|BUF_ISBLOCKED |BUF_ISPERSISTSKILL)

void BeUnit::SetFlag(int iFlag, bool bNeedRecordChange)
{
	int iTempFlag = iFlag;
	iTempFlag &= ~CLIENTNONEEDFLAG;
	if (iTempFlag && !HasFlag(iTempFlag))
	{
		SetShareUnitChangeFlag(BSUDCF_FLAG);
	}
	//	}
	m_iFlag |= iFlag;
}

void BeUnit::ClrFlag(int iFlag, bool bNeedRecordChange)
{
	if (/*gMain.IsServerMode() && */bNeedRecordChange)
	{
		int iTempFlag = iFlag;
		iTempFlag &= ~CLIENTNONEEDFLAG;
		if (iTempFlag && (m_iFlag & iTempFlag) != 0)
		{
			SetShareUnitChangeFlag(BSUDCF_FLAG);
		}
	}
	m_iFlag &= ~iFlag;
}

void BeUnit::UpdateLiveTime(int iDeltaTime)
{
	if (GetUnitAllLiveTime() != 0 && GetUnitCurLiveTime() > 0)
	{
		if (!HasFlag(BUF_ISPERSIST))
		{
			DecUnitCurTime(iDeltaTime);
		}

		if (GetUnitCreateTime() != 0 && ((int)gTime - (int)GetUnitCreateTime() > GetUnitAllLiveTime()))
		{
			DecUnitCurTime(GetUnitCurLiveTime());
		}
	}
}

void BeUnit::UpdateHP(int iDeltaTime)
{
	if (IsDead())
	{
		return;
	}
	if (gTime % 100 != 0)
	{
		return;
	}
	if (!HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		if (m_pkCurData->fRegenHP && m_pkCurData->fHP < m_pkCurData->fMaxHP)
		{
			SetHP(GetHP() + (m_pkCurData->fRegenHP * 100) / 1000.0f, false);
		}
	}
}

void BeUnit::UpdateMP(int iDeltaTime)
{
	if (IsDead())
	{
		return;
	}
	if (gTime % 100 != 0)
	{
		return;
	}
	if (HasProperty(UNIT_PROPERTY_POWERBAR) || HasProperty(UNIT_PROPERTY_HUDUNBAR) || HasProperty(UNIT_PROPERTY_NUQIBAR) || HasProperty(UNIT_PROPERTY_BULLETBAR) || HasProperty(UNIT_PROPERTY_NOBAR))
	{
		return;
	}
	if (!HasUnitCarryFlag(BUCF_NO_REVERTHPMP))
	{
		if (m_pkCurData->fRegenMP && m_pkCurData->fMP < m_pkCurData->fMaxMP)
		{
			SetMP(GetMP() + (m_pkCurData->fRegenMP * 100) / 1000.0f, false);
			if (m_pkCurData->fMP > m_pkCurData->fMaxMP)
			{
				SetMP(m_pkCurData->fMaxMP, false);
			}
		}
	}
}

void BeUnit::UpdateBuffer(int iDeltaTime)
{
	if (!m_apkBuffer.IsEmpty())
	{
		bool bNeedUpdate = false;
		bool bUpdateNormal = false;

		int iDelNum = 0;

		int iHasDelNum = 0;

		int iReserveNum = 0;

		for (int i = 0; i < (int)m_apkBuffer.Size(); ++i)
		{
			BeBuffer* pkBuffer = m_apkBuffer[i];
			if (!pkBuffer)
			{
				continue;
			}

			if (pkBuffer->GetHasDel())
			{
				bNeedUpdate = true;
				if (pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR))
				{
					bUpdateNormal = true;
				}

				bool bNeedRecordChange = false;
				{
					bNeedRecordChange = true;
				}
				SafeDelBuf(pkBuffer, bNeedRecordChange);

				m_apkBuffer.EraseIndexSwap(i);
				i--;
			}
			else if (pkBuffer->GetRemoveTime() != -1 && pkBuffer->GetRemoveTime() <= gTime)
			{
				bNeedUpdate = true;
				if (pkBuffer->HasFlag(BCF_HAS_NORMAL_ATTR))
				{
					bUpdateNormal = true;
				}

				bool bNeedRecordChange = false;
				OnDelBuffer(pkBuffer);
				SafeDelBuf(pkBuffer, bNeedRecordChange);

				m_apkBuffer.EraseIndexSwap(i);
				i--;
			}
			else if (pkBuffer->GetNeedUpdate())
			{
				bNeedUpdate = true;
				bUpdateNormal = true;
				pkBuffer->SetNeedUpdate(false);
			}
		}

		if (bNeedUpdate)
		{
			UpdateAttribute(bUpdateNormal);
		}
	}
}


const SkillTable* BeUnit::GetResSkill(int iTypeID) const
{
	const SkillTable* pkSkill = gMain.GetResSkill(iTypeID, this);

	return pkSkill;
}


void BeUnit::CopyAttribute(BeUnit* pkUnit)
{
	SetPlayer(pkUnit->GetPlayer());
	SetLevel(pkUnit->GetLevel());
	SetCurExp(pkUnit->GetCurExp());

	m_pkBackData->fSkillDamagePer = pkUnit->GetSkillDamagePer();
	OnPropertyUpdate(GetLevel());

	m_pkCurData->fBaseMaxHP = pkUnit->GetBaseMaxHP();
	m_pkCurData->fBaseMaxMP = pkUnit->GetBaseMaxMP();
	SetBaseArmor(pkUnit->GetBaseArmor());
	SetMaxHP(pkUnit->GetMaxHP());
	m_pkCurData->fMaxMP = pkUnit->GetMaxMP();
	m_fPreMaxHP = m_pkCurData->fMaxHP;
	m_fPreMaxMP = m_pkCurData->fMaxMP;


	SetHP(pkUnit->GetHP());
	SetMP(pkUnit->GetMP());
	SetFace(pkUnit->GetFace());
	SetTarFace(pkUnit->GetTarFace());
	AISetType(pkUnit->AIGetType());
	SetUsedSkillPoint(pkUnit->GetUsedSkillPoint());

	const BeUnitMorphType& rkMType = pkUnit->GetMorphUnitType();
	if (rkMType == BUMT_CHANGE_MODELSKILL)
	{
	}
	if (rkMType == BUMT_CHANGE_MODELSKILL)
	{
		m_pkCurData->iUnitProperty = pkUnit->GetProperty();
	}
}



void BeUnit::SetHP(float fHP, bool bChange)
{
	float fLastHP = m_pkCurData->fHP;

	if (bChange)
	{
		float	fSkillAddHp = 0.0f;
		float	fPerSkillAdd = m_pkCurData->fTreatment;
		if (fHP > fLastHP && fPerSkillAdd != 0.0f)
		{
			float	fDeltaHp = fHP - fLastHP;
			fSkillAddHp = fDeltaHp * fPerSkillAdd;

			fHP += fSkillAddHp;
		}
	}
	if (fHP > m_pkCurData->fMaxHP)
	{
		if (m_pkCurData->fMaxHP <= 0.0f)
		{
			SetMaxHP(fHP);
		}
		m_pkCurData->fHP = m_pkCurData->fMaxHP;
	}
	else if (fHP <= 0.0f)
	{
		m_pkCurData->fHP = 0.0f;
	}
	else
	{
		m_pkCurData->fHP = fHP;
	}

	if (bChange && fLastHP != m_pkCurData->fHP)
	{
		SetShareUnitChangeFlag(BSUDCF_CURHP);
	}
}

void BeUnit::UpdateHalloSkill(int iDeltaTime)
{
	if (!IsDead())
	{
		if (HasFlag(BUF_HASHALOSKILL) && gTime % 1000 == 0)
		{
			for (int i = 0; i < iMaxHeroSkillNum; ++i)
			{
				BeSkill* pkSkill = (m_pkCurData->apkUISkill)[i];
				if (!pkSkill)
				{
					continue;
				}

				if (pkSkill->IsHaloSkill())
				{
					ParseHaloSkill(pkSkill);
				}
			}

			for (int i = 0; i < (int)m_apkNormalSkill.size(); ++i)
			{
				BeSkill* pkSkill = m_apkNormalSkill[i];
				if (!pkSkill)
				{
					continue;
				}
				if (pkSkill->IsHaloSkill())
				{
					ParseHaloSkill(pkSkill);
				}
			}

			{
				for (int i = 0; i < UNIT_MAX_ITEM; ++i)
				{
					BeItem* pkItem = m_apkItem[i];
					if (pkItem && pkItem->GetOrgValid())
					{
						if (pkItem->GetOrgValid())
						{
							for (int iSkill = 0; iSkill < (int)pkItem->m_akSkill.size(); ++iSkill)
							{
								BeSkill* pkSkill = pkItem->m_akSkill[iSkill];
								if (pkSkill && pkSkill->IsHaloSkill())
								{
									ParseHaloSkill(pkSkill, pkItem->GetID());
								}
							}
						}

					}
				}
			}
		}
	}
}


bool BeUnit::IsDamageTime(int iDeltaTime)
{
	int iRealTime = GetRealDamagePt();
	int iTime = m_pkCurData->iActionCurTime + iDeltaTime;

	return iTime > iRealTime;
}

void	BeUnit::SellItem(int iPos)
{
	BeItem* pkItem = GetItemByPos(iPos);
	if (!pkItem)
	{
		return;
	}

	int	iPrice = pkItem->GetPriceOfBuy() * 7 / 10;

	if (pkItem->IsCanSplice())
	{
		if (pkItem->GetPileCount() > 1)
		{
			pkItem->SetPileCount(pkItem->GetPileCount() - 1);
			pkItem->SetUseCount(pkItem->GetOrgUseCount());
		}
		else
		{
			DelItemByPos(pkItem->GetPackagePos());
		}
	}
	else
	{
		DelItemByPos(iPos);
	}

	int iPlayer = GetPlayer();
	gMain.AddPlayerGold(iPlayer, iPrice, false);

	UpdateItemPassiveSkill();
}

void	BeUnit::BuyItem(int iItemTypeID)
{
	const ItemTable* pkItemRes = gMain.GetResItem(iItemTypeID);
	if (!pkItemRes)
	{
		return;
	}
	int iPlayer = GetPlayer();
	int iHasMoney = gMain.GetPlayerGold(iPlayer);
	int iPrice = pkItemRes->iItemPrice;
	int	iHasItemAllPrice = 0;

	if (pkItemRes->iPileCount > 1)
	{
		BeItem* pkHasItem = GetItemByTypeID(iItemTypeID);
		if (pkHasItem)
		{
			if (iHasMoney < iPrice)
			{
				return;
			}

			if (pkHasItem->GetPileCount() > pkItemRes->iPileCount)
			{
				return;
			}
			pkHasItem->SetPileCount(pkHasItem->GetPileCount() + 1);

			gMain.AddPlayerGold(iPlayer, -iPrice);

			return;
		}
	}

	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	{
		if (m_apkItem[i])
		{
			m_apkItem[i]->SetComposeUse(false);
		}
	}
	std::vector<int>	kHasItemVec;
	const	ItemComposeTable* pkComposeRes = gMain.GetResItemCompose(iItemTypeID);
	if (pkComposeRes)
	{
		int		iNeedItem1 = pkComposeRes->uiNeedItemTypeID1;
		BeItem* pkFindItem = GetComposeItemByTypeID(iNeedItem1);
		if (pkFindItem)
		{
			kHasItemVec.push_back(pkFindItem->GetPackagePos());
			iHasItemAllPrice += pkFindItem->GetPriceOfBuy();
			pkFindItem->SetComposeUse(true);
		}
		else
		{
			const	ItemComposeTable* pkSecondComposeRes = gMain.GetResItemCompose(iNeedItem1);
			if (pkSecondComposeRes)
			{
				int		iSecondNeed1 = pkSecondComposeRes->uiNeedItemTypeID1;
				BeItem* pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed1);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
				int		iSecondNeed2 = pkSecondComposeRes->uiNeedItemTypeID2;
				pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed2);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
				int		iSecondNeed3 = pkSecondComposeRes->uiNeedItemTypeID3;
				pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed3);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
			}
		}

		int		iNeedItem2 = pkComposeRes->uiNeedItemTypeID2;
		pkFindItem = GetComposeItemByTypeID(iNeedItem2);
		if (pkFindItem)
		{
			kHasItemVec.push_back(pkFindItem->GetPackagePos());
			iHasItemAllPrice += pkFindItem->GetPriceOfBuy();
			pkFindItem->SetComposeUse(true);
		}
		else
		{
			const	ItemComposeTable* pkSecondComposeRes = gMain.GetResItemCompose(iNeedItem2);
			if (pkSecondComposeRes)
			{
				int		iSecondNeed1 = pkSecondComposeRes->uiNeedItemTypeID1;
				BeItem* pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed1);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
				int		iSecondNeed2 = pkSecondComposeRes->uiNeedItemTypeID2;
				pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed2);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
				int		iSecondNeed3 = pkSecondComposeRes->uiNeedItemTypeID3;
				pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed3);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
			}
		}

		int		iNeedItem3 = pkComposeRes->uiNeedItemTypeID3;
		pkFindItem = GetComposeItemByTypeID(iNeedItem3);
		if (pkFindItem)
		{
			kHasItemVec.push_back(pkFindItem->GetPackagePos());
			iHasItemAllPrice += pkFindItem->GetPriceOfBuy();
			pkFindItem->SetComposeUse(true);
		}
		else
		{
			const	ItemComposeTable* pkSecondComposeRes = gMain.GetResItemCompose(iNeedItem3);
			if (pkSecondComposeRes)
			{
				int		iSecondNeed1 = pkSecondComposeRes->uiNeedItemTypeID1;
				BeItem* pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed1);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
				int		iSecondNeed2 = pkSecondComposeRes->uiNeedItemTypeID2;
				pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed2);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
				int		iSecondNeed3 = pkSecondComposeRes->uiNeedItemTypeID3;
				pkSecondFindItem = GetComposeItemByTypeID(iSecondNeed3);
				if (pkSecondFindItem)
				{
					kHasItemVec.push_back(pkSecondFindItem->GetPackagePos());
					iHasItemAllPrice += pkSecondFindItem->GetPriceOfBuy();
					pkSecondFindItem->SetComposeUse(true);
				}
			}
		}
	}

	iPrice -= iHasItemAllPrice;
	if (iHasMoney < iPrice)
	{
		return;
	}
	int iOrgItemData = 0;
	for (int iDelItemIndex = 0; iDelItemIndex < kHasItemVec.size(); iDelItemIndex++)
	{
		if (GetItemByPos(kHasItemVec[iDelItemIndex])->GetTypeID() == 'IP06') {
			iOrgItemData = GetItemByPos(kHasItemVec[iDelItemIndex])->GetData();
		}

		DelItemByPos(kHasItemVec[iDelItemIndex]);
	}

	bool	bHasPos = false;
	for (int iItemIndex = 0; iItemIndex < iMaxBuyItem; iItemIndex++)
	{
		if (!m_apkItem[iItemIndex])
		{
			bHasPos = true;
			break;
		}
	}
	if (!bHasPos)
	{
		return;
	}
	AddItem(iItemTypeID, -1, 0, iOrgItemData);
	gMain.AddPlayerGold(iPlayer, -iPrice);

	UpdateItemPassiveSkill();

	SetTabInfoFlag(BTCF_BIGSKILL);
}

void	BeUnit::UpdateItemPassiveSkill()
{
	ClearItemPassiveSkill();

	for (int iIndex = 0; iIndex < iMaxBuyItem; ++iIndex)
	{
		BeItem* pkItem = m_apkItem[iIndex];
		if (pkItem)
		{
			pkItem->ClearForbidSkill();
		}
	}

	for (int iIndex = 0; iIndex < iMaxBuyItem; ++iIndex)
	{
		BeItem* pkItem = m_apkItem[iIndex];
		if (pkItem)
		{
			if (gMain.IsPveMap())
			{
				const	ZhanChangItemTable* pkItemRes = pkItem->GetZhanChangResPtr();
				if (pkItemRes)
				{
					for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
					{
						int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
						const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
						if (pkItemPassiveRes)
						{
							for (int iPassiveSkillIndex = 0; iPassiveSkillIndex < 10; ++iPassiveSkillIndex)
							{
								int		iPassiveSkillTypeID = pkItemPassiveRes->iKillSkill[iPassiveSkillIndex];
								if (iPassiveSkillTypeID > 'S000')
								{
									AddItemPassiveSkill(iPassiveSkillTypeID);
								}
							}
						}
					}
				}
			}
			else
			{
				const	ItemTable* pkItemRes = pkItem->GetResPtr();
				if (pkItemRes)
				{
					for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
					{
						int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
						const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
						if (pkItemPassiveRes)
						{
							for (int iPassiveSkillIndex = 0; iPassiveSkillIndex < 10; ++iPassiveSkillIndex)
							{
								int		iPassiveSkillTypeID = pkItemPassiveRes->iKillSkill[iPassiveSkillIndex];
								if (iPassiveSkillTypeID > 'S000')
								{
									AddItemPassiveSkill(iPassiveSkillTypeID);
								}
							}
						}
					}
				}
			}
		}
	}

	for (int iIndex = 0; iIndex < iMaxBuyItem; ++iIndex)
	{
		BeItem* pkItem = m_apkItem[iIndex];
		if (pkItem)
		{
			if (gMain.IsPveMap())
			{
				const	ZhanChangItemTable* pkItemRes = pkItem->GetZhanChangResPtr();
				if (pkItemRes)
				{
					for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
					{
						int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
						if (!CanAddItemPassiveSkill(iSkillTypeID))
						{
							pkItem->AddForbidSkill(iSkillTypeID);

							continue;
						}
						const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
						if (pkItemPassiveRes)
						{
							AddItemPassiveSkill(iSkillTypeID);
						}
					}
				}
			}
			else
			{
				const	ItemTable* pkItemRes = pkItem->GetResPtr();
				if (pkItemRes)
				{
					for (int iItemSkillIndex = 0; iItemSkillIndex < 6; ++iItemSkillIndex)
					{
						int		iSkillTypeID = pkItemRes->iItemSkill[iItemSkillIndex];
						if (!CanAddItemPassiveSkill(iSkillTypeID))
						{
							pkItem->AddForbidSkill(iSkillTypeID);

							continue;
						}
						const	ItemPassiveTable* pkItemPassiveRes = ItemPassiveTableMgr::Get()->GetItemPassiveTable(iSkillTypeID);
						if (pkItemPassiveRes)
						{
							AddItemPassiveSkill(iSkillTypeID);
						}
					}
				}
			}

		}
	}

	for (int i = 0; i < UNIT_MAX_ITEM; ++i)
	{
		BeItem* pkOrgItem = GetItemByPos(i);
		if (pkOrgItem)
		{
			pkOrgItem->InitItemSkill();
		}
	}
	UpdateAttribute(true);
}

void BeUnit::SetCurrentTypeID(int iTypeID)
{
	if (m_pkCurData->iTypeID != iTypeID)
	{
		SetShareUnitChangeFlag(BSUDCF_TYPEID);
	}
	m_pkCurData->iTypeID = iTypeID;
}

void BeUnit::SetFace(float fFace, bool bChange/* = true*/)
{
	AdjustRadian(fFace);

	// 	if(fFace != m_pkCurData->fFace && bChange)
	{
		SetShareUnitChangeFlag(BSUDCF_FACE);
	}
	m_pkCurData->fFace = fFace;
}

void BeUnit::SetPreTurnFace(float fFace)
{
	AdjustRadian(fFace);
	m_pkCurData->fFace = fFace;
}

void BeUnit::SetMaxHP(float fMaxHP)
{
	if (fMaxHP != m_pkCurData->fMaxHP)
	{
		SetShareUnitChangeFlag(BSUDCF_MAXHP);
	}
	m_pkCurData->fMaxHP = fMaxHP;
}

void BeUnit::SetCurAttackCD(int iCurAttackCD)
{
	if (iCurAttackCD != m_pkCurData->iAttackCD)
	{
		SetShareUnitChangeFlag(BSUDCF_ATTACKCD);
	}
	m_pkCurData->iAttackCD = iCurAttackCD;
}

void BeUnit::UpdateBattleState(bool bBattle)
{
	if (!IsHero())
	{
		return;
	}
	if (bBattle)
	{
		m_iOutBattleTime = gTime + 6000;
		if (m_bOutOfBattle)
		{
			m_bOutOfBattle = false;
			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, this);
			kParam.SetParam(BTP_iIntData, 0);

			gTrgMgr.FireTrigger(BTE_CHANGE_BATTLE_STATE, kParam);
		}
	}
	else
	{
		if (gTime >= m_iOutBattleTime && !m_bOutOfBattle)
		{
			m_bOutOfBattle = true;

			TePtParam kParam;
			kParam.SetParam(BTP_pkTrgUnit, this);
			kParam.SetParam(BTP_iIntData, 1);

			gTrgMgr.FireTrigger(BTE_CHANGE_BATTLE_STATE, kParam);
		}
	}
}

void BeUnit::SetUD_Int(UserDataKey eKey, int i)
{
	m_akUserData[eKey] = BePointerType(i);
}

void BeUnit::SetUD_Float(UserDataKey eKey, float f)
{
	m_akUserData[eKey] = BePointerType(f);
}

int BeUnit::PopUD_Int(UserDataKey eKey)
{
	int iValue = 0;
	auto itr = m_akUserData.Find(eKey);
	if (itr != m_akUserData.End())
	{
		iValue = itr->Value().v.iValue;
		m_akUserData.Erase(itr);
	}

	return iValue;
}

float BeUnit::PopUD_Float(UserDataKey eKey)
{
	float fValue = 0.f;
	auto itr = m_akUserData.Find(eKey);
	if (itr != m_akUserData.End())
	{
		fValue = itr->Value().v.fValue;
		m_akUserData.Erase(itr);
	}

	return fValue;
}

int BeUnit::GetUD_Int(UserDataKey eKey, int i) const
{
	auto itr = m_akUserData.Find(eKey);
	if (itr != m_akUserData.End())
	{
		i = itr->Value().v.iValue;
	}

	return i;
}

float BeUnit::GetUD_Float(UserDataKey eKey, float f) const
{
	auto itr = m_akUserData.Find(eKey);
	if (itr != m_akUserData.End())
	{
		f = itr->Value().v.fValue;
	}

	return f;
}

bool BeUnit::HasUserData(UserDataKey eKey) const
{
	auto itr = m_akUserData.Find(eKey);
	if (itr != m_akUserData.End())
	{
		return true;
	}
	return false;
}

void BeUnit::ClearUserData(UserDataKey eKey)
{
	m_akUserData.Erase(eKey);
}

void BeUnit::ResetTurnTableInfo()
{
	unsigned int akBuffSkill[21] = {
		'SZ11', 'SZ21', 'SZ31', 'SZ12', 'SZ22', 'SZ32', 'SZ13',
		'SZ23', 'SZ33', 'SZ14', 'SZ24', 'SZ34', 'SZ15', 'SZ25',
		'SZ35', 'SZ16', 'SZ26', 'SZ36', 'SZ17', 'SZ27', 'SZ37' };

	for (int iIdx = 0; iIdx < 21; iIdx++)
	{
		int iRandIdx = gRandNum.RandInt(0, 21);
		if (iIdx != iRandIdx)
		{
			unsigned int uiTemp = akBuffSkill[iIdx];
			akBuffSkill[iIdx] = akBuffSkill[iRandIdx];
			akBuffSkill[iRandIdx] = uiTemp;
		}
	}
	for (int iIdx = 0; iIdx < 6; iIdx++)
	{
		SetTurnTableInfoByPos(iIdx, akBuffSkill[iIdx]);
	}
	m_uiReTurnNum = 0;
	m_uiConfirmResult = 0;
	memset(m_uiTurnResult, 0, sizeof(m_uiTurnResult));
	SetTurnTableFlag(BZCTF_TABLE_INFO);
}

unsigned int BeUnit::RollTurnTableBuffer()
{
	int iRandNum = gRandNum.RandInt(0, 6);
	unsigned int uiBufferSkillID = GetTurnTableInfoByPos(iRandNum);
	this->m_uiNowTurnResult = uiBufferSkillID;
	for (int i = 0; i < 6; i++)
	{
		if (m_uiTurnResult[i] == 0)
		{
			m_uiTurnResult[i] = uiBufferSkillID;
			break;
		}
	}
	if (uiBufferSkillID > 0) {
		SetTurnTableFlag(BZCTF_TRUN_RESULT);
	}
	return uiBufferSkillID;
}

bool BeUnit::AddTurnTableBuffer()
{
	BeBuffer* pkBuffer = GetBuffer('B513');
	if (pkBuffer)
	{
		pkBuffer->ClearAttr();
		for (int i = 0; i < 6; i++)
		{
			unsigned int uiSkillID = m_uiTurnTableAttrInfo[i];
			const SkillTable* pkSkillInfo = gMain.GetResSkill(uiSkillID);
			if (pkSkillInfo)
			{
				BeNormalAttType iTypeA = (BeNormalAttType)pkSkillInfo->uiDataContentA;
				float fValueA = pkSkillInfo->fValueA[0];
				pkBuffer->SetNormalAttr(iTypeA, pkBuffer->GetNormalAttrValue(iTypeA) + fValueA);
				UpdateAttribute(true);
			}
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			unsigned int uiSkillID = m_uiTurnTableAttrInfo[i];
			const SkillTable* pkSkillInfo = gMain.GetResSkill(uiSkillID);
			if (pkSkillInfo)
			{
				pkBuffer = AddBufferBegin('B513', -1);
				if (pkBuffer)
				{
					BeNormalAttType iTypeA = (BeNormalAttType)pkSkillInfo->uiDataContentA;
					float fValueA = pkSkillInfo->fValueA[0];
					pkBuffer->SetNormalAttr(iTypeA, pkBuffer->GetNormalAttrValue(iTypeA) + fValueA);
					pkBuffer->SetDeadNoRemove(true);
					AddBufferEnd(pkBuffer);
				}
			}
		}
	}


	m_uiConfirmResult = m_uiNowTurnResult;
	m_uiReTurnNum = 0;
	memset(m_uiTurnResult, 0, sizeof(m_uiTurnResult));
	SetTurnTableFlag(BZCTF_TABLE_INFO);

	return true;
}

void			BeUnit::PlayAction(int iEffectID, int iFffectTime)
{
	int		iLastUseTime = GetUD_Int(UDK_SHOW_ACTION_TIME);
	int		iCDTime = 1000;
	if (gTime - iLastUseTime < iCDTime)
	{
		return;
	}
	if (iEffectID) {
		AddAttach(iEffectID, iFffectTime, GetID());
		SetUD_Int(UDK_SHOW_ACTION_TIME, gTime);
	}
}
