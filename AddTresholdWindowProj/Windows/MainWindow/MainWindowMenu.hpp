#pragma once

#include "window_tool\message.h"
#include "window_tool\MenuApi.h"
#include "templates\templates.hpp"
#include "Dialogs\Dialogs.h"
#include "window_tool\WindowsPosition.h"
#include "Dates\SaveLoadDates.h"
#include "Dates\Compute.h"
#include "App\App.h"

namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile__ 
	{
		static void Do(HWND h)
		{
			LoadDateFile::Do(h);
			Singleton<Compute>::Instance().Recalculation();
		}
	};
	struct SaveDateFile__:  SaveDateFile{};//{static void Do(HWND){}};
	struct Compute__     
	{
		static void Do(HWND)
		{
			Singleton<Compute>::Instance().Recalculation();
		}
	};
	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Загрузить данные", LoadDateFile__)
	MENU_ITEM(L"Сохранить данные", SaveDateFile__)
	MENU_ITEM(L"Перерасчёт", Compute__)
	MENU_ITEM(L"Выход", MainExit)

		template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<LoadDateFile__>
			, MenuItem<SaveDateFile__>
			, Separator<0>
			, MenuItem<Compute__>
			, Separator<1>
			, MenuItem<MainExit>
		>::Result list;
	};

	struct MainOptionTypeSize{};
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct MainCreateTypesize: AddTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainDeleteTypeSize: DelTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainClearCounter {static void Do(HWND){App::ClearCounter();}};

	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Обнулить счётчик", MainClearCounter)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
			 MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
			, Separator<1>
			, MenuItem<MainClearCounter>
		>::Result list;
	};

	struct MainOptionUnits {};//static void Do(HWND h){zprint("");}};
	//struct L502ParamDlg {static void Do(HWND h){zprint("");}};
	//struct CommunicationOptions {static void Do(HWND h){zprint("");}};

	struct SolenoidParametersTable: SolenoidParametersTableDlg{};// {static void Do(HWND h){zprint("");}};
	struct NetOptions: TcpCommunicationsDlg{};
	//struct AdcInputPortsDlg {static void Do(HWND h){zprint("");}};
	//struct DInputPortsDlg {static void Do(HWND h){zprint("");}};
	//struct SyncroDlg {static void Do(HWND h){zprint("");}};
	struct SaveWindowPosition: SaveWindowPositionDlg{};//{static void Do(HWND h){zprint("");}};
	struct MessagePanel
	{
		static void Do(HWND h)
		{
			bool x = OpenMessagePanel::Do();
			CheckMenu<MenuItem<MessagePanel>>(h, x);
		}
	};

	MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
	//MENU_ITEM(L"Настройка аналоговой платы"       , L502ParamDlg)	
	//MENU_ITEM(L"Настройка сетевого подключения"   , CommunicationOptions)	
	MENU_ITEM(L"Настройка сетевого подключения", NetOptions)
	MENU_ITEM(L"Настройки генератора"          , SolenoidParametersTable)
	//MENU_ITEM(L"Настройки входов аналоговой платы", AdcInputPortsDlg)
	//MENU_ITEM(L"Настройки входов дискретной платы", DInputPortsDlg)
	//MENU_ITEM(L"Синхронизация", MenuItem<SyncroDlg>)
	MENU_ITEM(L"Сохранить координаты окна", SaveWindowPosition)//WindowPositionDlg<MainWindow>)
	MENU_ITEM(L"Панель сообщений", MessagePanel)



	template<>struct TopMenu<MainOptionUnits>
	{
		typedef TL::MkTlst<
			//			MenuItem<L502ParamDlg>
			//	, MenuItem<CommunicationOptions>	
			MenuItem<NetOptions>
			, MenuItem<SolenoidParametersTable>
			//, MenuItem<AdcInputPortsDlg>
			//, MenuItem<DInputPortsDlg>
			//, MenuItem<SyncroDlg>
			, Separator<1>			
			, MenuItem<SaveWindowPosition>
			, MenuItem<MessagePanel>
		>::Result list;
	};

	template<>struct EnableMenuInit<MenuItem<MessagePanel>>
	{
		int operator()(HWND)
		{
			return InitMessagePanel::Do();
		}
	};

	MENU_TEXT(L"О программе", TopMenu<AboutWindow>)
	MENU_ITEM(L"О программе", AboutWindow)

	template<>struct TopMenu<AboutWindow>
	{
		typedef TL::MkTlst<
			MenuItem<AboutWindow>
		>::Result list;
	};

	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
		, TopMenu<MainOptionUnits>
		, TopMenu<AboutWindow>
	>::Result items_list;	
}