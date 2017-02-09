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
	void SaveOptions(HWND h)
	{
		ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
		if(solidGroup.changeTresholds && TypesizePasswordDlg().Do(h)) 
		{
			solidGroup.Save();
			MessageBox(h, L"Данные cохранены!!!", L"Cообщение", MB_ICONINFORMATION | MB_OK);
			AddThresholdWindow &o = Singleton<AddThresholdWindow>::Instance();
			UpdateRow(o.grid.hWnd);
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
	struct EnableStandard
	{
		static void Do(HWND h)
		{
			App::isStandard ^= true;
			CheckMenu<MenuItem<EnableStandard>>(h, App::isStandard);
			Singleton<AddThresholdWindow>::Instance().ChangeStandard(App::isStandard);
		}
	};
	//struct OffsPoints    {static void Do(HWND h){__set_points__(h);}}; 



	MENU_ITEM(L"Сохранить координаты окна", WindowPos)	   
		MENU_ITEM(L"Эталон", EnableStandard)	
		//MENU_ITEM(L"Точки смещения", OffsPoints)

		template<>struct TopMenu<Options>						   
	{														   
		typedef TL::MkTlst<
			MenuItem<EnableStandard>
			, Separator<0>
			, MenuItem<WindowPos>						   
		>::Result list;										   
	};														   

	typedef TL::MkTlst<										   
		TopMenu<MainFile>									   
		, TopMenu<Options>									   
	>::Result MainMenu;										  

	template<>struct EnableMenuInit<MenuItem<EnableStandard>>
	{
		int operator()(HWND)
		{
			return MFS_UNCHECKED;
		}
	};

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
    ChangeStandard(false);
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

void AddThresholdWindow::ChangeStandard(bool isStandard)
{	
	
	ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	if(solidGroup.changeTresholds)
	{
		int res = MessageBox(hWnd, L"Данные изменены!\nСохранить?", L"Cообщение", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == res)
		{
			if(TypesizePasswordDlg().Do(hWnd))
			{
				solidGroup.Save();
			}
		}
	}

	wchar_t st[] = L"_standard";
	wchar_t buf[128];
	wcscpy(buf, Singleton<ComputeSolidGroup>::Instance().typeSizeName.c_str());
	int len = wcslen(buf);

	int k =  len - (dimention_of(st) - 1);
	bool standardBool  = k > 0;
	if(standardBool)
	{
		
		wchar_t *s = &buf[k];
		standardBool = standardBool && 0 == wcsncmp(s, st, dimention_of(st) - 1);
	}

	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		ParametersTable &table = Singleton<ParametersTable>::Instance();

		if(isStandard)
		{
			if(!standardBool) wcscat(buf, st);

			table.items.get<NameParam>().value = buf;
			int id = Select<ParametersTable>(base).eq<NameParam>(table.items.get<NameParam>().value).Execute();
			if(id == 0)
			{
				table.items.get<NameParam>().value = buf;
				Insert_Into<ParametersTable>(table, base).Execute();

				id = Select<ParametersTable>(base).eq<NameParam>(table.items.get<NameParam>().value).Execute();

				CurrentParametersTable t;
				t.items.get<CurrentID>().value = id;
				UpdateWhere<CurrentParametersTable>(t, base).ID(1).Execute();
			}
			
		}
		else
		{
			if(standardBool)
			{
				if(k > 0)buf[k]	= 0;
			}
		}
		ParametersTable typeSizeParam;
		table.items.get<NameParam>().value = buf;
		int id = Select<ParametersTable>(base).eq<NameParam>(table.items.get<NameParam>().value).Execute(typeSizeParam);
		if(id != 0)
		{
			CurrentParametersTable t;
			t.items.get<CurrentID>().value = id;
			UpdateWhere<CurrentParametersTable>(t, base).ID(1).Execute();
			AppBase::InitTypeSizeTables(base);
			Singleton<ComputeSolidGroup>::Instance().Load(buf);
			Singleton<ComputeSolidGroup>::Instance().typeSizeName = buf;
			Singleton<AddThresholdWindow>::Instance().gridViewer.Update();
		}
	}
	RepaintWindow(FindWindow(WindowClass<AddThresholdWindow>()(), 0));
}