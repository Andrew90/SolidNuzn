#pragma once
namespace Device1730
{
	extern const unsigned &input;
	extern const unsigned &output;
	bool Init(wchar_t *);
	void Destroy();
	bool IsOpen();
	unsigned Read();
	void Write();
	void Write(unsigned);
	unsigned ReadOutput();
}

