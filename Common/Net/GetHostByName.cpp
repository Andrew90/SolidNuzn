#include "stdafx.h"
#include "GetHostByName.h"
#include "tools_debug/DebugMess.h"

wchar_t *GetHostByName::operator()(wchar_t *name)
{
	char host_name[128];
	size_t count;
	wcstombs_s(&count, host_name, name, sizeof(host_name));
	struct hostent *remoteHost  = gethostbyname(host_name);	
	in_addr  addr;
	addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
	wsprintf(buf, L"%d.%d.%d.%d"
		, addr.S_un.S_un_b.s_b1
		, addr.S_un.S_un_b.s_b2
		, addr.S_un.S_un_b.s_b3
		, addr.S_un.S_un_b.s_b4
		);
	return buf;
} 