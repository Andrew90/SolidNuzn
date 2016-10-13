#include "stdafx.h"
#include "Net\InitTcp.h"
#include "Net\AsyncServer.h"
#include "Net\GetHostByName.h"
#include "Net\SendArray.hpp"
#include "SendArray/ThickArray.h"
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	InitTcp initTcp;
	AsyncServer::Create(8765);

	Sleep(1000);

	//StopCycleProtocol::Client::Do(GetHostByName()(L"Andrew"), 8765);
	for(int i = 10; i < 100; i+= 3)
	{
		SendArray::TemplClient<SendArr>::Do(GetHostByName()(L"Andrew"), 8765, i, i + 3);
		//Sleep(100);
	}
	getchar();
	return 0;
}