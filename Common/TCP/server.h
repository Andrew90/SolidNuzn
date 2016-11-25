#pragma once
#include "server.h"
class Server
{
	void *hThread;
	unsigned  ListenSocket;
	static void Do(Server *self);
	bool contine;
public:
	    void(*receiveHandler)(char *inputData, int inputCount, char *outputData, int &outputCount);
		Server();
		void Init(int port);
		void Destroy();
};