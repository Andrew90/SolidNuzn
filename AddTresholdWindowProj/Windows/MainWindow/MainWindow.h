#pragma once
#include "window_tool\message.h"
#include "MainWindow\MainWindowToolBar.h"
#include "window_tool\SelectTypeSizeList.h"
#include "MainWindow\SignalViewer.h"
#include "MainWindow\TopLabelViewer.h"
template<class>struct Singleton;
class App;
class MainWindow
{
	friend App;
	friend Singleton<MainWindow>;
	HWND hStatusWindow;
	MainWindowToolBar toolBar;
	SelectTypeSizeList select;
	TopLabelViewer topLabelViewer;
	SignalViewer signalViewer;
	MainWindow(){}
	MainWindow &operator =(const MainWindow &);
public:
	HWND hWnd;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);

	void operator()(TUser &);
	void operator()(TClose &);
	void operator()(TMouseWell &);

	void operator()(TGetMinMaxInfo &);
};

