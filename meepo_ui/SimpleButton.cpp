#include "SimpleButton.h"

namespace Meepo
{

CSimpleButton::CSimpleButton(void)
{
    m_pStateImage = new CStateImage;
    m_pStateImage->SetRelativePosition(REL_VAL(0), REL_VAL(0));
    m_pStateImage->SetRelativeSize(REL_VAL(1), REL_VAL(1));
    AddChildElement(m_pStateImage);

    m_pImage = new CStaticImage;
    m_pImage->SetRelativePosition(ABS_VAL(10), ABS_VAL(3));
    m_pImage->SetRelativeSize(ABS_VAL(0), ABS_VAL(0));
    m_pImage->SetMousePassThrough(true);
    m_pStateImage->AddChildElement(m_pImage);

    m_pText = new CText;
    m_pText->SetRelativePosition(ABS_VAL(40), ABS_VAL(10));
    m_pText->SetRelativeSize(ABS_VAL(100), ABS_VAL(20));
    m_pText->SetMousePassThrough(true);
    m_pStateImage->AddChildElement(m_pText);
}


CSimpleButton::~CSimpleButton(void)
{
}

void CSimpleButton::SetContainerWnd( CDUIWindow* root )
{
    CElement::SetContainerWnd(root);
    m_pStateImage->SetContainerWnd(root);
    m_pText->SetContainerWnd(root);
    m_pImage->SetContainerWnd(root);
}

void CSimpleButton::SetImage( CImage* img )
{ 
    m_pImage->SetImage(img);
    m_pImage->SetRelativeSize(ABS_VAL(img->GetWidth()), ABS_VAL(img->GetHeight()));
}

}


