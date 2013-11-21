/***********************************************************************
    filename:   Event.h
    created:    2011/10/3
    author:     ZhaoHao

    purpose:   	
*************************************************************************/
#pragma once

#include "Delegate.h"
#include "RefCountPtr.h"
#include "InputEvent.h"
#include "SubscribeSlot.h"
#include <vector>
#include <string>

namespace Meepo
{

typedef CRefCountPtr<CSubscribeSlot> SubscribeRecord;

class CEvent
{
public:

	CEvent(const StdStringW& name) : m_strName(name) {}
    ~CEvent();

    const StdStringW& GetName() const;
    SubscribeRecord	Subscribe(Subscriber& subscriber);
	void Unsubscribe(const CSubscribeSlot& slot);
	void Publish(CEventArgs& args);

private:
    typedef std::vector<SubscribeRecord> SubscribeRecordVector;
    SubscribeRecordVector m_vecSubscribeSlots; 
	const StdStringW m_strName; 
};

} // namespace Meepo