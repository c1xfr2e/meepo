/***********************************************************************
    filename:   StaticImage.h
    created:    2012/6/10
    author:     ZhaoHao
*************************************************************************/
#include "PreHeader.h"
#include "Element.h"
#include "Image.h"

#pragma once

namespace Meepo
{

class CStaticImage : public CElement
{
public:
    CStaticImage();
    void SetImage(CImage* image, bool redraw = false);
    void SetImage(const StdStringW& name, bool redraw = false);

    virtual void DrawSelf(CDCBuffer& renderBuffer); 
    virtual void SetAlpha(int alpha);
	virtual void SetAttribute(const StdStringW& name, const StdStringW& val);  

private:
    CImage* m_image;
    int m_alpha;
};

}
