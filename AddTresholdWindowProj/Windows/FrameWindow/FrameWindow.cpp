#include "stdafx.h"
#include "FrameWindow.h"
#include <commctrl.h>
#include "window_tool\MenuAPI.h"
#include "FrameWindow\FrameWindowMenu.hpp"
#include "window_tool\Emptywindow.h"

LRESULT FrameWindow::operator()(TCreate &l)
{
	Menu<FrameWindowMenu::items_list>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);

	thresholdsViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<ThresholdsViewer>::Proc, L"ThresholdsViewer", &thresholdsViewer);
	frameViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<FrameViewer>::Proc, L"FrameViewer", &frameViewer);
	return 0;
}

void FrameWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();

	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(l.hwnd, &r);	

	int height = (r.bottom - rt.bottom) / 2;
	int y = rt.bottom; 
	
	MoveWindow(thresholdsViewer.hWnd , 0, y, r.right, height, true);
	y += height;
	MoveWindow(frameViewer.hWnd , 0, y, r.right, height, true);
}

void FrameWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void FrameWindow::operator()(TMouseWell &)
{
}
	
void FrameWindow::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 600;
		l.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}

void FrameWindow::IncFrame()
{
	frameViewer.IncFrame();
}
void FrameWindow::DecFrame()
{
	frameViewer.DecFrame();
}