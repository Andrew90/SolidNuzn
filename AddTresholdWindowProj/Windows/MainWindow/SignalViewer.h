#pragma once
#include "window_tool/message.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "templates/typelist.hpp"
#include "Graphics/Borders.h"
#include "Dates/ChartData.h"
#include "Common/Viewer.h"

class SignalViewer: public View
{	
	TMouseMove storedMouseMove;
public:
	ChartData &signal, &reference;
private:	
	class VCursor1 : public VBorder
	{
	public:
		VCursor1(Chart &c):VBorder(c){color = 0xffffff00;}
	};
	class VCursor2 : public VBorder
	{
	public:
		VCursor2(Chart &c):VBorder(c){color = 0xffffff00;}
	};
	class SignalMin: public LineSeries{public:SignalMin(Chart &c): LineSeries(c){color = 0xff0000ff;}};
	class SignalMax: public LineSeries{public:SignalMax(Chart &c): LineSeries(c){color = 0xff0000aa;}};
	class ReferenceMin: public LineSeries{public:ReferenceMin(Chart &c): LineSeries(c){color = 0xffff0000;}};
	class ReferenceMax: public LineSeries{public:ReferenceMax(Chart &c): LineSeries(c){color = 0xffaa0000;}};
public:
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, SignalMin
		, SignalMax
		, ReferenceMin
		, ReferenceMax
		, Grid	
		, VCursor1
		, VCursor2
	>::Result> chart;
public:
	typedef View Parent;
	HWND hWnd;
	SignalViewer();
	void BeforeDraw(Gdiplus::Graphics &);	
};
