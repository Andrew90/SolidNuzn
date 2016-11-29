#pragma once
#include "window_tool\GridItems.h"

class GridWindow: public GridHandlers
{
public:
	GridWindow();
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
	void LClick(LPNMITEMACTIVATE );
	void RClick(LPNMITEMACTIVATE );	
};