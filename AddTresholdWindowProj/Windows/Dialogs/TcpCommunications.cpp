#include "stdafx.h"
#include "Dialogs.h"
#include "App/AppBase.h"
#include "DlgTemplates\ParamDlg.h"

PARAM_TITLE(PortTCP, L"Порт сетевого подключения")
PARAM_TITLE(AddresTCP, L"Адрес сетевого подключения")

MIN_VALUE(PortTCP, 1024)
MAX_VALUE(PortTCP, 0XFFFF)

DO_NOT_CHECK(AddresTCP)
template<>struct DlgSubItems<AddresTCP, AddresTCP::type_value>: EditItems<AddresTCP>{};

void TcpCommunicationsDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, TcpCommunications>(Singleton<TcpCommunications>::Instance()).Do(h, L"Настройка сетевого подключения"))
	{
		/*
		if(!clientTreshold.Connect(
			Singleton<TcpCommunications>::Instance().items.get<PortTCP>().value
			, Singleton<TcpCommunications>::Instance().items.get<AddresTCP>().value
			))
		{
			MessageBox(0, L"Не могу подключится к рабочей станции", L"Ошибка !!!", MB_ICONERROR);
		}
		*/
	}
}

