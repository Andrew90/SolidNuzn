#include "stdafx.h"
#include "App.h"
#include "AddThresholdsWindow\AddThresholdsWindow.h"
#include "SolidGroupAlgoritm\SolidBase.h"
#include "MainWindow\MainWindow.h"
#include "window_tool\ItemIni.h"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"
#include "App\AppBase.h"
#include "window_tool\Emptywindow.h"
#include "App\SelectHandler.h"
#include "Dates\CounterTubes.h"

#include "MainWindow\MainWindowMenu.hpp"
#include "ColorPanel\ColorPanel.h"
#include "App/config.h"
#include "Automat\Automat.h"
#include "L502\L502SolidGroup.h"
#include "App\UpdateMainChart.h"
#include "App\AppKeyHandler.h"
#include "window_tool\HookKey.h"
//----------debug
#include "AddThresholdsWindow\TreshWindow.h"
//debug

namespace
{
	MainWindow &mainWindow = Singleton<MainWindow>::Instance();
	ComputeSolidGroup &computeSolidGroup = Singleton<ComputeSolidGroup>::Instance();
	L502SolidGroup &l502SolidGroup = Singleton<L502SolidGroup>::Instance();
}

bool App::isStandard = false;

void App::Init()
{
#if 1
	AppBase().Init();
	NameParam::type_value &nameParam = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	computeSolidGroup.Load(nameParam);
	CounterTubes::Load(nameParam);

	bool run = Automat::Init1730();

	if(run)Device1730::Write(0);

	if(run && !(l502SolidGroup.Init() && l502SolidGroup.SetupParams()))
	{
		MessageBox(0, L"Не могу инициировать плату L502", L"Ошибка !!!", MB_ICONERROR);
		run = false;
	}
	RECT r;
	WindowPosition::Get<MainWindow>(r);
	HWND h = WindowTemplate(&mainWindow, L"Группа прочности", r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
	SelectHandler::Init(mainWindow.select.hWnd);
	mainWindow.select.ptr = SelectHandler::Do;

	StartKeyHook(h);

	AppKeyHandler::Init();


	if(run)IOportsDlg_Start();
	ColorPanel::Open();

	Automat::Init();
	if(run) 
	{
		Automat::Run();
	}
#else
	AppBase().Init();
	NameParam::type_value &nameParam = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	computeSolidGroup.Load(nameParam);
	TreshWindow::Show();
#endif
}

void App::Destroy()
{
}

struct App::Impl
{

};

void App::PrintTopLabel(wchar_t *txt)
{	
	mainWindow.topLabelViewer.SetMessage(txt);
}

void App::UpdateMainWindow()
{
	RepaintWindow(mainWindow.topLabelViewer.hWnd);
	mainWindow.gridCounterViewer.Update();
	UpdateMainChart(0, mainWindow.hWnd);
}

void App::UpdateLoopMainWindow(int delay)
{
	UpdateMainChart(delay, mainWindow.hWnd);
}

void App::AddMenuItem(wchar_t *n)
{
	mainWindow.select.AddMenuItem(n);
}

void App::DelMenuItem(wchar_t *n)
{
	mainWindow.select.DelMenuItem(n);
}

void App::SelMenuItem(wchar_t *n)
{
	mainWindow.select.SelMenuItem(n);
}

void App::ClearCounter()
{
	if(TypesizePasswordDlg().Do(mainWindow.hWnd))
	{
		CounterTubes::Clear();
		mainWindow.gridCounterViewer.Update();
	}
}

void App::UpdateGroupCounter()
{
	wchar_t *s = (wchar_t *)computeSolidGroup.currentGroupName.c_str();
	if('\0' != s[0])
	{
		CounterTubes::Inc(s);
		mainWindow.gridCounterViewer.Update();
	}
}

void App::CheckMenuItem()
{
	static bool x = true;
	x ^= true;
	CheckMenu<TopMenu<MainWindowMenu::MainOptionUnits>>(mainWindow.hWnd, x);	
}
HWND  App::MainWindowHWND()
{
	return mainWindow.hWnd;
}

HWND App::MainWindowToolBarHWND()
{
	return mainWindow.toolBar.hWnd;
}

