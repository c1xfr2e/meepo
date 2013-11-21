/***********************************************************************
    filename:   RenderBuffer.cpp
    created:    2012/6/2
    author:     ZhaoHao
*************************************************************************/
#include "DCBuffer.h"
#include "GdiplusHeader.h"
#include "GDIUtil.h"

namespace Meepo
{

using namespace Gdiplus;

CDCBuffer::CDCBuffer() : 
    m_nWidth(0),
    m_nHeight(0),
	m_hMemoryDC(0),
	m_hDCBitmap(0),
	m_hOldBitmap(0)
{
}

CDCBuffer::~CDCBuffer(void)
{
	Release();
}

void CDCBuffer::Release()
{
	if (m_hDCBitmap) 
	{
		::DeleteObject(m_hDCBitmap);
	}

	if (m_hMemoryDC)
	{
        if (m_hOldBitmap)
        {
            SelectObject(m_hMemoryDC, m_hOldBitmap);
            m_hOldBitmap = 0;
        }
		::DeleteDC(m_hMemoryDC);
		m_hMemoryDC = 0;
	}
}

HDC CDCBuffer::GetMemoryDC()
{
	return m_hMemoryDC;
}

bool CDCBuffer::Create(int width, int height)
{
	m_nWidth	= width;
	m_nHeight	= height;

	m_hMemoryDC = ::CreateCompatibleDC(NULL);
	m_hDCBitmap = CGDIUtil::CreateDefaultDIBSection(width, height);
	m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemoryDC, m_hDCBitmap);

    return (m_hMemoryDC != NULL && m_hDCBitmap != NULL);
}

bool CDCBuffer::Resize(int width, int height)
{
    Release();
    return Create(width, height);
}

void CDCBuffer::Clear()
{
	BitBlt(m_hMemoryDC, 0, 0, m_nWidth, m_nHeight, NULL, 0, 0, BLACKNESS);
}

void CDCBuffer::CopyTo(HDC hdc, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY)
{
	BitBlt(hdc, destX, destY, destWidth, destHeight, m_hMemoryDC, srcX, srcY, SRCCOPY);
}

void CDCBuffer::BlendTo(HDC hdc, int destX, int destY, int destWidth, int destHeight, 
                            int srcX, int srcY, int srcConstAlpha/* = 255*/)
{
	BLENDFUNCTION blendFunction = {AC_SRC_OVER, 0, AC_SRC_ALPHA, srcConstAlpha};
	AlphaBlend(hdc, destX, destY, destWidth, destHeight, 
		m_hMemoryDC, srcX, srcY, m_nWidth, m_nHeight, blendFunction);
}

void CDCBuffer::SetClipRegion(CRegion& region)
{
    SelectClipRgn(m_hMemoryDC, region.GetHRGN());
}

void CDCBuffer::SetClipRect(const CMyRect& rect)
{
    HRGN hRgn = CreateRectRgn(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom);
    SelectClipRgn(m_hMemoryDC, hRgn);
    DeleteObject(hRgn);
}

void CDCBuffer::IntersectClipRegion(CRegion& region)
{
    HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
    ::GetClipRgn(m_hMemoryDC, hRgn);
    IntersectRgn(hRgn, hRgn, region.GetHRGN());
    SelectClipRgn(m_hMemoryDC, hRgn);
    DeleteObject(hRgn);
}

void CDCBuffer::UnionClipRect(const CMyRect& rc)
{
    HRGN hRectRgn = CreateRectRgn(rc.m_left, rc.m_top, rc.m_right, rc.m_bottom);

    HRGN hCurrentRgn = CreateRectRgn(0, 0, 0, 0);
    ::GetClipRgn(m_hMemoryDC, hCurrentRgn);

    UnionRgn(hCurrentRgn, hCurrentRgn, hRectRgn);

    SelectClipRgn(m_hMemoryDC, hCurrentRgn);

    DeleteObject(hRectRgn);
    DeleteObject(hCurrentRgn);
}

void CDCBuffer::GetClipRegion(CRegion& out)
{
    ::GetClipRgn(m_hMemoryDC, out.GetHRGN());
}

void CDCBuffer::IntersectClipRect(const CMyRect& rc)
{
    ::IntersectClipRect(m_hMemoryDC, rc.m_left, rc.m_top, rc.m_right, rc.m_bottom);
}

int CDCBuffer::GetHeight() const
{
    return m_nHeight;
}

int CDCBuffer::GetWidth() const
{
    return m_nWidth;
}

}
