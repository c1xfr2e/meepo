/***********************************************************************
    created:    2012/6/2
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include "PreHeader.h"
#include "RawData.h"
#include <map>
#include "TinyParser.h"

namespace Meepo
{

class CResourceManager
{
public:
	static CResourceManager& Instance();
	void SetResourcePath(const StdStringW& resPath);
	int LoadFile(const StdStringW& filename, CRawData& rawData, unsigned int extraBytes = 0);
	int LoadImages(const StdStringW& imageXMLFile);
	int RegisterImage(CImage* img);
	CImage* GetImage(const StdStringW& name);

private:
	CResourceManager();
	~CResourceManager();

	StdStringW m_strResourcePath;

	typedef std::map<StdStringW,CImage*> ImageMap;
	ImageMap m_mapImages;

	typedef std::map<StdStringW,CSkin*> SkinMap;
	SkinMap m_mapSkins;

	CTinyParser m_tinyXmlParser;
};

}
