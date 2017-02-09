#include "stdafx.h"
#include "IOportsViewer.h"
#include "Graphics/ColorLabel.h"
#include "App/AppBase.h"
#include "tools_debug/DebugMess.h"
#include "Graphics/Chart.h"
#include "L502\L502SolidGroup.h"
#include "DlgTemplates\ParamDlg.h"
#include "1730\Device1730.h"
using namespace Gdiplus;
//-----------------------------------------------------------------------------
namespace
{
struct __draw_data__
{
	int x, dY;
	Graphics &g;
	int colorOn;
	int value;
	__draw_data__(int x, Graphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};

template<class T>struct MessageField
{
	wchar_t buf[128];
	wchar_t *Text()
	{		
		wsprintf(buf, L"%S", &__FUNCTION__[43]);
		int len = wcslen(buf);
		buf[len - 7] = 0;
		return buf;
	}
};

PARAM_TITLE(UnitOn, L"Модуль включён")
PARAM_TITLE(TubeInUnit          , L"Труба в модуле")

template<class O, class P>struct __draw__
{
	void operator()(O *o, P *p)
	{
		ColorLabel l;
		l.left = p->x;
		l.top = p->dY;
		wsprintf(l.buffer, L"<55>%s",  ParamTitle<O>()());
		l.Draw(p->g);
		int color = 0xffcccccc;
		p->g.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};
//------------------------------------------------------------------------------------
struct __io_update_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	int value;
	__io_update_data__(int x, HDCGraphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};
template<class O, class P>struct __update__
{
	void operator()(O *o, P *p)
	{
		int color = 0xffcccccc;
		if(o->value & p->value) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};

VOID CALLBACK __Update__(PVOID oo, BOOLEAN)
{	
	IOportsViewer *o = (IOportsViewer *)oo;
	
	HDCGraphics g(o->hWnd, o->backScreen);
    unsigned input = Device1730::Read();

	TL::foreach<InputBitTable::items_list, __update__>()(&Singleton<InputBitTable>::Instance().items, &__io_update_data__(40, g, 0xff0000ff, input));
}
}
//--------------------------------------------------------------------------------------
IOportsViewer::IOportsViewer(HWND &h, Bitmap *&bs)
	: hWnd(h)
	, backScreen(bs)
	, hTimer(NULL)
	, lastTitle(true)
{}
//----------------------------------------------------------------------------------------
void IOportsViewer::Size(Graphics &g, int width, int height)
{
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, width, height);
	unsigned input = Device1730::Read();
	TL::foreach<InputBitTable::items_list, __draw__>()(&Singleton<InputBitTable>::Instance().items, &__draw_data__(40, g, 0xff0000ff, input));
}
//---------------------------------------------------------------------------------------
void IOportsViewer::Start()
{
	lastTitle = false;
	CreateTimerQueueTimer(&hTimer, NULL, __Update__, this, 2000, 500, WT_EXECUTEDEFAULT);
}
//------------------------------------------------------------------------------------------
void IOportsViewer::Stop()
{
	if(hTimer)DeleteTimerQueueTimer( NULL, hTimer, NULL );
}
//--------------------------------------------------------------------------------------------
struct __mouse_down_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	unsigned &value;
	TLButtonDown &l;
	__mouse_down_data__(int x, HDCGraphics &g, int colorOn, unsigned &value, TLButtonDown &l)  : x(x), dY(15), g(g), colorOn(colorOn), value(value), l(l) {}
};


