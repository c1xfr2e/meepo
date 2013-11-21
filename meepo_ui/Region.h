/***********************************************************************
    filename:   Region.h
    created:    2012/6/17
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include <windows.h>
#include "Rect.h"

namespace Meepo
{

class CRegion
{
public:
	CRegion(HRGN hRgn = NULL);
	~CRegion();

	void SetRect(const CMyRect& rect);
	void SetPolygon(const CMyPoint* pts, int ptCount);
	bool IsPointIn(const CMyPoint& point);
	void Intersect(HRGN hrgn);
	void Union(HRGN hrgn);

	void CopyHRGN(HRGN hrgn);
	HRGN GetHRGN() { return m_hRgn; }

private:
	HRGN m_hRgn;
};

}