#pragma once

#include "PreHeader.h"
#include "Element.h"

namespace Meepo
{

class CStateImage : public CElement
{
public:
    static const StdStringW EventClicked; // mouse click event

    CStateImage();
    ~CStateImage();

    bool IsHovered() const { return m_bIsHoverd; }
    bool IsPushed()  const { return m_bIsPushed; }
    
    void SetNormalImage(CImage* image) { m_pNormalImage = image; }
    void SetHoverImage(CImage* image)  { m_pHoverImage = image; }
    void SetPushImage(CImage* image)   { m_pPushImage = image; }
    void SetDisableImage(CImage* image) { m_pDisableImage = image; }
	void SetSelectedImage(CImage* image) { m_pSelectedImage = image; }

	virtual void SetAttribute(const StdStringW& name, const StdStringW& val);

protected:
    bool m_bIsHoverd;
    bool m_bIsPushed;

    CImage*	m_pNormalImage;
    CImage*	m_pHoverImage;
    CImage*	m_pPushImage;
    CImage*	m_pDisableImage;
	CImage*	m_pSelectedImage;

    int m_iHoverAlpha;

    virtual void OnMouseEnter(CMouseEventArgs& arg);
    virtual void OnMouseLeave(CMouseEventArgs& e);
    virtual void OnMouseButtonDown(CMouseEventArgs& e);
    virtual void OnMouseButtonUp(CMouseEventArgs& e);

    virtual void DrawSelf(CDCBuffer& buffer);
};

}
