#include "stdafx.h"
#include "ChartData.h"

ChartData::ChartData()
	: count(0)
{}

void ChartData::Clear()
{
	count = 0;
}

void ChartData::Set(double *inputData, int inputCount, int outputCount)
{
	if(outputCount > 1024) outputCount = 1024;
	for(int i = 0; i < outputCount; ++i)
	{
		dataMin[i] = 9e99;
		dataMax[i] = -9e99;
	}
	double dx = (double)outputCount / inputCount;
	double x = 0;
	for(int i = 0; i < inputCount; ++i)
	{
		int ii = int(x);
		if(dataMin[ii] > inputData[i]) dataMin[ii] = inputData[i];
		if(dataMax[ii] < inputData[i]) dataMax[ii] = inputData[i];
		x += dx;
	}
	count = outputCount;
}