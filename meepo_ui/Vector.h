/***********************************************************************
    filename:   Vector.h
    created:    2010/10/1
    author:     ZhaoHao
*************************************************************************/
#pragma once

namespace Meepo
{

template <typename T>
class CVector2T
{
public:
	T m_x;
	T m_y;

	CVector2T(void) : m_x(0), m_y(0) {}
	CVector2T(T x, T y) : m_x(x), m_y(y) {}

	template<typename T1>
	CVector2T(T1 x, T1 y) : m_x(static_cast<T>(x)), m_y(static_cast<T>(y)) {}	

	template<typename T1>
	CVector2T(const CVector2T<T1> &other) : m_x(static_cast<T>(other.m_x)), m_y(static_cast<T>(other.m_y)) {}	

	template <typename T1>
	CVector2T& operator=(const CVector2T<T1>& rhs)
	{
		if (&rhs != this) 
		{
			m_x = static_cast<T>(rhs.m_x);
			m_y = static_cast<T>(rhs.m_y);
		}
		return *this;
	}

	CVector2T&  Offset(const CVector2T<T>& offset)		 { m_x += offset.m_x; m_y += offset.m_y; return *this; }
	CVector2T&  InverseOffset(const CVector2T<T>& offset) { m_x -= offset.m_x; m_y -= offset.m_y; return *this; }

	CVector2T& operator+=(const CVector2T& vec) { m_x += vec.m_x; m_y += vec.m_y; return *this; }
	CVector2T& operator-=(const CVector2T& vec) { m_x -= vec.m_x; m_y -= vec.m_y; return *this; }
	CVector2T& operator*=(const CVector2T& vec) { m_x *= vec.m_x; m_y *= vec.m_y; return *this; }
	CVector2T& operator/=(const CVector2T& vec) { m_x /= vec.m_x; m_y /= vec.m_y; return *this; }

	CVector2T operator+(const CVector2T& vec) const { return CVector2T(m_x + vec.m_x, m_y + vec.m_y); }
	CVector2T operator-(const CVector2T& vec) const { return CVector2T(m_x - vec.m_x, m_y - vec.m_y); }
	CVector2T operator*(const CVector2T& vec) const { return CVector2T(m_x * vec.m_x, m_y * vec.m_y); }

	CVector2T operator*(float c) const { return CVector2T((T)(m_x * c), (T)(m_y * c)); }

	bool operator==(const CVector2T& vec) const { return ((m_x == vec.m_x) && (m_y == vec.m_y)); }
	bool operator!=(const CVector2T& vec) const { return !(operator==(vec)); }
};

typedef CVector2T<int> CVector2;
typedef CVector2T<int> CMyPoint;
typedef CVector2T<float> CVector2F;
typedef CVector2T<float> CMyPointF;

}
