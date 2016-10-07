#pragma once
#include "Viewer.h"
#include "Graphics\Chart.hpp"
#include "templates\typelist.hpp"
#include "Graphics\FixedGridSeries.h"
#include "Graphics\GridChart.h"

class LongViewer: public View
{
	typedef ChartDraw<Chart, TL::MkTlst<
		FixedLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, FixedGrid
	>::Result>	TChart;
	TChart tchart;
public:
	typedef View Parent;
	LongViewer();
	bool Draw(TMouseMove &, VGraphics &);
};