/***********************************************************************
    filename:   StaticImage.cpp
    created:    2012/6/10
    author:     ZhaoHao
*************************************************************************/
#include "DCBuffer.h"
#include "Image.h"
#include "StaticImage.h"
#include "Event.h"
#include "ResourceManager.h"
#include "DUIWindow.h"

namespace Meepo
{

CStaticImage::CStaticImage() 
    : m_image(0), m_alpha(255)
{
}

void CStaticImage::SetImage(CImage* image, bool redraw/*= false*/)
{
    m_image = image;
    if (redraw)
        RedrawOnChange();
}

void CStaticImage::SetImage(const StdStringW& name, bool redraw/*= false*/)
{
    SetImage(CResourceManager::Instance().GetImage(name), redraw);
}

void CStaticImage::DrawSelf(CDCBuffer& renderBuffer)
{
    if (!m_image)
        return;
    m_image->Draw(renderBuffer, m_pixelRect.m_left, m_pixelRect.m_top, 
        m_pixelRect.Width(), m_pixelRect.Height(), m_alpha);
}

void CStaticImage::SetAlpha(int alpha)
{
    m_alpha = alpha;
}

void CStaticImage::SetAttribute(const StdStringW& name, const StdStringW& val)
{
	CElement::SetAttribute(name, val);
	if (name == L"image")
	{
		SetImage(val);
	}
}

}