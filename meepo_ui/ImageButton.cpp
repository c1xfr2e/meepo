#include "ImageButton.h"
#include "Image.h"
#include "InputEvent.h"
#include "DUIWindow.h"

namespace Meepo
{

const StdStringW CImageButton::EventClicked(L"Clicked");

CAlphaAnimation CImageButton::ms_alphaAnimation;

CImageButton::CImageButton()
    : m_bIsHoverd(false), m_bIsPushed(false), 
    m_pNormalImage(0), m_pHoverImage(0), m_pPushImage(0), m_pDisableImage(0), 
    m_iAlpha(0)
{
    m_mouseEnterAniInst.SetAnimation(&ms_alphaAnimation.m_aniMouseEnter);
    m_mouseEnterAniInst.SetTargetObject(this);
    m_mouseLeaveAniInst.SetAnimation(&ms_alphaAnimation.m_aniMouseLeave);
    m_mouseLeaveAniInst.SetTargetObject(this);
}

CImageButton::~CImageButton()
{
}

void CImageButton::OnMouseEnter(CMouseEventArgs& args)
{
    CElement::OnMouseEnter(args);

    m_bIsHoverd = true;

    m_mouseEnterAniInst.Start();
    m_mouseLeaveAniInst.Stop();
    
    m_pDUIWnd->SetRedrawFlag(true);

    //Draw(m_pDUIWnd->GetRenderBuffer(), true, true);
    //m_pDUIWnd->FlushRenderBuffer();
}

void CImageButton::OnMouseLeave(CMouseEventArgs& args)
{
    CElement::OnMouseLeave(args);

    m_bIsHoverd = false;
    m_bIsPushed = false;

    m_mouseEnterAniInst.Stop();
    m_mouseLeaveAniInst.Start();

    m_pDUIWnd->SetRedrawFlag(true);

    //Draw(m_pDUIWnd->GetRenderBuffer(), true, true);
    //m_pDUIWnd->FlushRenderBuffer();
}

void CImageButton::OnMouseButtonDown(CMouseEventArgs& args)
{
    CElement::OnMouseButtonDown(args);

    if (args.m_button == MouseButton_Left)
    {
        GetMouseCapture();
        m_bIsPushed = true;
        args.m_bHandled = true;
        //Draw(m_pDUIWnd->GetRenderBuffer(), true, true);
        //m_pDUIWnd->UpdateDisplay(0);


        m_mouseEnterAniInst.Stop();
        m_mouseLeaveAniInst.Stop();
        m_iAlpha = 255;
        
        RedrawOnChange();
    }
}

void CImageButton::OnMouseButtonUp(CMouseEventArgs& args)
{
    CElement::OnMouseButtonUp(args);
    if (args.m_button == MouseButton_Left)
    {
        ReleaseMouseCapture();
        bool flag_fireClickEvent= false;
        if (m_bIsPushed)
        {
            if (m_pixelRect.IsPointInRect(m_pDUIWnd->GetCurrentMousePosition()))
            {
                flag_fireClickEvent = true;
                m_bIsPushed = false;
            }
            else
            {
                CMouseEventArgs args_(this);
                OnMouseLeave(args_);
            }
        }

        //Draw(m_pDUIWnd->GetRenderBuffer(), true, true);
        //m_pDUIWnd->UpdateDisplay(0);

        RedrawOnChange();

        if (flag_fireClickEvent)
            PublishEvent(EventClicked, args);
    }
}

void CImageButton::DrawSelf(CDCBuffer& buffer)
{
    CMyRect rc = m_pixelRect;
    //CMyRect.InverseOffset(buffer.GetOffset());

    if (!IsEnabled())
    {
        m_pDisableImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());
    }
    else if (IsPushed())
    {
        m_pPushImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());
    }
    else if (IsHovered())
    {
        m_pNormalImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());
        m_pHoverImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height(), m_iAlpha);
    }
    else
    {
        m_pNormalImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());
        if (m_iAlpha != 0)
            m_pHoverImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height(), m_iAlpha);
    }

}

void CImageButton::SetContainerWnd(CDUIWindow* root)
{
    CElement::SetContainerWnd(root);
    root->AddAnimationInstance(&m_mouseEnterAniInst);
    root->AddAnimationInstance(&m_mouseLeaveAniInst);
}

void CImageButton::SetAlpha(int alpha, bool redraw/* = false*/)
{ 
    m_iAlpha = alpha; 
    if (redraw)
        RedrawOnChange();
}

}
