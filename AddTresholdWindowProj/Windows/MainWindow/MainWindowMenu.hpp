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
			Compute::Recalculation();
		}
	};
	struct SaveDateFile__:  SaveDateFile{};
	struct Compute__     
	{
		static void Do(HWND)
		{
			Compute::Recalculation();
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

	struct CounterShowDlg__
	{
		static void Do(HWND h)
		{
			bool x = CounterShowDlg::Do();
			CheckMenu<MenuItem<CounterShowDlg__>>(h, x);
		}
	};

	MENU_ITEM(L"Счётчик труб", CounterShowDlg__)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
			 MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
			, Separator<1>
			, MenuItem<CounterShowDlg__>
			, Separator<2>
			, MenuItem<MainClearCounter>			
		>::Result list;
	};

	struct MainOptionUnits {};//static void Do(HWND h){zprint("");}};

	struct SolenoidParametersTable: SolenoidParametersTableDlg{};// {static void Do(HWND h){zprint("");}};
	struct NetOptions: TcpCommunicationsDlg{};
	struct Descriptor1730Item : Descriptor1730Dlg{};
	struct InputBit: InputBitDlg{};
	struct SaveWindowPosition: SaveWindowPositionDlg{};//{static void Do(HWND h){zprint("");}};
	struct MessagePanel
	{
		static void Do(HWND h)
		{
			bool x = OpenMessagePanel::Do();
			CheckMenu<MenuItem<MessagePanel>>(h, x);
		}
	};

	struct IOportsDlg__
	{
		static void Do(HWND h)
		{
			bool x = IOportsDlg::Do();
			CheckMenu<MenuItem<IOportsDlg__>>(h, x);
		}
	};

	struct CommunicationIDMenu: CommunicationIDWindow{}; 

	MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
	MENU_ITEM(L"Настройка сетевого подключения", NetOptions)
	MENU_ITEM(L"Настройки генератора"          , SolenoidParametersTable)
	MENU_ITEM(L"Смещения входных дискретных портов", InputBit)
	MENU_ITEM(L"Дескриптор платы 1730"          , Descriptor1730Item)
	MENU_ITEM(L"Сохранить координаты окна", SaveWindowPosition)
	MENU_ITEM(L"Панель сообщений", MessagePanel)
	MENU_ITEM(L"Просмотр дискретных входов", IOportsDlg__)	

	MENU_ITEM(L"Идентификатор типоразмера для передачи по для сети", CommunicationIDMenu)

	struct Descriptor1730__{};
	
	template<>struct SubMenu<Descriptor1730__>
	{
		typedef TL::MkTlst<
			MenuItem<InputBit>
			, Separator<1>
			, MenuItem<Descriptor1730Item>
		>::Result list;
	};
	MENU_TEXT(L"Плата 1730" , SubMenu<Descriptor1730__>)

	template<>struct TopMenu<MainOptionUnits>
	{
		typedef TL::MkTlst<
			MenuItem<NetOptions>
			, MenuItem<SolenoidParametersTable>
			, SubMenu<Descriptor1730__>
			, MenuItem<CommunicationIDMenu>
			, Separator<1>			
			, MenuItem<SaveWindowPosition>
			, MenuItem<MessagePanel>
			, MenuItem<IOportsDlg__>			
		>::Result list;
	};

	template<>struct EnableMenuInit<MenuItem<MessagePanel>>
	{
		int operator()(HWND)
		{
			return MessagePanel_Init();
		}
	};

	template<>struct EnableMenuInit<MenuItem<IOportsDlg__>>
	{
		int operator()(HWND)
		{
			return IOportsDlg_Init();
		}
	};

	template<>struct EnableMenuInit<MenuItem<CounterShowDlg__>>
	{
		int operator()(HWND)
		{
			return MFS_UNCHECKED;
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