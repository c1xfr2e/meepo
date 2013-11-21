/***********************************************************************
    filename:   StringHelper.cpp
    created:    2010/10/1
    author:     ZhaoHao

    purpose:   	
*************************************************************************/
#include "StringHelper.h"
#include <cstdio>

namespace Meepo
{

int StringHelper::StringToInt(const std::wstring& str)
{	
	int val = 0;

	if (!str.empty())
	{
		swscanf(str.c_str(), L"%d", &val);
	}

	return val;
}

std::wstring StringHelper::IntToString(int val)
{
	wchar_t buff[64];
	_snwprintf(buff, sizeof(buff), L"%d", val);

	return std::wstring(buff);
}

Meepo::CRelRect StringHelper::StringToRelRect(const std::wstring& str)
{
	CRelRect rc;
	if (!str.empty()) 
	{
		swscanf(
			str.c_str(), L"{{%g,%g},{%g,%g},{%g,%g},{%g,%g}}",
			&rc.m_left.m_fScale, &rc.m_left.m_fOffset,
			&rc.m_top.m_fScale, &rc.m_top.m_fOffset,
			&rc.m_right.m_fScale, &rc.m_right.m_fScale,
			&rc.m_bottom.m_fScale, &rc.m_bottom.m_fOffset
			);
	}
	return rc;
}

Meepo::CRelVector StringHelper::StringToRelVector(const std::wstring& str)
{
	CRelVector v;
	if (!str.empty())
	{
		swscanf(
			str.c_str(), L"{{%g,%g},{%g,%g}}",
			&v.m_x.m_fScale,
			&v.m_x.m_fOffset, 
			&v.m_y.m_fScale,
			&v.m_y.m_fOffset
			);
	}
	return v;
}

bool StringHelper::StringToBool(const std::wstring& str)
{
	return (str == L"true" || str == L"1");
}

} // namespace Meepo
