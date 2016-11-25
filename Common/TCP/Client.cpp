#include "stdafx.h"
#include "Client.h"
#include <winsock2.h>

#define dprint printf

#include <stdio.h>
Client::Client()
	: conn(INVALID_SOCKET)
{
}

bool Client::Connect(int port, const char *addr)
{
	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (conn == INVALID_SOCKET) 
	{
        dprint("socket function failed with error: %u\n", WSAGetLastError());
		return false;
    }

	sockaddr_in service;
	service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(addr);
    service.sin_port = htons(port);

	if(INVALID_SOCKET == connect(conn, (const sockaddr *)&service, sizeof(sockaddr_in)))
	{
		closesocket(conn);
		return false;
	}
	
	return true;
}

bool Client::Recive()
{
	char buf[1024];
	int res = recv(conn, buf, 1024, 0);
	dprint("receive from client >>>>>>>>>>>>>>>>>> %s  %d", buf, res);
	return false;
}

bool Client::Transmit()
{
	char buf[] = "Send from client";
	send(conn, buf, strlen(buf) + 1, 0);
	return false;
}

void Client::Close()
{
	if (conn != INVALID_SOCKET) 
	{
		closesocket(conn);
		dprint("\nclose client");
		conn = INVALID_SOCKET;
	}
}



