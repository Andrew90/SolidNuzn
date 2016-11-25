#include "stdafx.h"
#include "CommonTcp.h"
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

CommonTcp::CommonTcp()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

CommonTcp::~CommonTcp()
{
	 WSACleanup();
}
