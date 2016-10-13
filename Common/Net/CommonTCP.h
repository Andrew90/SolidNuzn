#pragma once

#define str4(_0,_1,_2,_3) _0 | (_1 << 8) | (_2 << 16) | (_3 << 24)
struct CommonTCP
{
   static const int OK = str4('O','K',0,0);
   static const int ERR = str4('E','R','R',0);
};
#undef str4
void CALLBACK Stop(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
class CommonClient
{
protected:
	template<class T>static void CALLBACK Send(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if (0 == dwError && 0 != dwTransferred)
		{
			T *t = Frame::AdditionalData<T>(f);
			if(t->Do())
			{
				NetLink::Send(f);
				return;
			}
		}
		closesocket(f->socket);
		Frame::Drop(f);
	}
	template<class T>static void CALLBACK Recv(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if (0 == dwError && 0 != dwTransferred)
		{
			f->proc = Send<T>;
			NetLink::Recv(f);
		}
		else
		{
			closesocket(f->socket);
			Frame::Drop(f);
		}
	}
public:
	static void *operator new(size_t size, void *ptr)
	{
		return ptr;
	}
	static void operator delete(void *, void *){}
};

struct Frame;
template<class T>class CommonServer
{
public:
	static void CALLBACK RecvCallBack(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if(0 == dwError &&  0 != dwTransferred && T::Do(f))
		{
			if(NULL == f->proc) f->proc = Send;
			NetLink::Send(f);
		}
		else
		{
			closesocket(f->socket);
			Frame::Drop(f);
		}
	}
	static void CALLBACK Send(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if (0 == dwError && 0 != dwTransferred)
		{
			f->proc = RecvCallBack;
			NetLink::Recv(f);
			return;
		}
		closesocket(f->socket);
		Frame::Drop(f);
	}
};