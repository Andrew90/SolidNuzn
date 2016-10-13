#include "stdafx.h"
#include "StopCycleProtocol.h"
#include "Net/Frame.h"
#include "Net/NetFrame.h"
#include "tools_debug/DebugMess.h"

#include "Net/NetClient.h"
#include "..\IPprotocolProcedureList.hpp"
#include "templates/typelist.hpp"
#include "tools_debug/DebugMess.h"

namespace StopCycleProtocol
{
	bool Server::Do(Frame *f)
	{
		wchar_t *s = (wchar_t *)f->data.buf;
		
		f->data.id =  0 == wcscmp(L"Stop", s)
			? CommonTCP::OK
			: CommonTCP::ERR
			;

		dprint("receive %s\n", (char *)&f->data.id);
		
		//int len = 1 + wcslen(ret);
		//wcscpy_s((wchar_t *)f->data.buf, len, ret);
		f->length = 0;//2 * len;
		
		f->proc = Stop;
		return true;
	}
//----------------------------------------------------------------------------------------------
namespace Protected
{
	struct Stored: public CommonClient
	{
		Frame *f;
		Stored(Frame *f)
			: f(f)
		{
			f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, Server>::value;
			wchar_t *data = L"Stop";
			int len = 1 + wcslen(data);
			wcscpy_s((wchar_t *)f->data.buf, len, data);
			f->length = 2 * len;
			f->proc = Recv<Stored>;
		}

		bool Do()
		{
			//if(0 == wcscmp(L"Ok", (wchar_t *)f->data.buf))
			if(CommonTCP::OK == f->data.id)
			{
				dprint("Stoped\n");
			}

			f->proc = Stop;
			return true;
		}
	};
}
	void Client::Do(wchar_t *addr, int port)
	{
		Frame *f = Frame::Get();
		new(Frame::AdditionalData<Protected::Stored>(f)) Protected::Stored(f);
		NetClient::Connect(addr, port, f);
	}

}