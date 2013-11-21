/***********************************************************************
    filename:   CDCCache.cpp
    created:    2011/12/17
    author:     ZhaoHao
*************************************************************************/
#include "DCCache.h"

namespace Meepo
{

CDCCache::CDCCache()
{
	for (int i = 0; i < DC_CACHE_SIZE; i++)
		m_HDCs[i] = NULL;
}

CDCCache::~CDCCache()
{
	for (int i = 0; i < DC_CACHE_SIZE; i++)
	{
		if(m_HDCs[i] != NULL)
			::DeleteDC(m_HDCs[i]);
	}
}

HDC CDCCache::GetDC()
{
	HDC hDC;
	for (int i = 0; i < DC_CACHE_SIZE; i++)
	{
		if((hDC = m_HDCs[i]) != NULL)
        {
            m_HDCs[i] = NULL;
            return hDC ;      
        }
	} 

	hDC = ::CreateCompatibleDC(NULL);

	return hDC;
}

void CDCCache::ReleaseDC(HDC hDC)
{
	for (int i = 0; i < DC_CACHE_SIZE; i++)
	{
		if(m_HDCs[i])
		{
            m_HDCs[i] = hDC;
			return;
		}
	}

	if(hDC)
		::DeleteDC(hDC);
}

} // namespace Meepo