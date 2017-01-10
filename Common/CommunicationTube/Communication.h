#pragma once
namespace CommunicationTCP
{
	void Init();
	void Receive(char *inputData, int inputCount, char *outputData, int &outputCount);
	void SetResult(int);
};