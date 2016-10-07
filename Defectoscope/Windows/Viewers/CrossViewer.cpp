#include "stdafx.h"
#include "CrossViewer.h"
#include "App\App.h"

CrossViewer::CrossViewer()
	: View(tchart)
	, tchart(backScreen)
{
	maxCountSensors = App::count_cross_sensors;

	chart = &tchart;
	tchart.maxAxesX = App::count_zones;
	tchart.minAxesX = 0;
	tchart.maxAxesY = 1 + maxCountSensors;
	tchart.minAxesY = 1;
	tchart.rect.top = 17;

	cursor.SetMouseMoveHandler(this, &CrossViewer::Draw);
}

bool CrossViewer::Draw(TMouseMove &l, VGraphics &g)
{	
	wsprintf(label.buffer, L"<ff>Поперечный зона %d  датчик %d"
		, 1 + offsetX
		, 1 + offsetY			
		);
	label.Draw(g());
	return true;
}