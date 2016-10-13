#include "stdafx.h"
#include "TypeSizeProtocol.h"
#include "Net/Frame.h"
#include "Net/NetFrame.h"
#include "Net/NetClient.h"
#include "..\IPprotocolProcedureList.hpp"
#include "templates/templates.hpp"

#include "tools_debug/DebugMess.h"

namespace TypeSizeProtocol
{
	bool Server::Do(Frame *f)
	{
		wchar_t *s = (wchar_t *)f->data.buf;
		
		f->length = 0;
		f->proc = Stop;
		return true;
	}
}

namespace 
{
	struct Data
	{
		wchar_t Ok[4];
		double tresholdKlass1;
		double tresholdKlass2;
	};
	struct XXData
	{
		bool ok;
		wchar_t *typeSize;
		void Test(Data *s)
		{
			ok = 0 == wcscmp(L"Ok", s->Ok);
			wchar_t buf[512];
			if(ok)
			{
				wsprintf(buf, L"<ff00>Выбран типоразмер <ff>%s"
					, typeSize
					);
				//wsprintf(buf, L"\"1 класс %s\" \"2 класс %s\""
				//	, Wchar_from<double, 3>(s.tresholdKlass1)()
				//	, Wchar_from<double, 3>(s.tresholdKlass2)()
				//	);
			//	dprint("###@@@  thresh  1 klass %f  2 klass %f  ++++++\n", r.tresholdKlass1, r.tresholdKlass2);
			}
			else
			{
				wsprintf(buf, L"<ff0000>Типоразмер отсутствует <ff>%s", typeSize);
				dprint("###@@@  nhtshold EEERRRRRR XXXXXXXX +++++\n");
			}
		}
	} xxData;
}

namespace TypeSizeProtocol
{
		struct Stored: public CommonClient
		{
			XXData &item;
			Frame *f;
			Stored(XXData &item, Frame *f, wchar_t *data)
				: item(item)
				, f(f)
			{
				item.ok = false;
				f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, TypeSizeProtocol::Server>::value;
				int len = 1 + wcslen(data);
				wcscpy_s((wchar_t *)f->data.buf, len, data);
				f->length = 2 * len;
				f->proc = Recv<Stored>;
			}

			bool Do()
			{
				item.Test((Data *)f->data.buf);
				f->proc = Stop;
				return true;
			}

			//static void *operator new(size_t size, void *ptr)
			//{
			//	return ptr;
			//}
			//static void operator delete(void *, void *){}
		};		
	bool Client::Do(wchar_t *addr, int port, wchar_t *data)
	{
		Frame *f = Frame::Get();
		xxData.typeSize = data;
		Stored *stored = new(Frame::AdditionalData<Stored>(f)) Stored(xxData, f, data);
		NetClient::Connect(addr, port, f);
		Sleep(1000);
		return stored->item.ok;
	}

}