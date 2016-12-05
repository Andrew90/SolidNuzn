#include "stdafx.h"
#include "Resource.h"
#include "AddThresholdsWindow.h"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"
#include "templates\templates.hpp"
#include "ThresholdDataTable.h"
#include "tools_debug\DebugMess.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "SolidGroupAlgoritm\SolidBase.h"
#include "window_tool\MenuAPI.h"
#include "DlgTemplates\ParamDlg.h"
#include "FrameWindow\FrameWindow.h"

namespace
{
struct OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class O, class P>struct __ok_btn__
	{
		void operator()(O &o)
		{
			o.value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(o);
		}
	};
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(!TL::find<typename Owner::list, __test__>()(&owner.items, &h))return;
		TL::foreach<typename Owner::list, __ok_btn__>()(owner.items);
		EndDialog(h, TRUE);		
	}
};

typedef TL::CreateNumList<Point, 0, ComputeSolidGroup::count_points - 1>::Result __point_list__; 

template<class O, class P>struct __set_tresh__;
template<int N, class P>struct __set_tresh__<Point<N>, P>
{
	void operator()(Point<N> &o, P &p)
	{
		o.value = p[N];
	}
};

template<class O, class P>struct __get_tresh__;
template<int N, class P>struct __get_tresh__<Point<N>, P>
{
	void operator()(Point<N> &o, P &p)
	{
		p[N] = (int)o.value;
	}
};

template<int N>struct ParamTitle<Point<N>>
{
	wchar_t buf[32];
	wchar_t *operator()()
	{
		wsprintf(buf, L"Точка %d", 1 + N);
		return buf;
	}
};

template<int N>struct LessEqual<Point<N>>{typename Point<N>::type_value operator()(){return 0;}};
template<int N>struct Largen   <Point<N>>{typename Point<N>::type_value operator()(){return 100;}};

void __set_points__(HWND h)
{
	ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	TresholdsTable t;	
	TL::foreach<__point_list__, __set_tresh__>()(t.items, solidGroup.persents);

	if(TemplDialogList<NullType, TresholdsTable
		, __point_list__
		, TL::MkTlst<OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещение порогов"))
	{
		TL::foreach<__point_list__, __get_tresh__>()(t.items, solidGroup.persents);

		solidGroup.UpdateTresholds();
		HWND hh = FindWindow(WindowClass<FrameWindow>()(), 0);
		if(NULL != hh)
		{
			((FrameWindow *)GetWindowLongPtr(hh, GWLP_USERDATA))->IncDecFrame();
		}
	}
}
namespace
{
	void SaveOptions(HWND h)
	{
		ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
		if(solidGroup.changeTresholds && TypesizePasswordDlg().Do(h)) 
		{
			solidGroup.Save();
			MessageBox(h, L"Данные cохранены?", L"Cообщение", MB_ICONHAND | MB_OK);
			AddThresholdWindow &o = Singleton<AddThresholdWindow>::Instance();
			UpdateRow(o.grid.hWnd);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
	struct MainFile{};										   
	MENU_TEXT(L"Файл", TopMenu<MainFile>)					   
		   
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};
	struct MainSaveOptions{static void Do(HWND h){SaveOptions(h);}};
															   
	MENU_ITEM(L"Выход", MainExit)
	MENU_ITEM(L"Сохранить настройки", MainSaveOptions)
															   
	template<>struct TopMenu<MainFile>						   
	{														   
		typedef TL::MkTlst<	
			MenuItem<MainSaveOptions>
			, Separator<0>
			, MenuItem<MainExit>								   
		>::Result list;										   
	};														   
	   
	struct Options{};										   
	MENU_TEXT(L"Настройки", TopMenu<Options>)				   
	  
	struct WindowPos    : WindowPositionDlg<AddThresholdWindow>{}; 
	struct OffsPoints    {static void Do(HWND h){__set_points__(h);}}; 
															   
	MENU_ITEM(L"Сохранить координаты окна", WindowPos)	   
	MENU_ITEM(L"Точки смещения", OffsPoints)
															   
	template<>struct TopMenu<Options>						   
	{														   
		typedef TL::MkTlst<
			MenuItem<OffsPoints>
			, Separator<0>
			, MenuItem<WindowPos>						   
		>::Result list;										   
	};														   
	   
	typedef TL::MkTlst<										   
		TopMenu<MainFile>									   
		, TopMenu<Options>									   
	>::Result MainMenu;										   
}

LRESULT AddThresholdWindow::operator()(TCreate &l)
{
	Menu<MainMenu>().Init(l.hwnd);
	grid.Create(l.hwnd, &gridViewer);
	return 0;
}

void AddThresholdWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	grid.Size(0, 0, l.Width, l.Height);
}

void AddThresholdWindow::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 600;
		l.pMinMaxInfo->ptMinTrackSize.y = 200;
		l.pMinMaxInfo->ptMaxTrackSize.x = 1210;
		l.pMinMaxInfo->ptMaxTrackSize.y = 400;		
	}	
}

void AddThresholdWindow::operator()(TClose &l)
{
	ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	if(solidGroup.changeTresholds)
	{
		int res = MessageBox(l.hwnd, L"Данные изменены!\nСохранить?", L"Cообщение", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == res)
		{
			if(TypesizePasswordDlg().Do(l.hwnd))
			{
				solidGroup.Save();
			}
			else
			{
				return;
			}
		}
		else if(IDNO == res)
		{
			solidGroup.Clear();
			solidGroup.Load();
		}
		else if(IDCANCEL == res)
		{
			return;
		}
	}
	DestroyWindow(l.hwnd);
}

LRESULT AddThresholdWindow::operator()(TNotify &l)
{
	return EventDo(l);
}

void AddThresholdWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void AddThresholdWindow::Show()
{
	HWND h = FindWindow(WindowClass<AddThresholdWindow>()(), 0);
	if(NULL != h)
	{			
		RepaintWindow(h);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		AddThresholdWindow &o = Singleton<AddThresholdWindow>::Instance();
		RECT r;
		WindowPosition::Get<AddThresholdWindow>(r);
		HWND h = WindowTemplate(&o, L"Добавить порог", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);
	}		
}

 void AddThresholdWindow::Update()
 {
	HWND h = FindWindow(WindowClass<AddThresholdWindow>()(), 0);
	if(NULL != h)
	{			
		RepaintWindow(h);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
 }