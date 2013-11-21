#include "TinyParser.h"
#include "ResourceManager.h"
#include "XmlHandler.h"
#include "XmlAttributes.h"
#include "../tinyxml/tinyxml.h"

#include "atlconv.h"

namespace Meepo
{

	static void ProcessTiElement(CXmlHandler& handler, const TiXmlElement* element)
	{
		// build attributes for the element
		CXmlAttributes attrs;

		const TiXmlAttribute *currAttr = element->FirstAttribute();
		while (currAttr)
		{
			attrs.Add(StdStringW(ATL::CA2W(currAttr->Name())), StdStringW(ATL::CA2W(currAttr->Value())));
			currAttr = currAttr->Next();
		}

		// element start 
		handler.OnElementStart(StdStringW(ATL::CA2W(element->Value())), attrs);

		// do children
		const TiXmlNode* childNode = element->FirstChild();
		while (childNode)
		{
			switch(childNode->Type())
			{
			case TiXmlNode::TINYXML_ELEMENT:
				ProcessTiElement(handler, childNode->ToElement());
				break;
			case  TiXmlNode::TINYXML_TEXT:
				if (childNode->ToText()->Value() != '\0')
					handler.OnText(StdStringW(ATL::CA2W(childNode->ToText()->Value())));
				break;

				// Silently ignore unhandled node type
			};
			childNode = childNode->NextSibling();
		}

		// element end
		handler.OnElementEnd(StdStringW(ATL::CA2W(element->Value())));
	}

    int CTinyParser::ParseFile(CXmlHandler& handler, const std::wstring& filePathname)
    {
		CHeapData data;
		CResourceManager::Instance().LoadFile(filePathname, data, 2);

		// The addition of the newline is a kludge to resolve an issue
		// whereby parse returns 0 if the xml file has no newline at the end but
		// is otherwise well formed.
		unsigned int size = data.GetSize();
		data[size - 2] = '\n';
		data[size - 1] = 0;

		TiXmlDocument doc;
		if (!doc.Parse((const char*)data.GetBuf()))
			return -1;

		const TiXmlElement* rootElement = doc.RootElement();
		if (!rootElement)
			return -1;
		ProcessTiElement(handler, rootElement);

		return 0;
    }

} // End of  CEGUI namespace section
