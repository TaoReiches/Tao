#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum class TePointerValType
{
	BPVT_PVOID,
	BPVT_int,
	BPVT_float,
	BPVT_Int64,
};

struct TePointerType
{
	TePointerType(void* p = 0)
	{
		v.i64init = 0;
		v.pVoid = p;
		eValType = TePointerValType::BPVT_PVOID;
	}

	TePointerType(int i)
	{
		v.i64init = 0;
		v.iValue = i;
		eValType = TePointerValType::BPVT_int;
	}

	TePointerType(float f)
	{
		v.i64init = 0;
		v.fValue = f;
		eValType = TePointerValType::BPVT_float;
	}
	union
	{
		void*			pVoid;
		int				iValue;
		float			fValue;
		std::int64_t	i64init;
	} v;
	TePointerValType eValType;
};
