#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include "TW_Entity.h"
#include "TW_MemoryPool.h"
#include <Unit_table.hpp>
#include "TW_UnitData.h"
#include "TW_UnitDefine.h"
#include "TW_Commander.h"
#include "TW_CarryDefine.h"
#include "TW_ShareUnitData.h"
#include "TW_Functions.h"
#include "TW_UserDataDefine.h"
#include <TW_PointerType.h>
#include "TW_Command.h"
#include "TW_CommandExe.h"

#include "TW_UnitCarry.h"

class TwUnitOutput;

class TwUnit : public TwUnitCarry
{
public:
	TwUnit(int iID);
	~TwUnit(void);

    void Link(float fX, float fY, TwEntityMgr* pkMgr) override;
    void Unlink() override;

public:

	int GetAttackCD(void) const;

	void SetActionName(int iActionName, int iTime)
	{
		if (iActionName < BAN_ActionNum)
		{
			if (m_eActionName != iActionName || iActionName == BAN_attack || iActionName == BAN_Crit_Attack)
			{
				SetShareUnitChangeFlag(BSUDCF_ACTIONNAME);
			}
			m_eActionName = (BeActionName)iActionName;
			m_iActionStartTime = iTime;
		}
	}

	BeActionName GetActionName(void) const
	{
		return m_eActionName;
	}

	int GetActionSartTime(void) const
	{
		return m_iActionStartTime;
	}

	inline void SetBaseDamage(float fMaxDamage, bool bUpdateAttribute = true)
	{
		if (fMaxDamage != m_pkCurData->fBaseDamage)
		{
		}
		m_pkCurData->fBaseDamage = fMaxDamage;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddDamage(float fAddDamage, bool bUpdateAttribute = true)
	{
		if (fAddDamage != m_pkCurData->fAddDamage)
		{
		}
		m_pkCurData->fAddDamage = fAddDamage;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetBaseArmor(float fBaseArmor, bool bUpdateAttribute = true)
	{
		if (m_pkCurData->fBaseArmor != fBaseArmor)
		{
		}

		m_pkCurData->fBaseArmor = fBaseArmor;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddArmor(float fAddArmor, bool bUpdateAttribute = true)
	{
		if (fAddArmor != m_pkCurData->fAddArmor)
		{
		}

		m_pkCurData->fAddArmor = fAddArmor;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddArmorPer(float fAddArmorPer, bool bUpdateAttribute = true)
	{
		m_pkCurData->fAddArmorPer = fAddArmorPer;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddAntiMagic(float fAddMagicArmor, bool bUpdateAttribute = true)
	{
		m_pkCurData->fAddMagicArmor = fAddMagicArmor;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetAddAntiMagicPer(float fAddMagicArmorPer, bool bUpdateAttribute = true)
	{
		m_pkCurData->fAddMagicArmorPer = fAddMagicArmorPer;
		if (bUpdateAttribute)
		{
			UpdateAttribute(true);
		}
	}

	inline void SetBaseMaxHP(float fBaseMaxHP)
	{
		m_pkCurData->fBaseMaxHP = fBaseMaxHP;
		UpdateAttribute(true);
	}

	inline void SetBaseMaxMP(float fBaseMaxMP)
	{
		m_pkCurData->fBaseMaxMP = fBaseMaxMP;
		UpdateAttribute(true);
	}

	inline void SetBaseRegenHP(float fBaseRegenHP)
	{
		m_pkCurData->fBaseRegenHP = fBaseRegenHP;
		UpdateAttribute(true);
	}

	inline void SetBaseRegenMP(float fBaseRegenMP)
	{
		m_pkCurData->fBaseRegenMP = fBaseRegenMP;
		UpdateAttribute(true);
	}

	bool IsDamageTime(int iDeltaTime);

	inline void SetAbsFace(float fFace)
	{
		AdjustRadian(fFace);

		m_pkCurData->fFace = fFace;
	}

	void SetTarFace(float fTarFace)
	{
		AdjustRadian(fTarFace);
		if (fabs(m_pkCurData->fFace - fTarFace) > 0.01f)
		{
			SetShareUnitChangeFlag(BSUDCF_FACE);
		}
		m_pkCurData->fFace = fTarFace;
	}

	void SetScale(float fScale)
	{
		m_pkCurData->fScale = fScale;

		SetShareUnitChangeFlag(BSUDCF_SCALE);
	}

	bool HasSkill(int iID) const
	{
		return GetSkill(iID) == NULL ? false : true;
	}

	inline bool IsDead(void) const
	{
		return HasFlag(BUF_DEAD);
	}

	inline std::list<TwCommand>& GetUnitCommands(void)
	{
		return m_kCommander.GetCommands();
	}

	inline void CopyUnitCommands(std::shared_ptr<TwUnit> pkUnit)
	{
		m_kCommander.SetCommands(pkUnit->GetUnitCommands());
	}

	inline void ClearAllCommands()
	{
		m_kCommander.ClearAllCommands();
	}

	inline TwCommand& GetUnitCurCommand(void)
	{
		return m_kCommander.GetCurCommand();
	}

	inline const BeExeCommand* GetUnitCurCmd(void)
	{
		return m_kCommander.GetCurCmd();
	}

	inline const TwCommandType GetUnitCurCmdType(void)
	{
		BeExeCommand* pkECmd = m_kCommander.GetCurCmd();
		if (pkECmd)
		{
			return pkECmd->GetType();
		}

		return TwCommandType::BCT_NUMS;
	}

	inline void CopyUnitCurCommand(std::shared_ptr<TwUnit> pkUnit)
	{
		GiveCommand(pkUnit->GetUnitCurCommand());
	}

	inline void SetTransTime(int iTransTime, bool bForce = false)
	{
		m_iTransTime = iTransTime;

		if (bForce || m_iTransTime < m_iAddTransTime)
		{
			m_iAddTransTime = 0;
		}
	}

	inline int GetTransTime(void)
	{
		return m_iTransTime;
	}

	inline void SetDecMPCost(float fDecMPCost)
	{
		if (fDecMPCost != m_pkCurData->fDecMPCost)
		{
		}
		m_pkCurData->fDecMPCost = fDecMPCost;
	}

	inline bool IsPureNeedUpdate()
	{
		return m_bPureNeedUpdae;
	}

	inline void SetPathFindSucessTime(int iSucessTime)
	{
		if (m_iPathFindSucessTime != iSucessTime)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDERSUCESS);
		}
		m_iPathFindSucessTime = iSucessTime;
	}

	inline void SetActionState(int iActionState)
	{
		if (m_iActionState != iActionState)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDER);
		}
		m_iActionState = iActionState;
	}

	inline int GetActionState()
	{
		return m_iActionState;
	}

	inline float GetTarPosX() const
	{
		return m_fWalkTargetX;
	}

	inline float GetTarPosY() const
	{
		return m_fWalkTargetY;
	}

	inline void SetTarPosX(float fTarPosX)
	{
		if (m_fWalkTargetX != fTarPosX)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDER);
		}
		m_fWalkTargetX = fTarPosX;
	}

