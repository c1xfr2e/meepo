#include "StateImage.h"
#include "Image.h"
#include "InputEvent.h"
#include "DUIWindow.h"
#include "ResourceManager.h"

namespace Meepo
{

const StdStringW CStateImage::EventClicked(L"Clicked");


CStateImage::CStateImage()
    : m_bIsHoverd(false), m_bIsPushed(false), 
    m_pNormalImage(0), m_pHoverImage(0), m_pPushImage(0), m_pDisableImage(0), m_pSelectedImage(0)
{
}

CStateImage::~CStateImage()
{
}

void CStateImage::OnMouseEnter(CMouseEventArgs& args)
{
    CElement::OnMouseEnter(args);

    m_bIsHoverd = true;
   
    RedrawOnChange();
}

void CStateImage::OnMouseLeave(CMouseEventArgs& args)
{
    CElement::OnMouseLeave(args);

    m_bIsHoverd = false;
    m_bIsPushed = false;

    RedrawOnChange();
}

void CStateImage::OnMouseButtonDown(CMouseEventArgs& args)
{
    CElement::OnMouseButtonDown(args);

    if (args.m_button == MouseButton_Left)
    {
        GetMouseCapture();
        m_bIsPushed = true;
        args.m_bHandled = true;
        RedrawOnChange();
    }
}

void CStateImage::OnMouseButtonUp(CMouseEventArgs& args)
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

        RedrawOnChange();

        if (flag_fireClickEvent)
            PublishEvent(EventClicked, args);
    }
}

void CStateImage::DrawSelf(CDCBuffer& buffer)
{
    CMyRect rc = m_pixelRect;

    if (!IsEnabled() && m_pDisableImage)
        m_pDisableImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());
    else if (IsPushed() && m_pPushImage)
        m_pPushImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());
    else if (IsHovered() && m_pHoverImage)
        m_pHoverImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());
    else if (m_pNormalImage)
        m_pNormalImage->Draw(buffer, rc.m_left, rc.m_top, rc.Width(), rc.Height());

}

void CStateImage::SetAttribute(const StdStringW& name, const StdStringW& val)
{
	CElement::SetAttribute(name, val);
	if (name == L"normal_image")
	{
		SetNormalImage(CResourceManager::Instance().GetImage(val));
	}
	else if (name == L"hover_image")
	{
		SetHoverImage(CResourceManager::Instance().GetImage(val));
	}
	else if (name == L"push_image")
	{
		SetPushImage(CResourceManager::Instance().GetImage(val));
	}
	else if (name == L"disable_image")
	{
		SetDisableImage(CResourceManager::Instance().GetImage(val));
	}
	else if (name == L"selected_image")
	{
		SetSelectedImage(CResourceManager::Instance().GetImage(val));
	}
}

}
