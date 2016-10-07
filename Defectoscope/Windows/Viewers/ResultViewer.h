#pragma once
#include "Viewers\Viewer.h"
#include "Graphics\Chart.hpp"
#include "templates\typelist.hpp"
#include "Graphics\FixedGridSeries.h"
#include "Graphics\EmptyLeftAxes.h"
#include "Graphics\BarSeries.h"
#include "App\App.h"

class ResultViewer: public View
{
	class BottomAxesMeters__: public BottomAxesMeters
	{
	public:
		BottomAxesMeters__(Chart &c): BottomAxesMeters(c, App::zone_length){}
	};
	typedef ChartDraw<Chart, TL::MkTlst<
		EmptyLeftAxes
		, BottomAxesMeters__
		, BarSeries
		, FixedGrid		
	>::Result>	TChart;
	TChart tchart;
public:
	typedef View Parent;
	ResultViewer();
	bool Draw(TMouseMove &, VGraphics &);
};