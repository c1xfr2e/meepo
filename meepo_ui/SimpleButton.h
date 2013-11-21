#pragma once

#include "PreHeader.h"
#include "StateImage.h"
#include "Text.h"
#include "StaticImage.h"

namespace Meepo
{

class CSimpleButton : public CElement
{
public:
    CSimpleButton(void);
    ~CSimpleButton(void);

    void SetNormalImage(CImage* image) { m_pStateImage->SetNormalImage(image); }
    void SetHoverImage(CImage* image)  { m_pStateImage->SetHoverImage(image); }
    void SetPushImage(CImage* image)   { m_pStateImage->SetPushImage(image); }
    void SetDisableImage(CImage* image) { m_pStateImage->SetDisableImage(image); }

    void SetImage(CImage* img);


    virtual void SetContainerWnd(CDUIWindow* root);    

private:
    CStateImage* m_pStateImage;
    CStaticImage* m_pImage;
    CText* m_pText;
};

}