	inline void SetTarPosY(float fTarPosY)
	{
		if (m_fWalkTargetY != fTarPosY)
		{
			SetShareUnitChangeFlag(BSUDCF_PATHFINDER);
		}
		m_fWalkTargetY = fTarPosY;
	}

	unsigned int GetTransAlphaDec()
	{
		return m_dwTransAlphaDec;
	}
	int GetAlphaTransTime()
	{
		return m_iAlphaTransTime;
	}

	int GetMoveAllTime()
	{
		return m_iMoveAllTime;
	}

	float GetMinMoveDistance()
	{
		return m_fMinMoveDistance;
	}

	bool	Initialize(int iTypeID);
	void	Update(int iDeltaTime);

	void	OnPlayerChanged(void);
	void	OnSuspend(void);
	void	OnResume(void);
	void	SetUnitvisible(bool bVisible);

	void	BuyItem(int iItemTypeID);
	void	SellItem(int iPos);

	void	SetChangeScale(int iBufferID, float fChangeScale, int iChangeTime = 0);
	void	SetColorWithBuf(int iBufferID, unsigned int dwColor);
	void	AddAttach(int pcMdlFile, int iRemoveTime, int iAttackerID = 0, bool bChangeFace = true);
	bool	DelAttachByName(int iModelID);

	void    AddControlSummonUnitID(int iUnitID);

	void			AppyUnitSkill(void);
	bool IsInvincible(void) const;
	void			OnDelete(void);

	void			SetUnitReliveTime(unsigned int uiUnitReliveTime);
	void			SetPlayer(std::uint64_t iPlayerIdx);
	void			SetCastMove(int iMaxTime, int iUnitID, float fTarPosX, float fTarPosY, int iSkillTypeID, BeUnitFlyType kType = BUFT_MOVE, int iAttackUnitID = 0);
	void			ClrCastMove(void);

