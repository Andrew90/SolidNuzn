#include "stdafx.h"
#include "Automat/Automat.h"
#include "App/AppBase.h"
#include "CommunicationTube\ClientTreshold.h"
#include "App/config.h"
#include "L502\L502SolidGroup.h"
#include "Dates\SolidData.h"
#include "Dates\Compute.h"
#include "Dates\CounterTubes.h"

namespace Automat
{
	HANDLE hStart = NULL;
	HANDLE hThread = NULL;
	bool &communicationRemoveUnit = Singleton<DifferentOptionsTable>::Instance().items.get<CommunicationRemoveUnit>().value;
	TcpCommunications &tcpCommunications = Singleton<TcpCommunications>::Instance(); 
	ClientTreshold &clientTreshold = Singleton<ClientTreshold>::Instance();
	Device1730 &device1730 = Singleton<Device1730>::Instance();
	L502SolidGroup &l502SolidGroup = Singleton<L502SolidGroup>::Instance();
	SolidData &solidData = Singleton<SolidData>::Instance();
	//Compute &compute = Singleton<Compute>::Instance();

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
																  
	//void WaitBit(bool b)
	//{
	//	unsigned input;
	//	do
	//	{
	//		Sleep(5);
	//		input = device1730.Read();
	//		if(!(unitOn & input))
	//		{
	//			App::PrintTopLabel(L"<ff0000>Установка отключена");
	//			throw ResetException();
	//		}
	//	}
	//	while(b == (0 != (tubeInUnit & input)));
	//}

	void Init()
	{
		hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	DWORD WINAPI  __Run__(LPVOID)
	{
		while(true)
		{
			try
			{
				WaitForSingleObject(hStart, INFINITE);

				if(communicationRemoveUnit)
				{
					if(!clientTreshold.Connect(
						tcpCommunications.items.get<PortTCP>().value
						,tcpCommunications.items.get<AddresTCP>().value
						))
					{
						MessageBox(0, &workStationDontWork[8], L"Ошибка", MB_ICONERROR);
						App::PrintTopLabel((wchar_t *)workStationDontWork);
						ResetEvent(hStart);
						continue;
					}
				}

				App::PrintTopLabel(L"<ff>Ожидание трубы");

				unsigned input;
				///< Ожидание бита "Труба в модуле"
				do	 
				{
					Sleep(5);
					input = device1730.Read();
					///< Если бит "Устоновка включена" = 0 то выход из цикла
					if(!(unitOn & input))
					{
						App::PrintTopLabel(L"<ff0000>Установка отключена");
						throw ResetException();
					}
				}
				while(0 == (tubeInUnit & input));

				App::PrintTopLabel(L"<ff00>Сбор данных");

				l502SolidGroup.Start();
				solidData.Clear();

				int numberCycles = 0;
				double data[8 * 1024];
				///< Цикл сбора данных
				do
				{
					Sleep(10);
					input = device1730.Read();
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

				//compute.Recalculation();
				Compute::Recalculation();
				App::UpdateGroupCounter();

			}
			catch(ResetException)
			{
				ResetEvent(hStart);
				l502SolidGroup.Stop();
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
		if(!device1730.Init(Singleton<DifferentOptionsTable>::Instance().items.get<DeviceDescription1730>().value))
		{
			const wchar_t *mess = L"<ff0000>Не могу инициализировать плату 1730";
			MessageBox(0, &mess[8], L"Ошибка !!!", MB_ICONERROR);
			App::PrintTopLabel((wchar_t *)mess);
			return false;
		}
		return true;
	}
};