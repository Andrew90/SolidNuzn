#pragma once

#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "Graphics/ColorLabel.h"
#include "templates/templates.hpp"
#include "Common\Viewer.h"

class FrameViewer: View
{	
	int &width, &offset;
	int x;
	class Signal: public LineSeries{public:Signal(Chart &c): LineSeries(c){color = 0xff0000ff;}};
	class Reference: public LineSeries{public:Reference(Chart &c): LineSeries(c){color = 0xffff0000;}};
public:
	ColorLabel label;
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes  
		, Signal
		, Reference
		, Grid	
	>::Result> chart;
public:
	typedef View Parent;
	HWND hWnd;
	FrameViewer();	
	void BeforeDraw(Gdiplus::Graphics &);
	void IncFrame();
	void DecFrame();
};