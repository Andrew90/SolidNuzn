#include "stdafx.h"
#include <commctrl.h>
#include "MainWindow.h"
#include "window_tool\MenuAPI.h"
#include "tools_debug\DebugMess.h"
#include "MainWindow\MainWindowMenu.hpp"
#include "window_tool\Emptywindow.h"


LRESULT MainWindow::operator()(TCreate &l)
{
	Menu<MainWindowMenu::items_list>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);

	select.Create(toolBar.hWnd);

	signalViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<SignalViewer>::Proc, L"SignalViewer", &signalViewer);
	topLabelViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelViewer", &topLabelViewer);

	return 0;
}
void MainWindow::operator()(TDestroy &)
{
	PostQuitMessage(0);
}

void MainWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();

	static const int width = toolBar.Width();
	select.Size(width, 5, 400);

	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(l.hwnd, &r);	

	static const int topLabelHeight = 28;

	int y = rt.bottom;
	
	TSize tw = {topLabelViewer.hWnd, WM_SIZE, 0, (WORD)r.right, (WORD)topLabelHeight};
	SendMessage(MESSAGE(tw));
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);

	y += topLabelHeight;
	int t = r.bottom - rt.bottom - rs.bottom - topLabelHeight;
	TSize sv = {signalViewer.hWnd, WM_SIZE, 0, (WORD)r.right, (WORD)t};
	SendMessage(MESSAGE(sv));
	MoveWindow(signalViewer.hWnd , 0, y, r.right, t, true);
}

void MainWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void MainWindow::operator()(TUser &l){ l.ptr(l.data);}

void MainWindow::operator()(TClose &l)
{
	if(IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
	{
		DestroyWindow(l.hwnd);
	}
}

void MainWindow::operator()(TMouseWell &){}

void MainWindow::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 600;
		l.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
