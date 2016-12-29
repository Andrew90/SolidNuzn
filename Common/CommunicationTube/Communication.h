#pragma once
namespace CommunicationTCP
{
	//extern unsigned short TubeNumber;
	//extern int Result;
	void Init();
	void Receive(char *inputData, int inputCount, char *outputData, int &outputCount);
	void SetResult(int);
};