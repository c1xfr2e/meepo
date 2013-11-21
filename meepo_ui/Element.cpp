/***********************************************************************
    filename:   UIElement.cpp
    created:    2012/6/10
    author:     ZhaoHao
*************************************************************************/
#include "Element.h"
#include <stack>
#include "DUIWindow.h"
#include "StringHelper.h"

namespace Meepo
{

const StdStringW CElement::EventMouseButtonDown(L"MouseButtonDown");
const StdStringW CElement::EventMouseButtonUp(L"MouseButtonUp");
const StdStringW CElement::EventMouseEnter(L"MouseEnter");
const StdStringW CElement::EventMouseLeave(L"MouseLeave");
const StdStringW CElement::EventMouseClick(L"MouseClick");

CElement::CElement() :
    m_pDUIWnd(0), m_pParent(0), 
    m_xAlignment(Alignment_Left), m_yAlignment(Alignment_Top),
    m_bIsVisible(true),
    m_bIsMousePassThrough(false),
    m_bIsEnabled(true),
    m_bUsingClipRegion(false),
    m_bIsSnapshotValid(false),
    m_bUseSnapshot(false),
	m_zOrder(0)
{

}

CElement::~CElement()
{
}

void CElement::SetRelativePosition(CRelValue x, CRelValue y)
{
    m_relRect.SetPosition(CRelVector(x, y));
}

void CElement::SetRelativeSize(CRelValue w, CRelValue h)
{
    m_relRect.SetSize(CRelVector(w, h));
}

void CElement::SetContainerWnd(CDUIWindow* root)
{
    m_pDUIWnd = root;
}

void CElement::AddChildElement(CElement* child)
{
    m_vecChildElements.push_back(child);
    child->m_pParent = this;
}   

void CElement::SetClipPolygon(const std::vector<CRelVector>& points)
{
	m_vecClipPolygonPoints = points;
    m_bUsingClipRegion = true;
}

void CElement::UpdateClipRegion()
{
    if (m_vecClipPolygonPoints.empty())
    {
        m_clipRegion.SetRect(m_pixelRect);
    }
    else
    {
        CMyPoint polyPt[32];
        for (size_t i = 0; i < m_vecClipPolygonPoints.size(); ++i)
        {
            polyPt[i].m_x = 
                m_pixelRect.m_left + (int)m_vecClipPolygonPoints[i].m_x.ToAbsolute((float)m_pixelRect.Width());
            polyPt[i].m_y = 
                m_pixelRect.m_top + (int)m_vecClipPolygonPoints[i].m_y.ToAbsolute((float)m_pixelRect.Height());
        }
        m_clipRegion.SetPolygon(polyPt, m_vecClipPolygonPoints.size());
    }
}

void CElement::OnParentSized()
{
    bool moved = false;
    bool sized = false;
    
    UpdatePixelRect(moved, sized);

    if (moved) 
        OnMoved();

    if (sized) 
        OnSized();

    if (moved || sized)
        UpdateClipRegion();
    else
        LayoutChildElements();
}

void CElement::OnSized()
{
    ResizeSnapshotBuffer();
    LayoutChildElements();

    for (size_t i = 0; i < m_vecChildElements.size(); ++i)
        m_vecChildElements[i]->OnParentSized();
}

void CElement::OnMoved()
{
}

void CElement::Draw(CDCBuffer& buffer, const CMyRect& dirtyRect)
{
    if (!IsVisible())
        return;

    if (!m_pixelRect.IsIntersect(dirtyRect))
        return;

    /// insersect clip region of this element
    if (m_bUsingClipRegion)
        buffer.IntersectClipRegion(m_clipRegion);
    else
        buffer.IntersectClipRect(m_pixelRect);

    // reserve snapshot
    if (m_bUseSnapshot && m_pDUIWnd->GetRootElement() != this)
    {
        buffer.CopyTo(m_snapshotBuffer.GetMemoryDC(),
            0, 0, m_pixelRect.Width(), m_pixelRect.Height(), m_pixelRect.m_left, m_pixelRect.m_top);
    }

    // draw itself
    DrawSelf(buffer);

    // draw the children
    DrawChildren(buffer, dirtyRect);
}

void CElement::LocalRedraw(CDCBuffer& buffer, const CMyRect& dirtyRect)
{
    AccumulateClipRegion();
    RestoreSnapshot();
    DrawSelf(buffer);
    DrawChildren(buffer, dirtyRect);
    m_pDUIWnd->UpdateDisplay(0);
}

void CElement::DrawSelf(CDCBuffer& buffer)
{

}

void CElement::DrawChildren(CDCBuffer& buffer, const CMyRect& dirtyRect)
{
    CRegion reservedRegion;
    for (size_t i = 0; i < m_vecChildElements.size(); ++i)
    {
        // reserve the current clip region of the render buffer before drawing child
        buffer.GetClipRegion(reservedRegion);

        // draw child
        m_vecChildElements[i]->Draw(buffer, dirtyRect);

        // restore the previously reserved clip region after drawing child
        buffer.SetClipRegion(reservedRegion);
    }
}

void CElement::OnMouseEnter(CMouseEventArgs& e)
{
}

void CElement::OnMouseLeave(CMouseEventArgs& e)
{

}

void CElement::OnMouseMove(CMouseEventArgs& e)
{

}

void CElement::OnMouseButtonDown(CMouseEventArgs& e)
{
    PublishEvent(L"MouseButtonDown", e);
}

void CElement::OnMouseButtonUp(CMouseEventArgs& e)
{

}

void CElement::OnMouseClicked(CMouseEventArgs& e)
{

}

CElement* CElement::GetChildAtPosition(const CVector2& pt) const
{
    if (!IsHit(pt))
        return 0;

    for (ElementVector::const_iterator child = m_vecChildElements.begin();
        child != m_vecChildElements.end();
        ++child)
    {
        if ((*child)->IsVisible())
        {
            CElement* element = (*child)->GetChildAtPosition(pt);
            if (element)
                return element;
        }
    }

    if (IsMousePassThroughEnabled())
        return 0;
    else
        return (CElement*)this;
        
}

bool CElement::IsHit(const CVector2& pt) const
{
    if (!IsEnabled())
        return false;
    return GetPixelRect().IsPointInRect(pt);
}

bool CElement::IsVisible(bool localOnly /*= false*/) const
{
    bool parentVisible = ((m_pParent == 0) || localOnly) ? true : m_pParent->IsVisible();
    return (m_bIsVisible && parentVisible);
}

void CElement::SetVisible(bool set)
{
	m_bIsVisible = set;
}

bool CElement::IsEnabled(bool localOnly /*= false*/) const
{
    bool parentEanbled = ((m_pParent == 0) || localOnly) ? true : m_pParent->IsEnabled();
    return (m_bIsEnabled && parentEanbled);
}

bool CElement::IsUsingClipRegion() const
{
    return m_bUsingClipRegion;
}

bool CElement::IsMousePassThroughEnabled() const
{
    return m_bIsMousePassThrough;
}

CElement* CElement::GetParent() const
{
    return m_pParent;
}

CDUIWindow* CElement::GetUIControler() const
{
    return m_pDUIWnd;
}

Meepo::CMyRect CElement::GetPixelRect() const
{
    return m_pixelRect;
}

void CElement::AddEvent(const StdStringW& name)
{
    if (!IsEventPresent(name))
        m_mapEvents[name] = new CEvent(name);
}

CEvent* CElement::GetEvent(const StdStringW& name, bool autoAdd/* = false*/)
{
    EventMap::iterator iter = m_mapEvents.find(name);
    if (iter == m_mapEvents.end())
    {
        if (autoAdd)
        {
            AddEvent(name);
            return m_mapEvents.find(name)->second;
        }
        else
            return 0;
    }
    return iter->second;
}

void CElement::RemoveEvent(const StdStringW& name)
{
    EventMap::iterator iter = m_mapEvents.find(name);
    if (iter != m_mapEvents.end())
    {
        delete iter->second;
        m_mapEvents.erase(iter);
    }
}

void CElement::RemoveAllEvents(void)
{
    for (EventMap::iterator iter = m_mapEvents.begin(); 
        iter != m_mapEvents.end();
        ++iter)
        delete iter->second;

    m_mapEvents.clear();
}

bool CElement::IsEventPresent(const StdStringW& name)
{
    return (m_mapEvents.find(name) != m_mapEvents.end());
} 

SubscribeRecord CElement::SubscribeEvent(const StdStringW& name, Subscriber& subscriber)
{
    return GetEvent(name, true)->Subscribe(subscriber);
}

void CElement::PublishEvent(const StdStringW& name, CEventArgs& args)
{
    CEvent* e = GetEvent(name);
    if (e)
        e->Publish(args);
}

void CElement::UpdatePixelRect(bool& moved, bool& sized)
{
    moved = false;
    sized = false;

    CMyRectF rcParent;
    if (m_pParent)
        rcParent = m_pParent->GetPixelRect();
    else if (m_pDUIWnd)
    {
        rcParent.SetPosition(CVector2F(0,0));
        rcParent.SetSize(m_pDUIWnd->GetSize());
    }

    CMyRectF absRectF = m_relRect.ToAbsolute(rcParent);
    CMyRectF newPixelRectF;
    switch (m_xAlignment)
    {
    case Alignment_Right:
    case Alignment_Center:
        break;
    default: // Alignment_Left
        newPixelRectF.m_left = rcParent.m_left + absRectF.m_left;
        newPixelRectF.m_right = rcParent.m_left + absRectF.m_right;
        break;
    }
    switch (m_yAlignment)
    {
    case Alignment_Bottom:
    case Alignment_Center:
        break;
    default: // Alignment_Top
        newPixelRectF.m_top = rcParent.m_top + absRectF.m_top;
        newPixelRectF.m_bottom = rcParent.m_top + absRectF.m_bottom;
        break;
    }

    moved = (m_pixelRect.GetPosition() != newPixelRectF.GetPosition())
            || (m_relRect.m_left.m_fScale != 0)
            || (m_relRect.m_top.m_fScale != 0);
    sized = (m_pixelRect.GetSize() != newPixelRectF.GetSize())
            || (m_relRect.m_right.m_fScale != 0)
            || (m_relRect.m_bottom.m_fScale != 0);

    m_pixelRect.m_left = (int)newPixelRectF.m_left;
    m_pixelRect.m_top = (int)newPixelRectF.m_top;
    m_pixelRect.m_right = (int)newPixelRectF.m_right;
    m_pixelRect.m_bottom = (int)newPixelRectF.m_bottom;
}

void CElement::AccumulateClipRegion()
{
    CDCBuffer& rootRenderBuffer = m_pDUIWnd->GetRenderBuffer();
    CElement* pRootElement = m_pDUIWnd->GetRootElement();
    if (pRootElement->IsUsingClipRegion())
        rootRenderBuffer.SetClipRegion(pRootElement->GetClipRegion());
    else
        rootRenderBuffer.SetClipRect(pRootElement->GetPixelRect());

    // 把所有的祖先节点放到一个栈里
    std::stack<CElement*> ancestors;
    ancestors.push(this);
    CElement* parent = m_pParent;
    while (parent)
    {
        ancestors.push(parent);
        parent = parent->GetParent();
    }

    // 从上到下依次叠加裁剪区域
    while (!ancestors.empty())
    {
        CElement* ancest = ancestors.top();

        if (ancest->IsUsingClipRegion()) 
            rootRenderBuffer.IntersectClipRegion(ancest->GetClipRegion());
        else
            rootRenderBuffer.IntersectClipRect(ancest->GetPixelRect());
        ancestors.pop();
    }

}

CRegion& CElement::GetClipRegion()
{
    return m_clipRegion;
}

void CElement::ResizeSnapshotBuffer()
{
    if (m_snapshotBuffer.GetHeight() != m_pixelRect.Height()
        || m_snapshotBuffer.GetWidth() != m_pixelRect.Width())
        m_snapshotBuffer.Resize(m_pixelRect.Width(), m_pixelRect.Height());
    m_bIsSnapshotValid = false;
}

void CElement::SetSnapshotInvalid(bool recursive /* = false */)
{
    m_bIsSnapshotValid = false;

    for (ElementVector::iterator child = m_vecChildElements.begin();
        child != m_vecChildElements.end();
        ++child)
    {
        (*child)->SetSnapshotInvalid(true);
    }
}

void CElement::LayoutChildElements()
{

}

void CElement::SetUsingClipRegionFlag(bool flag)
{
    m_bUsingClipRegion = flag;
}

void CElement::RestoreSnapshot()
{
    m_snapshotBuffer.CopyTo(m_pDUIWnd->GetRenderBuffer().GetMemoryDC(),
        m_pixelRect.m_left, m_pixelRect.m_top, 
        m_pixelRect.Width(), m_pixelRect.Height(), 0, 0);
}

void CElement::GetMouseCapture()
{
    m_pDUIWnd->SetMouseCaptureElement(this);
}

void CElement::ReleaseMouseCapture()
{
    m_pDUIWnd->SetMouseCaptureElement(0);
    m_pDUIWnd->InjectMouseMove(m_pDUIWnd->GetCurrentMousePosition().m_x, 
        m_pDUIWnd->GetCurrentMousePosition().m_y);
}

void CElement::SetAlpha(int alpha, bool redraw/* = false*/)
{

}

void CElement::SetArea(const CRelRect& rc, bool redraw /*= false*/)
{
	SetArea(rc.GetPosition(), rc.GetSize(), redraw);
}

void CElement::SetArea(const CRelVector& rpos, const CRelVector& rsize, bool redraw /*= false*/)
{
//     if (redraw)
//     {
//         AccumulateClipRegion();
// 
//         /// insersect clip region of this element
//         if (m_bUsingClipRegion)
//             m_pDUIWnd->GetRenderBuffer().IntersectClipRegion(m_clipRegion);
//         else
//             m_pDUIWnd->GetRenderBuffer().IntersectClipRect(m_pixelRect);
// 
//         RestoreSnapshot();
//     }
    
    if (redraw)
        RedrawOnChange();

    m_relRect.SetPosition(rpos);
    m_relRect.SetSize(rsize);
    bool moved = false;
    bool sized = false;
    UpdatePixelRect(moved, sized);

    if (moved)
        OnMoved();
    if (sized)
        OnSized();

    if (redraw)
        RedrawOnChange();
}

void CElement::SetPosition(const CRelVector& rpos, bool redraw /*= false*/)
{
    SetArea(rpos, m_relRect.GetSize(), redraw);
}

void CElement::SetSize(const CRelVector& rsize, bool redraw /*= false*/)
{
    SetArea(m_relRect.GetPosition(), rsize, redraw);
}

void CElement::SetXPosition(const CRelValue& rx, bool redraw /*= false*/)
{
    SetArea(CRelVector(rx, m_relRect.m_top), m_relRect.GetSize(), redraw);
}

void CElement::SetMousePassThrough(bool flag)
{
    m_bIsMousePassThrough = flag;
}

void CElement::SetUseSnapshotFlag(bool flag)
{
    m_bUseSnapshot = flag;
}

void CElement::RedrawOnChange()
{
    if (m_bUseSnapshot && !m_pDUIWnd->GetRedrawFlag())
        LocalRedraw(m_pDUIWnd->GetRenderBuffer(), m_pixelRect);
    else
        m_pDUIWnd->SetDirtyRect(m_pixelRect);
}

void CElement::SetAttribute(const StdStringW& name, const StdStringW& val)
{
	if (name == L"pos")
	{
		CRelVector pos = StringHelper::StringToRelVector(val);
		SetRelativePosition(pos.m_x, pos.m_y);
	}
	else if (name == L"size")
	{
		CRelVector sz = StringHelper::StringToRelVector(val);
		SetRelativeSize(sz.m_x, sz.m_y);
	}
	else if (name == L"area")
	{
		CRelRect rc = StringHelper::StringToRelRect(val);
		SetArea(rc);
	}
	else if (name == L"alpha")
	{
		int alpha = StringHelper::StringToInt(val);
		SetAlpha(alpha);
	}
	else if (name == L"visible")
	{
		SetVisible(StringHelper::StringToBool(val));
	}
}

void CElement::SetZOrder(int zorder)
{
	m_zOrder = zorder;
}

int CElement::GetZOrder() const
{
	return m_zOrder;
}



}