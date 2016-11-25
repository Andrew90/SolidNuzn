#pragma once

class Client
{
	
public:
	unsigned conn;	
	Client();
	bool Connect(int port, const char *addr);
	bool Recive();
	bool Transmit();
	void Close();
};