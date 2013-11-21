/***********************************************************************
    filename:   Image.cpp
    created:    2012/6/2
    author:     ZhaoHao
*************************************************************************/
#include "Image.h"
#include "DCBuffer.h"

namespace Meepo
{

CImage::CImage(const StdStringW& name, CSkin* skin, int xPos, int yPos, int width, int height) : 
    m_strName(name),
	m_pOwningSkin(skin),
	m_rcSkinArea(xPos, yPos, width, height)
{}

const StdStringW& CImage::GetName() const
{
	return m_strName;
}

int CImage::GetWidth() const
{
	return m_rcSkinArea.Width();
}

int CImage::GetHeight() const
{
	return m_rcSkinArea.Height();
}

CMyRect CImage::GetArea() const
{
	return m_rcSkinArea;
}

CSkin* CImage::GetOwningSkin() const
{
	return m_pOwningSkin;
}

void CImage::SetOwningSkin(CSkin* pSkin)
{
	m_pOwningSkin = pSkin;
}

void CImage::SetStretchOption(const CStretchOption& stretchOption)
{
	m_stretchOption = stretchOption;
}

bool CImage::HasAlphaChannel() const
{
	return m_pOwningSkin->HasAlphaChannel();
}

void CImage::Draw(CDCBuffer& targetBuffer, int destX, int destY, int destWidth, int destHeight, int alpha)
{
	m_pOwningSkin->Draw(targetBuffer.GetMemoryDC(), destX, destY, destWidth, destHeight,
		m_rcSkinArea.m_left, m_rcSkinArea.m_top, m_rcSkinArea.Width(), m_rcSkinArea.Height(), m_stretchOption, alpha);
} 

}
