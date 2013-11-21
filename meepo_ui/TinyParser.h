#pragma once

#include "XMLParser.h"

namespace Meepo
{

class CTinyParser : public CXmlParser
{
public:
	virtual int ParseFile(CXmlHandler& handler, const std::wstring& filePathname);

};

}
