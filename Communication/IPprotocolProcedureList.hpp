#pragma once 

#include "templates/typelist.hpp"
#include "TypeSize/TypeSizeProtocol.h"
#include "Thickness/ThicknessProtocol.h"
#include "Strobes/StrobesProtocol.h"
#include "StopCycle\StopCycleProtocol.h"
#include "Net/SendArray.hpp"
#include "SendArray/ThickArray.h"


typedef TL::MkTlst<
		 ThicknessProtocol::Server
		 , TypeSizeProtocol::Server
		 , StrobesProtocol::Server
		 , StopCycleProtocol::Server
		 , SendArray::TemplServer<SendArr>
	 >::Result __ip_protocol_procedure_list__;