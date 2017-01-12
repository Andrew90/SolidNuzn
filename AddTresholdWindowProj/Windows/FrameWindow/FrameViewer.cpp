#include "stdafx.h"
#include "FrameViewer.h"
#include <stdio.h>
#include "window_tool/EmptyWindow.h"
#include "App/AppBase.h"
#include "Dates/SolidData.h"
#include "tools_debug/DebugMess.h"
#include "App\AppBase.h"

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
}
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

#pragma warning(disable : 4996)

void FrameViewer::VThreshold::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
		CombineModeReplace
		);
	for(int i = 0; i < dimention_of(owner->points); ++i)
	{
		value = owner->points[i];
		int offs = owner->points[i] - owner->x;
		VBorder::Draw();
		SumSig &sumSig = (*(TChart *)&chart).items.get<SumSig>();
		if(NULL != sumSig.reference && NULL != sumSig.signal)
		{
			double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
			int x =  int(chart.rect.left + chart.offsetAxesLeft + (value - chart.minAxesX) * dX);
			double minY = chart.minAxesY;
			double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
			double yOffs = chart.rect.bottom - chart.offsetAxesBottom;

			int y = int(yOffs - (sumSig.signal[offs] - sumSig.reference[offs] - minY) * dY);
			chart.g->FillEllipse(&SolidBrush(0xff000000), x - 4, y - 4, 8, 8);
		}		
	}
	chart.g->SetClip(&Region());
}

void FrameViewer::SumSig::Draw()
{
	if(NULL == reference || NULL == signal) return;
	Color col(color);
		Pen pen(col, 2);
		chart.g->SetClip(&Region(RectF(
			REAL(chart.rect.left + chart.offsetAxesLeft + 1)
			, REAL(chart.rect.top + chart.offsetAxesTop + 1)
			, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 2)
			, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 2)
			)),
			CombineModeReplace
			);
		double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);

		double yOffs = chart.rect.bottom - chart.offsetAxesBottom;

		int width = chart.rect.right - chart.rect.left - chart.offsetAxesRight - chart.offsetAxesLeft;
		double dX = (double)width / (count - 1);
		int x0 = chart.rect.left + chart.offsetAxesLeft;
		double minY = chart.minAxesY;
		int y0 = int(yOffs - (signal[0] - reference[0] - minY) * dY);
		double x = x0;
		int y = y0;
		for(int i = 1; i < count; ++i)
		{
			x += dX;
			y = int(yOffs - (signal[i] - reference[i] - minY) * dY);
			if(x0 != int(x) || y0 != y)
			{
				chart.g->DrawLine(&pen, x0, y0, (int)x, y);
				x0 = int(x);
				y0 = y;
			}
		}
		chart.g->SetClip(&Region());
}

void FrameViewer::SumSig::SetData(double *reference_, double *signal_, int count_)
{
   reference = reference_;
   signal = signal_;
   count = count_;
}

FrameViewer::FrameViewer()
	: View(chart)
	, chart(backScreen)	
	, offset(Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalOffset>().value)
	, width(Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalWidth>().value)
	, x(0)
{
	chart.rect.left = 10;
	chart.rect.top = 20;
	chart.offsetAxesBottom = 10;

	chart.items.get<VThreshold>().owner = this;
}
//------------------------------------------------------------------------------
void FrameViewer::BeforeDraw(Gdiplus::Graphics &)
{
	chart.minAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMin>().value;
	chart.maxAxesY = Singleton<GraphAxesTable>::Instance().items.get<PrimarySignalMax>().value;

	chart.minAxesX = x;
	chart.maxAxesX = x + width;

	chart.items.get<Signal>().SetData(&solidData.signal[x], width);
	chart.items.get<Reference>().SetData(&solidData.reference[x], width);
}
//----------------------------------------------------------------------------------------------------
void FrameViewer::IncFrame()
{
	x += offset;
	if(x + width >= solidData.currentOffset)	x =  solidData.currentOffset - width;
}

void FrameViewer::DecFrame()
{
	x -= offset;
	if(x < 0)	x =  0;
}



