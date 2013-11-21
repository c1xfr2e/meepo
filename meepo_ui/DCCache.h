/***********************************************************************
    filename:   CDCCache.h
    created:    2011/12/17
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include <windows.h>

namespace Meepo
{

// HDC 缓存机制，摘自 ATL CImage 类的代码

class CDCCache
{
public:
	CDCCache();
	~CDCCache();

	HDC  GetDC();
	void ReleaseDC(HDC hdc);

private:
	enum  { DC_CACHE_SIZE = 20 };
	HDC m_HDCs[DC_CACHE_SIZE];
};

}