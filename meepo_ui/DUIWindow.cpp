/***********************************************************************
    filename:   MyWindow.cpp
    created:    2012/6/3
    author:     ZhaoHao
*************************************************************************/
#include "DUIWindow.h"
#include "Skin.h"
#include "RawData.h"
#include "ResourceManager.h"
#include "InputEvent.h"
#include "Element.h"
#include "AnimationInstance.h"

extern unsigned int g_FrameCount;
extern unsigned int g_StartTime;
extern unsigned int g_LocaoRedrawFrameCount;

extern bool g_Flag;

namespace Meepo
{

CDUIWindow::CDUIWindow() : 
	m_nMinimalWidth(0),
    m_nMinimalHeight(0),
	m_nCaptionHeight(0),
	m_nBorderSize(0),
	m_bMaximizable(true),
	m_bSizable(true),
	m_bCenterAlignment(false),
	m_bToExitModalLoop(false),
	m_iModalResult(0),
	m_IsMouseEventTracked(false),
	m_bModal(false),
	m_bNeedFocus(true),
    m_nWidth(0),
    m_nHeight(0),
    m_pRootElement(0),
    m_pElementWithMouse(0),
    m_pMouseCaptureElement(0),
    m_bNeedRedraw(false)
{	
}

CDUIWindow::~CDUIWindow()
{
    if (IsWindow())
        DestroyWindow();
}

void CDUIWindow::SetMinimalSize(int width, int height)
{
    m_nMinimalWidth = width;
    m_nMinimalHeight = height;
}

void CDUIWindow::SetBorderSize(int size)
{
    m_nBorderSize = size;
}

void CDUIWindow::SetCaptionHeight(int height)
{
    m_nCaptionHeight = height;
}

void CDUIWindow::SetSizable(bool set)
{
    m_bSizable = set;
}

void CDUIWindow::SetMaximizable(bool set)
{
    m_bMaximizable = set;
}

void CDUIWindow::SetCenterAlignment(bool set)
{
    m_bCenterAlignment = set;
}

void CDUIWindow::SetNeedFocus(bool set)
{
    m_bNeedFocus = set;
}

void CDUIWindow::OnSize(UINT flag, CSize size)
{
	if (size.cx == 0 || size.cy == 0 || size.cx < m_nMinimalWidth || size.cy < m_nMinimalHeight)
	{
		return;
	}
    
    Resize(size.cx, size.cy);
    
    CRect rc;
    GetClientRect(&rc);
    CMyRect myrc(0, 0, rc.Width(), rc.Height());
    m_dirtyRects.push_back(myrc);

    m_bNeedRedraw = true;

//     Gdiplus::GraphicsPath path;
//     Gdiplus::Point* points = new Gdiplus::Point[_countof(pts)];
//     for (int i = 0; i < _countof(pts); ++i)
//     {
//         points[i].X = pts[i].x;
//         points[i].Y = pts[i].y;
//     }
//     path.AddPolygon(points, _countof(pts));
//     delete[] points;
//     Gdiplus::Region region(&path);
//     HDC hWndDC = GetWindowDC();
//     Gdiplus::Graphics graphics(hWndDC);
//     ReleaseDC(hWndDC);
//     HRGN hrgn = region.GetHRGN(&graphics);
//     SetWindowRgn(hrgn);
//     DeleteObject(hrgn);
}

void CDUIWindow::OnPaint(HDC hdc)
{
    PAINTSTRUCT ps;
    HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

    m_bNeedRedraw = true;
    //Draw();
    //UpdateDisplay(hPaintDC);

    ::EndPaint(m_hWnd, &ps);
}

LRESULT CDUIWindow::OnCreate(LPCREATESTRUCT crst)
{
    SetTimer(8888, 20, NULL);
    m_bNeedRedraw = true;

	return TRUE;
}

void CDUIWindow::OnDestroy()
{
}	

BOOL CDUIWindow::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
{
    return 0;
	if (bCalcValidRects)
	{
		CRect rcWindow;
		GetWindowRect(rcWindow);

		LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;

		if (SWP_NOSIZE == (SWP_NOSIZE & pParam->lppos->flags))
			return 0;

		if (0 == (SWP_NOMOVE & pParam->lppos->flags))
		{
			rcWindow.left = pParam->lppos->x;
			rcWindow.top = pParam->lppos->y;
		}

		rcWindow.right = rcWindow.left + pParam->lppos->cx;
		rcWindow.bottom = rcWindow.top + pParam->lppos->cy;
		pParam->rgrc[0] = rcWindow;
		pParam->rgrc[1] = pParam->rgrc[0];
	}

	return 0;
}

void CDUIWindow::OnMouseMove(UINT key, CPoint pt)
{
	if (!m_IsMouseEventTracked)
		TrackMouseEvent();

    InjectMouseMove(pt.x, pt.y);
}

BOOL CDUIWindow::OnLButtonDown(UINT nFlags, CPoint point)
{	
    bool isMsgHandled = InjectMouseButtonDown(MouseButton_Left, point.x, point.y);

    if (!isMsgHandled)
    {
        int rootWindowY = (int)GetRootElement()->GetPixelRect().m_top;
        if (point.y < rootWindowY + m_nCaptionHeight)
        {
            ::PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
            return FALSE;
        }
    }

    SetCapture();

    return TRUE;
}


BOOL CDUIWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
    bool isMsgHandled = InjectMouseButtonUp(MouseButton_Left, point.x, point.y);
	ReleaseCapture();
	return TRUE;
}


