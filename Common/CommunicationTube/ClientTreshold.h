#pragma once
#include "TCP\Client.h"
#include "Communication.h"
class ClientTreshold
{
	Client client;
	bool contine;
	static void Do(ClientTreshold *);
public:
	void(*receiveHandler)(char *inputData, int inputCount, char *outputData, int &outputCount);
  ClientTreshold();
  bool Connect(int , wchar_t *);
  void Close();
};

extern ClientTreshold clientTreshold;