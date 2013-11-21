#pragma once

#include "XmlHandler.h"

namespace Meepo
{

class CChainedXmlHandler : public CXmlHandler
{
public:
    CChainedXmlHandler();
    virtual ~CChainedXmlHandler();

    virtual void ElementStart(const std::wstring& element, const CXmlAttributes& attributes);
    virtual void ElementEnd(const std::wstring& element);
	virtual void RoolElementStart(const CXmlAttributes& attributes) {}

    bool IsCompleted() const; 

protected:
    // function that handles elements locally (used at end of handler chain)
	virtual void ElementStartLocal(const std::wstring& element, const CXmlAttributes& attributes) = 0;

    // function that handles elements locally (used at end of handler chain)
	virtual void ElementEndLocal(const std::wstring& element) = 0;

    // clean up any chained handler.
    void CleanupChainedHandler();

    // chained xml handler object.
    CChainedXmlHandler* m_pChainedHandler;

    bool m_bIsCompleted;
};

} // namespace Meepo

