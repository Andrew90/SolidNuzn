   #include "stdafx.h"
#include "ThickViewer.h"

#include "App\App.h"

ThickViewer::ThickViewer()
	: View(tchart)
	, tchart(backScreen)		
{
	chart = &tchart;
	tchart.maxAxesX = App::count_zones;
	tchart.minAxesX = 0;
	tchart.maxAxesY = 20;
	tchart.minAxesY = 1;
	tchart.rect.top = 17;

	maxCountSensors = (int)tchart.maxAxesY;

	cursor.SetMouseMoveHandler(this, &ThickViewer::Draw);
	cursor.cross = false;
}

bool ThickViewer::Draw(TMouseMove &l, VGraphics &g)
{	
	wsprintf(label.buffer, L"<ff>Толщина зона %d"
		, 1 + offsetX
		, 1 + offsetY			
		);
	label.Draw(g());
	return true;
}