#include "stdafx.h"
#include "App.h"
#include <CommCtrl.h>
#include "MainWindow\MainWindow.h"
#include "window_tool\Emptywindow.h"
#include "window_tool\HookKey.h"
#include "templates\templates.hpp"
#include "AppKeyHandler.h"
#include "SelectHandler.h"
#include "AppBase.h"

namespace
{
	MainWindow &mainWindow = Singleton<MainWindow>::Instance();

	static const wchar_t *title = L"Application";
}

void App::Init()
{
	AppBase().Init();
	HWND h = WindowTemplate(&mainWindow, (wchar_t *)title);
	SelectHandler::Init(mainWindow.select.hWnd);
	mainWindow.select.ptr =  SelectHandler::Do;
	ShowWindow(h, SW_SHOWNORMAL);
	StartKeyHook(h, AppKeyHandler::KeyPressed);
}

void App::Destroy()
{
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}

App app;