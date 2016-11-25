#pragma once
struct CommunicationTCP
{
	static unsigned short TubeNumber;
	static int Result;
	static void Receive(char *inputData, int inputCount, char *outputData, int &outputCount);
};