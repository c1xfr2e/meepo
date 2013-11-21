#pragma once

#include "DUIWindow.h"

using namespace Meepo;

class CTestWnd : public CDUIWindow
{
public:
	BEGIN_MSG_MAP_EX(CDUIWindow)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(CDUIWindow)
	END_MSG_MAP()

	void OnSize(UINT flag, CSize size)
	{
        CDUIWindow::OnSize(flag, size);

        int coner = 3;

		POINT pts[] = {
			{0,coner},
			{coner,0},
			{size.cx - coner, 0},
			{size.cx, coner + 1}, 
			{size.cx - 1, size.cy - coner}, 
			{size.cx - coner - 1, size.cy},
			{coner, size.cy - 1},
			{0, size.cy - coner - 1}
		};

		HRGN hrgn1 = CreatePolygonRgn(pts, _countof(pts), ALTERNATE);
		SetWindowRgn(hrgn1);

		Resize(size.cx, size.cy);
	}
};