BOOL CDUIWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
 	if (m_bMaximizable && point.y < m_nCaptionHeight)
 	{
 		::PostMessage(m_hWnd, WM_NCLBUTTONDBLCLK, HTCAPTION, MAKELPARAM(point.x, point.y));
 		return FALSE;
 	}
	return TRUE;
}

LRESULT CDUIWindow::OnNcHitTest(CPoint point)
{
	if (IsZoomed() || !m_bSizable)
		return HTCLIENT;

	ScreenToClient(&point);
	CRect rc;
	GetClientRect(&rc);

	if (point.x < m_nBorderSize)
	{
		if (point.y < m_nBorderSize)
			return HTTOPLEFT;
		if (point.y > rc.bottom - m_nBorderSize)
			return HTBOTTOMLEFT;
		return HTLEFT;
	}
	else if (point.x > rc.right - m_nBorderSize)
	{
		if (point.y < m_nBorderSize)
			return HTTOPRIGHT;
		if (point.y > rc.bottom - m_nBorderSize)
			return HTBOTTOMRIGHT;
		return HTRIGHT;
	}
	else 
	{
		if (point.y < m_nBorderSize)
			return HTTOP;
		if (point.y > rc.bottom - m_nBorderSize)
			return HTBOTTOM;
	}

	return HTCLIENT;
}

LRESULT CDUIWindow::OnEraseBkgnd(HDC hdc)
{
	return 1;
}

void CDUIWindow::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 8888)
        InjectAnimationTimer();
}

void CDUIWindow::OnSizing(UINT fwSide, LPRECT pRect)
{
	if (pRect->right - pRect->left < m_nMinimalWidth)
	{
		if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
			pRect->left = pRect->right - m_nMinimalWidth;
		else
			pRect->right = pRect->left + m_nMinimalWidth;
	}
	if (pRect->bottom - pRect->top < m_nMinimalHeight)
	{
		if (fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT)
			pRect->top = pRect->bottom - m_nMinimalHeight;
		else
			pRect->bottom = pRect->top + m_nMinimalHeight;
	}
}

LRESULT CDUIWindow::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_IsMouseEventTracked = false;
	return TRUE;
}

void CDUIWindow::TrackMouseEvent()
{
	TRACKMOUSEEVENT tme;   
	tme.cbSize = sizeof(TRACKMOUSEEVENT);  
	tme.dwFlags = TME_LEAVE;   
	tme.hwndTrack = m_hWnd;  
	::_TrackMouseEvent(&tme);
}

void CDUIWindow::OnShowWindow(BOOL bShow, UINT nStatus)
{

}

void CDUIWindow::OnClose()
{
	if (m_bModal)
	{
		EndModal(m_iModalResult);
	}
	else
	{
		Destroy();
	}
}

int CDUIWindow::DoModal(HWND hWndParent)
{
    if (!IsWindow()) 
    {
        return -1;
    }

	m_bModal = true;

	bool isParentDisabled = false;
	if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		isParentDisabled = true;
	}

	Show();
	RunModalLoop();

	if (isParentDisabled)
	{
		::EnableWindow(hWndParent, TRUE);
	}

	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	{
		::SetActiveWindow(hWndParent);
	}

	if (IsWindow())
	{	
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		DestroyWindow();
	}

	return m_iModalResult;
}

