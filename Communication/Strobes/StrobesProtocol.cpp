#include "stdafx.h"
#include "StrobesProtocol.h"
#include "Net/Frame.h"
#include "Net/NetFrame.h"
#include "tools_debug/DebugMess.h"

#include "templates/typelist.hpp"
#include "Net/NetClient.h"
#include "Net/CommonTCP.h"
#include "..\IPprotocolProcedureList.hpp"

namespace StrobesProtocol
{
	bool Server::Do(Frame *f)
	{
		wchar_t *s = (wchar_t *)f->data.buf;
		
		wchar_t *ret =  0 == wcscmp(L"1234567890", s)
			? L"Ok"
			: L"Err"
			;

		dprint("receive %S  %S\n", s, ret);
		
		int len = 1 + wcslen(ret);
		wcscpy_s((wchar_t *)f->data.buf, len, ret);		
		f->length = 2 * len;
		f->proc = Stop;
		return true;
	}
}

struct StrobesData
{
	int count;
	unsigned times[60];
};

namespace StrobesProtocol
{
	struct Stored: public CommonClient
	{
		Frame *f;
		Stored(Frame *f)
			: f(f)
		{						
			dprint("count %%\n");
			f->length = sizeof(StrobesData);
			f->proc = Recv<Stored>;
		}

		bool Do()
		{
			if(0 == wcscmp(L"Ok", (wchar_t *)f->data.buf))
			{
#pragma message("ƒанные стробов переданы дописать подтверждение")
				zprint("Data Ok\n");
			}
			return false;
		}

		//static void *operator new(size_t size, void *ptr)
		//{
		//	return ptr;
		//}
		//static void operator delete(void *, void *){}
	};		
	void Client::Do(wchar_t *addr, int port)
	{
		Frame *f = Frame::Get();

		new(Frame::AdditionalData<Stored>(f)) Stored(f);
		NetClient::Connect(addr, port, f);
	}

}