/***********************************************************************
    filename:   XmlParser.h
    created:    2012/7/15
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include <string>

namespace Meepo
{

class CXmlHandler;

class CXmlParser
{
public:
	CXmlParser();
    virtual ~CXmlParser();
    virtual int ParseFile(CXmlHandler& handler, const std::wstring& filePathname) = 0;
};

} // namespace Meepo