void CDUIWindow::RunModalLoop()
{
	m_bToExitModalLoop = false;

	BOOL bRet;
	MSG	msg;
	while (!m_bToExitModalLoop)
	{
		bRet = ::GetMessage(&msg, NULL, 0, 0);
		if(!bRet)
		{
			break;   //!< WM_QUIT, exit message loop
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void CDUIWindow::EndModal(int result)
{
	m_iModalResult = result;
	m_bToExitModalLoop =  true;
	PostMessage(WM_NULL);
}

void CDUIWindow::Show(CDUIWindow* parent/* = NULL*/)
{
    if (m_bCenterAlignment)
    {
        CenterWindow();
    }
	int focusFlag = m_bNeedFocus? 0 : SWP_NOACTIVATE;
    SetWindowPos(NULL, 0, 0, 0, 0, focusFlag | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
}

void CDUIWindow::Hide()
{
	ShowWindow(SW_HIDE);
}

void CDUIWindow::Destroy()
{
	if(!IsWindow())
	{
		return;
	}
	DestroyWindow();
}

void CDUIWindow::Close()
{
	if(!IsWindow())
		return;
	::CloseWindow(m_hWnd);
}


void CDUIWindow::UpdateDisplay(HDC hdc)
{
    HDC targetDC = hdc? hdc : GetDC();
    BitBlt(targetDC, 0, 0, m_nWidth, m_nHeight, m_renderBuffer.GetMemoryDC(), 0, 0, SRCCOPY);
    if (hdc)
        ReleaseDC(targetDC);

    if (g_Flag)
        g_LocaoRedrawFrameCount++;
}


CElement* CDUIWindow::GetRootElement()
{
    return m_pRootElement;
}

void CDUIWindow::Resize(int width, int height)
{
    m_nWidth = width;
    m_nHeight = height;
    m_renderBuffer.Resize(width, height);
    m_renderBuffer.SetClipRect(CMyRect(0, 0, m_nWidth, m_nHeight));
    if (m_pRootElement)
        m_pRootElement->OnParentSized();
}

CVector2 CDUIWindow::GetSize() const
{
    return CVector2(m_nWidth, m_nHeight);
}

void CDUIWindow::Draw(const CMyRect& dirtyRect)
{
    m_pRootElement->Draw(m_renderBuffer, dirtyRect);
}

CDCBuffer& CDUIWindow::GetRenderBuffer()
{
    return m_renderBuffer;
}

void CDUIWindow::SetRootElement(CElement* elem)
{
    m_pRootElement = elem;
}   

bool CDUIWindow::InjectMouseMove(int x, int y)
{
    CVector2 newPos(x, y);
    CMouseEventArgs ma(0);
    ma.m_position = newPos;
    ma.m_moveDelta.m_x = x - m_currentMousePosition.m_x;
    ma.m_moveDelta.m_y = y - m_currentMousePosition.m_y;
    ma.m_systemKeys = 0;
    ma.m_wheelChange = 0;
    ma.m_clickCount = 0;
    ma.m_button = MouseButton_None;

    m_currentMousePosition = newPos;

    CElement* targetElement = GetTargetElement(m_currentMousePosition);

    // has window containing mouse changed?
    if (targetElement != m_pElementWithMouse)
    {
        CElement* oldElement = m_pElementWithMouse;
        m_pElementWithMouse = targetElement;

        // inform previous window the mouse has left it
        if (oldElement)
        {
            ma.m_pElement = oldElement;
            oldElement->OnMouseLeave(ma);
        }

        // inform window containing mouse that mouse has entered it
        if (m_pElementWithMouse)
        {
            ma.m_pElement  = m_pElementWithMouse;
            ma.m_bHandled = false;
            m_pElementWithMouse->OnMouseEnter(ma);
        }
    }

    if (!m_pElementWithMouse)
        return false;

    ma.m_bHandled = false;
    CElement* destElement = m_pElementWithMouse;

    // loop backwards until event is handled or we run out of windows.
    while (!ma.m_bHandled && destElement)
    {
        ma.m_pElement = destElement;
        destElement->OnMouseMove(ma);
        destElement = destElement->GetParent();
    }

    return ma.m_bHandled;
}

bool CDUIWindow::UpdateWindowContainingMouse()
{
    CMouseEventArgs ma(0);
    CElement* targetElement = GetTargetElement(m_currentMousePosition);

    // has window containing mouse changed?
    if (targetElement != m_pElementWithMouse)
    {
        CElement* oldElement = m_pElementWithMouse;
        m_pElementWithMouse = targetElement;

        // inform previous window the mouse has left it
        if (oldElement)
        {
            ma.m_pElement = oldElement;
            oldElement->OnMouseLeave(ma);
        }

        // inform window containing mouse that mouse has entered it
        if (m_pElementWithMouse)
        {
            ma.m_pElement  = m_pElementWithMouse;
            ma.m_bHandled = false;
            m_pElementWithMouse->OnMouseEnter(ma);
        }
    }

    return true;
}

CElement* CDUIWindow::GetTargetElement(const CVector2& pt) const
{
    // if there is no root window, then there is nowhere to send input
    if (!m_pRootElement)
        return 0;

    if (m_pMouseCaptureElement)
        return m_pMouseCaptureElement;

    CElement* destElement = m_pRootElement->GetChildAtPosition(pt);
    if (destElement)
        return destElement;

    return m_pRootElement;
}

bool CDUIWindow::InjectMouseButtonDown(EMouseButton button, int x, int y)
{
    m_currentMousePosition.m_x = x;
    m_currentMousePosition.m_y = y;

    CMouseEventArgs ma(0);
    ma.m_position = CVector2(x, y);
    ma.m_moveDelta = CVector2(0, 0);
    ma.m_button = button;

    CElement* targetElement = GetTargetElement(m_currentMousePosition);

    while (!ma.m_bHandled && targetElement)
    {
        ma.m_pElement = targetElement;
        targetElement->OnMouseButtonDown(ma);
        targetElement = targetElement->GetParent();
    }

    return ma.m_bHandled;
}

bool CDUIWindow::InjectMouseButtonUp(EMouseButton button, int x, int y)
{
    m_currentMousePosition.m_x = x;
    m_currentMousePosition.m_y = y;

    CMouseEventArgs ma(0);
    ma.m_position = CVector2(x, y);
    ma.m_moveDelta = CVector2(0, 0);
    ma.m_button = button;

    CElement* targetElement = GetTargetElement(m_currentMousePosition);

    while (!ma.m_bHandled && targetElement)
    {
        ma.m_pElement = targetElement;
        targetElement->OnMouseButtonUp(ma);
        targetElement = targetElement->GetParent();
    }

    return ma.m_bHandled;
}

void CDUIWindow::SetMouseCaptureElement(CElement* elem)
{
    m_pMouseCaptureElement = elem;
}

CElement* CDUIWindow::GetMouseCaptureElement() const
{
    return m_pMouseCaptureElement;
}

Meepo::CVector2 CDUIWindow::GetCurrentMousePosition() const
{
    return m_currentMousePosition;
}

bool CDUIWindow::InjectAnimationTimer()
{
    AnimationiInstanceSet::iterator it = m_animationInstanceSet.begin();
    for (; it != m_animationInstanceSet.end(); ++it)
    {
        if ((*it)->IsRunning())
        {
            if ((*it)->Step())
            {
                m_dirtyRects.push_back((*it)->GetTargetObject()->GetPixelRect());
            }
        }
    }

    if (m_bNeedRedraw)
    {
        CRect dirtyRect(0,0,0,0);

        for (size_t i = 0; i < m_dirtyRects.size(); i++)
        {
            CRect rc(m_dirtyRects[i].m_left, m_dirtyRects[i].m_top, 
                m_dirtyRects[i].m_right, m_dirtyRects[i].m_bottom);
            dirtyRect.UnionRect(&dirtyRect, &rc);
        }

        CMyRect dirtyRectMy(dirtyRect.left, dirtyRect.top, dirtyRect.right, dirtyRect.bottom);
        m_renderBuffer.SetClipRect(dirtyRectMy);
        Draw(dirtyRectMy);
        UpdateDisplay(0);
        m_bNeedRedraw = false;
        m_dirtyRects.clear();

        if (g_Flag)
            g_FrameCount++;
    }

    return true;
}

void CDUIWindow::AddAnimationInstance(CAnimationInstance* pAnimationInst)
{
    m_animationInstanceSet.insert(pAnimationInst);   
}

void CDUIWindow::RemoveAnimation(CAnimationInstance* pAnimationInst)
{
    AnimationiInstanceSet::iterator it = m_animationInstanceSet.find(pAnimationInst);
    if (it != m_animationInstanceSet.end())
        m_animationInstanceSet.erase(it);
}

void CDUIWindow::SetRedrawFlag(bool flag)
{
    m_bNeedRedraw = flag;
}

void CDUIWindow::SetDirtyRect(const CMyRect& rect)
{
    m_dirtyRects.push_back(rect);
    m_bNeedRedraw = true;
}

bool CDUIWindow::GetRedrawFlag() const
{
    return m_bNeedRedraw;
}

}