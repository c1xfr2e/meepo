/***********************************************************************
    filename:   CImage.h
    created:    2012/6/3
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include "PreHeader.h"
#include "Rect.h"
#include "Skin.h"

namespace Meepo
{

class CImage
{
public:
	CImage(const StdStringW& name, CSkin* skin, int xPos, int yPos, int width, int height);

	const StdStringW& GetName() const;
	int GetWidth() const;
	int GetHeight() const;
	CMyRect GetArea() const;
	CSkin* GetOwningSkin() const;
	void SetOwningSkin(CSkin* pSkin);
    void SetStretchOption(const CStretchOption& stretchOption);
	bool HasAlphaChannel() const;
	void Draw(CDCBuffer& targetBuffer, int destX, int destY, 
              int destWidth, int destHeight, int alpha = 255);

private:
	StdStringW m_strName;
	CSkin* m_pOwningSkin;
	CMyRect m_rcSkinArea;
    CStretchOption m_stretchOption;
};

}
