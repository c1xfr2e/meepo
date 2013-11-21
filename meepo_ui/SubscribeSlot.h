/***********************************************************************
    filename:   eSubscribeSlot.h
    created:    2011/11/27
    author:     ZhaoHao

    purpose:   	
*************************************************************************/
#pragma once

#include "Delegate.h"

namespace Meepo
{

class CEvent;
class CEventArgs;

typedef CDelegate<bool, const CEventArgs&> Subscriber;

class CSubscribeSlot
{
public:
	CSubscribeSlot(CEvent& event, const Subscriber& subscriber) 
        : m_pSubscriber(new Subscriber(subscriber)),
		  m_pEvent(&event) {}
	CSubscribeSlot(const CSubscribeSlot& other) : 
		m_pSubscriber(other.m_pSubscriber),
		m_pEvent(other.m_pEvent)
	{}
	~CSubscribeSlot()
	{
		Disconnect();
		delete m_pSubscriber;
	}

	bool IsConnected() const
	{
		return (m_pSubscriber != 0) && m_pSubscriber->IsInUse();
	}

	void Disconnect();

	bool operator==(const CSubscribeSlot& other) const
	{
		return m_pSubscriber == other.m_pSubscriber;
	}

	bool operator!=(const CSubscribeSlot& other) const
	{
		return !(*this == other);
	}

private:
	friend class CEvent;

	Subscriber*	m_pSubscriber;	// The actual slot object.
	CEvent* m_pEvent;		    // The event to which the subscriber was attached
};

} // namespace Meepo
