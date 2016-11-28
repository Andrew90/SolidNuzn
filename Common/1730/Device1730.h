#pragma once
#include "App/config.h"
#ifndef DEBUG_ITEMS
#include "Inc/BDaqCL.h" 
#endif
class Device1730
{
	static const int startPoint = 0;
#ifndef DEBUG_ITEMS
	Automation::BDaq::BDaqDevice *device;
	Automation::BDaq::BDaqDio    *dio;
#endif
	bool isOpen;
public:
	unsigned input;
	unsigned output;
public:	
	Device1730();
	void Start(){}
	bool Init(int);
	void Destroy();
	bool IsOpen();
	unsigned Read();
	void Write();
	void Write(unsigned);
	unsigned ReadOutput();
};

