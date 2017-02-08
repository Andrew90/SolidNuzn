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
#include "tools_debug/DebugMess.h"
#include "App\AppKeyHandler.h"
#include "ColorPanel\ColorPanel.h"
#include "App\ChangeStandard.h"

namespace Automat
{
	HANDLE hStart = NULL;
	HANDLE hStop = NULL;
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
	
	const wchar_t *workStationDontWork =  L"<ff0000>�� ���� ����������� � ������� �������";

	const wchar_t *messWait = L"<ff>�������� �����";

	void Start()
	{
		App::PrintTopLabel((wchar_t *)messWait);
		SetEvent(hStart);
	}

	void Stop()
	{
		ResetEvent(hStart);
		App::PrintTopLabel(L"<ff0000>�������� ����� �� ����� ���������");
		SetEvent(hStop);
		AppKeyHandler::Stop();	   
	}

	void Init()
	{
		hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
		hStop = CreateEvent(NULL, FALSE, FALSE, NULL);	
		CommunicationTCP::Init();
	}

	DWORD WINAPI  __Run__(LPVOID)
	{
		bool initTcp = true;
		wchar_t buf[1024];		
		wchar_t *mess = (wchar_t *)messWait;
		HANDLE hEvents[] = {hStart, hStop};		
		while(true)
		{
START:
			try
			{
				if(1 + WAIT_OBJECT_0 == WaitForMultipleObjects(2, hEvents, FALSE, INFINITE))
				{
					continue;
				}

				if(initTcp && communicationRemoveUnit)
				{
					initTcp = false;
					if(!clientTreshold.Connect(
						tcpCommunications.items.get<PortTCP>().value
						,tcpCommunications.items.get<AddresTCP>().value
						))
					{
						MessageBox(0, &workStationDontWork[8], L"������", MB_ICONERROR);
						App::PrintTopLabel((wchar_t *)workStationDontWork);
						ResetEvent(hStart);
						initTcp = true;
						continue;
					}
				}

				App::PrintTopLabel(mess);

				unsigned input;
				///< �������� ���� "����� � ������"
				do	 
				{
					if(WAIT_OBJECT_0 == WaitForSingleObject(hStop, 10))
					{
						goto START;
					}
					input = Device1730::Read();
					///< ���� ��� "��������� ��������" = 0 �� ����� �� �����
					if(!(unitOn & input))
					{
						App::PrintTopLabel(L"<ff0000>��������� ���������");
						throw ResetException();
					}
				}
				while(0 == (tubeInUnit & input));

				if(paintMarker)
				{
					Device1730::Write(0);
				}

				App::PrintTopLabel(L"<ff00>���� ������");

				l502SolidGroup.Start();
				solidData.Clear();

				int numberCycles = 0;
				double data[8 * 1024];
				///< ���� ����� ������
				unsigned tme = GetTickCount();
				do
				{
					if(WAIT_OBJECT_0 == WaitForSingleObject(hStop, 10))
					{
						throw ResetException();
					}
					input = Device1730::Read();
					if(!(unitOn & input))
					{
						App::PrintTopLabel(L"<ff0000>��������� ���������");
						throw ResetException();
					}
					unsigned first = 0;
					
					unsigned res = dimention_of(data);
					l502SolidGroup.Read(first, data, res);
					solidData.SetData(&data[first], res - first);
					App::UpdateLoopMainWindow(5000);
				}
				while(tubeInUnit & input);

				l502SolidGroup.Stop();

				double result = 0; 
				wchar_t *groupName = L""; 
				unsigned color;
				Compute::Recalculation(result, groupName, color);
				computeSolidGroup.currentGroupName = groupName;
				App::UpdateGroupCounter();
				

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

				mess = buf;

				wsprintf(buf, L"<ff00>%s<ff>������ ��������� <%6x>%s <ff>���������� <ffffff>%s"
					, computeSolidGroup.currentFile.c_str()
					, color
					, groupName
					, Wchar_from<double>(result)()
					);

				unsigned communicationID = computeSolidGroup.communicationIDItems.GetID(groupName);
				if(-1 != communicationID)
				{
					if(paintMarker && communicationID >= 10 && communicationID <= 15)
					{
						Device1730::Write( 1 << communicationID);
					}
				}
				CommunicationTCP::SetResult(communicationID);
				App::UpdateMainWindow();				

				StoreResultBase(
					CounterTubes::CountTypeAll()
					, (wchar_t *)computeSolidGroup.currentGroupName.c_str()
					, groupName
					);
				ColorPanel::SetText(groupName, color);

				ChangeStandard(0 == result);

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
			const wchar_t *mess = L"<ff0000>�� ���� ���������������� ����� 1730";
			MessageBox(0, &mess[8], L"������ !!!", MB_ICONERROR);
			App::PrintTopLabel((wchar_t *)mess);
			return false;
		}
		return true;
	}
};