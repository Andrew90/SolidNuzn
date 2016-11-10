#pragma once
#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "Graphics/ColorLabel.h"
#include "templates/typelist.hpp"

class ThresholdsViewer
{	
	struct Lines: public LineSeries
	{
		Lines(Chart &);
		void Draw();
	};
	class FrameLine: public LineSeries
	{
	public:
		FrameLine(Chart &);
		void Draw();
	};
	Gdiplus::Bitmap *backScreen;	
public:
	ColorLabel label;
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesInt  
		, Lines
		, FrameLine
		, Grid	
	>::Result> chart;
public:
	HWND hWnd;
	ThresholdsViewer();
	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
};