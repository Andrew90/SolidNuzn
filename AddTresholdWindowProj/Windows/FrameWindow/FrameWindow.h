#pragma once
#include "window_tool\message.h"
#include "FrameWindow\FrameWindowToolBar.h"
#include "FrameWindow\ThresholdsViewer.h"
#include "FrameWindow\FrameViewer.h"

template<class>struct Singleton;
class App;
class FrameWindow
{
	friend App;
	friend Singleton<FrameWindow>;
	HWND hStatusWindow;
	FrameWindowToolBar toolBar;
	ThresholdsViewer thresholdsViewer;
	FrameViewer frameViewer;
	FrameWindow(){}
	FrameWindow &operator =(const FrameWindow &);
public:
	HWND hWnd;
	LRESULT operator()(TCreate &);

	void operator()(TSize &);
	void operator()(TCommand &);
	
	void operator()(TMouseWell &);

	void operator()(TGetMinMaxInfo &);

	void IncFrame();
	void DecFrame();
};