#pragma once
#include "window_tool\GridItems.h"
#include "window_tool\Grid.h"
class GridCounterViewer : public GridHandlers
{
public:

public:
	GridNotify grid;
	GridCounterViewer();
public:	
	void Create(HWND);
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);

	void Update();
};
/*
В родительском окне добавить обработчик TNotify
*/