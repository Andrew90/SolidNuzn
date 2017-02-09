#include "stdafx.h"
#include "GridWindow.h"
#include "resource.h"
#include <Windows.h>
#include "templates/templates.hpp"
#include <strsafe.h>
#include "window_tool/Grid.h"
#include "AddThresholdsWindow\TableData.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "SolidGroupAlgoritm\SolidBase.h"
#include "DlgTemplates\ParamDlg.h"
#include "window_tool/MenuApi.h"
#include "window_tool/DlgFiles.h"
#include "FrameWindow\FrameWindow.h"

namespace
{
	 ComputeSolidGroup &solidGroup = Singleton<ComputeSolidGroup>::Instance();
	
	struct Number  
	{
		static void Do(LV_DISPINFO *d)
		{
			wchar_t s[32];
			wsprintf(s, L"%3.d", 1 + d->item.iItem);
			StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
		}
	};
	struct Color   
	{
		static void Do(LV_DISPINFO *d)
		{
		}
	};	
	struct Selected
	{
		static void Do(LV_DISPINFO *d)
		{
			d->item.mask = LVIF_IMAGE;
			d->item.iImage = solidGroup.solidItems[ d->item.iItem].status;
		}
	};
	struct SolidG
	{
		static void Do(LV_DISPINFO *d)
		{
			wchar_t *s = (wchar_t *)solidGroup.groupNameList[solidGroup.solidItems[ d->item.iItem].groupName].name.c_str();
			StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
		}
	};
	struct Corel   
	{
		static void Do(LV_DISPINFO *d)
		{			
			Wchar_from<double, 5> s(solidGroup.solidItems[ d->item.iItem].corelation);
			StringCchCopy(d->item.pszText, wcslen(s()) + 1, s());
		}
	};
	template<int N>struct __Point__
	{
		static void Do(LV_DISPINFO *d)
		{
			Wchar_from<double, 5> s(solidGroup.solidItems[ d->item.iItem].points[N]);
			StringCchCopy(d->item.pszText, wcslen(s()) + 1, s());
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
		static void Do(LV_DISPINFO *d)
		{
			wchar_t s[32];
			wsprintf(s, L"%10.d", solidGroup.solidItems[ d->item.iItem].offset);
			StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
		}
	};
	struct File
	{
		static void Do(LV_DISPINFO *d)
		{
			wchar_t *s = (wchar_t *)solidGroup.fileNameList[solidGroup.solidItems[ d->item.iItem].solidFile].name.c_str();
			StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
		}
	};

	HEADER_TABLE(Number  , 40, L"№");
	HEADER_TABLE(Color   , 30, L"");
	HEADER_TABLE(Selected, 20, L"");
	HEADER_TABLE(SolidG, 80, L"Гр.прочности");
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
	HEADER_TABLE(File, 170, L"Файл");

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

    void(*proc[TL::Length<__header_list__>::value])(LV_DISPINFO *);
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

GridWindow::GridWindow()
{}

void GridWindow::SetHeaderToGrid(HWND h)
{	
	hWnd = h;
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

	SetRow(h, solidGroup.solidItems.size());
}

void GridWindow::Update()
{
	SetRow(hWnd, solidGroup.solidItems.size());
}

void GridWindow::SetDataToGrid(LV_DISPINFO *d)
{
	unsigned i = d->item.iItem;
	if(i < solidGroup.solidItems.size())
	{
		proc[d->item.iSubItem](d);
	}
}
void GridWindow::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	unsigned i = d->nmcd.dwItemSpec;
	if(i < solidGroup.solidItems.size())
	{
		ComputeSolidGroup::TStatus status = solidGroup.solidItems[i].status;
		if(2 == d->iSubItem)
		{
			unsigned backColor = solidGroup.groupNameList[solidGroup.solidItems[i].groupName].color;
			d->clrTextBk = (((backColor & 0xff)<< 16) | ((backColor & 0xff0000)>> 16) | (backColor & 0xff00))& 0xffffff;
		}
		else if(ComputeSolidGroup::enabled == status)
		{
			d->clrText = 0x0;
			d->clrTextBk = 0xffffff;
		}
		else  if(ComputeSolidGroup::disabled == status || ComputeSolidGroup::new_item == status)
		{
			d->clrText = 0xffffff;
			d->clrTextBk = 0xcccccc;
		}
		else  if(ComputeSolidGroup::deleted == status)
		{
			d->clrText = 0xcccccc;
			d->clrTextBk = 0x777777;
		}
	}
}
void GridWindow::LClick(LPNMITEMACTIVATE d)
{	
	unsigned i = d->iItem;
	if(i < solidGroup.solidItems.size())
	{
		FrameWindow::SelectTreshGraph(solidGroup.solidItems[i].points);
	}
}

namespace
{
template<class O, class P>struct __ok_btn__
{
	void operator()(O &o)
	{
        o.value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(o);
	}
};

struct OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(!TL::find<typename Owner::list, __test__>()(&owner.items, &h))return;
		TL::foreach<typename Owner::list, __ok_btn__>()(owner.items);
		EndDialog(h, TRUE);
	}
};

