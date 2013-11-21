/***********************************************************************
    filename:   XmlAttributes.h
    created:    2012/7/15
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include <string>
#include <map>

namespace Meepo
{

class CXmlAttributes
{
public:
    void Add(const std::wstring& name, const std::wstring& value);
    void Remove(const std::wstring& attrName);
    bool Exist(const std::wstring& attrName) const;

    size_t GetCount() const;

    const std::wstring&	GetName(size_t index) const;
    const std::wstring&	GetValue(size_t index) const;
    const std::wstring&	GetValue(const std::wstring& name) const;

private:
    typedef std::map<std::wstring, std::wstring> AttributeMap;
    AttributeMap m_Attrs;
};

} // namespace Meepo
