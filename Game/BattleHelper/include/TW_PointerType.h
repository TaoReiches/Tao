#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum class TwPointerValType
{
	BPVT_PVOID,
	BPVT_int,
	BPVT_float,
	BPVT_Int64,
};

class TwPointerType
{
public:
	TwPointerType(void* p = 0)
	{
		v.i64init = 0;
		v.pVoid = p;
		eValType = TwPointerValType::BPVT_PVOID;
	}

	TwPointerType(int i)
	{
		v.i64init = 0;
		v.iValue = i;
		eValType = TwPointerValType::BPVT_int;
	}

	TwPointerType(float f)
	{
		v.i64init = 0;
		v.fValue = f;
		eValType = TwPointerValType::BPVT_float;
	}
	union
	{
		void*			pVoid;
		int				iValue;
		float			fValue;
		std::int64_t	i64init;
	} v;
	TwPointerValType eValType;
};
