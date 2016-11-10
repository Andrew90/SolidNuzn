#include "stdafx.h"
#include "FrameWindow\ThresholdsViewer.h"
#include "App\AppBase.h"

using namespace Gdiplus;

namespace
{
}

ThresholdsViewer::Lines::Lines(Chart &c)
	: LineSeries(c)
{}

void ThresholdsViewer::Lines::Draw()
{
}

ThresholdsViewer::FrameLine::FrameLine(Chart &c)
	: LineSeries(c)
{}

void ThresholdsViewer::FrameLine::Draw()
{
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

	label.fontHeight = 15;
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

	chart.minAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMin>().value;
	chart.maxAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMax>().value;

	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;

	chart.Draw(g);	
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