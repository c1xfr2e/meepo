#include "ChainedXmlHandler.h"

namespace Meepo
{

CChainedXmlHandler::CChainedXmlHandler() :
    m_pChainedHandler(0),
    m_bIsCompleted(false)
{
}

CChainedXmlHandler::~CChainedXmlHandler()
{
    CleanupChainedHandler();
}

void CChainedXmlHandler::ElementStart(const std::wstring& element, const CXmlAttributes& attributes)
{
    if (m_pChainedHandler)
    {
        m_pChainedHandler->ElementStart(element, attributes);

        if (m_pChainedHandler->IsCompleted())
		{        
			CleanupChainedHandler();
		}
	}
    else
	{
		ElementStartLocal(element, attributes);
	}
}

void CChainedXmlHandler::ElementEnd(const std::wstring& element)
{
    if (m_pChainedHandler)
    {
        m_pChainedHandler->ElementEnd(element);

        if (m_pChainedHandler->IsCompleted())
		{
			CleanupChainedHandler();
		}
    }
    else
	{
		ElementEndLocal(element);
	}
}

bool CChainedXmlHandler::IsCompleted() const
{
    return m_bIsCompleted;
}

void CChainedXmlHandler::CleanupChainedHandler()
{
    delete m_pChainedHandler;
    m_pChainedHandler = 0;
}

} // End of Meepo

