// AddTresholdWindowProj.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <commctrl.h>
#include "AddTresholdWindowProj.h"
#include "App\App.h"
#include "Base\Base.hpp"
#include "window_tool\Emptywindow.h"
#include "MainWindow\MainWindow.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	CreateSemaphore(0, 0, 1, L"solid_group_161026");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND h = FindWindow(WindowClass<MainWindow>()(), 0);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
		return 0;
	}
	
	MSG msg;

	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize=sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC=0;
	InitCommonControlsEx(&iccx);

	ULONG_PTR gdiplusToken; 
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;    
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Initialize initialize;

	App::Init();

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	App::Destroy();
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return (int) msg.wParam;
}

