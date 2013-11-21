/***********************************************************************
    filename:   XmlHandler_Image.cpp
    created:    2012/07/15
    author:     ZhaoHao
*************************************************************************/
#include "XmlHandler_Image.h"
#include "XmlAttributes.h"
#include "Image.h"
#include "Skin.h"
#include "ResourceManager.h"
#include <Shlwapi.h>
 
namespace Meepo
{

static const std::wstring ImageElement(L"image");
static const std::wstring SubElement(L"sub");
static const std::wstring ImageNameAttribute(L"name");
static const std::wstring ImageFileAttribute(L"file");
static const std::wstring ImageXPosAttribute(L"x");
static const std::wstring ImageYPosAttribute(L"y");
static const std::wstring ImageWidthAttribute(L"w");
static const std::wstring ImageHeightAttribute(L"h");
static const std::wstring ImageStretchModeAttribute(L"stretch_mode");
static const std::wstring ImageStretchDimAttribute(L"stretch_dim");
static const std::wstring StretchModeXY(L"xy");
static const std::wstring StretchModeX(L"x");
static const std::wstring StretchModeY(L"y");
static const std::wstring StretchModeFixed(L"fixed");

CXmlHandler_Image::CXmlHandler_Image()
{}

CXmlHandler_Image::~CXmlHandler_Image()
{}

void CXmlHandler_Image::OnElementStart(const std::wstring& element, const CXmlAttributes& attributes)
{
    if (element == ImageElement)
    {
        OnElementImageStart(attributes);
    }
	else if (element == SubElement)
	{
		OnElementSubStart(attributes);
	}
    else
    {
		//assert(0);
    }
}

void CXmlHandler_Image::OnElementEnd(const std::wstring& element)
{
    if (element == ImageElement)
    {
        OnElementImageEnd();
    }
}

void CXmlHandler_Image::OnElementImageStart(const CXmlAttributes& attributes)
{
	const std::wstring& file = attributes.GetValue(ImageFileAttribute);
    if (file.empty())
    {
        return;
    }

    CSkin* skin = new CSkin;
    CGlobalData data;
    CResourceManager::Instance().LoadFile(file, data);
    skin->Load(data);
    data.Dealloc();
    
    m_pCurrentSkin = skin;
	CreateImage(attributes, skin);
}

void CXmlHandler_Image::OnElementImageEnd()
{
	m_pCurrentSkin = NULL;
}

void CXmlHandler_Image::OnElementSubStart(const CXmlAttributes& attributes)
{
    if (m_pCurrentSkin != NULL)
    {
        CreateImage(attributes, m_pCurrentSkin);
    }
}

void CXmlHandler_Image::CreateImage(const CXmlAttributes& attributes, CSkin* skin)
{
	const std::wstring& name = attributes.GetValue(ImageNameAttribute);

	int x = StrToIntW(attributes.GetValue(ImageXPosAttribute).c_str());
	int y = StrToIntW(attributes.GetValue(ImageYPosAttribute).c_str());
	int w = StrToIntW(attributes.GetValue(ImageWidthAttribute).c_str());
	int h = StrToIntW(attributes.GetValue(ImageHeightAttribute).c_str());
    if (w == 0)
    {
        w = skin->GetWidth();
    }
    if (h == 0)
    {
        h = skin->GetHeight();
    }
    
	CImage* newImage = new CImage(name, skin, x, y, x + w, y + h);

	const std::wstring& stretchMode = attributes.GetValue(ImageStretchModeAttribute);
	if (!stretchMode.empty())
	{
		CStretchOption stretchOption;
		if (stretchMode == StretchModeXY)
		{
			stretchOption.m_stretchMode = Stretch_Mode_XY;
		}
		else if (stretchMode == StretchModeX)
		{
			stretchOption.m_stretchMode = Stretch_Mode_X;
		}
		else if (stretchMode == StretchModeY)
		{
			stretchOption.m_stretchMode = Stretch_Mode_Y;
		}
		else if (stretchMode == StretchModeFixed)
		{
			stretchOption.m_stretchMode = Stretch_Mode_Fixed;
		}
		const std::wstring& stretchDim = attributes.GetValue(ImageStretchDimAttribute);
		if (!stretchDim.empty())
		{
			swscanf(
				stretchDim.c_str(), L"l:%d,r:%d,t:%d,b:%d", 
				&stretchOption.m_nLeft,
				&stretchOption.m_nRight, 
				&stretchOption.m_nTop,
				&stretchOption.m_nBottom
				);
		}
		newImage->SetStretchOption(stretchOption);
	}

    CResourceManager::Instance().RegisterImage(newImage);
}

} // namespace Meepo
