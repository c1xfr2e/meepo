/***********************************************************************
    filename:   DUIWindow.h
    created:    2012/6/3
    author:     ZhaoHao
*************************************************************************/
#pragma once

//! define mouse hover & leave message(Windows' header only define these when _WIN32_WINNT >= 0x0400)
#ifndef WM_MOUSEHOVER
#define WM_MOUSEHOVER	0x02A1
#endif
#ifndef WM_MOUSELEAVE
#define WM_MOUSELEAVE	0x02A3
#endif

#pragma comment(lib, "Comctl32.lib") 

#include "PreHeader.h"
#include "atlcrack.h"
#include "Rect.h"
#include "DCBuffer.h"
#include "InputEvent.h"
#include "Animation.h"
#include <set>
#include <vector>

namespace Meepo
{

typedef CWinTraits<WS_POPUP, 0> ChildWndTraits;
typedef CWinTraits<WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE> 
                   MainWndTraits;
typedef CWinTraits<WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE> 
                   LayeredWndTraits;


class CDUIWindow : public ATL::CWindowImpl<CDUIWindow, ATL::CWindow>
{
public:
	CDUIWindow();
	virtual ~CDUIWindow();

	void SetMinimalSize(int width, int height);
	void SetBorderSize(int size);
	void SetCaptionHeight(int height);
	void SetSizable(bool set);
	void SetMaximizable(bool set);
	void SetCenterAlignment(bool set);
	void SetNeedFocus(bool set);

	void Show(CDUIWindow* parent = NULL);
	void Hide();
	int  DoModal(HWND hWndParent);
	void EndModal(int result);
	void Destroy();
	void Close();

    void UpdateDisplay(HDC hdc);

    void Resize(int width, int height);
    CVector2 GetSize() const;

    void Draw(const CMyRect& dirtyRect);

    CDCBuffer& GetRenderBuffer();

    void SetRootElement(CElement* elem);
    CElement* GetRootElement();

    CVector2 GetCurrentMousePosition() const;

    bool InjectMouseMove(int x, int y);
    bool InjectMouseButtonDown(EMouseButton button, int x, int y);
    bool InjectMouseButtonUp(EMouseButton button, int x, int y);	

    void SetMouseCaptureElement(CElement* elem);
    CElement* GetMouseCaptureElement() const;

    bool InjectAnimationTimer();
    void AddAnimationInstance(CAnimationInstance* pAnimationInst);
    void RemoveAnimation(CAnimationInstance* pAnimationInst);

    void SetRedrawFlag(bool flag);
    bool GetRedrawFlag() const;
    void SetDirtyRect(const CMyRect& rect);

	//! default message processing
	BEGIN_MSG_MAP_EX(CDUIWindow)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)	//!< to let client area fill the entire window
		MSG_WM_NCHITTEST(OnNcHitTest)	//!< to tell Windows where the mouse is
		MSG_WM_SIZING(OnSizing)			//!< to contrain max and min size 
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MSG_WM_SIZE(OnSize)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_CLOSE(OnClose)
	END_MSG_MAP()
	//!~

protected:
	void	TrackMouseEvent();
	void	RunModalLoop();

	//! message handlers
	BOOL	OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
	LRESULT OnNcHitTest(CPoint point);
	void	OnSizing(UINT fwSide, LPRECT pRect);
	LRESULT	OnCreate(LPCREATESTRUCT crst);
	void	OnDestroy();
	void	OnMouseMove(UINT key, CPoint pt);
	BOOL	OnLButtonDown(UINT nFlags, CPoint point);
	BOOL	OnLButtonUp(UINT nFlags, CPoint point);
	BOOL	OnLButtonDblClk(UINT nFlags, CPoint pt);
	void	OnSize(UINT flag, CSize size);
	void	OnPaint(HDC hdc);
	LRESULT OnEraseBkgnd(HDC hdc);
	LRESULT	OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void	OnShowWindow(BOOL bShow, UINT nStatus);
	void	OnTimer(UINT_PTR nIDEvent);
	void	OnClose();
	//!~

    int m_nMinimalWidth;
    int m_nMinimalHeight;
	int m_nCaptionHeight;
	int m_nBorderSize;

	bool m_bMaximizable;
	bool m_bSizable;
	bool m_bCenterAlignment;
	bool m_bNeedFocus;
	bool m_bModal;
	bool m_bToExitModalLoop;
	bool m_IsMouseEventTracked;

    int m_iModalResult;

    //CDUIWindow m_uiControler;

    bool UpdateWindowContainingMouse();
    CElement* GetTargetElement(const CVector2& pt) const;

    int m_nWidth;
    int m_nHeight;

    CDCBuffer m_renderBuffer;

    CVector2 m_currentMousePosition;

    CElement* m_pRootElement;
    CElement* m_pElementWithMouse; // element that currently contains the mouse.
    CElement* m_pMouseCaptureElement; // element with mouse captured

    typedef std::set<CAnimationInstance*> AnimationiInstanceSet;
    AnimationiInstanceSet m_animationInstanceSet;

    bool m_bNeedRedraw;
    std::vector<CMyRect> m_dirtyRects;
};

}