#pragma once
namespace Device1730
{
	extern const unsigned &input;
	extern const unsigned &output;
	bool Init(int);
	void Destroy();
	bool IsOpen();
	unsigned Read();
	void Write();
	void Write(unsigned);
	unsigned ReadOutput();
}

