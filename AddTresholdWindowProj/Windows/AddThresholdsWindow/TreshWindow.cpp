#include "stdafx.h"
#include "TreshWindow.h"
#include "window_tool/MenuApi.h"
#include "window_tool/EmptyWindow.h"
#include "tools_debug\DebugMess.h"
#include "window_tool\WindowsPosition.h"
#include "templates/templates.hpp"
#include "FrameWindow\FrameWindow.h"
#include "App\SelectHandler.h"
//------------------------------------------------------------------------
TreshWindow::TreshWindow()
    : dlg_items(items)
{}
//-------------------------------------------------------------------------
template<class O, class P>struct __get_tresh__;
template<int N, class P>struct __get_tresh__<Offset<N>, P>
{
	void operator()(Offset<N> &o, P &p)
	{
		p[N] = (int)o.value;
	}
};
template<class O, class P>struct __set_tresh__;
template<int N, class P>struct __set_tresh__<Offset<N>, P>
{
	void operator()(Offset<N> &o, P &p)
	{
		o.value = p[N];
	}
};
template<class O, class P>struct TreshWindow_ok_btn__
{
	void operator()(O &o)
	{
		o.value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(o);
	}
};
void TreshWindow::OkBtn::Do(TCommand &l)
{
	if(!TL::find<dlg_list, __test__>()(&owner->dlg_items, &l.hwnd))return;
	TL::foreach<dlg_list, TreshWindow_ok_btn__>()(owner->dlg_items);
	ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	TL::foreach<offset_list, __get_tresh__>()(owner->items, solidGroup.persents);
	solidGroup.persentsChanged = true;
	solidGroup.UpdateTresholds();
	HWND hh = FindWindow(WindowClass<FrameWindow>()(), 0);
	if(NULL != hh)
	{
		((FrameWindow *)GetWindowLongPtr(hh, GWLP_USERDATA))->IncDecFrame();
	}
}
//-------------------------------------------------------------------------------
void TreshWindow::CancelBtn::Do(TCommand &l)
{
	ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	if(solidGroup.persentsChanged)
	{
		if(TypesizePasswordDlg().Do(l.hwnd))
		{
			solidGroup.Save();
		}
		else
		{
			SelectHandler::Restore();
		}
		solidGroup.UpdateTresholds();
		HWND hh = FindWindow(WindowClass<FrameWindow>()(), 0);
		if(NULL != hh)
		{
			((FrameWindow *)GetWindowLongPtr(hh, GWLP_USERDATA))->IncDecFrame();
		}
		solidGroup.persentsChanged = false;
		solidGroup.changeTresholds = false;
	}
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	DestroyWindow(l.hwnd);
}
//------------------------------------------------------------------------
void TreshWindow::operator()(TCommand &l)
{
	EventDo(l);
	static int c;
	++c;
}
//------------------------------------------------------------------------
void TreshWindow::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = width;
		l.pMinMaxInfo->ptMinTrackSize.y = height;
		l.pMinMaxInfo->ptMaxTrackSize.x = width;
		l.pMinMaxInfo->ptMaxTrackSize.y = height;		
	}		
}
//-------------------------------------------------------------------------
PARAM_TITLE(Offset<0>, L"Смещение 1")
PARAM_TITLE(Offset<1>, L"Смещение 2")
PARAM_TITLE(Offset<2>, L"Смещение 3")
PARAM_TITLE(Offset<3>, L"Смещение 4")
PARAM_TITLE(Offset<4>, L"Смещение 5")
PARAM_TITLE(Offset<5>, L"Смещение 6")
PARAM_TITLE(Offset<6>, L"Смещение 7")
PARAM_TITLE(Offset<7>, L"Смещение 8")

MIN_EQUAL_VALUE(Offset<0>, 0)
MAX_EQUAL_VALUE(Offset<0>, 100)
MIN_EQUAL_VALUE(Offset<1>, 0)
MAX_EQUAL_VALUE(Offset<1>, 100)
MIN_EQUAL_VALUE(Offset<2>, 0)
MAX_EQUAL_VALUE(Offset<2>, 100)
MIN_EQUAL_VALUE(Offset<3>, 0)
MAX_EQUAL_VALUE(Offset<3>, 100)
MIN_EQUAL_VALUE(Offset<4>, 0)
MAX_EQUAL_VALUE(Offset<4>, 100)
MIN_EQUAL_VALUE(Offset<5>, 0)
MAX_EQUAL_VALUE(Offset<5>, 100)
MIN_EQUAL_VALUE(Offset<6>, 0)
MAX_EQUAL_VALUE(Offset<6>, 100)
MIN_EQUAL_VALUE(Offset<7>, 0)
MAX_EQUAL_VALUE(Offset<7>, 100)

template<class T, int edit_width = 140>struct XEditItems
{
	HWND Init(HWND h, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", Wchar_from<typename T::type_value>(t.value)()
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			, 10, dy, edit_width, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		HWND q = CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, edit_width + 20, dy + 3, width - edit_width - 20, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += 25;
		return hWnd;
	}
};

template<int N>struct DlgSubItems<Offset<N>, int>: XEditItems<Offset<N>>{};
LRESULT TreshWindow::operator()(TCreate &l)
{
	height = 10;
	int __width = width;

	ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();

	TL::foreach<offset_list, __set_tresh__>()(items, solidGroup.persents);
	TL::foreach<dlg_list, __init__>()(&dlg_items, &__table_data__(l.hwnd, __width, height));
	height += 80;
	static const int btnH = 30;
	static const int w = (width - 35) / 2;
	HWND h = CreateWindow(L"button",L"Применить"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		, 5, height - btnH - 45, w, btnH, l.hwnd, NULL, l.create->hInstance, NULL
		);
	okBtn.owner = this;
	SetWindowLongPtr(h, GWLP_USERDATA, (DWORD)&okBtn);
	h = CreateWindow(L"button",L"Закрыть"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		, 10 + w, height - btnH - 45, w, btnH, l.hwnd, NULL, l.create->hInstance, NULL
		);
	cancelBtn.owner = this;
	SetWindowLongPtr(h, GWLP_USERDATA, (DWORD)&cancelBtn);
	RECT r;
	GetClientRect(GetDesktopWindow(), &r);
	MoveWindow(l.hwnd, (r.right - width)/ 2, (r.bottom - height)/2, width, height, FALSE);
	return 0;
}
//------------------------------------------------------------------------
void TreshWindow::Show()
{
	HWND h = FindWindow(WindowClass<TreshWindow>()(), 0);
	if(NULL == h)
	{			
		TreshWindow &self = Singleton<TreshWindow>::Instance();
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);
		HWND h = WindowTemplate(&self, L"Смещение порогов", 0,0,0,0, IDI_UPLOAD, (HBRUSH)COLOR_WINDOW);		
		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
		dwStyle &= ~(WS_SYSMENU);
		SetWindowLong(h, GWL_STYLE, dwStyle);
		ShowWindow(h, SW_SHOWNORMAL);
	}	
}
//------------------------------------------------------------------------