	bool			IsIdle(void) const;
	bool			GiveCommand(TwCommand& kCmd, TwGiveCmdType eType = TwGiveCmdType::BCT_IMMEDIATE, bool bPlayerControl = false, bool bNeedHangCurrent = true, bool bCheckChaoFeng = true);
	int				GetCommandCount() const;

	void			SetPosition(float fX, float fY, float fZ = 0.0f, bool bNoRecordChange = false);

	void			SetUnitAction(BeUnitAction eAction, int eActionName, int iAllTime = 2000, bool bSetShow = true, bool bForceRefresh = false);

	void			UpdateAttribute(bool bUpdateNormal = true);

	void			DamagedByAbsNum(BeAttackType eAttackType, float fDamage, float fRawDamage, const std::shared_ptr<TwUnit> kAttacker, int iPlayer, int iFlag, bool bCanDead = true, bool bFirstAttack = true, int iSkillTypeID = 0, int iSkillLevel = 0, float fLeech = 0.0f, float fMultiDamage = 0.f, int iBeDamagedEffect = 0);
	void			OperateUnitDead(BeAttackType eAttackType = BAT_NONE, float fDamage = 0.0f, std::shared_ptr<TwUnit> pkAttacker = NULL, int iPlayer = -1, int iFlag = 0, bool bIsTimeOver = false, int iSkillTypeID = 0);
	void			OperateUnitRemove();
	float			GetDamagedByFormula(const std::shared_ptr<TwUnit> pkAttacker, const BeAttackingAttr& rkAttackingAttr, float fDamage) const;
	void			OnBeDamaged(BeAttackingAttr& rkAttackingAttr, bool bCanDead = true, bool bIgnoreInvincible = false, float fMaxHeroAbsDamage = 0.0f);

	BeCarry* AddSuiting(int iTypeID, int iNumber);
	BeCarry* AddLevelStrengthenSuit(int iSkillTypeID, int iSkillLevel);

	void 			OnItemUpDate(int iID);
	BeItem* GetItemByID(int iID) const;
	BeItem* GetItemByPos(int iPos);
	BeItem* GetItemByTypeID(int iTypeID, int iOwnPlayer = -1);
	BeItem* GetComposeItemByTypeID(int iTypeID);
	void			GetItemGroupByTypeID(std::vector<BeItem*>& rkItems, int iTypeID, int iOwnerPlayer = -1);
	int				GetItemNumsByID(int iTypeID) const;
	bool			SwapItemPos(char bySrcPos, char byTarPos);
	void			DropItemByPos(int iPos, float fX, float fY, bool bForce = false);
	void			DelItemByPos(int iPos);
	void			DelAllItem(void);
	void			DelItemByTypeID(int iTypeID, bool bFirstOnly = false);

	bool			HasItem(int iTypeID, bool bOwner = false);
	int				HasItemNum(int iTypeID, bool bOwner = false);
	int				GetFreeItemPos();
	bool			HasItemSkill(int iTypeID);
	bool			IsPackageFull(void) const;
	bool			IsPackageEmpty(void) const;
	bool			PickAroundItem(void);
	unsigned	int	GetItemTypeIDByPos(int iPos);

	BeSkill* AddSkill(int iTypeID, int iLevel = 1, bool bCurrent = false, bool bGenus = false, bool bGenusSkill = false, int iForceID = 0);
	BeSkill* GetSkill(int iTypeID, bool bCurrent = true) const;
	BeSkill* GetSkillByPos(int iPos, bool bOrg = false);
	int				GetCanLearnSkillIDByPos(int iPos) const;
	BeSkill* GetSkillByUIPos(int iPos) const;
	int				GetCanLearnSkillByIdx(int iPos) const;
	void			DelSkill(int iTypeID);
	void			DelAllSkill(bool bHeroSkillOnly = false);
	void			SetSkillLevel(int iTypeID, int iLevel);
	int				GetSkillLevel(int iTypeID);
	const std::vector<std::shared_ptr<BeSkill>>& GetNormalSkill(void);
	int				GetItemSkillTypeID(int iItemID);
	int				GetItemUseSkill(int iItemTypeID);

	
	bool			IsForbidSkill(BeSkill* pkSkill) const;

