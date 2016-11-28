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
	MENU_TEXT(L"����", TopMenu<MainFile>)
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

	MENU_ITEM(L"��������� ������", LoadDateFile__)
	MENU_ITEM(L"��������� ������", SaveDateFile__)
	MENU_ITEM(L"����������", Compute__)
	MENU_ITEM(L"�����", MainExit)

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
	MENU_TEXT(L"����������", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct MainCreateTypesize: AddTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainDeleteTypeSize: DelTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainClearCounter {static void Do(HWND){App::ClearCounter();}};

	MENU_ITEM(L"������� ����������", MainCreateTypesize)
	MENU_ITEM(L"������� ����������", MainDeleteTypeSize)
	MENU_ITEM(L"�������� �������", MainClearCounter)

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

	MENU_TEXT(L"���������", TopMenu<MainOptionUnits>)
	MENU_ITEM(L"��������� �������� �����������", NetOptions)
	MENU_ITEM(L"��������� ����������"          , SolenoidParametersTable)
	MENU_ITEM(L"�������� ������� ���������� ������", InputBit)
	MENU_ITEM(L"���������� ����� 1730"          , Descriptor1730Item)
	MENU_ITEM(L"��������� ���������� ����", SaveWindowPosition)
	MENU_ITEM(L"������ ���������", MessagePanel)
	MENU_ITEM(L"�������� ���������� ������", IOportsDlg__)

	struct Descriptor1730__{};
	template<>struct SubMenu<Descriptor1730__>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<InputBit, Descriptor1730Item>::Result
			, MenuItem
		>::Result list;
	};
	MENU_TEXT(L"����� 1730" , SubMenu<Descriptor1730__>)

	template<>struct TopMenu<MainOptionUnits>
	{
		typedef TL::MkTlst<
			MenuItem<NetOptions>
			, MenuItem<SolenoidParametersTable>
		//	, MenuItem<InputBit>
			, SubMenu<Descriptor1730__>
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

	MENU_TEXT(L"� ���������", TopMenu<AboutWindow>)
	MENU_ITEM(L"� ���������", AboutWindow)

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