#include "stdafx.h"
#include "server.h"

#include <winsock2.h>

#define dprint printf

#include <stdio.h>

Server::Server()
	: hThread(INVALID_HANDLE_VALUE)
	, receiveHandler(NULL)
{
}

void Server::Do(Server *self)
{
	self->contine = true;
	while(self->contine)
	{
		SOCKET ClientSocket = accept(self->ListenSocket, NULL, NULL);
		int res = 0;
		char recvbuf[1024];
		do
		{
			res = recv(ClientSocket, recvbuf, 1024, 0);

			if(res > 0)
			{
				if(NULL != self->receiveHandler)
				{
					char outputBuf[1024] = {};
					int outputCount = 1024;
					(self->receiveHandler)(recvbuf, res, outputBuf, outputCount);
					if(outputCount > 0)
					{
						int s = send( ClientSocket, outputBuf, outputCount, 0 );
					}
				}
			}
		}
		while(res > 0);
		shutdown(ClientSocket, SD_SEND);
	}
}

void Server::Init(int port)
{
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) 
	{
        dprint("socket function failed with error: %u\n", WSAGetLastError());
    }

	sockaddr_in service;
	service.sin_family = AF_INET;
    service.sin_addr.s_addr = htonl(INADDR_ANY);
    service.sin_port = htons(port);

	int res = bind(ListenSocket, (SOCKADDR *) &service, sizeof (service));
    if (res == SOCKET_ERROR) {
        dprint("bind failed with error %u\n", WSAGetLastError());
        closesocket(ListenSocket);
    }

	if (listen(ListenSocket, 5) != SOCKET_ERROR)
	{
        hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Server::Do, this, 0, NULL);
	}
	else
	{
		dprint("listen function failed with error: %d", WSAGetLastError());
	}
}

void Server::Destroy()
{
	closesocket(ListenSocket);
}