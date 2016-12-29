#include "stdafx.h"
#include "Communication.h"
#include "tools_debug/DebugMess.h"

namespace CommunicationTCP
{
	unsigned short TubeNumber = 0;
	int Result = 11;
	HANDLE hEvent;

	void Init()
	{
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	void Receive(char *inputData, int inputCount, char *outputData, int &outputCount)
	{
		switch(inputData[4])
		{
		case 1:
			TubeNumber = *(unsigned short *)&inputData[8];
			dprint("tube number  %d\n", TubeNumber);
			outputCount = 0;
			break;
		case 3:
			WaitForSingleObject(hEvent, INFINITE);
			*(int *)&outputData[28] = Result;
			dprint("send  %d\n", Result);
			outputCount = 40;
			break;
		}
	}
	void SetResult(int id)
	{
		Result = id;
		SetEvent(hEvent);
	}
}