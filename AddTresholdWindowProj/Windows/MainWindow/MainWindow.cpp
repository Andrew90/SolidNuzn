#include "stdafx.h"
#include <commctrl.h>
#include "MainWindow.h"
#include "window_tool\MenuAPI.h"
#include "tools_debug\DebugMess.h"
#include "MainWindow\MainWindowMenu.hpp"
#include "window_tool\Emptywindow.h"
#include "App/AppBase.h"
#include "window_tool\CheckBoxWidget.hpp"

template<class T>struct CommunicationCheckBox
{
	bool &value;
	CommunicationCheckBox()
		: value(Singleton<DifferentOptionsTable>::Instance().items.get<T>().value)
	{}
protected:
	void Command(TCommand &m, bool b)
	{
		value = b;
		CBase base(
			ParametersBase().name()
			);
		if(base.IsOpen())
		{
		   Update<DifferentOptionsTable>(base).set<T>(b).Where().ID(1).Execute();
		}
	}
	bool Init(HWND h)
	{		
		return value;
	}
};

CheckBoxWidget<CommunicationCheckBox<CommunicationRemoveUnit>> __communicationRemoveUnit__;
CheckBoxWidget<CommunicationCheckBox<TubesStored>> __counterTubesStored__;
CheckBoxWidget<CommunicationCheckBox<PaintMarker>> __paintMarker__;

MainWindow::MainWindow()
{}

LRESULT MainWindow::operator()(TCreate &l)
{
	Menu<MainWindowMenu::items_list>().Init(l.hwnd);
	toolBar.Init(l.hwnd);

	__communicationRemoveUnit__.Init(toolBar.hWnd, L"Работа со станцией");
	__counterTubesStored__.Init(toolBar.hWnd, L"Сохранение первичного сигнала");
	__paintMarker__.Init(toolBar.hWnd, L"Краскоотметчик");

	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);

	select.Create(toolBar.hWnd);

	signalViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<SignalViewer>::Proc, L"SignalViewer", &signalViewer);
	topLabelViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelViewer", &topLabelViewer);

	gridCounterViewer.Create(toolBar.hWnd);

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

	MoveWindow(gridCounterViewer.grid.hWnd, width + 400 + 30,  2, 145, rt.bottom - rt.top - 2 - 2, TRUE);

	__communicationRemoveUnit__.Size(width, 52, 250, 20);
	__counterTubesStored__.Size(width, 69, 250, 20);
	__paintMarker__.Size(width + 250, 52, 150, 20);

	static const int topLabelHeight = 28;

	int y = rt.bottom;
	
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	int t = r.bottom - rt.bottom - rs.bottom - topLabelHeight;
	MoveWindow(signalViewer.hWnd , 0, y, r.right, t, true);
}


void MainWindow::operator()(TCommand &l)
{
	EventDo(l);
}

LRESULT MainWindow::operator()(TNotify &l)
{
	return EventDo(l);
}

void MainWindow::operator()(TUser &l){ l.ptr(l.data);}

void MainWindow::operator()(TClose &l)
{
	if(IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
	{
		DestroyWindow(l.hwnd);
	}
}

void MainWindow::operator()(TMouseWell &l)
{
	signalViewer(l);
}

void MainWindow::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 600;
		l.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
