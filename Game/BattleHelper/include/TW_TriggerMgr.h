#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include <vector>
#include <map>
#include <memory>
#include "TW_PointerType.h"

typedef unsigned short lc_t;

class pt
{
public:
	pt()
	{
		lc = 0;
		pkAttachMain = nullptr;
		for (int i = 0; i < 10; i++)
		{
			iIntValue[i] = 0;
			fFloatValue[i] = 0.0f;
		}
	}
	lc_t                lc;
	void*               pkAttachMain;
	int                 iIntValue[10];
	float               fFloatValue[10];
	std::vector<int>    aiVector;
	std::map<int, int>  aiMap;
};

#define PT_WAITING 0
#define PT_YIELDED 1
#define PT_EXITED  2
#define PT_ENDED   3

#define LC_SET(s) s = __LINE__; case __LINE__:

#define PT_WAIT_UNTIL(pt, condition)	        \
	{										\
	LC_SET((pt)->lc);							\
	if(!(condition)) {							\
	return PT_WAITING;							\
	}											\
	}

#define PtTrgReturn				(gTrgMgr.Return())

#define PT_CONDITION_MAIN()		TwMain* pkAttachMain = (TwMain*)(pkPama)

#define PT_ACTION_MAIN()		TwMain* pkAttachMain = (TwMain*)((pt*)(pkPama))->pkAttachMain;char PT_YIELD_FLAG=1;

#define PT_BEGIN(name)			{switch(((pt*)(pkPama))->lc){ case 0:

#define PT_END(name)			};						\
								POS_##name:				\
								PT_YIELD_FLAG = 0;		\
								(((pt*)(pkPama))->lc) = 0;gTrgMgr.Return(); return PT_ENDED; }

#define PT_RETURN(name)			{PT_YIELD_FLAG = 0;(((pt*)(pkPama))->lc) = 0;gTrgMgr.Return(); return PT_ENDED;}

#define PT_INTVALUE()			((pt*)(pkPama))->iIntValue[0]
#define PT_INT1()				((pt*)(pkPama))->iIntValue[1]
#define PT_INT2()				((pt*)(pkPama))->iIntValue[2]
#define PT_INT3()				((pt*)(pkPama))->iIntValue[3]
#define PT_INT4()				((pt*)(pkPama))->iIntValue[4]
#define PT_INT5()				((pt*)(pkPama))->iIntValue[5]
#define PT_INT6()				((pt*)(pkPama))->iIntValue[6]
#define PT_INT7()				((pt*)(pkPama))->iIntValue[7]
#define PT_INT8()				((pt*)(pkPama))->iIntValue[8]
#define PT_INT9()				((pt*)(pkPama))->iIntValue[9]
#define PT_FLOAT1()				((pt*)(pkPama))->fFloatValue[0]
#define PT_FLOAT2()				((pt*)(pkPama))->fFloatValue[1]
#define PT_FLOAT3()				((pt*)(pkPama))->fFloatValue[2]
#define PT_FLOAT4()				((pt*)(pkPama))->fFloatValue[3]
#define PT_FLOAT5()				((pt*)(pkPama))->fFloatValue[4]
#define PT_FLOAT6()				((pt*)(pkPama))->fFloatValue[5]
#define PT_FLOAT7()				((pt*)(pkPama))->fFloatValue[6]
#define PT_FLOAT8()				((pt*)(pkPama))->fFloatValue[7]
#define PT_FLOAT9()				((pt*)(pkPama))->fFloatValue[8]
#define PT_FLOATVALUE()			((pt*)(pkPama))->fFloatValue[9]
#define PT_INTVECTOR()			((pt*)(pkPama))->aiVector;
#define PT_INTMAP()				((pt*)(pkPama))->aiMap;

#define PT_WAIT(time)				gTrgMgr.TrgWait(time);PT_WAIT_UNTIL(((pt*)(pkPama)),gTrgMgr.WaitFinish())

#define PT_CONTION_FUN(funName)		TwPtCondResult	funName(void* pkPama)

#define PT_ACTION_FUN(funName)		char funName(void* pkPama)

enum class TwPtCondResult
{
	PTCR_OK = 0,
	PTCR_END,
	PTCR_END_THIS,
};

typedef		TwPtCondResult(*PtConditionFun)(void*);
typedef		char			(*PtActionFun)(void*);

enum class TwTrgParamID;

class TwPtParam
{
public:
	TwPtParam(void);
	~TwPtParam(void);

	void	SetParam(TwTrgParamID iParamID, int iValue);
	void	SetParam(TwTrgParamID iParamID, float fValue);
	void	SetParam(TwTrgParamID iParamID, void* pkValue);

	bool	HasParam(TwTrgParamID iParamID);
	int		GetParamInt(TwTrgParamID iParamID, int iDefault = 0);
	float	GetParamFloat(TwTrgParamID iParamID, float fDefault = 0.0f);
	void*	GetParamVoid(TwTrgParamID iParamID);
	void	OnDeleteUnit(void* pkUnit);

protected:
	struct ParamData
	{
		ParamData(TwTrgParamID id, const TwPointerType& data);

        TwTrgParamID iID;
		TwPointerType kData;
	};
	std::vector<ParamData> m_akParams;
};

enum class TwTriggerEvent;
class TwPtTriggerMgr
{
public:
	TwPtTriggerMgr(void* pkParam = nullptr);
	~TwPtTriggerMgr(void);

	bool		Initialize(int iMaxEvent);
	void		Update(int iDeltaTime);
	void		Finialize(void);

	void		TrgWait(int iTime);
	void		Return(void);
	void		Enable(void);
	void		Disable(void);
	void		Finish(void);
	bool		WaitFinish(void);

	void		FireTrigger(TwTriggerEvent iEvent, TwPtParam& rkParam);
	bool		FireTriggerBegin(TwTriggerEvent iEvent, TwPtParam& rkParam, std::shared_ptr<TwPtParam>& pkBackupParam);
	void		FireTriggerEnd(std::shared_ptr<TwPtParam>& pkBackupParam);
	void		RegisterEventTrg(TwTriggerEvent iEvent, PtActionFun pAction, PtConditionFun pCondition = nullptr);
	int			StartTimerTrg(int iTimeOut, PtActionFun pAction, const TwPtParam& rkParam,
                            PtConditionFun pCondition = nullptr, unsigned int uiCounts = ((unsigned int)-1), int iDelayTime = 0);

	unsigned int    GetTimeNow(void);
	int			GetTriggerID(void);

	bool		HasParam(TwTrgParamID iParamID);
	int			GetParamInt(TwTrgParamID iParamID, int iDefault = 0);
	float		GetParamFloat(TwTrgParamID iParamID, float fDefault = 0.0f);
	void*       GetParamVoid(TwTrgParamID iParamID);
	void        OnUnitDelete(void* pkUnit);

protected:
	void* m_pkData;
};
