#include "stdafx.h"
#include "SignalViewer.h"
#include <stdio.h>
#include "window_tool/EmptyWindow.h"
#include "App/AppBase.h"
#include "Dates/SolidData.h"
#include "tools_debug/DebugMess.h"

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
}
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

#pragma warning(disable : 4996)

SignalViewer::SignalViewer()
	: View(chart)
	, chart(backScreen)
	, signal   (Singleton<L502Signal>::Instance())
	, reference(Singleton<L502Reference>::Instance())
{
	chart.rect.left = 10;
	chart.rect.top = 10;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.maxAxesX = 99;
	chart.minAxesY = -10;
	chart.maxAxesY = 10;

	cursor.cross = false;
}
//------------------------------------------------------------------------------
void SignalViewer::BeforeDraw(Gdiplus::Graphics &)
{
	 chart.maxAxesX = solidData.currentOffset;
	 chart.items.get<SignalMin>().SetData(signal.dataMin, signal.count);
	 chart.items.get<SignalMax>().SetData(signal.dataMax, signal.count);
	 chart.items.get<ReferenceMin>().SetData(reference.dataMin, reference.count);
	 chart.items.get<ReferenceMax>().SetData(reference.dataMax, reference.count);
	 chart.items.get<VCursor1>().value = solidData.start;
	 chart.items.get<VCursor2>().value = solidData.stop;
}
//----------------------------------------------------------------------------------------------------



