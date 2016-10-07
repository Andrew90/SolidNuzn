 #pragma once
#include "Viewer.h"
#include "Graphics\Chart.hpp"
#include "templates\typelist.hpp"
#include "Graphics\FixedGridSeries.h"
#include "Graphics\GridChart.h"
#include "Graphics\BarSeries.h"

class ThickViewer: public View
{
	typedef ChartDraw<Chart, TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxesGrid
		, BarSeriesDouble
		, FixedGrid		
	>::Result>	TChart;
	TChart tchart;	
public:
	typedef View Parent;
	ThickViewer();
	bool Draw(TMouseMove &, VGraphics &);
};