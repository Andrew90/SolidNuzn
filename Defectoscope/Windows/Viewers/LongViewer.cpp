#include "stdafx.h"
#include "LongViewer.h"
#include "App\App.h"

LongViewer::LongViewer()
	: View(tchart)
	, tchart(backScreen)
{
	maxCountSensors = App::count_long_sensors;

	chart = &tchart;
	tchart.maxAxesX = App::count_zones;
	tchart.minAxesX = 0;
	tchart.maxAxesY = 1 + maxCountSensors;
	tchart.minAxesY = 1;
	tchart.rect.top = 17;

	cursor.SetMouseMoveHandler(this, &LongViewer::Draw);
}

bool LongViewer::Draw(TMouseMove &l, VGraphics &g)
{	
	wsprintf(label.buffer, L"<ff>Продольный зона %d  датчик %d"
		, 1 + offsetX
		, 1 + offsetY			
		);
	label.Draw(g());
	return true;
}