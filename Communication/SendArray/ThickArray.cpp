#include "stdafx.h"
#include "ThickArray.h"
#include "tools_debug\DebugMess.h"
#include "Net\SendArray.hpp"
#include "..\IPprotocolProcedureList.hpp"

double data[300];
double serv[300];

SendArr::SendArr()
	: clientData((char *)data)
	, serverData((char *)serv)
	, size(sizeof(data))
	, sizeItem(sizeof(double))
{
	for(int i = 0; i < 300; ++i)
	{
		serv[i] = 8888888;
		data[i] = 1 + i;
	}
}

void SendArr::Do()
{
	zprint("Ok\n");
	for(int i = start; i < stop; ++i)
	{	
		zprint("%d %f\n", i, serv[i]);
	}
}

int SendArr::Id()
{
	return TL::IndexOf<__ip_protocol_procedure_list__, SendArray::TemplServer<SendArr>>::value;
}

SendArr &SendArr::Instance(){static SendArr x; return x;};
