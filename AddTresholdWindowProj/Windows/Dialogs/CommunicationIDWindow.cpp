#include "stdafx.h"
#include "Dialogs.h"
#include "window_tool\GridItems.h"
#include <strsafe.h>
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "templates/templates.hpp"
#include "window_tool\Grid.h"
#include "resource.h"
#include "window_tool\MenuAPI.h"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"
#include "SolidGroupAlgoritm\CommunicationIDItems.h"
#include "Base\TablesDefine.h"
#include "Base\tables.hpp"
#include "DlgTemplates\ParamDlg.hpp"
#include "SolidGroupAlgoritm\SolidBase.h"

namespace
{
	ComputeSolidGroup &solidGroupX = Singleton<ComputeSolidGroup>::Instance();

	class GridH: public GridHandlers
	{
	public:	
		void SetHeaderToGrid(HWND);
		void SetDataToGrid(LV_DISPINFO *);
		void RClick(LPNMITEMACTIVATE );	
	};

	class Comm_OkBtn: TEvent
	{
		void Do(TCommand &);
	};

	class Comm_CancelBtn: TEvent
	{
		void Do(TCommand &);
	};

	class CommunicationWindow
	{
	public:
		static const int height = 200;
		static const int width = 250;
		static const int btnH = 30;

		enum Status{eOk, eCancel, eAdd};
		struct Item
		{
			std::wstring hiddenName;
			std::wstring name;
			int id;
			Status status;
			bool isNew;
		};

		std::vector<Item> items;

		HWND hWnd;
		GridH gridViewer;
		GridNotify grid;

		Comm_OkBtn okBtn;
		Comm_CancelBtn cancelBtn;
		LRESULT operator()(TCreate &);

		void operator()(TSize &);
		void operator()(TGetMinMaxInfo &);

		LRESULT operator()(TNotify &);
		void operator()(TCommand &);
	};

