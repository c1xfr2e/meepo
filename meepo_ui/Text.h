/***********************************************************************
    filename:   Text.h
    created:    2013/2/28
    author:     ZhaoHao
*************************************************************************/
#include "PreHeader.h"
#include "Element.h"
#include "Image.h"

#pragma once

namespace Meepo
{

class CText : public CElement
{
public:
    CText();
	void SetText(const StdStringW& text);
    virtual void DrawSelf(CDCBuffer& renderBuffer); 
	virtual void SetAttribute(const StdStringW& name, const StdStringW& val);


private:
     Gdiplus::Font* m_pFont;
	 StdStringW m_text;
};

}
