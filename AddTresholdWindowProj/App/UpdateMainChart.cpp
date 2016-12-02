#include "stdafx.h"
#include "UpdateMainChart.h"
#include "MainWindow/MainWindow.h"
#include "Dates\SolidData.h"
#include "Dates\ChartData.h"
#include "window_tool/EmptyWindow.h"
#include "templates\templates.hpp"
#include "Dates\SolidData.h"
#include "App\App.h"

#include "tools_debug/DebugMess.h"

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
	L502Reference &l502Reference = Singleton<L502Reference>::Instance();
	L502Signal &l502Signal = Singleton<L502Signal>::Instance();
}

DWORD WINAPI Updata(LPVOID h)
{
	MainWindow *o = (MainWindow *)GetWindowLongPtr((HWND)h, GWLP_USERDATA);
	if(o)
	{
		int length = o->signalViewer.chart.rect.right 
			- o->signalViewer.chart.rect.left 
			- o->signalViewer.chart.offsetAxesLeft 
			- o->signalViewer.chart.offsetAxesRight;
		if(solidData.currentOffset < length)
		{
			l502Signal.Set(solidData.signal, solidData.currentOffset, solidData.currentOffset);
			l502Reference.Set(solidData.reference, solidData.currentOffset, solidData.currentOffset);
			o->signalViewer.chart.maxAxesX = solidData.currentOffset;
		}
		else
		{
			l502Signal.Set(solidData.signal, solidData.currentOffset, length);
			l502Reference.Set(solidData.reference, solidData.currentOffset, length);
			o->signalViewer.chart.maxAxesX = solidData.currentOffset;
		}
		RepaintWindow(o->signalViewer.hWnd);
	}
	return 0;
}

void UpdateMainChart(int delay, HWND h)
{
	static unsigned lastTime = 0;
	unsigned t = GetTickCount();
	if(lastTime + delay < t)
	{
		lastTime = t;
		QueueUserWorkItem(Updata, h, WT_EXECUTEDEFAULT);
	}
}