/***********************************************************************
    filename:   CDelegate.h
    created:    2010/10/1
    author:     ZhaoHao

    purpose:    Simple delegate(only one parameter) to be used in event system.
*************************************************************************/
#pragma once

#include <new.h>
#include <memory.h>

namespace Meepo
{

template <class R, class P>
class CDelegate
{
private:
	class CInvoker
	{
	public:
		virtual R Invoke(P args) = 0;
	};

	class CFreeFunctionInvoker : public CInvoker
	{
	public:
		typedef R (*FunPtr)(P);

		CFreeFunctionInvoker(FunPtr fun) : m_Function(fun) {}

		virtual R Invoke(P args)
		{
			return m_Function(args);
		}

	private:
		FunPtr	m_Function;
	};

	template <class T>
	class CMemberFunctionInvoker : public CInvoker
	{
	public:
		typedef R (T::*MemberFunctionPtr)(P);

		CMemberFunctionInvoker(T* object, MemberFunctionPtr memberFun) :
		m_Object(object), 
			m_Function(memberFun)
		{}

		virtual R Invoke(P args)
		{
			return (m_Object->*m_Function)(args);
		}

	private:
		T*	m_Object;
		MemberFunctionPtr	m_Function;
	};

public:
	CDelegate(R (*fun)(P))
	{
		m_pInvoker = new (m_invokerObjBuffer) CFreeFunctionInvoker(fun);
	}

	template<class T, class R, class P>
	CDelegate(T* object, R (T::*function)(P))
	{ 
		m_pInvoker = new (m_invokerObjBuffer) CMemberFunctionInvoker<T>(object, function);
	}

	CDelegate(const CDelegate& other)
	{
		memcpy(m_invokerObjBuffer, other.m_invokerObjBuffer, sizeof(m_invokerObjBuffer));
		m_pInvoker = (CInvoker*)m_invokerObjBuffer;
	}

	R operator()(P args)
	{
		return m_pInvoker->Invoke(args);
	}

	bool IsInUse() const { return m_pInvoker != 0; }
	void Clean() { m_pInvoker = 0; }

private:
	CInvoker* m_pInvoker;
	char m_invokerObjBuffer[16]; // buffer for placement new invoker object
};

} // namespace Meepo

