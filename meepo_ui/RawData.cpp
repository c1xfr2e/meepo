/***********************************************************************
    filename:   RawData.cpp
    created:	2012/6/2
    author:     ZhaoHao
*************************************************************************/
#include "RawData.h"

namespace Meepo
{

void* CRawData::GetBuf()
{
    return m_pBuf;
}

unsigned int CRawData::GetSize()
{
    return m_nSize;
}

char& CRawData::operator[](int index)
{
	return ((char*)m_pBuf)[index];
}

CHeapData::~CHeapData(void) 
{ 
    Dealloc();
}

void* CHeapData::Alloc(unsigned int size)
{
    m_pBuf = operator new(size);
    m_nSize = size;
    return m_pBuf;
}

void CHeapData::Dealloc()
{
    if(m_pBuf)
    {
        delete m_pBuf;
        m_pBuf = 0;
        m_nSize = 0;
    }
}


CGlobalData::CGlobalData() : m_hGlobal(0) 
{}

CGlobalData::~CGlobalData(void) 
{ 
    Dealloc();
}

void* CGlobalData::Alloc(unsigned int size)
{
    m_hGlobal = ::GlobalAlloc(GMEM_FIXED, size);
    m_pBuf = (char*)::GlobalLock(m_hGlobal);
    m_nSize = size;
    return m_pBuf;
}

void CGlobalData::Dealloc()
{
    if(m_hGlobal)
    {
        ::GlobalUnlock(m_hGlobal);
        ::GlobalFree(m_hGlobal);
        m_hGlobal = 0;
        m_pBuf = 0;
        m_nSize = 0;
    }
}

}