#pragma once
#include "Net/CommonTCP.h"

namespace TypeSizeProtocol
{
	class Server: public CommonServer<Server>
	{
	public:
		static bool Do(Frame *);
	};
	class Client: public CommonClient
	{
	public:
		static bool Do(wchar_t *addr, int port, wchar_t *);
	};
}