	BeLearnSkillData* GetLearnSkillData(int iPos);
	BeLearnSkillData* GetAllLearnSkillData();
	bool			LearnSkill(int iSkillPos);
	int				GetCanLearnLevel(int iTypeID, int iLevel);
	int				GetSkillPointSum(void);
	void			SetSkillPointSum(int iSkillPoint);

	BeBuffer* AddBufferBegin(int iTypeID, int iOrgUnitID, int iLevel = 1, int iUnitID = 0, bool bAddDead = false, int iForceID = 0);
	void			AddBufferEnd(BeBuffer* pkBuffer);
	BeBuffer* GetBuffer(int iTypeID, int iUnitID = 0);
	const BeBuffer* GetBuffer(int iTypeID, int iUnitID = 0) const;
	bool			DelBuffer(int iTypeID, int iUnitID = 0);
	bool			DelBufferByID(int iBufferID);
	void			UpdateState(int iDeltaTime);
	void			DelBufferByClean(bool bNegative = true, bool bGood = true);
	void			DelBufferByAutoClean(void);
	void			RemoveBufferAttr(BeNormalAttType eType);

	template<class T>
	void TrgOnPreDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr, int iItemPos = -1);
	template<class T>
	void TrgOnBeDamaged_T(T& kSkill, int iCount, int eAttackType, float& fDamage, float fRawDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos = -1);
	template<class T>
	void TrgOnDamage_T(T& kSkill, int iCount, int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, int iItemPos = -1, int iItemID = 0);
	template<class T>
	void TrgOnAttack_T(T& kSkill, int iCount, std::shared_ptr<TwUnit> pkTarget, BeAttackingAttr* pkAttackAttr, int iItemPos = -1);

	void			TrgOnPreBeDamaged(int eAttackType, float& fDamage, float fRawDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID, BeAttackingAttr& kAttr);
	void			TrgOnBeDamaged(int eAttackType, float& fDamage, float fRawDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID);
	void			TrgOnPreDamage(int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, BeAttackingAttr& kAttr);
	void			TrgOnDamage(int eAttackType, float& fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, bool bCanDead, int iAttackSkillTypeID);
	void			TrgOnDead(int eAttackType, float fDamage, std::shared_ptr<TwUnit> pkAttacker, int iPlayer, int iFlag, int iSkillTypeID);

	void			TrgOnPreAttack(std::shared_ptr<TwUnit> pkTarget, BeAttackingAttr* pkAttackAttr);
	void			TrgOnAttack(int iTargetID, BeAttackingAttr* pkAttackAttr);
	void			TrgOnSpell(int iSkillTypeID, int iSkillLevel, int iItemID, int iTargetID, float fTargetPosX, float fTargetPosY);
	void			TrgOnUpdate(int iDeltaTime);
	void			TrgOnKill(int eAttackType, float fDamage, std::shared_ptr<TwUnit> pkTarget, int iPlayer, int iFlag, int iSkillTypeID);

	void			GetAttackingAttr(std::shared_ptr<TwUnit> pkTarget, BeAttackingAttr*& rkAttackingAttr);
	bool			GetAttackingMiss(void) const;
	bool			GetAttackedAvoid(void) const;
	float			GetAttackedAntiLeech(void) const;
	void			GetAttackedPhysicAttr(float& fAntiPhysic, const std::shared_ptr<TwUnit> pkAttacker, float fDecArmorValue, float fDecArmorPer) const;
	void			GetAttackedMagicAttr(float& fAntiMagic, const std::shared_ptr<TwUnit> pkAttacker, float fDecEnemyAntiMagic, float fDecMagicArmorPer) const;
	float			GetAttackedAttackCDAttr(void) const;
	float			GetAttackedBaoJiAttr(void) const;

	void			ResetSkill(void);
	bool UnitCanBeControl(int iPlayer) const;
	bool UnitCanMove(bool bTemp = false) const;
	bool UnitCanAttack(const std::shared_ptr<TwUnit> pkTarget = 0, bool bTemp = false, bool bForceAttackSameGroup = false) const;

	UnitUseSkillResultType UnitCanUseSkill(int iSkillTypeID, const std::shared_ptr<TwUnit> pkTarget = NULL, bool bTemp = false, bool bCheckPos = false, float fTargetPosX = 0.0f, float fTargetPosY = 0.0f, bool bArea = false, std::shared_ptr<TwUnit>* ppkTarget = NULL);

	bool UnitCanUseActiveSkill(int iAttackedID, int& iSkillTypeID, int& iSkillLevel);

	void TranslateSkillTargetType(int iSkillTargetFlag, int& iStaticProcFlag, int& iDynaPropFlag) const;
	bool IsSkillTargetType(const std::shared_ptr<const SkillTable> pkRes, const std::shared_ptr<TwUnit> pkTarget = 0) const;
	bool IsTargetUnit(const std::shared_ptr<TwUnit> pkUnit, int iStaticProcFlag, int iDynaPropFlag) const;

	bool PickMapItem(BeMapItem* pkMapItem);

	void				AddIgnoreUnit(int iUnitID, int iTime = 5000);
	bool				IsIgnoreUnit(int iUnitID) const;

	void				CopyAttribute(std::shared_ptr<TwUnit> pkUnit);

	bool			IsChangeSkill()
	{
		return m_bChangeSkill;
	}
	void			ClearnChangeSkill()
	{
		m_bChangeSkill = false;
	}

