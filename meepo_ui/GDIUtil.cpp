/***********************************************************************
    filename:   GDIUtil.cpp
    created:    2011/12/7
    author:     ZhaoHao
*************************************************************************/
#include "GDIUtil.h"

namespace Meepo
{

HBITMAP Meepo::CGDIUtil::CreateDefaultDIBSection(int width, int height)
{  
	BITMAPINFOHEADER	bmih;   
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));   

	bmih.biSize				= sizeof(BITMAPINFOHEADER) ;   
	bmih.biWidth			= width;   
	bmih.biHeight			= height;   
	bmih.biPlanes			= 1;   
	bmih.biBitCount			= 32;
	bmih.biCompression		= BI_RGB;   
	bmih.biSizeImage		= 0;   
	bmih.biXPelsPerMeter	= 0;   
	bmih.biYPelsPerMeter	= 0;   
	bmih.biClrUsed			= 0;   
	bmih.biClrImportant		= 0;   

	BYTE*	pBits = NULL;   
	HBITMAP hBitMap = CreateDIBSection (NULL, (BITMAPINFO*) &bmih, 0, (VOID**)&pBits, NULL, 0) ;  

	return hBitMap;  
} 

void Meepo::CGDIUtil::StretchWithFixedLeftRight(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, 
                                       HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc, int nLeft, int nRight)
{
    // fix left
    BitBlt(hdcDest, xDest, yDest, nLeft, cySrc, hdcSrc, xSrc, ySrc, SRCCOPY);
    // fix right
    BitBlt(hdcDest, xDest + cxDest - nRight, yDest, nRight, cySrc, hdcSrc, xSrc + cxSrc - nRight, ySrc, SRCCOPY);
    // stretch mid
    StretchBlt(hdcDest, xDest + nLeft, yDest, cxDest - nLeft - nRight, cySrc, 
        hdcSrc, xSrc + nLeft, ySrc, cxSrc - nLeft - nRight, cySrc, SRCCOPY);
}

void Meepo::CGDIUtil::StretchWithFixedTopBottom(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, 
                                       HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc, int lenTop, int lenBottom)
{
    // fix top
    BitBlt(hdcDest, xDest, yDest, cxSrc, lenTop, hdcSrc, xSrc, ySrc, SRCCOPY);
    // fix bottom
    BitBlt(hdcDest, xDest, yDest + cyDest - lenBottom, cxSrc, lenBottom, hdcSrc, xSrc, ySrc + cySrc - lenBottom, SRCCOPY);
    // stretch mid
    StretchBlt(hdcDest, xDest, yDest + lenTop, cxSrc, cyDest - lenTop - lenBottom,
        hdcSrc, xSrc, ySrc + lenTop, cxSrc, cySrc - lenTop - lenBottom, SRCCOPY);
}

void Meepo::CGDIUtil::StretchWithFixedLeftRightTopBottom(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest, HDC hdcSrc,
                                        int xSrc, int ySrc, int cxSrc, int cySrc, int nLeft, int nRight, int lenTop, int lenBottom)
{
    if (cyDest >= cySrc)
    {
        StretchWithFixedLeftRight(hdcDest, xDest, yDest, cxDest, cySrc, hdcSrc, xSrc, ySrc, cxSrc, cySrc, nLeft, nRight);
        StretchWithFixedTopBottom(hdcDest, xDest, yDest, cxDest, cyDest, hdcDest, xDest, yDest, cxDest, cySrc, lenTop, lenBottom);
    }
    else
    {
        StretchWithFixedTopBottom(hdcDest, xDest, yDest, cxSrc, cyDest, hdcSrc, xSrc, ySrc, cxSrc, cySrc, lenTop, lenBottom);
        StretchWithFixedLeftRight(hdcDest, xDest, yDest, cxDest, cyDest, hdcDest, xSrc, ySrc, cxSrc, cyDest, nLeft, nRight);
    }
}

}

	/*
	BOOL AlphaBlt(
		BYTE *pDest,
		DWORD wlined,
		DWORD hlined,
		int startX,
		int startY,
		BYTE *pSrc,
		DWORD wlines,
		DWORD hlines,
		DWORD Alpha
		) 
	{ 
		int Xd,Yd,Xs=0,Ys=0; 
		int loopH,loopW; 
		if (startX>=(int)wlined) return FALSE; 
		if (startY>=(int)hlined) return FALSE; 

		if (startX + (int)wlines <=0) return FALSE; 
		if (startY + (int)hlines <=0) return FALSE; 

		Xd = startX; 
		Yd = startY; 
		loopW = (startX + wlines); 
		loopH = (startY + hlines); 
		if (loopH>hlined) loopH = hlined; 
		if (loopW>wlined) loopW = wlined; 
		if (startX<0) 
		{ 
			Xd = 0; 
			Xs = -startX; 
		} 
		if (startY<0) 
		{ 
			Yd = 0; 
			Ys = -startY;  
		} 

		DWORD factorA = hlines-Ys+Yd-1; 
		for (DWORD j= Yd ; j< loopH ; j++) 
		{ 
			DWORD dwOffSrc,dwOffDest; 
			dwOffSrc = (factorA-j)*wlines; 

			dwOffDest = (hlined-j-1)*wlined; 
			DWORD srcPosX = Xs; 
			for (DWORD i = Xd ; i< loopW ; i++) 
			{ 

				BYTE *BufSrc = pSrc + (dwOffSrc + srcPosX)*4; 
				DWORD dwSrc =((DWORD)BufSrc[3] << 24) |((DWORD)BufSrc[2] << 16) | ((WORD)BufSrc[1] << 8) | (BufSrc[0]); 
				DWORD *dwpDest = (DWORD *)pDest + dwOffDest + i; 
				__asm{ 
					pxor mm2,mm2  
						mov edx,dwpDest 
						movd mm0,[edx] 
					movd mm1,dwSrc 
						punpcklbw mm0,mm2  
						punpcklbw mm1,mm2  
						movq mm3,mm1 
						punpckhwd mm3,mm3  
						punpckhdq mm3,mm3  
						movd mm4,Alpha  
						punpcklwd mm4,mm4 
						punpckldq mm4,mm4  
						pmullw mm3,mm4 //Alpha * SrcAlpha 
						psrlw mm3,8 
						movq mm4,mm0  
						movq mm5,mm1  
						psubusw mm4,mm1  
						psubusw mm5,mm0  
						pmullw mm4,mm3  
						pmullw mm5,mm3  
						psrlw mm4,8  
						psrlw mm5,8 
						paddusw mm0,mm5  
						psubusw mm0,mm4 
						packuswb mm0,mm0 

						movd [edx],mm0 //±£´æ½á¹û 
						emms  
				}  

				srcPosX++; 
			} 
		} 
		return TRUE; 
	}
	*/