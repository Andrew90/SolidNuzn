#include "stdafx.h"
#include "resource.h"
#include "AddThresholdsWindow\ThresholdDataTable.h"
#include <strsafe.h>
#include <CommCtrl.h>
#include "templates\templates.hpp"
#include "window_tool\Grid.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"

namespace
{
	 ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	
	struct Number  
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{
			wchar_t s[32];
			wsprintf(s, L"%3.d", 1 + row);
			StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
			return L"";
		}
	};
	struct Color   
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{
			return L"";
		}
	};
	
	struct Selected
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{
			d->item.mask = LVIF_IMAGE;
			d->item.iImage = solidGroup.solidItems[row].status;
			return L"";
		}
	};
	struct SolidG
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{
			return (wchar_t *)solidGroup.groupNameList[solidGroup.solidItems[row].groupName].name.c_str();
		}
	};
	struct Corel   
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{			
			Wchar_from<double, 5> s(solidGroup.solidItems[row].corelation);
			StringCchCopy(d->item.pszText, wcslen(s()) + 1, s());
			return L"";
		}
	};
	template<int N>struct __Point__
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{
			Wchar_from<double, 5> s(solidGroup.solidItems[row].points[N]);
			StringCchCopy(d->item.pszText, wcslen(s()) + 1, s());
			return L"";
		}
	};
	template<int>struct Point;
	template<>struct Point<0>:__Point__<0>{};
	template<>struct Point<1>:__Point__<1>{};
	template<>struct Point<2>:__Point__<2>{};
	template<>struct Point<3>:__Point__<3>{};
	template<>struct Point<4>:__Point__<4>{};
	template<>struct Point<5>:__Point__<5>{};
	template<>struct Point<6>:__Point__<6>{};
	template<>struct Point<7>:__Point__<7>{};

	struct Offset
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{
			wchar_t s[32];
			wsprintf(s, L"%10.d", solidGroup.solidItems[row].offset);
			StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
			return L"";
		}
	};
	struct File
	{
		static wchar_t *Do(int row, int col, LV_DISPINFO *d)
		{
			return (wchar_t *)solidGroup.fileNameList[solidGroup.solidItems[row].solidFile].name.c_str();
		}
	};

	HEADER_TABLE(Number  , 25, L"№");
	HEADER_TABLE(Color   , 30, L"");
	HEADER_TABLE(Selected, 20, L"");
	HEADER_TABLE(SolidG, 60, L"Гр.пр.");
	HEADER_TABLE(Corel   , 85, L"Кореляция");
	HEADER_TABLE(Point<0>, 85, L"Точка1");
	HEADER_TABLE(Point<1>, 85, L"Точка2");
	HEADER_TABLE(Point<2>, 85, L"Точка3");
	HEADER_TABLE(Point<3>, 85, L"Точка4");
	HEADER_TABLE(Point<4>, 85, L"Точка5");
	HEADER_TABLE(Point<5>, 85, L"Точка6");
	HEADER_TABLE(Point<6>, 85, L"Точка7");
	HEADER_TABLE(Point<7>, 85, L"Точка8");

	HEADER_TABLE(Offset, 80, L"Смещение");
	HEADER_TABLE(File, 200, L"Файл");

	typedef TL::MkTlst<
		Selected
		, Number  
		, Color 
		, SolidG
		, Corel   
		, Point<0>
		, Point<1>
		, Point<2>
		, Point<3>
		, Point<4>
		, Point<5>
		, Point<6>
		, Point<7>
		, Offset
		, File
	>::Result __header_list__;

    wchar_t *(*proc[TL::Length<__header_list__>::value])(int, int, LV_DISPINFO *);
  	template<class List>struct __cell__;
	template<class Head, class Tail>struct __cell__<Tlst<Head, Tail>>: __cell__<Tail>
	{
		__cell__(): __cell__<Tail>()
		{
			proc[TL::IndexOf<__header_list__, Head>::value]	= Head::Do;
		}
	};
	template<>struct __cell__<NullType>{};

	__cell__<__header_list__> cell;
}

void TresholtDataTable::Header(HWND h)
{
	SetGridHeader<__header_list__> _(h);
	HIMAGELIST	himlSmall = ImageList_Create(
		GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),
		ILC_MASK, 4, 1);

	HICON hIcon = LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Ok));
	ImageList_AddIcon(himlSmall, hIcon);

	hIcon = LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Exclam));
	ImageList_AddIcon(himlSmall, hIcon);

	hIcon = LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Cancel));
	ImageList_AddIcon(himlSmall, hIcon);
	
	hIcon = LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Add));
	ImageList_AddIcon(himlSmall, hIcon);

	ListView_SetImageList(h, himlSmall, LVSIL_SMALL);

	SetRow(h, Count());

}
unsigned TresholtDataTable::Count()
{
	return solidGroup.solidItems.size();
}
const wchar_t *TresholtDataTable::Cell(int row, int col, LV_DISPINFO *d)
{
	return proc[col](row, col, d);
}