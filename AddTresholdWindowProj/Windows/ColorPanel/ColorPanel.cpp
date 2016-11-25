#include "stdafx.h"
#include "ColorPanel/ColorPanel.h"
#include "templates/templates.hpp"
#include "window_tool/EmptyWindow.h"
#include "App/AppBase.h"
#include "window_tool\WindowsPosition.h"
#include "window_tool\MenuAPI.h"

using namespace Gdiplus;

namespace ColorPanelMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	struct MainExit
	{
		static void Do(HWND h)
		{
			ColorPanel &colorPanel = Singleton<ColorPanel>::Instance();
			DestroyWindow(colorPanel.hWnd);
			colorPanel.hWnd = NULL;
		}
	};

	MENU_ITEM(L"Выход", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<MainExit>
		>::Result list;
	};

	struct Options{};
	MENU_TEXT(L"Настройки", TopMenu<Options>)

	MENU_ITEM(L"Сохранить координаты окна", WindowPositionDlg<ColorPanel>)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<WindowPositionDlg<ColorPanel>>
		>::Result list;
	};

	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<Options>
	>::Result items_list;	
};

ColorPanel::ColorPanel() 
	: backScreen(NULL)
	, backColor(0xffaaaaaa)
	, hWnd(NULL)
{
	buf[0] = 0;
}

LRESULT ColorPanel::operator()(TCreate &l)
{
	//Menu<ColorPanelMenu::items_list>().Init(l.hwnd);
	return 0;
}

void ColorPanel::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED)return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
    Graphics g(backScreen);
	g.FillRectangle(&SolidBrush(Color(backColor | 0xff000000)), 0, 0, l.Width, l.Height);   
	if(0 != buf[0])
	{
		int len = wcslen(buf);
		PointF origin((REAL)0, (REAL)0);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
	
		SolidBrush brushFontColor(-1);
		Gdiplus::Font font(L"Arial", (REAL)48, FontStyleBoldItalic);
		RectF rect(0, 0, l.Width, l.Height);
		g.MeasureString(buf, len, &font, origin, &format, &rect);  
	
		origin.Y += rect.GetTop() + l.Height / 2;
		origin.X += rect.GetLeft() + l.Width / 2;
		g.DrawString(buf, len, &font, origin, &brushFontColor);
	}
}
//--------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ColorPanel::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
	}
	EndPaint(l.hwnd, &p);
}
//---------------------------------------------------------------------------------
void ColorPanel::operator()(TCommand &l)
{
	EventDo(l);
}

void ColorPanel::operator()(TDestroy &m)
{
	StoredWindowPosition<ColorPanel>::Do(m.hwnd);
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
	delete backScreen;
    backScreen = NULL;	
}
//--------------------------------------------------------------------------------
namespace
{
	bool &openColorPanel = Singleton<ParametersTable>::Instance().items.get<MessagePanelVisible>().value;
}
//-------------------------------------------------------------------------------------------------
void ColorPanel::__SetText__(wchar_t *txt, unsigned color)
{	
	wcsncpy(buf, txt, dimention_of(buf));
	backColor = color;

	if(openColorPanel)
	{

		HWND h = FindWindow(WindowClass<ColorPanel>()(), 0);
		if(NULL != h)
		{			
			RepaintWindow(h);
			SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(h);
		}
		else
		{		
			RECT r;
			WindowPosition::Get<ColorPanel>(r);
			HWND h = WindowTemplate(this, L"", r.left, r.top, r.right, r.bottom);
			ShowWindow(h, SW_SHOWNORMAL);
		}	
	}
}
//---------------------------------------------------
void ColorPanel::SetText(wchar_t *txt, unsigned color)
{
	 Singleton<ColorPanel>::Instance().__SetText__(txt, color);
}
//---------------------------------------------
void ColorPanel::__Clear__()
{	
	buf[0] = 0;
	backColor = 0xffaaaaaa;

	if(openColorPanel)
	{	
		HWND h = FindWindow(WindowClass<ColorPanel>()(), 0);
		if(NULL != h)
		{			
			RepaintWindow(h);
			SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(h);
		}
		else
		{		
			RECT r;
			WindowPosition::Get<ColorPanel>(r);
			HWND h = WindowTemplate(this, L"", r.left, r.top, r.right, r.bottom);
			ShowWindow(h, SW_SHOWNORMAL);
		}	
	}
}

void ColorPanel::Clear()
{
	Singleton<ColorPanel>::Instance().__Clear__();
}

void ColorPanel::Close()
{
	HWND hh = FindWindow(WindowClass<ColorPanel>()(), 0);
	if(NULL != hh)
	{
		DestroyWindow(hh);
	}
}

void ColorPanel::Open()
{
	if(openColorPanel)
	{
		RECT r;
		WindowPosition::Get<ColorPanel>(r);
		HWND h = WindowTemplate(&Singleton<ColorPanel>::Instance()
			, L"", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);
		DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
		dwStyle &= ~(WS_SYSMENU);
		SetWindowLong(h, GWL_STYLE, dwStyle);
	}
}

void ColorPanel::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 300;
		l.pMinMaxInfo->ptMinTrackSize.y = 200;
	}	
}