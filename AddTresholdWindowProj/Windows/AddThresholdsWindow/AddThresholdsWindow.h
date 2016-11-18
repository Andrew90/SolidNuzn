#pragma once 
#include "window_tool\message.h"
#include "AddThresholdsWindow\GridWindow.h"
#include "window_tool\Grid.h"

class AddThresholdWindow
{
public:
	HWND hWnd;
	GridWindow gridViewer;
	GridNotify grid;
	LRESULT operator()(TCreate &);

	void operator()(TSize &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TClose &);
	//void operator()(TDestroy &);

	LRESULT operator()(TNotify &);
	void operator()(TCommand &);

	static void Show();
	static void Update();
};