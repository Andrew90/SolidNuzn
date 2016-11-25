#include "stdafx.h"
#include "ClientTreshold.h"
#include <winsock2.h>

ClientTreshold::ClientTreshold()
	: receiveHandler(CommunicationTCP::Receive)
{
}

void ClientTreshold::Do(ClientTreshold *self)
{
	self->contine = true;
	while(self->contine)
	{
		int res = 0;
		char recvbuf[1024];
		do
		{
			res = recv(self->client.conn, recvbuf, 1024, 0);

			if(res > 0)
			{
				if(NULL != self->receiveHandler)
				{
					char outputBuf[1024] = {};
					int outputCount = 1024;
					(self->receiveHandler)(recvbuf, res, outputBuf, outputCount);
					if(outputCount > 0)
					{
						int s = send(self->client.conn, outputBuf, outputCount, 0 );
					}
				}
			}
		}
		while(res > 0);
	}
}

bool ClientTreshold::Connect(int port, wchar_t *address)
{
	char t[20];
	if(client.conn != INVALID_SOCKET)
	{
		contine = false;
		client.Close();
	}
	for(int i = 0; i < 20; ++i)
	{
		t[i] = (char)address[i];
	}
	bool res = client.Connect(port, t);
	if(res)
	{
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientTreshold::Do, this, 0, NULL));
	}
	return res;
}

void ClientTreshold::Close()
{
	client.Close();
}