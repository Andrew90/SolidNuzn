#pragma once
#include "Inc/BDaqCL.h" 
#include "../DebugApp/Debug1730.h"
class Device1730
{
	static const int startPoint = 0;
	Automation::BDaq::BDaqDevice *device;
	Automation::BDaq::BDaqDio    *dio;
	bool isOpen;
	unsigned &input;
	unsigned &output;
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

//#ifndef DEBUG_ITEMS
//extern Device1730 device1730;
//#else
//extern Debug1730 device1730;
//#endif

