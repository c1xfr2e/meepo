/***********************************************************************
    filename:   RelativeCoord.h
    created:    2012/6/9
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include "Vector.h"
#include "Rect.h"

#define ABS_VAL(x) (CRelValue(0,(float)(x)))
#define REL_VAL(x) (CRelValue((float)(x),0))

namespace Meepo
{

class CRelValue
{
public:
	float m_fScale;
	float m_fOffset;

	CRelValue() : m_fScale(0.0f), m_fOffset(0.0f) {}
    CRelValue(float scale, float offset) : m_fScale(scale), m_fOffset(offset) {}
    CRelValue(const CRelValue& other): m_fScale(other.m_fScale), m_fOffset(other.m_fOffset) {}

    float ToAbsolute(float base) const { return (base * m_fScale) + m_fOffset; }

    CRelValue operator+(const CRelValue& other) const { return CRelValue(m_fScale + other.m_fScale, m_fOffset + other.m_fOffset); }
    CRelValue operator-(const CRelValue& other) const { return CRelValue(m_fScale - other.m_fScale, m_fOffset - other.m_fOffset); }
    CRelValue operator*(const CRelValue& other) const { return CRelValue(m_fScale * other.m_fScale, m_fOffset * other.m_fOffset); }
	CRelValue operator/(const CRelValue& other) const { return CRelValue(other.m_fScale == 0.0f ? 0.0f : m_fScale / other.m_fScale, other.m_fOffset == 0 ? 0 : m_fOffset / other.m_fOffset); }

    const CRelValue& operator+=(const CRelValue& other) { m_fScale += other.m_fScale; m_fOffset += other.m_fOffset; return *this; }
    const CRelValue& operator-=(const CRelValue& other) { m_fScale -= other.m_fScale; m_fOffset -= other.m_fOffset; return *this; }
    const CRelValue& operator*=(const CRelValue& other) { m_fScale *= other.m_fScale; m_fOffset *= other.m_fOffset; return *this; }
    const CRelValue& operator/=(const CRelValue& other) { m_fScale = (other.m_fScale == 0.0f ? 0.0f : m_fScale / other.m_fScale); m_fOffset = (other.m_fOffset == 0 ? 0 : m_fOffset / other.m_fOffset); return *this; }

    bool operator==(const CRelValue& other) const { return m_fScale == other.m_fScale && m_fOffset == other.m_fOffset; }
    bool operator!=(const CRelValue& other) const { return !operator==(other); }
};

class CRelVector
{
public:
	CRelValue m_x;
	CRelValue m_y;

	CRelVector() {}
    CRelVector(const CRelValue& x, const CRelValue& y) : m_x(x), m_y(y) {}
    CRelVector(const CRelVector& other): m_x(other.m_x), m_y(other.m_y) {}

    CVector2F ToAbsolute(const CVector2F& base) const 
	{
		return CVector2F(m_x.ToAbsolute(base.m_x), m_y.ToAbsolute(base.m_y)); 
	}

	CRelVector operator+(const CRelVector& other) const { return CRelVector(m_x + other.m_x, m_y + other.m_y); }
    CRelVector operator-(const CRelVector& other) const { return CRelVector(m_x - other.m_x, m_y - other.m_y); }
    CRelVector operator/(const CRelVector& other) const { return CRelVector(m_x / other.m_x, m_y / other.m_y); }
    CRelVector operator*(const CRelVector& other) const { return CRelVector(m_x * other.m_x, m_y * other.m_y); }

    const CRelVector& operator+=(const CRelVector& other) { m_x += other.m_x; m_y += other.m_y; return *this; }
    const CRelVector& operator-=(const CRelVector& other) { m_x -= other.m_x; m_y -= other.m_y; return *this; }
    const CRelVector& operator/=(const CRelVector& other) { m_x /= other.m_x; m_y /= other.m_y; return *this; }
    const CRelVector& operator*=(const CRelVector& other) { m_x *= other.m_x; m_y *= other.m_y; return *this; }

    CRelVector operator+(const CRelValue& dim) const { return CRelVector(m_x + dim, m_y + dim); }
	CRelVector operator-(const CRelValue& dim) const { return CRelVector(m_x - dim, m_y - dim); }
	CRelVector operator/(const CRelValue& dim) const { return CRelVector(m_x / dim, m_y / dim); }
    CRelVector operator*(const CRelValue& dim) const { return CRelVector(m_x * dim, m_y * dim); }

    const CRelVector& operator+=(const CRelValue& dim) { m_x += dim; m_y += dim; return *this; }
    const CRelVector& operator-=(const CRelValue& dim) { m_x -= dim; m_y -= dim; return *this; }
    const CRelVector& operator/=(const CRelValue& dim) { m_x /= dim; m_y /= dim; return *this; }
    const CRelVector& operator*=(const CRelValue& dim) { m_x *= dim; m_y *= dim;  return *this; }

    bool operator==(const CRelVector& other) const { return m_x == other.m_x && m_y == other.m_y; }
    bool operator!=(const CRelVector& other) const { return !operator==(other); }
};

class CRelRect
{
public:
    CRelValue m_left;
    CRelValue m_top;
    CRelValue m_right;
    CRelValue m_bottom;

	CRelRect() {}
	CRelRect(const CRelValue& left, const CRelValue& top, const CRelValue& right, const CRelValue& bottom)
        : m_left(left), m_right(right), m_top(top), m_bottom(bottom)
    {}

    CMyRectF ToAbsolute(const CMyRectF& base) const
    {
        return CMyRectF(
            m_left.ToAbsolute(base.Width()),
            m_top.ToAbsolute(base.Height()),
            m_right.ToAbsolute(base.Width()),
            m_bottom.ToAbsolute(base.Height())
            );     
    }

    CRelValue GetWidth() const { return m_right - m_left; }
    CRelValue GetHeight() const { return m_bottom - m_top; }
    void SetWidth(const CRelValue& w) { m_right = m_left + w; }
    void SetHeight(const CRelValue& h) { m_bottom = m_top + h; }
    CRelVector GetPosition() const { return CRelVector(m_left, m_top); }
	void SetPosition(const CRelVector& pos) 
    { 
        CRelValue w = GetWidth();
        CRelValue h = GetHeight();
        m_left = pos.m_x;
        m_top = pos.m_y;
        SetWidth(w);
        SetHeight(h);
    }
    CRelVector GetSize() const { return CRelVector(GetWidth(), GetHeight()); }
	void SetSize(const CRelVector& sz) 
    {
        m_right = m_left + sz.m_x;
        m_bottom = m_top + sz.m_y;
    }

    CRelRect operator+(const CRelRect& r) const  
    {
        return CRelRect(m_left + r.m_left, m_top + r.m_top, m_right + r.m_right, m_bottom + r.m_bottom); 
    }
	CRelRect operator*(const CRelValue& v) const 
    { 
        return CRelRect(m_left * v, m_top * v, m_right * v, m_bottom * v); 
    }
};

}

