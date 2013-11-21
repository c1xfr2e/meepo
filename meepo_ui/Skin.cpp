/***********************************************************************
    filename:   Skin.cpp
    created:    2012/6/2
    author:     ZhaoHao
*************************************************************************/
#include "Skin.h"
#include "GDIUtil.h"
#include "ResourceManager.h"

using namespace Gdiplus;

namespace Meepo
{

Meepo::CDCCache CSkin::ms_dcCache;

CSkin::CSkin() : 
	m_pGdiplusImage(0),
	m_hBitmapBuffer(0),
	m_hOldBitmapOfDCBuffer(0),
	m_hDCBuffer(0),
	m_nWidth(0),
	m_nHeight(0)
{}

CSkin::~CSkin()
{
	delete m_pGdiplusImage;
	m_pGdiplusImage = NULL;
	if (m_hBitmapBuffer) 
	{
		DeleteObject(m_hBitmapBuffer);
	}
	if (m_hDCBuffer)
	{
		SelectObject(m_hDCBuffer, m_hOldBitmapOfDCBuffer);
		ms_dcCache.ReleaseDC(m_hDCBuffer);
	}
}

int CSkin::GetWidth() const
{
	return m_nWidth;
}

int CSkin::GetHeight() const 
{
	return m_nHeight;
}

bool CSkin::HasAlphaChannel() const
{
	PixelFormat pf = m_pGdiplusImage->GetPixelFormat();
	int alpha = (pf & PixelFormatAlpha);
	return alpha != 0;
}

void CSkin::Load(const StdStringW& strFilePath)
{
    m_pGdiplusImage = Gdiplus::Image::FromFile(strFilePath.c_str());
    Init();
}

void CSkin::Load(CRawData& rawData)
{
    IStream* pStm = NULL;
    ::CreateStreamOnHGlobal((HGLOBAL)rawData.GetBuf(), FALSE, &pStm);
    m_pGdiplusImage = Gdiplus::Image::FromStream(pStm);
    pStm->Release();
    Init();
}

void CSkin::Init()
{
    m_nWidth  = m_pGdiplusImage->GetWidth();
    m_nHeight = m_pGdiplusImage->GetHeight();

    m_hBitmapBuffer = CGDIUtil::CreateDefaultDIBSection(m_pGdiplusImage->GetWidth(), m_pGdiplusImage->GetHeight());
    m_hDCBuffer = ms_dcCache.GetDC();
    m_hOldBitmapOfDCBuffer = (HBITMAP)::SelectObject(m_hDCBuffer, m_hBitmapBuffer);

    Gdiplus::ImageAttributes imAtt; 	
    imAtt.SetWrapMode(Gdiplus::WrapModeTile);
    Gdiplus::Graphics memGraphics(m_hDCBuffer);
    memGraphics.SetCompositingMode(CompositingModeSourceOver);
    memGraphics.SetCompositingQuality(CompositingQualityHighSpeed);

	// 在这里可以应用一些色彩和alpha参数

    //BitBlt(m_hDCBuffer, 0, 0, m_nWidth, m_nHeight, NULL, 0, 0, BLACKNESS);
    memGraphics.DrawImage(
        m_pGdiplusImage, Gdiplus::Rect(0, 0, m_nWidth, m_nHeight), 0, 0, m_nWidth, m_nHeight, 
		Gdiplus::UnitPixel, &imAtt, NULL, NULL
        );
}

void CSkin::Draw(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha/* = 255*/)
{
	switch (stretchOption.m_stretchMode)
	{
	case Stretch_Mode_XY:
		DrawFixedStretchXY(destDC, xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, stretchOption, alpha);
		break;

	case Stretch_Mode_X:
		DrawFixedYStretchX(destDC, xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, stretchOption, alpha);
		break;

	case Stretch_Mode_Y:
		DrawFixedXStretchY(destDC, xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, stretchOption, alpha);
		break;

	case Stretch_Mode_Fixed:
		DrawFixed(destDC, xDest, yDest, xSrc, ySrc, srcWidth, srcHeight, alpha);
		break;

	default:
		TransferBits(destDC, xDest, yDest, destWidth, destHeight, m_hDCBuffer, xSrc, ySrc, srcWidth, srcHeight, alpha);
		break;
	}
}

void CSkin::TransferBits(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, HDC srcDC, int xSrc, int ySrc, int srcWidth, int srcHeight, int alpha)
{
	if (HasAlphaChannel() || alpha < 255)
	{
		BLENDFUNCTION blendFunction = {0};
		blendFunction.BlendOp = AC_SRC_OVER;  
		blendFunction.BlendFlags = 0;
		blendFunction.AlphaFormat = AC_SRC_ALPHA;
		blendFunction.SourceConstantAlpha = alpha;
		AlphaBlend(destDC, xDest, yDest, destWidth, destHeight, srcDC, xSrc, ySrc, srcWidth, srcHeight, blendFunction);
	}
	else
	{
		BitBlt(destDC, xDest, yDest, destWidth, destHeight, srcDC, xSrc, ySrc, SRCCOPY);
	}
}

void CSkin::DrawFixedStretchXY(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha)
{
	if (destWidth <= stretchOption.m_nLeft + stretchOption.m_nRight)
	{
		DrawFixedXStretchY(destDC, xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, stretchOption, alpha);
		return;
	}

	if (destHeight <= stretchOption.m_nTop + stretchOption.m_nBottom)
	{
		DrawFixedYStretchX(destDC, xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, stretchOption, alpha);
		return;
	}

	HBITMAP tmpBmp = CGDIUtil::CreateDefaultDIBSection(destWidth, destHeight);
	HDC tmpDC = ms_dcCache.GetDC();
	HBITMAP oldBitmap = (HBITMAP)::SelectObject(tmpDC, tmpBmp);

	CGDIUtil::StretchWithFixedLeftRightTopBottom(tmpDC, 0, 0, destWidth, destHeight, 
        m_hDCBuffer, xSrc, ySrc, srcWidth, srcHeight, stretchOption.m_nLeft, stretchOption.m_nRight, stretchOption.m_nTop, stretchOption.m_nBottom);

	TransferBits(destDC, xDest, yDest, destWidth, destHeight, tmpDC, 0, 0, destWidth, destHeight, alpha);

	DeleteObject(tmpBmp);
	SelectObject(tmpDC, oldBitmap);
	ms_dcCache.ReleaseDC(tmpDC);
}

void CSkin::DrawFixedYStretchX(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha)
{
	if (destWidth <= stretchOption.m_nLeft + stretchOption.m_nRight)
	{
		DrawFixed(destDC, xDest, yDest, xSrc, ySrc, srcWidth, srcHeight, alpha);
		return;
	}

	HBITMAP tmpBmp = CGDIUtil::CreateDefaultDIBSection(destWidth, destHeight);
	HDC tmpDC = ms_dcCache.GetDC();
	HBITMAP oldBitmap = (HBITMAP)::SelectObject(tmpDC, tmpBmp);

	CGDIUtil::StretchWithFixedLeftRight(tmpDC, 0, 0, destWidth, destHeight, 
        m_hDCBuffer, xSrc, ySrc, srcWidth, srcHeight, stretchOption.m_nLeft, stretchOption.m_nRight);

	TransferBits(destDC, xDest, yDest, destWidth, destHeight, tmpDC, 0, 0, destWidth, destHeight, alpha);

	DeleteObject(tmpBmp);
	SelectObject(tmpDC, oldBitmap);
	ms_dcCache.ReleaseDC(tmpDC);
}

void CSkin::DrawFixedXStretchY(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha)
{
	if (destHeight <= stretchOption.m_nTop + stretchOption.m_nBottom)
	{
		DrawFixed(destDC, xDest, yDest, xSrc, ySrc, srcWidth, srcHeight, alpha);
		return;
	}

	HBITMAP tmpBmp = CGDIUtil::CreateDefaultDIBSection(destWidth, destHeight);
	HDC tmpDC = ms_dcCache.GetDC();
	HBITMAP oldBitmap = (HBITMAP)::SelectObject(tmpDC, tmpBmp);

	CGDIUtil::StretchWithFixedTopBottom(tmpDC, 0, 0, destWidth, destHeight, 
        m_hDCBuffer, xSrc, ySrc, srcWidth, srcHeight, stretchOption.m_nTop, stretchOption.m_nBottom);

	TransferBits(destDC, xDest, yDest, destWidth, destHeight, tmpDC, 0, 0, destWidth, destHeight, alpha);

	DeleteObject(tmpBmp);
	SelectObject(tmpDC, oldBitmap);
	ms_dcCache.ReleaseDC(tmpDC);
}

void CSkin::DrawFixed(HDC destDC, int xDest, int yDest, int xSrc, int ySrc, int srcWidth, int srcHeight, int alpha)
{
	TransferBits(destDC, xDest, yDest, srcWidth, srcHeight, m_hDCBuffer, xSrc, ySrc, srcWidth, srcHeight, alpha);
}

}