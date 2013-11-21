/***********************************************************************
    filename:   CGDIUtil.h
    created:    2011/11/20
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include <windows.h>

namespace Meepo
{

class CGDIUtil
{
public:
	static HBITMAP CreateDefaultDIBSection(int width, int height);

    static void StretchWithFixedLeftRight(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, 
                                          HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc, 
                                          int nLeft, int nRight);
    static void StretchWithFixedTopBottom(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, 
                                          HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc, 
                                          int nTop, int nBottom);
    static void StretchWithFixedLeftRightTopBottom(
		HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, 
		HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc, 
		int nLeft, int nRight, int nTop, int nBottom);
};

}