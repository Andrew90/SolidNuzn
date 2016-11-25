#include "stdafx.h"
#include "Communication.h"
#include "tools_debug/DebugMess.h"

unsigned short CommunicationTCP::TubeNumber;
int CommunicationTCP::Result = 11;

void CommunicationTCP::Receive(char *inputData, int inputCount, char *outputData, int &outputCount)
{
	switch(inputData[4])
	{
	case 1:
		TubeNumber = *(unsigned short *)&inputData[8];
		dprint("tube number  %d\n", TubeNumber);
        outputCount = 0;
		break;
	case 3:
		for(int i = 0; i < 200; ++i)
		{
			if(0 != Result)
			{
				break;
			}
			else
			{
				Sleep(10);
			}
		}
		*(int *)&outputData[28] = Result;
		dprint("send  %d\n", Result);
		outputCount = 40;
		break;
	}
}