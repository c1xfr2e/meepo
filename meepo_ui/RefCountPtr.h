/***********************************************************************
    filename:   CRefCountPtr.h
    created:    2010/10/2
    author:     ZhaoHao

    purpose:   	Simple reference counter class.
*************************************************************************/
#pragma once 

namespace Meepo
{

// 一个简单的引用计数智能指针
template<typename T>
class CRefCountPtr
{
public:
    CRefCountPtr() : m_pObject(0), m_pCounter(0) {}
    CRefCountPtr(T* object) 
        : m_pObject(object), m_pCounter(object?new int(1):0) {}
    CRefCountPtr(const CRefCountPtr<T>& other)
        : m_pObject(other.m_pObject), m_pCounter(other.m_pCounter)
    {
        if (m_pCounter)
			AddRef();
    }

    ~CRefCountPtr()
    {
        if (m_pObject)
            Release();
	}

    CRefCountPtr<T>& operator=(const CRefCountPtr<T>& other)
    {
        if (*this == other)
            return *this;

        if (m_pObject)
            Release();

        m_pObject = other.m_pObject;
        m_pCounter = m_pObject ? other.m_pCounter : 0;

        if (m_pCounter)
            AddRef();

        return *this;
    }

    bool operator==(const CRefCountPtr<T>& other) const
    {
        return m_pObject == other.m_pObject;
    }

    bool operator!=(const CRefCountPtr<T>& other) const
    {
        return m_pObject != other.m_pObject;
    }

    const T& operator*() const{ return *m_pObject; }
    T& operator*() { return *m_pObject; }

    const T* operator->() const { return m_pObject;}
    T* operator->() { return m_pObject;}

private:
    void AddRef() { ++*m_pCounter; }
    void Release()
    {
        if (--*m_pCounter == 0)
        {
            delete m_pObject;
            delete m_pCounter;
            m_pObject = 0;
            m_pCounter = 0;
        }
    }

    T*	m_pObject;            
    int* m_pCounter;
};

} // namespace Meepo