public:
	void			OnDelBuffer(std::shared_ptr<BeBuffer> pkBuffer, bool bUpdate = true, bool bDelEffect = true);
	void SetCheatAttr(void);

	void SetAllLiveTime(int iAllTime);

protected:
	void			SetDead(void);
	void			DelAllBuffer(bool bRelive = false);
	void			SafeDelBuf(BeBuffer* pkBuffer, bool bNeedRecordChange = true);

	void			UpdateValidItem(void);
	void			UpdateValidBuffer(void);
	void			UpdateValidSkill(bool bReset = false);
	void			OnAddBuffer(BeBuffer* pkBuffer);

public:
	BeItem* AddItem(int iTypeID, int iPos = -1, int iForceID = 0, int iOrgData = 0);
protected:
	TwCommander									m_kCommander;
	bool										m_bChangeSkill;
	int							m_iMoveToUnitID;
	float                       m_fMoveTarPosX;
	float                       m_fMoveTarPosY;
	int							m_iMoveAllTime;
	float						m_fMinMoveDistance;
	float						m_fWalkTargetX;
	float						m_fWalkTargetY;

	int					m_iTransTime;
	int					m_iAddTransTime;
	UnitActorTransType  m_eActorTransType;
	bool				m_bNeedTrans;
	unsigned int		m_dwTransAlphaDec;
	unsigned int		m_dwStartAlpha;
	int					m_iAlphaTransTime;	

	BeActionName		m_eActionName;
	int					m_iActionStartTime;	

	TwCommand			m_kLastCommand;

protected:
	void	UpdateMP(int iDeltaTime);
	void	UpdateHP(int iDeltaTime);
	void	UpdateLiveTime(int iDeltaTime);
	void	UpdateBuffer(int iDeltaTime);

public:
	int		m_iActionState;
	int		m_iPathFindSucessTime;
private:
	bool	m_bPureNeedUpdae;
	void	UpdateTransrate(int iDeltaTime);

public:
	void SetFlag(int iFlag, bool bNeedRecordChange = true);
	void ClrFlag(int iFlag, bool bNeedRecordChange = true);
	void	ClrAllPureData();
	void SetFace(float fFace, bool bChange = true);
	void SetPreTurnFace(float fFace);
	void SetCurAttackCD(int iCurAttackCD);
	void	UpdateItemPassiveSkill();
private:
	BeShareSelfData			m_kShareSelfDataCur;
	BeShareSelfData			m_kShareSelfDataLast;
	int						m_iEffectID;
	int						m_iActionStayTime;
	int						m_iGrassIndex;
	int						m_iGiveAttackCmdTime;
	bool					m_bNeedUpdateObstacle;
	bool					m_bSetObstacle;
	std::shared_ptr<TwUnit>		m_spSharePtr;
	std::vector<int>		m_kItemPassiveSkill;
	int						m_iLastAttackTime;
	int						m_iLastAttackHeroTime;

