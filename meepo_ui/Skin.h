/***********************************************************************
    filename:   Skin.h
    created:    2012/6/2
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include "PreHeader.h"
#include "GdiplusHeader.h"
#include "DCCache.h"

namespace Meepo
{

// CSkin 
// 对 Gdiplus 中的 Image 类做一层封装
// 1. 在内存中做一个缓存，初始化时把图像绘制到缓存里，后续绘制都使用缓存，从而提高绘制效率
// 2. 支持绘制同一张图片的不同区域

enum EStretchMode
{
    Stretch_Mode_Default,
    Stretch_Mode_Fixed,
    Stretch_Mode_X,
    Stretch_Mode_Y,
    Stretch_Mode_XY,
    Stretch_Mode_Tiled
};

class CStretchOption
{	
public:
    CStretchOption() 
		: m_stretchMode(Stretch_Mode_Default), m_nLeft(0), m_nRight(0), m_nTop(0), m_nBottom(0) 
	{}

    EStretchMode m_stretchMode;
    int m_nLeft;
    int m_nRight;
    int m_nTop;
    int m_nBottom;
};

class CSkin
{
public:
	CSkin();
	~CSkin();

	Gdiplus::Image* GetGdiplusImage() { return m_pGdiplusImage; }
	int GetWidth() const;
	int GetHeight() const;
	bool HasAlphaChannel() const;
	void Load(const StdStringW& strImageFilePath);
    void Load(CRawData& rawData);
	void Draw(HDC hdc, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha = 255);

private:
    void Init();
	void DrawFixed(HDC destDC, int xDest, int yDest, int xSrc, int ySrc, int srcWidth, int srcHeight, int alpha);
	void DrawFixedYStretchX(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha);
	void DrawFixedXStretchY(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha);
	void DrawFixedStretchXY(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, const CStretchOption& stretchOption, int alpha);
	void TransferBits(HDC destDC, int xDest, int yDest, int destWidth, int destHeight, HDC srcDC, int xSrc, int ySrc, int srcWidth, int srcHeight, int alpha);

	Gdiplus::Image*	m_pGdiplusImage;

	HBITMAP			m_hBitmapBuffer;
	HDC				m_hDCBuffer;
	HBITMAP			m_hOldBitmapOfDCBuffer;

	int				m_nWidth;
	int				m_nHeight; 

	static CDCCache ms_dcCache;
};

}
