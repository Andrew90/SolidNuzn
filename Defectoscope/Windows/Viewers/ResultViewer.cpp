#include "stdafx.h"
#include "ResultViewer.h"
#include "App\App.h"
#include "templates\templates.hpp"

ResultViewer::ResultViewer()
	: View(tchart)
	, tchart(backScreen)
{
	maxCountSensors = App::count_cross_sensors;

	chart = &tchart;
	tchart.maxAxesX = App::count_zones;
	tchart.minAxesX = 0;
	tchart.maxAxesY = 1;
	tchart.minAxesY = 1;
	tchart.rect.top = 17;

	cursor.SetMouseMoveHandler(this, &ResultViewer::Draw);
	cursor.cross = false;

	tchart.items.get<BottomAxesMeters__>().minBorder = 0;
	tchart.items.get<BottomAxesMeters__>().maxBorder = 0.001 * App::count_zones * App::zone_length;
}

bool ResultViewer::Draw(TMouseMove &l, VGraphics &g)
{	
	double m = 0.001 * App::zone_length * (1 + offsetX); 
	wsprintf(label.buffer, L"<ff>Результат зона %d смещение %s м."
		, 1 + offsetX
		, Wchar_from<double, 4>(m)		()
		);
	label.Draw(g());
	return true;
}