#pragma once

struct SendArr
{
	char *clientData;
	char *serverData;
	const int size;
	const int sizeItem;
	int start, stop;
	SendArr();
	void Do();
	int Id();
	static SendArr &Instance();
};