#include "stdafx.h"
#include "MainWindow.h"
#include <commctrl.h>
#include "MainWindowMenu.hpp"
#include "templates/templates.hpp"
#include "Common.hpp"

MainWindow::MainWindow()
	:  select(Singleton<SelectTypeSizeList>::Instance())
{
	//topLabel.label.fontHeight = 12;
	//topLabel.label.top = 0;	

	topLabel.label = L"<ff00>Test";
}

LRESULT MainWindow::operator()(TCreate &l)
{
	Menu<MainWindowMenu::items_list>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	select.Create(toolBar.hWnd);
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int pParts[] = {200, 400, 600, 900};
	SendMessage(hStatusWindow, SB_SETPARTS, dimention_of(pParts), (LPARAM)pParts);

	crossCheckBox	 .Init(toolBar.hWnd, L"Измерение поперечных дефектов стенки трубы");
	longCheckBox	 .Init(toolBar.hWnd, L"Измерение продольных дефектов стенки трубы");
	thickCheckBox.Init(toolBar.hWnd, L"Измерение толщины стенки трубы");

	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);

	topLabel.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelWindow", &topLabel);
	topLabel.label.fontHeight = 16;
	return 0;
}

void MainWindow::operator()(TDestroy &)
{
	PostQuitMessage(0);
}

void MainWindow::operator()(TClose &l)
{
	if(IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
	{
		DestroyWindow(l.hwnd);
	}
}

namespace {
	static const int resultViewerHeight = 100;
	struct __move_window_data__
	{
		int y, width, height;
	};
	template<class O, class P>struct __move_window__
	{
		void operator()(O *o, P *p)
		{			
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, p->height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, p->height, true);
			p->y += p->height;
		}
	};

	template<class P>struct __move_window__<ResultViewer, P>
	{
		typedef ResultViewer O;
		void operator()(O *o, P *p)
		{
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, resultViewerHeight};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, resultViewerHeight, true);
		}
	};
}

void MainWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	toolBar.Size();
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);

	static const int width = toolBar.Width();
	select.Size(width, 5, 400);

	crossCheckBox	 .Size (width + 425, 5 , 400, 20);
	longCheckBox	 .Size (width + 425, 25, 400, 20);
	thickCheckBox.Size (width + 425, 45, 400, 20);

	RECT rt;
	GetWindowRect(toolBar.hWnd, &rt);
	 RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(l.hwnd, &r);	

	static const int topLabelHeight = 28;
	int y = rt.bottom - rt.top - 1;
	int t = r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight - resultViewerHeight;

	MoveWindow(topLabel.hWnd, 0, y, r.right, topLabelHeight, true);

	y += topLabelHeight;
	t /= 3;
	__move_window_data__ data = {y, r.right, t};
	TL::foreach<viewers_list, __move_window__>()(&viewers, &data);
}

void MainWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void MainWindow::operator()(TUser &l)
{
	(*l.ptr)(l.data);
}

namespace Common
{

	template<class O, class P>struct __sub_rect__
	{
		void operator()(O &o, P &p)
		{
			p.l.hwnd = o.hWnd;
			p.l.y = o.storedMouseMove.y;
			o.offsetX = p.owner.viewers.get<ResultViewer>().offsetX;
			SendMessage(MESSAGE(p.l));
		}
	};

	template<class P>struct __in_rect__<ResultViewer, P>
	{
		typedef ResultViewer O;
		bool operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			if(InRect(p->l.x, p->l.y, r))
			{
				p->l.hwnd = o->hWnd;
				SendMessage(MESSAGE(p->l));
				typedef TL::EraseItem<MainWindow::viewers_list, ResultViewer>::Result list;
				p->l.delta = 0;
				TL::foreach<list, __sub_rect__>()(p->owner.viewers, *p);
				return false;
			}
			return true;
		}
	};	
}

void MainWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, MainWindow>(*this, l)
		);
}

void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}		
}

void CheckBoxStateStoreInBase()
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		UpdateWhere<OnTheJobTable>(Singleton<OnTheJobTable>::Instance(), base).ID(1).Execute();
	}
}