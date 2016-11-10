#include "stdafx.h"
#include "FrameWindowToolBar.h"
#include "..\resource.h"
#include "templates\templates.hpp"
#include "window_tool\InitToolbar.hpp"
#include "FrameWindow\FrameWindow.h"

#include "tools_debug\DebugMess.h"

namespace 
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_LeftArrow   , L"Лево")
	KEY(IDB_RightArrow      , L"Право")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_LeftArrow)
		, BUTTON_KEY(IDB_RightArrow)
		, SeparatorToolbar<1>
		>::Result tool_button_list;
#undef BUTTON_KEY
//----------------------------------------------
	FrameWindow &frameWindow = Singleton<FrameWindow>::Instance();
//---------------------------------------------------------------------------------
	void Key<IDB_LeftArrow>::Click(HWND h)
	{
		frameWindow.DecFrame();
	}	
//-------------------------------------------------------------------------------
	void Key<IDB_RightArrow>::Click(HWND h)
	{
		frameWindow.IncFrame();
	}
}
//------------------------------------------------------------------------------
HWND FrameWindowToolBar::Init(HWND h)
{
	return hWnd = InitToolbar<tool_button_list, 16>()(h);
}
//-------------------------------------------------------------------
void FrameWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int FrameWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//-------------------