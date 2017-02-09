#pragma once
#include "window_tool\GridItems.h"

class GridWindow: public GridHandlers
{
	HWND hWnd;
public:
	GridWindow();
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
	void LClick(LPNMITEMACTIVATE );
	void RClick(LPNMITEMACTIVATE );	
	void Update();
};