
#include "GdiplusHeader.h"
#include "TestWnd.h"
#include "ResourceManager.h"
#include "StaticImage.h"
#include "Skin.h"
#include "Image.h"
#include "Delegate.h"
#include "ImageButton.h"
#include "Interpolator.h"
#include "Animation.h"
#include "AnimationInstance.h"
#include "Text.h"
#include "SimpleButton.h"

using namespace Meepo;

CAnimationInstance* g_ani1Inst;
CAnimationInstance* g_ani2Inst;
CElement* g_pTarget;

unsigned int g_FrameCount;
unsigned int g_StartTime;
unsigned int g_LocaoRedrawFrameCount;
bool g_Flag;

bool fun(const CEventArgs& args)
{
    g_ani2Inst->Start();
    g_StartTime = GetTickCount();
    g_Flag = true;
    //g_ani1Inst->Start();
    //g_pTarget->SetPosition(CRelVector(ABS_VAL(450), ABS_VAL(250)), true);
    return true;
}

bool end(const CEventArgs& args)
{

    unsigned int eclipse = GetTickCount() - g_StartTime;
    unsigned int fps = g_FrameCount / (eclipse / 1000);

    unsigned int localRedrawFps = g_LocaoRedrawFrameCount / (eclipse / 1000);


    char msg[128] = {0};
    sprintf(msg, "%u, %u", g_LocaoRedrawFrameCount, localRedrawFps);
    MessageBoxA(0, msg, 0, 0);

    return true;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR szCmdLine, int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CResourceManager::Instance().SetResourcePath(L"../../../res/");
    CResourceManager::Instance().LoadImages(L"Default.image");

    CTestWnd uiWnd;
    CRect rc(0, 0, 800, 600);
    uiWnd.SetCaptionHeight(30);
    uiWnd.SetBorderSize(10);
    uiWnd.SetMinimalSize(100, 100);
    uiWnd.SetCenterAlignment(true);

    CImage* image1 = CResourceManager::Instance().GetImage(L"BmpBkg");
    CImage* image2 = CResourceManager::Instance().GetImage(L"Bkg3");
    CImage* image3 = CResourceManager::Instance().GetImage(L"rotate");

    CStaticImage* pStaticImage1 = new CStaticImage;
    pStaticImage1->SetContainerWnd(&uiWnd);
    pStaticImage1->SetImage(image1);
   // pStaticImage1->SetRelativePosition(REL_VAL(0), REL_VAL(0));
   // pStaticImage1->SetRelativeSize(REL_VAL(1), REL_VAL(1));
	pStaticImage1->SetAttribute(L"pos", L"{{0,0},{0,0}}");
	pStaticImage1->SetAttribute(L"size", L"{{1,0},{1,0}}");

    CStaticImage* pStaticImage2 = new CStaticImage;
    pStaticImage2->SetContainerWnd(&uiWnd);
    pStaticImage2->SetImage(image2);
    pStaticImage2->SetRelativePosition(REL_VAL(0), REL_VAL(0));
    pStaticImage2->SetRelativeSize(REL_VAL(1), REL_VAL(1));
    pStaticImage1->AddChildElement(pStaticImage2);


    //{1,3},{3,1},{-3,1},{-1,3},{-1,-4},{-4,-1},{4,-1},{1,-4}
    std::vector<CRelVector> clipPolygon;
    clipPolygon.push_back(CRelVector(CRelValue(0,1), CRelValue(0,3)));
    clipPolygon.push_back(CRelVector(CRelValue(0,3), CRelValue(0,1)));
    clipPolygon.push_back(CRelVector(CRelValue(1,-3), CRelValue(0,1)));
    clipPolygon.push_back(CRelVector(CRelValue(1,-1), CRelValue(0,3)));
    clipPolygon.push_back(CRelVector(CRelValue(1,-1), CRelValue(1,-4)));
    clipPolygon.push_back(CRelVector(CRelValue(1,-4), CRelValue(1,-1)));
    clipPolygon.push_back(CRelVector(CRelValue(0,4), CRelValue(1,-1)));
    clipPolygon.push_back(CRelVector(CRelValue(0,1), CRelValue(1,-4)));
    pStaticImage2->SetClipPolygon(clipPolygon);

    CImage* innerBkg = CResourceManager::Instance().GetImage(L"QQInnerMask");
    CStaticImage* QQInnerMask11 = new CStaticImage;
    QQInnerMask11->SetContainerWnd(&uiWnd);
    QQInnerMask11->SetImage(innerBkg);
    QQInnerMask11->SetRelativePosition(ABS_VAL(20), ABS_VAL(40));
    QQInnerMask11->SetRelativeSize(ABS_VAL(1000), ABS_VAL(700));
    pStaticImage2->AddChildElement(QQInnerMask11);

    CImage* QQInnerMaskImage = CResourceManager::Instance().GetImage(L"QQMainBgLight");
    CStaticImage* QQInnerMask = new CStaticImage;
    QQInnerMask->SetContainerWnd(&uiWnd);
    QQInnerMask->SetImage(QQInnerMaskImage);
    QQInnerMask->SetRelativePosition(REL_VAL(0), REL_VAL(1));
    QQInnerMask->SetRelativeSize(REL_VAL(0), REL_VAL(1));
    QQInnerMask11->AddChildElement(QQInnerMask);

    CStaticImage* pStaticImage3 = new CStaticImage;
    pStaticImage3->SetContainerWnd(&uiWnd);
    pStaticImage3->SetImage(image3);
    pStaticImage3->SetRelativePosition(ABS_VAL(250), ABS_VAL(450));
    pStaticImage3->SetRelativeSize(ABS_VAL(image3->GetWidth()), ABS_VAL(image3->GetHeight()));
    QQInnerMask11->AddChildElement(pStaticImage3);
    g_pTarget = pStaticImage3;

    CImage* btnNormal = CResourceManager::Instance().GetImage(L"ClsBtnNormal");
    CImage* btnHover = CResourceManager::Instance().GetImage(L"ClsBtnHover");
    CImage* btnPushed = CResourceManager::Instance().GetImage(L"ClsBtnPush");

    CImageButton* pImageButton = new CImageButton;
    pImageButton->SetNormalImage(btnNormal);
    pImageButton->SetHoverImage(btnHover);
    pImageButton->SetPushImage(btnPushed);
    pImageButton->SetContainerWnd(&uiWnd);
    pImageButton->SetRelativePosition(ABS_VAL(150), ABS_VAL(250));
    pImageButton->SetRelativeSize(ABS_VAL(btnNormal->GetWidth()), ABS_VAL(btnNormal->GetHeight()));
    QQInnerMask11->AddChildElement(pImageButton);

    pImageButton->SubscribeEvent(CImageButton::EventClicked, Subscriber(fun));

    CImage* btnNormal1 = CResourceManager::Instance().GetImage(L"MaxBtnNormal");
    CImage* btnHover1 = CResourceManager::Instance().GetImage(L"MaxBtnHover");
    CImage* btnPushed1 = CResourceManager::Instance().GetImage(L"MaxBtnPush");
    CImageButton* pImageButton1 = new CImageButton;
    pImageButton1->SetNormalImage(btnNormal1);
    pImageButton1->SetHoverImage(btnHover1);
    pImageButton1->SetPushImage(btnPushed1);
    pImageButton1->SetContainerWnd(&uiWnd);
    pImageButton1->SetRelativePosition(ABS_VAL(150 - btnNormal1->GetWidth()), ABS_VAL(250));
    pImageButton1->SetRelativeSize(ABS_VAL(btnNormal1->GetWidth()), ABS_VAL(btnNormal1->GetHeight()));
    QQInnerMask11->AddChildElement(pImageButton1);

    CImage* btnNormal2 = CResourceManager::Instance().GetImage(L"MinBtnNormal");
    CImage* btnHover2 = CResourceManager::Instance().GetImage(L"MinBtnHover");
    CImage* btnPushed2 = CResourceManager::Instance().GetImage(L"MinBtnPush");
    CImageButton* pImageButton2 = new CImageButton;
    pImageButton2->SetNormalImage(btnNormal2);
    pImageButton2->SetHoverImage(btnHover2);
    pImageButton2->SetPushImage(btnPushed2);
    pImageButton2->SetContainerWnd(&uiWnd);
    pImageButton2->SetRelativePosition(ABS_VAL(150 - btnNormal1->GetWidth() - btnNormal2->GetWidth()), 
                                       ABS_VAL(250));
    pImageButton2->SetRelativeSize(ABS_VAL(btnNormal2->GetWidth()), ABS_VAL(btnNormal2->GetHeight()));
    QQInnerMask11->AddChildElement(pImageButton2);


    CImage* btnNormal3 = CResourceManager::Instance().GetImage(L"LoginButtonNormal");
    CImage* btnHover3 = CResourceManager::Instance().GetImage(L"LoginButtonHover");
    CImage* btnPushed3 = CResourceManager::Instance().GetImage(L"LoginButtonPush");
    CImageButton* pImageButton3 = new CImageButton;
    pImageButton3->SetNormalImage(btnNormal3);
    pImageButton3->SetHoverImage(btnHover3);
    pImageButton3->SetPushImage(btnPushed3);
    pImageButton3->SetContainerWnd(&uiWnd);
    pImageButton3->SetRelativePosition(ABS_VAL(200), ABS_VAL(250));
    pImageButton3->SetRelativeSize(ABS_VAL(btnNormal3->GetWidth()), ABS_VAL(btnNormal3->GetHeight()));
    QQInnerMask11->AddChildElement(pImageButton3);

    pImageButton3->SubscribeEvent(CImageButton::EventClicked, Subscriber(end));

    CText* pText = new CText;
    //pText->SetRelativePosition(ABS_VAL(14), ABS_VAL(9));
    //pText->SetRelativeSize(ABS_VAL(100), ABS_VAL(20));
	pText->SetAttribute(L"area", L"{{0,14},{0,9},{0,114},{0,29}}");

    pText->SetContainerWnd(&uiWnd);
    pImageButton3->AddChildElement(pText);


    CSimpleButton* pSimpleButton = new CSimpleButton;
    //pSimpleButton->SetNormalImage(CResourceManager::Instance().GetImage(L"toobarBtnHoverHighlight"));
    pSimpleButton->SetHoverImage(CResourceManager::Instance().GetImage(L"toobarBtnHoverHighlight"));
    pSimpleButton->SetPushImage(CResourceManager::Instance().GetImage(L"toobarBtnPushHighlight"));
    pSimpleButton->SetImage(CResourceManager::Instance().GetImage(L"toolbar1"));
    pSimpleButton->SetContainerWnd(&uiWnd);
    pSimpleButton->SetRelativePosition(CRelValue(0,350), CRelValue(0,100));
    pSimpleButton->SetRelativeSize(ABS_VAL(110), ABS_VAL(40));
    QQInnerMask11->AddChildElement(pSimpleButton);

    // »ÆÉ«±³¾°
    CImage* yellow = CResourceManager::Instance().GetImage(L"thunderAniBtnBkg");
    CStaticImage* pYellowBkg = new CStaticImage;
    pYellowBkg->SetAlpha(150);
    pYellowBkg->SetContainerWnd(&uiWnd);
    pYellowBkg->SetImage(yellow);
    pYellowBkg->SetRelativePosition(ABS_VAL(20), ABS_VAL(20));
    pYellowBkg->SetRelativeSize(ABS_VAL(yellow->GetWidth()), ABS_VAL(yellow->GetHeight()));
    QQInnerMask11->AddChildElement(pYellowBkg);

    CImage* ani_image = CResourceManager::Instance().GetImage(L"running1");
    CStaticImage* pAniImage = new CStaticImage;
    pAniImage->SetContainerWnd(&uiWnd);
    pAniImage->SetImage(ani_image);
    pAniImage->SetRelativePosition(ABS_VAL(20), ABS_VAL(20));
    pAniImage->SetRelativeSize(ABS_VAL(ani_image->GetWidth()), ABS_VAL(ani_image->GetHeight()));
    pYellowBkg->AddChildElement(pAniImage);
    pAniImage->SetUseSnapshotFlag(true);

    uiWnd.SetRootElement(pStaticImage1);

    CAnimation ani1(1000);
    CPositionAffector pa1;
    pa1.AddKeyFrame(CPositionAffector::KeyFrame(0, CRelVector(ABS_VAL(250), ABS_VAL(250))));
    pa1.AddKeyFrame(CPositionAffector::KeyFrame(1000, CRelVector(ABS_VAL(450), ABS_VAL(250))));
    ani1.AddAffector(&pa1);

    CAlphaAffector aa1;
    aa1.AddKeyFrame(CKeyFrame<int>(0, 255));
    aa1.AddKeyFrame(CKeyFrame<int>(1000, 255));
    ani1.AddAffector(&aa1);

    CAnimationInstance ani1Inst(&ani1, pStaticImage3);
    uiWnd.AddAnimationInstance(&ani1Inst);
    g_ani1Inst = &ani1Inst;

    CAnimation image_seq_ani(1150);
    image_seq_ani.SetReplayMode(ReplayMode_Loop);
    CImageSequenceAffector image_seq_affector;
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(0, L"running1", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(50, L"running2", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(100, L"running3", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(150, L"running4", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(200, L"running5", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(250, L"running6", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(300, L"running7", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(350, L"running8", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(400, L"running9", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(450, L"running10", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(500, L"running11", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(550, L"running12", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(600, L"running13", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(650, L"running14", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(700, L"running15", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(750, L"running16", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(800, L"running17", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(850, L"running18", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(900, L"running19", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(950, L"running20", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(1000, L"running21", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(1000, L"running22", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(1100, L"running23", ProgressionMode_Discrete));
    image_seq_affector.AddKeyFrame(CKeyFrame<StdStringW>(1150, L"running24", ProgressionMode_Discrete));
    image_seq_ani.AddAffector(&image_seq_affector);

    CAnimationInstance image_seq_ani_inst(&image_seq_ani, pAniImage);
    uiWnd.AddAnimationInstance(&image_seq_ani_inst);
    g_ani2Inst = &image_seq_ani_inst;

    uiWnd.Create(NULL, &rc, _T("Hello"), MainWndTraits::GetWndStyle(0), MainWndTraits::GetWndExStyle(0));
    uiWnd.DoModal(NULL);



    Gdiplus::GdiplusShutdown(gdiplusToken);


    

    return 0;
}