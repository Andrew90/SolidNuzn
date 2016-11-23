#pragma once
#include "Inc/BDaqCL.h" 
class Device1730
{
	static const int startPoint = 0;
	Automation::BDaq::BDaqDevice *device;
	Automation::BDaq::BDaqDio    *dio;
	bool isOpen;
public:
	unsigned input;
	unsigned output;
public:	
	Device1730();
	void Start(){}
	bool Init(wchar_t *);
	void Destroy();
	bool IsOpen();
	unsigned Read();
	void Write();
	void Write(unsigned);
	unsigned ReadOutput();
};
