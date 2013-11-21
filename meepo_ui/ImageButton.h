#pragma once

#include "PreHeader.h"
#include "Element.h"
#include "Animation.h"
#include "Affectors.h"
#include "AnimationInstance.h"

namespace Meepo
{

class CAlphaAnimation
{
public:
    CAlphaAnimation()
        : m_aniMouseEnter(200), m_aniMouseLeave(200)
    {
        m_affectorMouseEnter.AddKeyFrame(CKeyFrame<int>(0, 0));
        m_affectorMouseEnter.AddKeyFrame(CKeyFrame<int>(200, 255));
        m_aniMouseEnter.AddAffector(&m_affectorMouseEnter);

        m_affectorMouseLeave.AddKeyFrame(CKeyFrame<int>(0, 255));
        m_affectorMouseLeave.AddKeyFrame(CKeyFrame<int>(200, 0));
        m_aniMouseLeave.AddAffector(&m_affectorMouseLeave);
    }

    CAnimation m_aniMouseEnter;
    CAlphaAffector m_affectorMouseEnter;
    
    CAnimation m_aniMouseLeave;
    CAlphaAffector m_affectorMouseLeave;
};

class CImageButton : public CElement
{
public:
    static const StdStringW EventClicked; // mouse click event

    CImageButton();
    ~CImageButton();

    bool IsHovered() const { return m_bIsHoverd; }
    bool IsPushed()  const { return m_bIsPushed; }
    
    void SetNormalImage(CImage* image) { m_pNormalImage = image; }
    void SetHoverImage(CImage* image)  { m_pHoverImage = image; }
    void SetPushImage(CImage* image)   { m_pPushImage = image; }
    void SetDisableImage(CImage* image) { m_pDisableImage = image; }

    virtual void SetAlpha(int alpha, bool redraw = false);

    virtual void SetContainerWnd(CDUIWindow* root);    

protected:
    bool m_bIsHoverd;
    bool m_bIsPushed;

    CImage*	m_pNormalImage;
    CImage*	m_pHoverImage;
    CImage*	m_pPushImage;
    CImage*	m_pDisableImage;

    int m_iAlpha;

    virtual void OnMouseEnter(CMouseEventArgs& arg);
    virtual void OnMouseLeave(CMouseEventArgs& e);
    virtual void OnMouseButtonDown(CMouseEventArgs& e);
    virtual void OnMouseButtonUp(CMouseEventArgs& e);

    virtual void DrawSelf(CDCBuffer& buffer);

    static CAlphaAnimation ms_alphaAnimation;

    CAnimationInstance m_mouseEnterAniInst;
    CAnimationInstance m_mouseLeaveAniInst;

};

}
