/***********************************************************************
    filename:   DCBuffer.h
    created:    2012/6/3
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include "PreHeader.h"
#include "Region.h"

namespace Meepo
{

class CDCBuffer
{
public:
	CDCBuffer();
	~CDCBuffer();

	HDC	GetMemoryDC();
	bool Create(int width, int height);
    void Clear();
    bool Resize(int width, int height);
	void CopyTo(HDC hdc, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY);
	void BlendTo(HDC hdc, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcConstAlpha = 255);
    void SetClipRegion(CRegion& region);
    void SetClipRect(const CMyRect& rect);
    void IntersectClipRegion(CRegion& region);
    void IntersectClipRect(const CMyRect& rc);
    void UnionClipRect(const CMyRect& rc);
    void GetClipRegion(CRegion& out);
    int GetHeight() const;
    int GetWidth() const;

private:
	void Release();
    
    int m_nWidth;
    int m_nHeight;

	HDC		m_hMemoryDC;
	HBITMAP	m_hDCBitmap;
	HBITMAP	m_hOldBitmap;
};

}
