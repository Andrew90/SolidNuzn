#pragma once
#include "Net/CommonTCP.h"
#include "tools_debug\DebugMess.h"
#include "Net/Frame.h"
#include "Net\NetClient.h"

namespace SendArray
{
	enum Status { eOk, eNew, eNext, eLast, eErr };
	struct Receive
	{
		int from, count;
		Status status;
	};
	struct Response
	{
		int from, count;
		Status status;
		int Next;
	};
	template<class T>class TemplServer: public CommonServer<TemplServer<T>>
	{
	public:
		static bool Do(Frame *);
	};
	template<class T>struct TemplClient
	{
		static void Do(wchar_t *addr, int port, int from, int to);
	};
	//-------------------------------------------------------------------------------------
	template<class T>bool TemplServer<T>::Do(Frame *f)
	{
		T &item = T::Instance();

		Receive *recvData = (Receive *)&f->data.buf;

		int offs = recvData->from;
		int count = recvData->count;

		char *dta0 = (char *)recvData + sizeof(Receive);
		char *dta1 = &((char *)item.serverData)[offs];

		if(offs + count < item.size)
		{
			memmove(dta1, dta0, count);
		}

		if(eLast == recvData->status)
		{
			item.Do();
			return false;
		}
		else
		{
			Response *r = (Response *)&f->data.buf;
			r->status = eOk;
			r->Next = offs + count;
			f->length = sizeof(Response);
			dprint("server %d\n", r->Next);
		}
		return true;
	}
	//----------------------------------------------------------------------------------------
	template<class T>struct Stored: public CommonClient
	{
		int to;
		Frame *f;
		Stored(Frame *f, int from_, int to_)
			: f(f)
		{
			T &item = T::Instance();
			item.start = from_;
		    item.stop = to_;
			from_ *= item.sizeItem;
			to_ *= item.sizeItem;
			to = to_;
			if(to > item.size) to = item.size;
			Response *res = (Response *)f->data.buf;
			res->count = (sizeof(f->data.buf) - sizeof(Receive));
			if(res->count > to_) res->count = to_;
			res->Next = from_;
			f->data.id = item.Id();
		}

		bool Do()
		{
			Response res = *(Response *)f->data.buf;
			Receive *r = (Receive *)f->data.buf;
			if(res.Next + res.count < to)
			{
				r->from = res.Next;
				r->count = res.count;
				f->proc = Recv<Stored>;
				r->status = eOk;
			}
			else if(res.Next < to)
			{
				r->from = res.Next;
				r->count = to - res.Next; 
				f->proc = Stop;
				r->status = eLast;
			}
			else
			{
				return false;
			}
			T &item = T::Instance();

			f->length = r->count + sizeof(Receive);
			memmove(f->data.buf + sizeof(Receive), &item.clientData[r->from], r->count);

			dprint("%d    %d\n", res.Next + res.count, to);
			return true;
		}
	};

	template<class T>void TemplClient<T>::Do(wchar_t *addr, int port, int from, int to)
	{
		Frame *f = Frame::Get();
		
		Stored<T> *s = new(Frame::AdditionalData<Stored<T>>(f)) Stored<T>(f, from, to);
		if(s->Do())
		{
			dprint("start client \n");
			NetClient::Connect(addr, port, f);
		}
		else
		{
			Frame::Drop(&f);
		}
	}
}