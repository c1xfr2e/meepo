/***********************************************************************
    filename:   XmlHandler.h
    created:    2012/7/15
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include <string>

namespace Meepo
{

class CXmlAttributes;

class CXmlHandler
{
public:
    CXmlHandler();
	virtual ~CXmlHandler();
	virtual void OnElementStart(const std::wstring& element, const CXmlAttributes& attributes);
	virtual void OnElementEnd(const std::wstring& element);
	virtual void OnText(const std::wstring& text);
};

} // namespace Meepo
