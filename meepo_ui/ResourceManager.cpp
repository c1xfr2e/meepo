/***********************************************************************
    created:    2012/6/2
    author:     ZhaoHao
*************************************************************************/
#include "ResourceManager.h"
#include <windows.h>
#include "XmlHandler_Image.h"
#include "Image.h"

namespace Meepo
{

CResourceManager& CResourceManager::Instance()
{
    static CResourceManager instance;
    return instance;
}

CResourceManager::CResourceManager()
{}

CResourceManager::~CResourceManager()
{}


void CResourceManager::SetResourcePath(const StdStringW& resPath)
{
    m_strResourcePath = resPath;
}

int CResourceManager::LoadFile(const StdStringW& filename, CRawData& rawData, unsigned int extraBytes/* = 0*/)
{
    if (filename.empty())
    {
        return -1;
    }

    StdStringW fileFullPath = m_strResourcePath + filename;
    HANDLE hFile = ::CreateFileW(fileFullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    DWORD size = ::GetFileSize(hFile, NULL);
    void* buf = rawData.Alloc(size + extraBytes);
    DWORD bytesRead = 0;
    BOOL ret = ::ReadFile(hFile, buf, size, &bytesRead, NULL);
    if (!ret || bytesRead != size)
    {
        rawData.Dealloc();
        return -1;
    }
    ::CloseHandle(hFile);
    return 1;
}

int CResourceManager::LoadImages(const StdStringW& imageXMLFile)
{
	CXmlHandler_Image handler;
    return m_tinyXmlParser.ParseFile(handler, imageXMLFile);
}

int CResourceManager::RegisterImage(CImage* img)
{
    if (img == NULL)
    {
        return -1;
    }
    ImageMap::iterator it = m_mapImages.find(img->GetName());
    if (it != m_mapImages.end())
    {
        return -1;
    }
    m_mapImages[img->GetName()] = img;
    return 0;
}

CImage* CResourceManager::GetImage(const StdStringW& name)
{
    return m_mapImages[name];
}

}
 