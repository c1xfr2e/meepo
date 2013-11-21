#pragma once

#include <windows.h>
#include <windowsx.h>
#include <string>
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>

namespace Meepo
{

typedef std::wstring  StdStringA;
typedef std::wstring StdStringW;

class CRawData;
class CResourceManager;
class CSkin;
class CDUIWindow;
class CDCBuffer;
class CResourceManager;
class CImage;
class CElement;
class CEvent;
class CEventArgs;
class CMouseEventArgs;

template <class T> class CKeyFrame;
class CAffectorBase;
class CAnimation;
class CAnimationInstance;

}