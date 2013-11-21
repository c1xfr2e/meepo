/***********************************************************************
    filename:   RawData.h
    created:	2012/6/2
    author:     ZhaoHao
*************************************************************************/
#pragma once

#include <windows.h>

namespace Meepo
{
// 为了方便使用，定义一个load内存数据的wrapper
class CRawData
{
public:
	CRawData() : m_pBuf(0), m_nSize(0) {}

	virtual void* Alloc(unsigned int size) = 0;
	virtual void Dealloc() = 0;

	void* GetBuf();
	unsigned int GetSize();
	char& operator[](int index);


protected:
	void* m_pBuf;
	unsigned int m_nSize;
};

// 使用 operator new 配置的数据
class CHeapData : public CRawData
{
public:
	~CHeapData();
	virtual void* Alloc(unsigned int size);
	virtual void Dealloc();
};

// 使用 GlobalAlloc 配置的数据
class CGlobalData : public CRawData
{
public:
	CGlobalData();
	~CGlobalData();
	virtual void* Alloc(unsigned int size);
	virtual void Dealloc();

private:
	HGLOBAL m_hGlobal;
};

}
