#include "stdafx.h"
#include "Device1730.h"
#include "DebugMess.h"
#include "ArchiveEvents.h"

#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	

using namespace Automation::BDaq;

#define Mess(n)

Device1730::Device1730()
	: device(NULL)
	, dio(NULL)
	, input(archiveEvents.map->input)
	, output(archiveEvents.map->output)
{}
//------------------------------------------------------------------------------
bool Device1730::Init(wchar_t *deviceDescription)
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

