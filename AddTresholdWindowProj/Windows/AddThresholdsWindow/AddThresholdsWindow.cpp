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
			MessageBox(h, L"������ c��������!!!", L"C��������", MB_ICONINFORMATION | MB_OK);
			AddThresholdWindow &o = Singleton<AddThresholdWindow>::Instance();
			UpdateRow(o.grid.hWnd);
		}
	}
//////////////////////////////////////////////////////////////////////////////////
	struct MainFile{};										   
	MENU_TEXT(L"����", TopMenu<MainFile>)					   
		   
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};
	struct MainSaveOptions{static void Do(HWND h){SaveOptions(h);}};
															   
	MENU_ITEM(L"�����", MainExit)
	MENU_ITEM(L"��������� ���������", MainSaveOptions)
															   
	template<>struct TopMenu<MainFile>						   
	{														   
		typedef TL::MkTlst<	
			MenuItem<MainSaveOptions>
			, Separator<0>
			, MenuItem<MainExit>								   
		>::Result list;										   
	};														   
	   
	struct Options{};										   
	MENU_TEXT(L"���������", TopMenu<Options>)				   
	  
	struct WindowPos    : WindowPositionDlg<AddThresholdWindow>{}; 
	//struct OffsPoints    {static void Do(HWND h){__set_points__(h);}}; 
															   
	MENU_ITEM(L"��������� ���������� ����", WindowPos)	   
	//MENU_ITEM(L"����� ��������", OffsPoints)
															   
	template<>struct TopMenu<Options>						   
	{														   
		typedef TL::MkTlst<
		//	MenuItem<OffsPoints>
		//	, Separator<0>
			 MenuItem<WindowPos>						   
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
		int res = MessageBox(l.hwnd, L"������ ��������!\n���������?", L"C��������", MB_ICONQUESTION | MB_YESNOCANCEL);
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
		HWND h = WindowTemplate(&o, L"�������� �����", r.left, r.top, r.right, r.bottom);
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