#pragma once
#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "IOportsWindow.h"
#include "window_tool/EmptyWindow.h"
#include "App/AppBase.h"
#include <CommCtrl.h>
#include "window_tool\WindowsPosition.h"

namespace
{
	 DifferentOptionsTable &params = Singleton<DifferentOptionsTable>::Instance();
}

int IOportsDlg_Init()
{
	return params.items.get<IOPortsVisible>().value
			? MFS_CHECKED
			: MFS_UNCHECKED;
}

bool IOportsDlg_Check()
{
	try
	{
		bool &result = params.items.get<IOPortsVisible>().value;
		result ^= true;
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			Update<ParametersTable>(base).set<IOPortsVisible>(result)
				.Where().ID(1).Execute();
		}
		return result;
	}
	catch(...)
	{
		return false;
	}
};


void IOportsDlg_Open()
{
	HWND h = FindWindow(L"IOportsWindow", 0);
	if(NULL != h)
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		RECT r;
		WindowPosition::Get<IOportsWindow>(r);
		wchar_t *title = //App::measurementOfRunning
		//	? 
			L"Просмотр дискретных входов-выходов"
		//	: L"Управление дискретными выходами"
		;
		
		h = WindowTemplate(
			 &IOportsWindow::Instance()
			, title
			, r.left, r.top
			, IOportsWindow::width, IOportsWindow::height
			);
		ShowWindow(h, SW_SHOWNORMAL);
		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
		//dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		dwStyle &= ~(WS_SYSMENU);
		SetWindowLong(h, GWL_STYLE, dwStyle);
	}
}

void IOportsDlg_Start()
{
	if(params.items.get<IOPortsVisible>().value)
	{
		IOportsDlg_Open();
	}
}

void IOportsDlg_Close()
{
	HWND hh = FindWindow(L"IOportsWindow", 0);
	if(NULL != hh)
	{
		DestroyWindow(hh);
	}
}

bool IOportsDlg::Do()
{
	bool result = IOportsDlg_Check();
	if(result)
	{
		IOportsDlg_Open();
	}
	else
	{
		IOportsDlg_Close();
	}
	return result;
}


