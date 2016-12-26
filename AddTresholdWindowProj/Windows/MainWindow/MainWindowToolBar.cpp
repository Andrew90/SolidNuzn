#include "stdafx.h"
#include "App\config.h"
#include "MainWindowToolBar.h"
#include "..\resource.h"
#include "templates\templates.hpp"
#include "window_tool\InitToolbar.hpp"
#include "FrameWindow\FrameWindow.h"
#include "window_tool\WindowsPosition.h"
#include "window_tool\Emptywindow.h"
#include "Automat\Automat.h"
#include "tools_debug\DebugMess.h"
#include "App\AppKeyHandler.h"

#ifdef DEBUG_ITEMS
#include "Dates\CounterTubes.h"
#include "App\App.h"
#include "window_tool\MenuAPI.h"
#endif

namespace 
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};

	KEY(IDB_CycleBtn   , L"F4 Цикл")
	KEY(IDB_Reset      , L"F9 Стоп")
	KEY(IDB_QueryBtn   , L"Тест")
	KEY(IDB_OptionsBtn , L"Настр.пор.")
	KEY(IDB_arrow_down , L"F5 Вниз")
	KEY(IDB_arrow_up   , L"F6 БАЗА")
	KEY(IDB_arrow_left , L"F7 Влево")
	KEY(IDB_arrow_right, L"F8 Clear")
	KEY(IDB_Continue, L"F11 Продолжить")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_OptionsBtn) 		
		, BUTTON_KEY(IDB_Reset)
		, SeparatorToolbar<1>
#ifdef DEBUG_ITEMS
		, BUTTON_KEY(IDB_arrow_down) 
		, BUTTON_KEY(IDB_arrow_up)
		, BUTTON_KEY(IDB_arrow_left) 
		, BUTTON_KEY(IDB_arrow_right)
		, BUTTON_KEY(IDB_Reset)
		, SeparatorToolbar<2>
#endif
		>::Result tool_button_list;
#undef BUTTON_KEY
//----------------------------------------------------------------------------------
	static bool closed_packet_dialog = true;
	static bool run_once_per_sycle = false;
	void Key<IDB_CycleBtn>::Click(HWND h)
	{
		Automat::Start();
		AppKeyHandler::Run();
		dprint("IDB_CycleBtn\n");
	}
//-----------------------------------------------------------------------------
	void Key<IDB_Reset>::Click(HWND h)
	{
		Automat::Stop();	
		dprint("IDB_Reset\n");
	}
//-------------------------------------------------------------------------------
	void Key<IDB_OptionsBtn>::Click(HWND h)
	{
		HWND hh = FindWindow(WindowClass<FrameWindow>()(), 0);
		if(NULL != hh)
		{
			RepaintWindow(hh);
			SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(hh);
		}
		else
		{
			RECT r;
			WindowPosition::Get<FrameWindow>(r);
			hh = WindowTemplate(&Singleton<FrameWindow>::Instance(), L"", r.left, r.top, r.right, r.bottom);
			ShowWindow(hh, SW_SHOWNORMAL);
		}
	}
//------------------------------------------------------------------------------
#ifdef DEBUG_ITEMS
	void Key<IDB_arrow_down>::Click(HWND h)
	{
		App::CheckMenuItem();
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_up>::Click(HWND h)
	{
		CounterTubes::Inc(L"D");
		 App::UpdateMainWindow();
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_left>::Click(HWND h)
	{
		CounterTubes::Inc(L"K");
		 App::UpdateMainWindow();
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_right>::Click(HWND h)
	{
		CounterTubes::Inc(L"E");
		 App::UpdateMainWindow();
	}
//----------------------------------------------------------------------------
#endif
}
//--------------------------------------------------------------------------------------------
HWND MainWindowToolBar::Init(HWND h)
{
	return hWnd = InitToolbar<tool_button_list>()(h);
}
//-------------------------------------------------------------------
void MainWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int MainWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//------------------------------------------------------------------------

