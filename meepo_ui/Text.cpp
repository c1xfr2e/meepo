/***********************************************************************
    filename:   Text.cpp
    created:    2013/2/28
    author:     ZhaoHao
*************************************************************************/
#include "DCBuffer.h"
#include "Text.h"

using namespace Gdiplus;

namespace Meepo
{

CText::CText() 
{
    FontFamily* pFontFamily = new FontFamily(L"Î¢ÈíÑÅºÚ");
    int style = FontStyleRegular;
    m_pFont = new Font(pFontFamily, (float)15, style, UnitPixel);
    delete pFontFamily;
    m_bIsMousePassThrough = true;
}

void CText::SetText(const StdStringW& text)
{
	m_text = text;
}

void CText::DrawSelf(CDCBuffer& renderBuffer)
{
    Gdiplus::Graphics graphics(renderBuffer.GetMemoryDC());
    Gdiplus::Color strColor(254, 33, 66, 99);
    SolidBrush  solidBrush(strColor);

    graphics.DrawString(L"È·¶¨", -1, m_pFont, 
        Gdiplus::PointF((float)m_pixelRect.m_left + 10, (float)m_pixelRect.m_top - 5), &solidBrush);
}

void CText::SetAttribute(const StdStringW& name, const StdStringW& val)
{
	CElement::SetAttribute(name, val);
	if (name == L"text")
		SetText(val);
}

}