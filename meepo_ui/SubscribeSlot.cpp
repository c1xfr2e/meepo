/***********************************************************************
    filename:   SubscribeSlot.cpp
    created:    2011/11/27
    author:     ZhaoHao

    purpose:   	
*************************************************************************/
#include "SubscribeSlot.h"
#include "Event.h"

namespace Meepo
{

void CSubscribeSlot::Disconnect()
{
	if (IsConnected())
		m_pSubscriber->Clean();

	if (m_pEvent)
	{
		m_pEvent->Unsubscribe(*this);
		m_pEvent = 0;
	}
}

} // namespace Meepo

