#include "stdafx.h"
#include "SolidData.h"
#include <math.h>
#include "ChartData.h"

#include "tools_debug/DebugMess.h"

SolidData::SolidData()
	: currentOffset(0)
{}
void SolidData::Clear()
{
	currentOffset = 0;
}

void SolidData::SetData(double *data, int count)
{
	if(count <= 0) return;
	if(currentOffset + count / 2 > MAX_ZONES_COUNT - 10) return;
	for(int i = 0; i < count; i += 2)
	{
		reference[currentOffset] = data[i];
		signal[currentOffset] = data[i + 1];
		++currentOffset;
	}
}

