#pragma once

#include "window_tool\message.h"
#include "window_tool\MenuApi.h"
#include "templates\templates.hpp"
#include "Dialogs\Dialogs.h"
#include "window_tool\WindowsPosition.h"
#include "Dates\SaveLoadDates.h"
#include "Dates\Compute.h"

namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile__ {static void Do(HWND h)
	{
		LoadDateFile::Do(h);
		Singleton<Compute>::Instance().Recalculation();
	}};
	struct SaveDateFile{static void Do(HWND){}};
	struct Compute     {static void Do(HWND){}};
	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Загрузить данные", LoadDateFile__)
	MENU_ITEM(L"Сохранить данные", SaveDateFile)
	MENU_ITEM(L"Перерасчёт", Compute)
	MENU_ITEM(L"Выход", MainExit)

		template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<LoadDateFile__>
			, MenuItem<SaveDateFile>
			, Separator<0>
			, MenuItem<Compute>
			, Separator<1>
			, MenuItem<MainExit>
		>::Result list;
	};

	struct MainOptionTypeSize{};
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct MainCreateTypesize: AddTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainDeleteTypeSize: DelTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainAddItem {static void Do(HWND h){zprint("");}};
	struct MainClearCounter {static void Do(HWND h){zprint("");}};

	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Добавить образец", MainAddItem)
	MENU_ITEM(L"Обнулить счётчик", MainClearCounter)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
			MenuItem<MainAddItem>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
			, Separator<1>
			, MenuItem<MainClearCounter>
		>::Result list;
	};

	struct MainOptionUnits {static void Do(HWND h){zprint("");}};
	struct L502ParamDlg {static void Do(HWND h){zprint("");}};
	struct CommunicationOptions {static void Do(HWND h){zprint("");}};
	struct SolenoidParametersTableDlg {static void Do(HWND h){zprint("");}};
	struct AdcInputPortsDlg {static void Do(HWND h){zprint("");}};
	struct DInputPortsDlg {static void Do(HWND h){zprint("");}};
	struct SyncroDlg {static void Do(HWND h){zprint("");}};

	MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
	MENU_ITEM(L"Настройка аналоговой платы"       , L502ParamDlg)	
	MENU_ITEM(L"Настройка сетевого подключения"   , CommunicationOptions)	
	MENU_ITEM(L"Настройки генератора"             , SolenoidParametersTableDlg)
	MENU_ITEM(L"Настройки входов аналоговой платы", AdcInputPortsDlg)
	MENU_ITEM(L"Настройки входов дискретной платы", DInputPortsDlg)
	MENU_ITEM(L"Синхронизация", MenuItem<SyncroDlg>)
	MENU_ITEM(L"Сохранить координаты окна", WindowPositionDlg<MainWindow>)

	template<>struct TopMenu<MainOptionUnits>
	{
		typedef TL::MkTlst<
			MenuItem<L502ParamDlg>
			, MenuItem<CommunicationOptions>	
			, MenuItem<SolenoidParametersTableDlg>
			, MenuItem<AdcInputPortsDlg>
			, MenuItem<DInputPortsDlg>
			, MenuItem<SyncroDlg>
			, Separator<1>
			, MenuItem<WindowPositionDlg<MainWindow>>
		>::Result list;
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