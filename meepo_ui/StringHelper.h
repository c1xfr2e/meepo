/***********************************************************************
    filename:   StringHelper.h
    created:    2011/10/1
    author:     ZhaoHao

    purpose:   	Define helper functions to convert string to other types and vise versa. 
*************************************************************************/
#ifndef _Meepo_StringHelper_h_
#define _Meepo_StringHelper_h_

#include "Rect.h"
#include <string>
#include <cstdio>
#include "RelativeCoord.h"

namespace Meepo
{

class StringHelper
{
public:
	static int StringToInt(const std::wstring& str);
	static std::wstring	IntToString(int val);

	static CRelRect StringToRelRect(const std::wstring& str);
	static CRelVector StringToRelVector(const std::wstring& str);
	static bool StringToBool(const std::wstring& str);

};

}// namespace Meepo

#endif	// _Meepo_StringHelper_h_
