#pragma once
#include "Graphics\Chart.h"
#include "window_tool\message.h"
#include "Graphics\ColorLabel.h"

class View
{
public:
	HWND hWnd;	
public:
	Chart *chart;
	Gdiplus::Bitmap *backScreen;	
	bool mouseMove;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	double currentX, currentY;
	int offsetX, offsetY;
public:
	View(Chart &);
	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TLButtonDbClk &);
	void operator()(TLButtonDown &);

	void operator()(TMouseWell &);
	void operator()(TMouseMove &);

	virtual void BeforeDraw(Gdiplus::Graphics &){}
protected:
	void SizeOffs(TMouseMove &);
};