PARAM_TITLE(GroupName, L"Группа прочности")
DO_NOT_CHECK(GroupName)

	void ChangeGroupDlg(HWND h, int i)
	{
	    GroupTable t;
		wchar_t buf[64];
		wsprintf(buf, L"Сменить название группы прочности(строка № %d)", 1 + i);
		t.items.get<GroupName>().value = (wchar_t *)solidGroup.groupNameList[solidGroup.solidItems[i].groupName].name.c_str();		
		if(TemplDialogList<NullType, GroupTable
			, TL::MkTlst<GroupName>::Result
			, TL::MkTlst<OkBtn, CancelBtn>::Result>(t).Do(h, buf))
		{
			solidGroup.SetGroupName(i, t.items.get<GroupName>().value);
			solidGroup.changeTresholds = true;			
			UpdateRow(h);
		};
	}

struct EnableItem{};
struct DisableItem{};
struct DeleteItem{};
template<class T>struct TopMenu{typedef NullType list;};
MENU_TEXT(L"Включить", TopMenu<EnableItem>)
MENU_TEXT(L"Отключить", TopMenu<DisableItem>)
MENU_TEXT(L"Удалить", TopMenu<DeleteItem>)

typedef TL::MkTlst<	
	TopMenu<EnableItem>	
	, TopMenu<DisableItem>
	, Separator<0>
	, TopMenu<DeleteItem>
>::Result items_list;

struct EventData
{
	HWND h;
	int row;
	EventData(HWND h, int row)
		: h(h)
		, row(row)
	{}
};

template<>struct Event<TopMenu<EnableItem> >	 
{										 
	static void Do(EventData *data)				 
	{									 
		zprint("\n");
		solidGroup.solidItems[data->row].status = ComputeSolidGroup::enabled;
		solidGroup.changeTresholds = true;
		solidGroup.solidItems[data->row].changed = true;
		UpdateRow(data->h);
	}									 
};										 
template<>struct Event<TopMenu<DisableItem> >	 
{										 
	static void Do(EventData *data)				 
	{									 
		zprint("\n");	
		solidGroup.solidItems[data->row].status = ComputeSolidGroup::disabled;
		solidGroup.changeTresholds = true;
		solidGroup.solidItems[data->row].changed = true;
		UpdateRow(data->h);
	}									 
};	
template<>struct Event<TopMenu<DeleteItem> >	 
{										 
	static void Do(EventData *data)				 
	{									 
		zprint("\n");
		solidGroup.solidItems[data->row].status = ComputeSolidGroup::deleted;
		solidGroup.changeTresholds = true;
		solidGroup.solidItems[data->row].changed = true;
		UpdateRow(data->h);
	}									 
};

template<class O, class P>struct __pop_up__
{
	bool operator()(P &p)
	{
		if(TL::IndexOf<typename TL::EraseItem<items_list, Separator<0>>::Result, O>::value == solidGroup.solidItems[p.row].status)
		{
			PopupMenu<typename TL::EraseItem<items_list, O>::Result>::Do(p.h, &p);
			return false;
		}
		return true;
	}
};

void EnableDisable(HWND h, int i)
{
	if(solidGroup.solidItems[i].status == ComputeSolidGroup::new_item)
	{
		PopupMenu<TL::MkTlst<TopMenu<DeleteItem>>::Result>::Do(h, &EventData(h, i));
	}
	else
	{
		TL::find<TL::EraseItem<items_list, Separator<0>>::Result, __pop_up__>()(EventData(h, i));
	}
}

void ColorCell(HWND h, int i)
{
	ColorDialog c(h);
	if(c())
	{
		unsigned clr = c.color.rgbResult;
		clr = ((clr&0xff)<<16)|((clr>>16)&0xff)|(clr&0xff00);
		solidGroup.groupNameList[solidGroup.solidItems[i].groupName].color = clr;
		solidGroup.groupNameList[solidGroup.solidItems[i].groupName].added = true;
	//	solidGroup.solidItems[i].status = ComputeSolidGroup::new_item;
		solidGroup.changeTresholds = true;
		UpdateRow(h);
	}
}

}

void GridWindow::RClick(LPNMITEMACTIVATE d)
{
	unsigned i = d->iItem;
	if(i < solidGroup.solidItems.size())
	{
		switch(d->iSubItem)
		{
		case 0:
			EnableDisable(d->hdr.hwndFrom, i);
			break;
		case 2:
			ColorCell(d->hdr.hwndFrom, i);
			break;
		case 3:
			ChangeGroupDlg(d->hdr.hwndFrom, i);
			break;
		}
	}
}
