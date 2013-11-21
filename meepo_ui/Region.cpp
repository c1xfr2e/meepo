/***********************************************************************
    filename:   Region.cpp
    created:    2012/6/17
    author:     ZhaoHao
*************************************************************************/
#include "Region.h"
#include <WindowsX.h>

namespace Meepo
{

CRegion::CRegion(HRGN hRgn) : m_hRgn(hRgn)
{
    if (!hRgn)
        m_hRgn = CreateRectRgn(0, 0, 0, 0);
}

CRegion::~CRegion()
{
    if (m_hRgn)
    {
        DeleteRgn(m_hRgn);
    }
}

void CRegion::SetPolygon(const CMyPoint* pts, int ptCount)
{
	if (m_hRgn != 0) 
		DeleteRgn(m_hRgn);

	m_hRgn = CreatePolygonRgn((POINT*)pts, ptCount, ALTERNATE);
}

void CRegion::SetRect(const CMyRect& rect)
{
	if (m_hRgn != 0) 
		DeleteRgn(m_hRgn);

	m_hRgn = CreateRectRgn(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom);
}

bool CRegion::IsPointIn(const CMyPoint& point)
{
	if (PtInRegion(m_hRgn, point.m_x, point.m_y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CRegion::Intersect(HRGN hrgn)
{
	IntersectRgn(m_hRgn, m_hRgn, hrgn);
}

void CRegion::Union(HRGN hrgn)
{
	UnionRgn(m_hRgn, m_hRgn, hrgn);
}

void CRegion::CopyHRGN(HRGN hrgn)
{
	CopyRgn(m_hRgn, hrgn);
}

} 
