#include "stdafx.h"
#include "Device1730.h"
#include "tools_debug/DebugMess.h"

#ifndef DEBUG_ITEMS
using namespace Automation::BDaq;

Device1730::Device1730()
	: device(NULL)
	, dio(NULL)
{}
//------------------------------------------------------------------------------
bool Device1730::Init(int deviceDescription)
{
	Destroy();
	long errorCode = BDaqDevice::Open( deviceDescription, ModeWrite, device);
	if(BioFailed(errorCode))
	{
		Mess(DeviceOpenError)
		return isOpen = false;
	}
	errorCode = device->GetModule(0,dio);
	if(BioFailed(errorCode)) 
	{
		Mess(GetModuleError)
		return isOpen = false;
	}
	Mess(AllOk)	
	return isOpen = true;
}
//-------------------------------------------------------------------------
void Device1730::Destroy()
{
	if(device != NULL)
	{
		device->Close();
	}
}
//--------------------------------------------------------------------------
bool Device1730::IsOpen()
{
	return isOpen;
}
//--------------------------------------------------------------------------
unsigned Device1730::Read()
{
	dio->DiRead(startPoint, 2, (BYTE *)&input);
	return input;
}
//--------------------------------------------------------------------------
void Device1730::Write()
{
	dio->DoWrite(startPoint, 2, (BYTE *)&output);
}
void Device1730::Write(unsigned out)
{
	output = out;
	dio->DoWrite(startPoint, 2, (BYTE *)&output);
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{
	unsigned res;
	dio->DoRead(startPoint, 2, (BYTE *)&res);
	return res;
}
//----------------------------------------------------------------------------
#else
Device1730::Device1730()
{}
//------------------------------------------------------------------------------
bool Device1730::Init(int deviceDescription)
{
	return true;
}
//-------------------------------------------------------------------------
void Device1730::Destroy()
{	
}
//--------------------------------------------------------------------------
bool Device1730::IsOpen()
{
	return true;
}
//--------------------------------------------------------------------------
unsigned Device1730::Read()
{
	return input = rand() % 0xffff;
}
//--------------------------------------------------------------------------
void Device1730::Write()
{
	output = rand() % 0xffff;
}
void Device1730::Write(unsigned out)
{
	output = out;
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{
	return rand() % 0xffff;
}
#endif

