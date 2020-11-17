/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_TriggerMgr.h"

TwPtParam::TwPtParam(void)
{
}

TwPtParam::~TwPtParam(void)
{
	m_akParams.clear();
}

void TwPtParam::SetParam(int iParamID, int iValue)
{
	m_akParams.push_back(ParamData(iParamID, TwPointerType(iValue)));
}
void TwPtParam::SetParam(int iParamID, float fValue)
{
	m_akParams.push_back(ParamData(iParamID, TwPointerType(fValue)));
}
void TwPtParam::SetParam(int iParamID, void* pkValue)
{
	m_akParams.push_back(ParamData(iParamID, TwPointerType(pkValue)));
}

bool TwPtParam::HasParam(int iParamID)
{
	for (std::vector<ParamData>::iterator kIter = m_akParams.begin(); kIter != m_akParams.end(); ++kIter)
	{
		ParamData& rkData = *kIter;
		if (rkData.iID == iParamID)
		{
			return true;
		}
	}
	return false;
}

int TwPtParam::GetParamInt(int iParamID, int iDefault)
{
	for (std::vector<ParamData>::iterator kIter = m_akParams.begin(); kIter != m_akParams.end(); ++kIter)
	{
		ParamData& rkData = *kIter;
		if (rkData.iID == iParamID)
		{
			switch (rkData.kData.eValType)
			{
			case TwPointerValType::BPVT_int:
			{
				return rkData.kData.v.iValue;
				break;
			}
			case TwPointerValType::BPVT_float:
			{
				return (int)rkData.kData.v.fValue;
				break;
			}
			default:
				break;
			}
		}
	}
	return iDefault;
}

float TwPtParam::GetParamFloat(int iParamID, float fDefault)
{
	for (std::vector<ParamData>::iterator kIter = m_akParams.begin(); kIter != m_akParams.end(); ++kIter)
	{
		ParamData& rkData = *kIter;
		if (rkData.iID == iParamID)
		{
			switch (rkData.kData.eValType)
			{
			case TwPointerValType::BPVT_int:
			{
				return (float)rkData.kData.v.iValue;
				break;
			}
			case TwPointerValType::BPVT_float:
			{
				return rkData.kData.v.fValue;
				break;
			}
			default:
				break;
			}
		}
	}
	return fDefault;
}

void* TwPtParam::GetParamVoid(int iParamID)
{
	for (std::vector<ParamData>::reverse_iterator kIter = m_akParams.rbegin(); kIter != m_akParams.rend(); ++kIter)
	{
		ParamData& rkData = *kIter;
		if (rkData.iID == iParamID)
		{
			return rkData.kData.v.pVoid;
		}
	}
	return NULL;
}

void	TwPtParam::OnDeleteUnit(void* pkUnit)
{
	for (auto kIter = m_akParams.begin(); kIter != m_akParams.end(); ++kIter)
	{
		ParamData& rkData = *kIter;
		if (rkData.kData.eValType == TwPointerValType::BPVT_PVOID)
		{
			if (rkData.kData.v.pVoid == pkUnit)
			{
				rkData.kData.v.pVoid = nullptr;
			}
		}
	}
}

TwPtParam::ParamData::ParamData(int id, const TwPointerType& data) :iID(id), kData(data)
{
}
