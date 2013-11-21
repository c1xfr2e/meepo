/***********************************************************************
    filename:   Rect.h
    created:    2012/6/3
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include "Vector.h"

namespace Meepo
{

template <typename T>
class CRectT
{
public:
	T m_left;
	T m_top;
	T m_right;
	T m_bottom;

	CRectT() : m_top(0), m_bottom(0), m_left(0), m_right(0) {}
	CRectT(T left, T top, T right, T bottom) : m_top(top), m_bottom(bottom), m_left(left), m_right(right) {}
	CRectT(CVector2T<T> pos, CVector2T<T> sz) : m_top(pos.m_y), m_bottom(pos.m_y + sz.m_y), m_left(pos.m_x), m_right(pos.m_x + sz.m_x) {}

	template <typename T1>
	CRectT(T1 left, T1 top, T1 right, T1 bottom) : 
		m_top(static_cast<T>(top)), m_bottom(static_cast<T>(bottom)), m_left(static_cast<T>(left)), m_right(static_cast<T>(right)) 
		{}
	template <typename T1>
	CRectT(const CRectT<T1>& other) : 
		m_top(static_cast<T>(other.m_top)), m_bottom(static_cast<T>(other.m_bottom)), m_left(static_cast<T>(other.m_left)), m_right(static_cast<T>(other.m_right)) 
		{}

	T Width() const { return m_right - m_left; }
	T Height() const { return m_bottom - m_top; }
	void SetWidth(T width) { m_right = m_left + width; }
	void SetHeight(T height) { m_bottom = m_top + height; }

	CVector2T<T> GetPosition() const { return CVector2T<T>(m_left, m_top); }
	void SetPosition(const CVector2T<T>& pt) { CVector2T<T> sz(GetSize()); m_left = pt.m_x; m_top  = pt.m_y; SetSize(sz); }

	CVector2T<T> GetSize(void) const { return CVector2T<T>(Width(), Height()); }
	void SetSize(const CVector2T<T>& size) { SetWidth(size.m_x); SetHeight(size.m_y); }

	CRectT<T>& Offset(const CVector2T<T>& offset) { m_left += offset.m_x; m_right += offset.m_x; m_top += offset.m_y; m_bottom += offset.m_y; return *this; }
	CRectT<T>& InverseOffset(const CVector2T<T>& offset) { m_left -= offset.m_x; m_right -= offset.m_x; m_top -= offset.m_y; m_bottom -= offset.m_y; return *this; }

	void Deflate(T x, T y) { m_left += x; m_right -= x; m_top += y; m_bottom -= y; }
	void Inflate(T x, T y) { m_left -= x; m_right += x; m_top	-= y; m_bottom += y; }

	bool IsPointInRect(const CVector2T<T>& pt) const 
	{
		if (((m_left > pt.m_x) || (m_right <= pt.m_x) || (m_top > pt.m_y) || (m_bottom <= pt.m_y))) 
		{
			return false;
		}
		return true;
	}
	
	CRectT& ConstrainSizeMax(const CVector2T<T>& size)
	{
		if (Width() > size.m_x)
		{
			SetWidth(size.m_x);
		}

		if (Height() > size.m_y)
		{
			SetHeight(size.m_y);
		}

		return *this;
	}

	CRectT& ConstrainSizeMin(const CVector2T<T>& size)
	{
		if (Width() < size.m_x)
		{
			SetWidth(size.m_x);
		}

		if (Height() < size.m_y)
		{
			SetHeight(size.m_y);
		}

		return *this;
	}

	CRectT& ConstrainSize(const CVector2T<T>& max, const CVector2T<T>& min)
	{
		CVector2T currSize(GetSize());

		if (currSize.m_x > max.m_x)
		{
			SetWidth(max.m_x);
		}
		else if (currSize.m_x < min.m_x)
		{
			SetWidth(min.m_x);
		}

		if (currSize.m_y > max.m_y)
		{
			SetHeight(max.m_y);
		}
		else if (currSize.m_y < min.m_y)
		{
			SetHeight(min.m_y);
		}

		return *this;
	}

	CRectT GetIntersection(const CRectT& rect) const
	{
		if ((m_right > rect.m_left) 
			&& (m_left < rect.m_right) 
			&& (m_bottom > rect.m_top) 
			&& (m_top < rect.m_bottom))
		{
			CRectT temp;

			temp.m_left = (m_left > rect.m_left) ? m_left : rect.m_left;
			temp.m_right = (m_right < rect.m_right) ? m_right : rect.m_right;
			temp.m_top = (m_top > rect.m_top) ? m_top : rect.m_top;
			temp.m_bottom = (m_bottom < rect.m_bottom) ? m_bottom : rect.m_bottom;

			return temp;
		}
		else
			return CRectT(0, 0, 0, 0);
	}

    bool IsIntersect(const CRectT& rect) const 
    {
        CRectT tmp = GetIntersection(rect);
        return (tmp != CRectT(0, 0, 0, 0));
    }

	bool operator==(const CRectT& rhs) const
	{
		return ((m_left == rhs.m_left) && (m_right == rhs.m_right) && (m_top == rhs.m_top) && (m_bottom == rhs.m_bottom));
	}

	bool operator!=(const CRectT& rhs) const
	{ 
		return !operator==(rhs);
	}

	CRectT& operator=(const CRectT& rhs)
	{
		m_left = rhs.m_left; 
		m_top = rhs.m_top; 
		m_right = rhs.m_right;
		m_bottom = rhs.m_bottom; 
		return *this;
	}

	CRectT operator+(const CRectT& rhs) const
	{
		return CRectT(m_left + rhs.m_left, m_top + rhs.m_top, m_right + rhs.m_right, m_bottom + rhs.m_bottom);
	}
	
	CRectT operator-(const CRectT& rhs) const
	{
		return CRectT(m_left - rhs.m_left, m_top - rhs.m_top, m_right - rhs.m_right, m_bottom - rhs.m_bottom);
	}

	CRectT operator*(float scalar) const 
	{
		return CRectT((T)(m_left * scalar), (T)(m_top * scalar), (T)(m_right * scalar), (T)(m_bottom * scalar));
	}

    const CRectT& operator*=(T scalar) 
	{ 
		m_left *= scalar;
		m_top *= scalar; 
		m_right *= scalar;
		m_bottom *= scalar;
		return *this; 
	}
};

typedef CRectT<int> CMyRect;
typedef CRectT<float> CMyRectF;

}
