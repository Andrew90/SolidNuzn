#include "stdafx.h"
#include "FrameWindow\ThresholdsViewer.h"
#include "App\AppBase.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "window_tool\MenuAPI.h"
#include "FrameWindow\FrameWindow.h"
#include "window_tool\Emptywindow.h"

using namespace Gdiplus;

namespace
{
	ComputeSolidGroup &computeSolidGroup = Singleton<ComputeSolidGroup>::Instance();
}

ThresholdsViewer::Lines::Lines(Chart &c)
	: LineSeries(c)
{}

void ThresholdsViewer::Lines::Draw()
{
	for(auto i = computeSolidGroup.solidItems.begin(); i != computeSolidGroup.solidItems.end(); ++i)
	{
		if(i->status == ComputeSolidGroup::enabled || i->status == ComputeSolidGroup::new_item)
		SetData(i->points, 8);
		color = computeSolidGroup.groupNameList[i->groupName].color | 0xff000000;
		LineSeries::Draw();
	}
}

ThresholdsViewer::FrameLine::FrameLine(Chart &c)
	: LineSeries(c)
{
	color = 0xff000000;
	LineSeries::SetData(data, 8);
}

LRESULT ThresholdsViewer::operator()(TCreate &)
{
	chart.minAxesX = 1;
	chart.maxAxesX = 8;
	return 0;
}

ThresholdsViewer::ThresholdsViewer()
	: chart(backScreen)
{
	chart.rect.left = 10;
	chart.rect.top = 20;
	label.top = 0;	
	label.fontHeight = 12;
}

void ThresholdsViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	

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
	SolidBrush solidBrush(Color(0xffaaaaaa));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);

	chart.minAxesY = Singleton<GraphAxesTable>::Instance().items.get<OffsetPointsMin>().value;
	chart.maxAxesY = Singleton<GraphAxesTable>::Instance().items.get<OffsetPointsMax>().value;

	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;

	label.Draw(g);
	chart.Draw(g);	
}

void ThresholdsViewer::Draw(double *data)
{
	HWND h = FindWindow(WindowClass<FrameWindow>()(), 0);
	if(NULL != h)
	{			
		ThresholdsViewer &w = ((FrameWindow *)GetWindowLongPtr(h, GWLP_USERDATA))->thresholdsViewer;
		if(NULL == w.backScreen) return;
		HDC hdc = GetDC(w.hWnd);
		Graphics g(hdc);		
		w.chart.g = &g;
		w.chart.DrawItems<TL::MkTlst<Lines, FrameLine>::Result>(g);
		LineSeries c(w.chart);
		c.SetData(data, 8);
		c.color = -1;
		c.Draw();
		
		ReleaseDC(w.hWnd, hdc);
	}
}

void ThresholdsViewer::operator()(TPaint &l)
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
//------------------------------------------------------------------------------------------
namespace 
{
	template<>struct TopMenu<ThresholdsViewer>			   
	{										   
		typedef NullType list;				   
	};										   
	MENU_TEXT(L"Добавить порог", TopMenu<ThresholdsViewer>)	   
											   
	typedef TL::MkTlst<						   
		TopMenu<ThresholdsViewer>						   
	>::Result items_list;					   
											   
											   
	template<>struct Event<TopMenu<ThresholdsViewer> >	   
	{										   
		static void Do(HWND h)				   
		{									   
			Singleton<FrameWindow>::Instance().AddTreshold();   
		}									   
	};										   
}
void ThresholdsViewer::operator()(TRButtonDown &l)
{
	PopupMenu<items_list>::Do(l.hwnd, l.hwnd);
}
//--------------------------------------------------------------------------------
void ThresholdsViewer::operator()(TUser &l)
{
	l.ptr(l.data);
}