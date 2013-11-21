/***********************************************************************
    filename:   UIEvent.cpp
    created:    2011/10/3
    author:     ZhaoHao

    purpose:   	
*************************************************************************/
#include "Event.h"
#include "SubscribeSlot.h"
#include <algorithm>

namespace Meepo
{

class CSubscribeSlotCompare
{
public:
    CSubscribeSlotCompare(const CSubscribeSlot& s) 
        : m_refSubscribeSlot(s) {}

    bool operator()(const SubscribeRecord& conn) const
    {
        return *(conn) == m_refSubscribeSlot;
    }
private:
	const CSubscribeSlot& m_refSubscribeSlot;
};

CEvent::~CEvent()
{
	for (SubscribeRecordVector::iterator it = m_vecSubscribeSlots.begin();
		it != m_vecSubscribeSlots.end();
		++it)
	{
		(*it)->m_pEvent = 0;
		(*it)->m_pSubscriber->Clean();
	}

	m_vecSubscribeSlots.clear();
}

SubscribeRecord CEvent::Subscribe(Subscriber& subscriber)
{
	SubscribeRecord conn(new CSubscribeSlot(*this, subscriber));
	m_vecSubscribeSlots.push_back(conn);
	return conn;
}

void CEvent::Unsubscribe(const CSubscribeSlot& slot)
{
	// try to find the slot in our collection
	SubscribeRecordVector::iterator curr =
		std::find_if(m_vecSubscribeSlots.begin(), m_vecSubscribeSlots.end(), CSubscribeSlotCompare(slot));

	// erase our reference to the slot, if we had one.
	if (curr != m_vecSubscribeSlots.end())
		m_vecSubscribeSlots.erase(curr);
}

void CEvent::Publish(CEventArgs& args)
{
	for (SubscribeRecordVector::iterator slot = m_vecSubscribeSlots.begin();
		 slot != m_vecSubscribeSlots.end();
		 ++slot)
	{
		args.m_bHandled |= (*slot)->m_pSubscriber->operator()(args);
	}
}

const StdStringW& CEvent::GetName() const
{
    return m_strName; 
}

} // namespace Meepo