public:
	inline		unsigned int			GetClass(void) const;
	inline		int						GetEffectID();
	inline		void					SetEffectID(int iValue);
	inline		void					SetActionStayTime(int iValue);
	inline		void					ClearActionStayTime();
	inline		const int				GetGrassIndex() const;
	inline		BeShareSelfData& GetCurShareSelfData();
	inline		const BeShareSelfData& GetLastShareSelfData();
	inline		int						GetOrgAttackCD();
	inline		void					SetHaveSetObstacle(bool bSet);
	inline		bool					GetHaveSetObstacle() const;
	inline		bool					GetNeedUpdateObstacle() const;
	inline		const std::shared_ptr<TwUnit> GetSharePtr() const;
	inline		float						GetDamageNum();

	inline		unsigned	int			GetSkillTypeIDByPos(int iPos);
	inline		void					AddItemPassiveSkill(int iSkillTypeID);
	inline		void					ClearItemPassiveSkill();
	inline		bool					CanAddItemPassiveSkill(int iSkillTypeID);
	inline		void					SetLastAttackTime(int iTime);
	inline		int						GetLastAttackTime();
	inline		int						GetLastGiveAttackCmdTime();
	inline		void					SetLastAttackHeroTime(int iTime);
	inline		int						GetLastAttackHeroTime();

private:
    std::unique_ptr<TwUnitOutput>         UnitOutput;
};

inline unsigned int TwUnit::GetClass(void) const
{
	if (!m_pkBackData || !m_pkBackData->pkRes)
	{
		return 0;
	}

	return m_pkBackData->pkRes->uiClassType;
}

inline int			TwUnit::GetEffectID()
{
	return m_iEffectID;
}
inline void			TwUnit::SetEffectID(int iValue)
{
	m_iEffectID = iValue;
}
inline void			TwUnit::SetActionStayTime(int iValue)
{
	if (iValue > m_iActionStayTime)
	{
		m_iActionStayTime = iValue;
	}
}
inline		void	TwUnit::ClearActionStayTime()
{
	m_iActionStayTime = 0;
}
inline const int	TwUnit::GetGrassIndex()  const
{
	return m_iGrassIndex;
}
inline BeShareSelfData& TwUnit::GetCurShareSelfData()
{
	return m_kShareSelfDataCur;
}
inline const BeShareSelfData& TwUnit::GetLastShareSelfData()
{
	return m_kShareSelfDataLast;
}

inline int			TwUnit::GetOrgAttackCD()
{
	return m_pkCurData->iOrgAttackCD;
}

inline void			TwUnit::SetHaveSetObstacle(bool bSet)
{
	m_bSetObstacle = bSet;
}
inline bool			TwUnit::GetHaveSetObstacle() const
{
	return m_bSetObstacle;
}
inline bool			TwUnit::GetNeedUpdateObstacle() const
{
	return m_bNeedUpdateObstacle;
}
inline const std::shared_ptr<TwUnit> TwUnit::GetSharePtr() const
{
	return m_spSharePtr;
}
inline float			TwUnit::GetDamageNum()
{
	return m_pkCurData->fBaseDamage + m_pkCurData->fAddDamage;
}

inline		unsigned	int			TwUnit::GetSkillTypeIDByPos(int iPos)
{
	unsigned	int		iSkillTypeID = 0;
	if (m_pkCurData->pkRes && iPos <= 4 && iPos >= 0)
	{
		iSkillTypeID = m_pkCurData->pkRes->iSkillList[iPos];
	}

	return iSkillTypeID;
}

inline		void					TwUnit::AddItemPassiveSkill(int iSkillTypeID)
{
	m_kItemPassiveSkill.push_back(iSkillTypeID);
}
inline		void					TwUnit::ClearItemPassiveSkill()
{
	m_kItemPassiveSkill.clear();
}
inline		bool					TwUnit::CanAddItemPassiveSkill(int iSkillTypeID)
{
	if (!m_kItemPassiveSkill.empty())
	{
		for (int iIndex = 0; iIndex < m_kItemPassiveSkill.size(); ++iIndex)
		{
			if (m_kItemPassiveSkill[iIndex] == iSkillTypeID)
			{
				return false;
			}
		}
	}
	return true;
}
inline		void					TwUnit::SetLastAttackTime(int iTime)
{
	m_iLastAttackTime = iTime;
}
inline		int						TwUnit::GetLastAttackTime()
{
	return m_iLastAttackTime;
}

inline		int						TwUnit::GetLastGiveAttackCmdTime()
{
	return m_iGiveAttackCmdTime;
}

inline		void					TwUnit::SetLastAttackHeroTime(int iTime)
{
	m_iLastAttackHeroTime = iTime;
}
inline		int						TwUnit::GetLastAttackHeroTime()
{
	return m_iLastAttackHeroTime;
}

inline bool TwUnit::IsInvincible(void) const
{
	return (HasUnitCarryFlag(BUCF_ISINVINCIBLE) || HasFlag(BUF_ISINVINCIBLE));
}
