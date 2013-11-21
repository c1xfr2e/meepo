/***********************************************************************
    filename:   Element.h
    created:    2012/6/10
    author:     ZhaoHao
*************************************************************************/
#include "PreHeader.h"
#include "RelativeCoord.h"
#include "Region.h"
#include "Event.h"
#include <vector>
#include <map>
#include "DCBuffer.h"

#pragma once

namespace Meepo
{

enum EAlignment
{
    Alignment_Left,
    Alignment_Top,
    Alignment_Right,
    Alignment_Bottom,
    Alignment_Center
};

class CElement
{
public:
    // 事件名字
    static const StdStringW	EventMouseButtonDown;
    static const StdStringW	EventMouseButtonUp;
    static const StdStringW	EventMouseEnter;
    static const StdStringW	EventMouseLeave;
    static const StdStringW	EventMouseClick;

    CElement();
    ~CElement();

    void AddEvent(const StdStringW& name);
    CEvent* GetEvent(const StdStringW& name, bool autoAdd = false);
    void RemoveEvent(const StdStringW& name);
    void RemoveAllEvents(void);
    bool IsEventPresent(const StdStringW& name);
    SubscribeRecord SubscribeEvent(const StdStringW& name, Subscriber& subscriber);
    void PublishEvent(const StdStringW& name, CEventArgs& args);

    void SetRelativePosition(CRelValue x, CRelValue y);
    void SetRelativeSize(CRelValue w, CRelValue h);    
    CMyRect GetPixelRect() const;
    void AddChildElement(CElement* child);

    CElement* GetParent() const;
    CDUIWindow* GetUIControler() const;

	void SetClipPolygon(const std::vector<CRelVector>& points);

    CElement* GetChildAtPosition(const CVector2& pt) const;

    bool IsHit(const CVector2& pt) const;
    bool IsVisible(bool localOnly = false) const;
	void SetVisible(bool set);
    bool IsMousePassThroughEnabled() const;
    void SetMousePassThrough(bool flag);
    bool IsEnabled(bool localOnly = false) const;
    bool IsValid() const;

    bool IsUsingClipRegion() const;
    CRegion& GetClipRegion();
    void SetUsingClipRegionFlag(bool set);

	void SetArea(const CRelRect& rc, bool redraw = false);
    void SetArea(const CRelVector& rpos, const CRelVector& rsize, bool redraw = false);
    void SetPosition(const CRelVector& rpos, bool redraw = false);
    void SetXPosition(const CRelValue& rx, bool redraw = false);
    void SetSize(const CRelVector& rsize, bool redraw = false);

    void SetUseSnapshotFlag(bool flag);

	void SetZOrder(int zorder);
	int GetZOrder() const;

    virtual void OnParentSized();
    virtual void Draw(CDCBuffer& buffer, const CMyRect& dirtyRect);
    virtual void LocalRedraw(CDCBuffer& buffer, const CMyRect& dirtyRect);
    virtual void DrawSelf(CDCBuffer& buffer);
    virtual void DrawChildren(CDCBuffer& buffer, const CMyRect& dirtyRect);
    virtual void OnMouseEnter(CMouseEventArgs& e);
    virtual void OnMouseLeave(CMouseEventArgs& e);
    virtual void OnMouseMove(CMouseEventArgs& e);
    virtual void OnMouseButtonDown(CMouseEventArgs& e);
    virtual void OnMouseButtonUp(CMouseEventArgs& e);
    virtual void OnMouseClicked(CMouseEventArgs& e);
    virtual void SetContainerWnd(CDUIWindow* containerWnd);
    virtual void SetAlpha(int alpha, bool redraw = false);

	virtual void SetAttribute(const StdStringW& name, const StdStringW& val);
	
protected:
    typedef std::map<StdStringW, CEvent*> EventMap;
    EventMap m_mapEvents; // 事件列表

    CDUIWindow* m_pDUIWnd;
    CElement* m_pParent;
        
    CRelRect m_relRect;
    CMyRect m_pixelRect;
        
    typedef std::vector<CElement*> ElementVector;
    ElementVector m_vecChildElements; // 子窗口列表

	std::vector<CRelVector> m_vecClipPolygonPoints;
	CRegion m_clipRegion;
    bool m_bUsingClipRegion;   // 是否使用裁剪区间

    EAlignment m_xAlignment;  // 水平对齐属性
    EAlignment m_yAlignment;  // 垂直对齐属性

    bool m_bIsVisible;          // 是否可见
    bool m_bIsMousePassThrough; // 是否穿透鼠标
    bool m_bIsEnabled;          // 是否使能

    CDCBuffer m_snapshotBuffer; // 快照缓存 
    bool m_bIsSnapshotValid; // 当前快照是否有效

    bool m_bUseSnapshot; // 是否保存快照

	int m_zOrder;

    virtual void OnSized();
    virtual void OnMoved();

    void UpdatePixelRect(bool& moved, bool& sized);
    void AccumulateClipRegion();
    void UpdateClipRegion();
    void ResizeSnapshotBuffer();
    void LayoutChildElements();
    void SetSnapshotInvalid(bool recursive = false);
    void RestoreSnapshot();
    void GetMouseCapture();
    void ReleaseMouseCapture();
    void RedrawOnChange();
};

}