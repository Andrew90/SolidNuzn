#include "stdafx.h"
#include "FrameViewer.h"
#include <stdio.h>
#include "window_tool/EmptyWindow.h"
#include "App/AppBase.h"
#include "Dates/SolidData.h"
#include "tools_debug/DebugMess.h"
#include "App\AppBase.h"

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
}
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

#pragma warning(disable : 4996)

FrameViewer::FrameViewer()
	: View(chart)
	, chart(backScreen)	
	, offset(Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalOffset>().value)
	, width(Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalWidth>().value)
	, x(0)
{
	chart.rect.left = 10;
	chart.rect.top = 10;
	chart.offsetAxesBottom = 10;
	
	//chart.minAxesX = 0;
	//chart.maxAxesX = 99;
	//chart.minAxesY = -10;
	//chart.maxAxesY = 10;

	cursor.cross = false;
}
//------------------------------------------------------------------------------
void FrameViewer::BeforeDraw(Gdiplus::Graphics &)
{
	//chart.maxAxesX = solidData.currentOffset;
	//chart.items.get<SignalMin>().SetData(signal.dataMin, signal.count);
	//chart.items.get<SignalMax>().SetData(signal.dataMax, signal.count);
	//chart.items.get<ReferenceMin>().SetData(reference.dataMin, reference.count);
	//chart.items.get<ReferenceMax>().SetData(reference.dataMax, reference.count);
	//chart.items.get<VCursor1>().value = solidData.start;
	//chart.items.get<VCursor2>().value = solidData.stop;
	chart.minAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMin>().value;
	chart.maxAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMax>().value;

	chart.minAxesX = x;
	chart.maxAxesX = x + width;

	chart.items.get<Signal>().SetData(&solidData.signal[x], width);
	chart.items.get<Reference>().SetData(&solidData.reference[x], width);
}
//----------------------------------------------------------------------------------------------------
void FrameViewer::IncFrame()
{
	x += width;
	if(x >= solidData.currentOffset)	x =  solidData.currentOffset - width;
	RepaintWindow(hWnd);
}

void FrameViewer::DecFrame()
{
	x -= width;
	if(x < 0)	x =  0;
	RepaintWindow(hWnd);
}



