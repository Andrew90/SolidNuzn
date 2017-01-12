#include "stdafx.h"
#include "Dialogs.h"
#include "MainWindow\MainWindow.h"
#include "templates\templates.hpp"

bool CounterShowDlg::Do()
{
	HWND hh = Singleton<MainWindow>::Instance().gridCounterViewer.grid.hWnd;
	bool b = 0 != IsWindowVisible(hh);
	ShowWindow(hh, b ? SW_HIDE: SW_SHOWNORMAL);
	return !b;
}
