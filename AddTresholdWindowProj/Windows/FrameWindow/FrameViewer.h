#pragma once

#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "Graphics/ColorLabel.h"
#include "templates/templates.hpp"
#include "Common\Viewer.h"
#include "Graphics\Borders.h"

class FrameViewer: public View
{
public:	
	class Signal: public LineSeries{public:Signal(Chart &c): LineSeries(c){color = 0xff0000ff;}};
	class Reference: public LineSeries{public:Reference(Chart &c): LineSeries(c){color = 0xffff0000;}};
	class VThreshold : public VBorder
	{
	    public:
			FrameViewer *owner;
			VThreshold(Chart &c) : VBorder(c){thinck = 1;}
			void Draw();
	};
	class SumSig
	{
		unsigned color;
		Chart &chart;
	public:
		double *reference, *signal;
		int count;
	public:
		SumSig(Chart &c)
			: color(0xff00ffff)
			, chart(c)
			, reference(NULL)
			, signal(NULL)
			{}
			void Draw();
			void SetData(double *reference, double *signal, int count);
		};
	int &width, &offset;
	int x;
	int points[8];
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes  
		, Signal
		, Reference
		, SumSig
		, Grid	
		, VThreshold
	>::Result> TChart;
	TChart chart;
public:
	typedef View Parent;
	HWND hWnd;
	FrameViewer();	
	void BeforeDraw(Gdiplus::Graphics &);
	void IncFrame();
	void DecFrame();
};