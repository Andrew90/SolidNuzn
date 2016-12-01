#include "stdafx.h"
#include "Automat/Automat.h"
#include "App/AppBase.h"
#include "CommunicationTube\ClientTreshold.h"
#include "App/config.h"
#include "L502\L502SolidGroup.h"
#include "Dates\SolidData.h"
#include "Dates\Compute.h"
#include "Dates\CounterTubes.h"
#include "App/AppBase.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "Dates\SaveLoadDates.h"
#include "Dates\StoreResultBase.h"

namespace Automat
{
	HANDLE hStart = NULL;
	HANDLE hThread = NULL;
	bool &communicationRemoveUnit = Singleton<DifferentOptionsTable>::Instance().items.get<CommunicationRemoveUnit>().value;
	bool &tubesStored = Singleton<DifferentOptionsTable>::Instance().items.get<TubesStored>().value;
	bool &paintMarker = Singleton<DifferentOptionsTable>::Instance().items.get<PaintMarker>().value;
	TcpCommunications &tcpCommunications = Singleton<TcpCommunications>::Instance(); 
	ClientTreshold &clientTreshold = Singleton<ClientTreshold>::Instance();
	L502SolidGroup &l502SolidGroup = Singleton<L502SolidGroup>::Instance();
	SolidData &solidData = Singleton<SolidData>::Instance();
	ComputeSolidGroup &computeSolidGroup = Singleton<ComputeSolidGroup>::Instance();
	const unsigned &tubeInUnit = Singleton<InputBitTable>::Instance().items.get<TubeInUnit>().value;
	const unsigned &unitOn = Singleton<InputBitTable>::Instance().items.get<UnitOn>().value;

	struct ResetException{};
	
	const wchar_t *workStationDontWork =  L"<ff0000>Не могу подключится к рабочей станции";

	void Start()
	{
		SetEvent(hStart);
	}

	void Stop()
	{
	   App::PrintTopLabel(L"<ff0000>Оператор вышел из цикла измерений");
	   throw ResetException();
	}

	void Init()
	{
		hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	DWORD WINAPI  __Run__(LPVOID)
	{
		bool initTcp = true;
		while(true)
		{
			try
			{
				WaitForSingleObject(hStart, INFINITE);

				if(initTcp && communicationRemoveUnit)
				{
					initTcp = false;
					if(!clientTreshold.Connect(
						tcpCommunications.items.get<PortTCP>().value
						,tcpCommunications.items.get<AddresTCP>().value
						))
					{
						MessageBox(0, &workStationDontWork[8], L"Ошибка", MB_ICONERROR);
						App::PrintTopLabel((wchar_t *)workStationDontWork);
						ResetEvent(hStart);
						initTcp = true;
						continue;
					}
				}

				App::PrintTopLabel(L"<ff>Ожидание трубы");

				unsigned input;
				///< Ожидание бита "Труба в модуле"
				do	 
				{
					Sleep(5);
					input = Device1730::Read();
					///< Если бит "Устоновка включена" = 0 то выход из цикла
					if(!(unitOn & input))
					{
						App::PrintTopLabel(L"<ff0000>Установка отключена");
						throw ResetException();
					}
				}
				while(0 == (tubeInUnit & input));

				CommunicationTCP::Result = 0;
				if(paintMarker)
				{
					Device1730::Write(0);
				}

				App::PrintTopLabel(L"<ff00>Сбор данных");

				l502SolidGroup.Start();
				solidData.Clear();

				int numberCycles = 0;
				double data[8 * 1024];
				///< Цикл сбора данных
				do
				{
					Sleep(10);
					input = Device1730::Read();
					if(!(unitOn & input))
					{
						App::PrintTopLabel(L"<ff0000>Установка отключена");
						throw ResetException();
					}
					unsigned first = 0;
					
					unsigned res = dimention_of(data);
					l502SolidGroup.Read(first, data, res);
					solidData.SetData(&data[first], res - first);
					if(++numberCycles > 500)
					{
						numberCycles = 0;
						///< обновление экрана через ~5 сек
						App::UpdateMainWindow();
					}
				}
				while(tubeInUnit & input);

				l502SolidGroup.Stop();

				double result; 
				wchar_t *groupName = NULL; 
				unsigned color;
				Compute::Recalculation(result, groupName, color);

				if(tubesStored)
				{
					wchar_t path[1024];
					computeSolidGroup.currentFile = CreateNameFile(
						L"..\\Stored"
						, (wchar_t *)computeSolidGroup.typeSizeName.c_str()
						, groupName
						, path
						);
					StoreDataFile(path);
				}
				else
				{
					computeSolidGroup.currentFile = L"";
				}

				wchar_t buf[1024];

				wsprintf(buf, L"<ff00>%s<ff>Группа прочности <%6x>%s <ff>корреляция <ffffff>%s"
					, computeSolidGroup.currentFile.c_str()
					, color
					, groupName
					, Wchar_from<double>(result)()
					);

				unsigned communicationID = computeSolidGroup.communicationIDItems.GetID(groupName);
				if(-1 == communicationID)
				{
					CommunicationTCP::Result = communicationID;
					if(paintMarker && communicationID >= 10 && communicationID <= 15)
					{
						Device1730::Write( 1 << communicationID);
					}
				}

				App::UpdateMainWindow();

				App::UpdateGroupCounter();

				StoreResultBase(
					CounterTubes::CountTypeAll()
					, (wchar_t *)computeSolidGroup.currentGroupName.c_str()
					, groupName
					);

			}
			catch(ResetException)
			{
				ResetEvent(hStart);
				l502SolidGroup.Stop();
				if(!initTcp && communicationRemoveUnit)
				{
					clientTreshold.Close();
				}
				initTcp = true;
			}
		}
		return 0;
	}

	void Run()
	{
		if(NULL == hThread) hThread = CreateThread(NULL, 0, __Run__, NULL, 0, NULL);
	}

	bool Suspend()
	{
		if(NULL != hThread)
		{
			SuspendThread(hThread);
			return true;
		}
		return false;
	}

	bool Resume()
	{
		if(NULL != hThread) 
		{
			while(ResumeThread(hThread));
			return true;
		}
		return false;
	}

	bool Init1730()
	{
		if(!Device1730::Init(Singleton<DifferentOptionsTable>::Instance().items.get<DeviceDescription1730>().value))
		{
			const wchar_t *mess = L"<ff0000>Не могу инициализировать плату 1730";
			MessageBox(0, &mess[8], L"Ошибка !!!", MB_ICONERROR);
			App::PrintTopLabel((wchar_t *)mess);
			return false;
		}
		return true;
	}
};