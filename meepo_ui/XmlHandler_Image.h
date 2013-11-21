/***********************************************************************
    filename:   XmlHandler_Image.h
    created:    2012/07/15
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include "XmlHandler.h"
#include <string>
#include <stack>

namespace Meepo
{

class CSkin;

class CXmlHandler_Image : public CXmlHandler
{
public:
	CXmlHandler_Image();
	virtual ~CXmlHandler_Image();

    virtual void OnElementStart(const std::wstring& element, const CXmlAttributes& attributes);
    virtual void OnElementEnd(const std::wstring& element);

private:
	void OnElementImageStart(const CXmlAttributes& attributes);
	void OnElementImageEnd();
	void OnElementSubStart(const CXmlAttributes& attributes);

	void CreateImage(const CXmlAttributes& attr, CSkin* theTexture);

	CSkin* m_pCurrentSkin;
};

} // namespace Meepo
