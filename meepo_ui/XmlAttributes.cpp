/***********************************************************************
    filename:   CXmlAttributes.cpp
    created:    2012/7/15
    author:     ZhaoHao
*************************************************************************/
#include "XmlAttributes.h"

namespace Meepo
{

void CXmlAttributes::Add(const std::wstring& name, const std::wstring& value)
{
    m_Attrs[name] = value;
}

void CXmlAttributes::Remove(const std::wstring& name)
{
    AttributeMap::iterator pos = m_Attrs.find(name);
    if (pos != m_Attrs.end())
    {
        m_Attrs.erase(pos);
    }
}

bool CXmlAttributes::Exist(const std::wstring& name) const
{
    return (m_Attrs.find(name) != m_Attrs.end());
}

size_t CXmlAttributes::GetCount() const
{
    return m_Attrs.size();
}

const std::wstring& CXmlAttributes::GetName(size_t index) const
{
    if (index >= m_Attrs.size())
    {
		static std::wstring empty(L"");
        return empty;
    }
    AttributeMap::const_iterator iter = m_Attrs.begin();
    std::advance(iter, index);
    return (*iter).first;
}

const std::wstring& CXmlAttributes::GetValue(size_t index) const
{
	if (index >= m_Attrs.size())
	{
		static std::wstring empty(L"");
		return empty;
	}
    AttributeMap::const_iterator iter = m_Attrs.begin();
    std::advance(iter, index);
    return (*iter).second;
}

const std::wstring& CXmlAttributes::GetValue(const std::wstring& name) const
{
    AttributeMap::const_iterator pos = m_Attrs.find(name);
    if (pos != m_Attrs.end())
    {
        return (*pos).second;
    }
    else
    {
		static std::wstring empty(L"");
		return empty;
    }
}

} // namespace Meepo