	struct Number  
	{
		static void Do(LV_DISPINFO *d)
		{
			CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(d->hdr.hwndFrom), GWLP_USERDATA);
			unsigned i = d->item.iItem;
			if(o->items.size())
			{
				wchar_t s[32];
				wsprintf(s, L"%3.d", 1 + i);
				StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
			}
		}
	};

	struct Selected
	{
		static void Do(LV_DISPINFO *d)
		{
			CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(d->hdr.hwndFrom), GWLP_USERDATA);
			unsigned i = d->item.iItem;
			if(o->items.size())
			{
				d->item.mask = LVIF_IMAGE;
				d->item.iImage = o->items[i].status;
			}
		}
	};

	struct SolidG
	{
		static void Do(LV_DISPINFO *d)
		{
			CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(d->hdr.hwndFrom), GWLP_USERDATA);
			unsigned i = d->item.iItem;
			if(o->items.size())
			{
				wchar_t *s = (wchar_t *)o->items[i].name.c_str();
				StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
			}
		}
	};

	struct TCPID  
	{
		static void Do(LV_DISPINFO *d)
		{
			CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(d->hdr.hwndFrom), GWLP_USERDATA);
			unsigned i = d->item.iItem;
			if(o->items.size())
			{
				wchar_t s[32];
				wsprintf(s, L"%3.d", o->items[i].id);
				StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
			}
		}
	};

	HEADER_TABLE(Number  , 40, L"№");
	HEADER_TABLE(Selected, 20, L"");
	HEADER_TABLE(SolidG, 100, L"Гр.прочности");
	HEADER_TABLE(TCPID   , 40, L"ID");

	typedef TL::MkTlst<
		Selected
		, Number  
		, SolidG
		, TCPID   
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

	

	void GridH::SetHeaderToGrid(HWND h)
	{	
		CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(h), GWLP_USERDATA);
		SetGridHeader<__header_list__> _(h);
		HIMAGELIST	himlSmall = ImageList_Create(
			GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),
			ILC_MASK, 3, 1);

		HICON hIcon = LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Ok));
		ImageList_AddIcon(himlSmall, hIcon);

		hIcon = LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Cancel));
		ImageList_AddIcon(himlSmall, hIcon);

		hIcon = LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Add));
		ImageList_AddIcon(himlSmall, hIcon);

		ListView_SetImageList(h, himlSmall, LVSIL_SMALL);

		SetRow(h, o->items.size());
	}
	void GridH::SetDataToGrid(LV_DISPINFO *d)
	{
		CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(d->hdr.hwndFrom), GWLP_USERDATA);
		unsigned i = d->item.iItem;
		if(i < o->items.size())
		{
			proc[d->item.iSubItem](d);
		}
	}

	struct AddItem{};
	struct DelItem{};
	template<class T>struct TopMenu{typedef NullType list;};
	MENU_TEXT(L"Добавить", TopMenu<AddItem>)
    MENU_TEXT(L"Удалить", TopMenu<DelItem>)

	typedef TL::MkTlst<	
	TopMenu<AddItem>	
	, Separator<0>
	, TopMenu<DelItem>
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
	DEFINE_PARAM(TcpID, int, 0)
	STR_PARAM(SolidGroup, 32, L"Группа прочности")

	PARAM_TITLE(SolidGroup, L"Группа прочности")
	PARAM_TITLE(TcpID, L"Идентификатор типоразмера")

	struct AddItemTable
	{
		typedef TL::MkTlst<	
			SolidGroup
			, TcpID
		>::Result items_list;
		typedef NullType unique_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
	};

	template<class O, class P>struct __ok_btn__
	{
		void operator()(O &o)
		{
			o.value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(o);
		}
	};

	struct OkBtnAI
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			TL::foreach<typename Owner::list, __ok_btn__>()(owner.items);
			EndDialog(h, TRUE);
		}
	};

	template<>struct Event<TopMenu<AddItem> >	 
	{										 
		static void Do(EventData *data)				 
		{		
			CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(data->h), GWLP_USERDATA);
			AddItemTable t;
			if(TemplDialog<NullType, AddItemTable
				, TL::MkTlst<OkBtnAI, CancelBtn>::Result
			>(t).Do(data->h, L"Добавить параметры типоразмера"))
			{
				CommunicationWindow::Item x;
				x.name = t.items.get<SolidGroup>().value;
				x.id = t.items.get<TcpID>().value;
				x.status = CommunicationWindow::eAdd;
				x.isNew = true;
				o->items.insert(o->items.begin(), x);
				SetRow(data->h, o->items.size());
				UpdateRowAll(data->h);
			}
		}									 
	};

	template<>struct Event<TopMenu<DelItem>>
	{										 
		static void Do(EventData *data)				 
		{			
			CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(data->h), GWLP_USERDATA);
			zprint("\n");
			if((unsigned)data->row < o->items.size())
			{
				o->items[data->row].status = CommunicationWindow::eCancel;
			}
			UpdateRow(data->h);
		}									 
	};

	void AddDel(HWND h, int i)
	{
	   PopupMenu<items_list>::Do(h, &EventData(h, i));
	}

	void GridH::RClick(LPNMITEMACTIVATE d)
	{
		CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(GetParent(d->hdr.hwndFrom), GWLP_USERDATA);
		AddDel(d->hdr.hwndFrom, d->iItem);
	}
	
	void Comm_OkBtn::Do(TCommand &l)
	{
		if(TypesizePasswordDlg().Do(l.hwnd))
		{
			CommunicationWindow *o = (CommunicationWindow *)GetWindowLongPtr(l.hwnd, GWLP_USERDATA);
			CBase base(SolidBase().name());
			if(base.IsOpen())
			{
				int currentID = Singleton<CurrentParametersTable>::Instance().items.get<CurrentID>().value;
				for(auto i = o->items.rbegin(); i != o->items.rend(); ++i)
				{
					switch(i->status)
					{
					case CommunicationWindow::eCancel:
						{
							Delete<CommunicationTypeTable>(base)
								.eq<CurrentID>(currentID)
								.eq<CommunicationTypeName>((wchar_t *)i->hiddenName.c_str())
								.Execute();

							solidGroupX.communicationIDItems.Del((wchar_t *)i->hiddenName.c_str());
						}
						break;
					case CommunicationWindow::eAdd: 
						{
							if(i->isNew)
							{
								CommunicationTypeTable t;
								t.items.get<CurrentID>().value = currentID;
								t.items.get<CommunicationTypeID>().value = i->id;
								t.items.get<CommunicationTypeName>().value = (wchar_t *)i->name.c_str();
								int id = Select<CommunicationTypeTable>(base)
									.eq<CurrentID>(currentID)
									.eq<CommunicationTypeName>(t.items.get<CommunicationTypeName>().value)
									.Execute();
								if(0 == id)
								{
									Insert_Into<CommunicationTypeTable>(t, base).Execute();
								}
								else
								{
									CommunicationTypeName::type_value name = (wchar_t *)i->name.c_str(); 
									Update<CommunicationTypeTable>(base)
										.set<CommunicationTypeID>(i->id)
										.set<CommunicationTypeName>(name)
										.set<CurrentID>(currentID)
										.Where().ID(id).Execute();
								}
								solidGroupX.communicationIDItems.Add((wchar_t *)i->name.c_str(), i->id);
							}
							else
							{
								CommunicationTypeName::type_value name = (wchar_t *)i->name.c_str();
								Update<CommunicationTypeTable>(base)
									.set<CommunicationTypeID>(i->id)
									.set<CommunicationTypeName>(name)
									.Where().ID(currentID).Execute();
								solidGroupX.communicationIDItems.Update((wchar_t *)i->hiddenName.c_str(), (wchar_t *)i->name.c_str(), i->id);
							}						
						}
						break;
					}
				}
			}
			SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
			DestroyWindow(l.hwnd);
		}		
	}

	void Comm_CancelBtn::Do(TCommand &l)
	{
		SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
		DestroyWindow(l.hwnd);
	}

	LRESULT CommunicationWindow::operator()(TCreate &l)
	{
		items.clear();
		for(auto i = solidGroupX.communicationIDItems.items.begin(); i != solidGroupX.communicationIDItems.items.end(); ++i)
		{
			Item t;
			t.hiddenName = i->name;
			t.name = i->name;
			t.id = i->id;
			t.status = eOk;
			t.isNew = false;
			items.push_back(t);
		}
		static const int dy = 50;
		int w = (width - 35) / 2;

		HWND h = CreateWindow(L"button",L"Применить"
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			, 5, height - btnH - 45, w, btnH, l.hwnd, NULL, l.create->hInstance, NULL
			);
		SetWindowLongPtr(h, GWLP_USERDATA, (DWORD)&okBtn);
		h = CreateWindow(L"button",L"Отмена"
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			, 10 + w, height - btnH - 45, w, btnH, l.hwnd, NULL, l.create->hInstance, NULL
			);
		SetWindowLongPtr(h, GWLP_USERDATA, (DWORD)&cancelBtn);
		grid.Create(l.hwnd, &gridViewer);
		SetRow(grid.hWnd, items.size());
		return 0;
	}

	void CommunicationWindow::operator()(TSize &l)
	{
		if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
		grid.Size(0, 0, l.Width, height - btnH - 50);
	}
	void CommunicationWindow::operator()(TGetMinMaxInfo &l)
	{
		if(NULL != l.pMinMaxInfo)
		{
			l.pMinMaxInfo->ptMinTrackSize.x = width;
			l.pMinMaxInfo->ptMinTrackSize.y = height;
			l.pMinMaxInfo->ptMaxTrackSize.x = width;
			l.pMinMaxInfo->ptMaxTrackSize.y = height;		
		}
	}	
	LRESULT CommunicationWindow::operator()(TNotify &l)
	{
		return EventDo(l);
	}

	void CommunicationWindow::operator()(TCommand &l)
	{
		EventDo(l);
	}
}
//------------------------------------------------------------------------------------------------
void CommunicationIDWindow::Do(HWND)
{
	HWND h = FindWindow(WindowClass<CommunicationWindow>()(), 0);
	if(NULL != h)
	{			
		RepaintWindow(h);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		CommunicationWindow &o = Singleton<CommunicationWindow>::Instance();
		RECT r;
		HWND hD = GetDesktopWindow();
		GetClientRect(hD, &r);

		int left = (r.right - CommunicationWindow::width) / 2;
		int top = (r.bottom - CommunicationWindow::height) / 2;

		HWND h = WindowTemplate(&o, L"Идентификатор типоразмера для передачи по для сети"
			, left, top, CommunicationWindow::width, CommunicationWindow::height, IDI_UPLOAD, (HBRUSH)COLOR_WINDOW);
		ShowWindow(h, SW_SHOWNORMAL);
		DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
		dwStyle &= ~(WS_SYSMENU);
		SetWindowLong(h, GWL_STYLE, dwStyle);
		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}		
}