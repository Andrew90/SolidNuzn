#include "stdafx.h"
#include "Device1730.h"
#include "tools_debug/DebugMess.h"
#include "App/config.h"

#ifndef DEBUG_ITEMS
#include "Inc/BDaqCL.h" 
using namespace Automation::BDaq;
#endif

namespace Device1730
{
	unsigned __input;
	unsigned __output;
#ifndef DEBUG_ITEMS
	//------------------------------------------------------------------------------
	static const int startPoint = 0;
	Automation::BDaq::BDaqDevice *device = NULL;
	Automation::BDaq::BDaqDio    *dio = NULL;	
	const unsigned &input = __input;
	const unsigned &output = __output;

	bool Init(wchar_t *deviceDescription)
	{
		Destroy();
		long errorCode = BDaqDevice::Open( deviceDescription, ModeWrite, device);
		if(BioFailed(errorCode))
		{
			device = NULL;
			return false;
		}
		errorCode = device->GetModule(0,dio);
		if(BioFailed(errorCode)) 
		{
			device->Close();
			device = NULL;
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------
	void Destroy()
	{
		if(device != NULL)
		{
			device->Close();
			device = NULL;
		}
	}
	//--------------------------------------------------------------------------
	bool IsOpen()
	{
		return device != NULL;
	}
	//--------------------------------------------------------------------------
	unsigned Read()
	{
		dio->DiRead(startPoint, 2, (BYTE *)&input);
		return input;
	}
	//--------------------------------------------------------------------------
	void Write()
	{
		dio->DoWrite(startPoint, 2, (BYTE *)&output);
	}
	void Write(unsigned out)
	{
		__output = out;
		dio->DoWrite(startPoint, 2, (BYTE *)&output);
	}
	//--------------------------------------------------------------------------
	unsigned ReadOutput()
	{
		unsigned res;
		dio->DoRead(startPoint, 2, (BYTE *)&res);
		return res;
	}
	//----------------------------------------------------------------------------
#else
	//------------------------------------------------------------------------------
	bool Init(int deviceDescription)
	{
		return true;
	}
	//-------------------------------------------------------------------------
	void Destroy()
	{	
	}
	//--------------------------------------------------------------------------
	bool IsOpen()
	{
		return true;
	}
	//--------------------------------------------------------------------------
	unsigned Read()
	{
		return __input = rand() % 0xffff;
	}
	//--------------------------------------------------------------------------
	void Write()
	{
		__output = rand() % 0xffff;
	}
	void Write(unsigned out)
	{
		__output = out;
	}
	//--------------------------------------------------------------------------
	unsigned ReadOutput()
	{
		return rand() % 0xffff;
	}
#endif
}

