#include "stdafx.h"
#include "Dialogs.h"
#include "App\AppBase.h"
#include "App\App.h"
#include "window_tool\SelectTypeSizeList.h"
#include "App\SelectHandler.h"
#include "App\PasswordDlg.hpp"


namespace 
{
	PARAM_TITLE(NameParam, L"")
		template<int N>struct DlgSubItems<NameParam, Holder<N> >: EditItems<NameParam, 420>{};

	struct OkBtnX
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<Owner::Base, Owner::Table>()(h))
			{
				HWND hWnd = owner.items.get<DlgItem<NameParam>>().hWnd;
				wchar_t buf[128];
				GetWindowText(hWnd, buf, dimention_of(buf));
				if(0 == buf[0])
				{
					MessageBox(h, L"Необходимо ввести наименование типоразмера", L"Ошибка!!!", MB_ICONERROR);
					return;
				}
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					NameParam n;
					n.value = buf;
					unsigned id = Select<Owner::Table>(base).eq<NameParam>(n.value).Execute();
					if(0 != id)
					{
						MessageBox(h, L"Название типоразмера есть в базе", L"Предупреждение!!!", MB_ICONWARNING);
						return;
					}

					Owner::Table &table = Singleton<Owner::Table>::Instance();
					table.items.get<NameParam>().value = buf;

					Insert_Into<Owner::Table>(table, base).Execute();
					CurrentID cId;
					cId.value = Select<Owner::Table>(base).eq<NameParam>(n.value).Execute();
					Update<CurrentParametersTable>(base).set<CurrentID>(cId.value).Where().ID(1).Execute();
					Singleton<SelectTypeSizeList>::Instance().AddMenuItem(buf);

					App::AddMenuItem(buf);

					EndDialog(h, TRUE);
				}
			}
		}
	};
}

void AddTypeSizeDlg::Do(HWND h)
{
	TemplDialogList<ParametersBase, ParametersTable, TL::MkTlst<NameParam>::Result, TL::MkTlst<OkBtnX, CancelBtn>::Result>
		(Singleton<ParametersTable>::Instance()).Do(h, L"Создать типоразмер");
}

namespace
{	
	struct NameParamD : NameParam{};
	template<int N>struct DlgSubItems<NameParamD, Holder<N> >: EditReadOnlyItems<NameParamD, 420>{};

	struct ParametersTableD
	{
		typedef TL::MkTlst<	
			NameParamD
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
	};

	PARAM_TITLE(NameParamD, L"")

	struct OkBtnD
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<ParametersBase, ParametersTable>()(h))
			{
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					NameParamD &t = owner.items.get<DlgItem<NameParamD>>().value;
					int count = CountRowsTable<ParametersTable>()(base);
					if(count <= 1)
					{
						MessageBox(h, L"В настройках должно быть не менее одного типоразмера", L"Предупреждение!!!", MB_ICONWARNING);
						EndDialog(h, TRUE);
						return;
					}
					Delete<ParametersTable>(base).eq<NameParam>(t.value).Execute();
					App::DelMenuItem(t.value);
					wchar_t buf[128];
					CMD(base).CommandText(L"select top 1 NameParam from ParametersTable").Execute().GetValue(L"NameParam", buf);
					SelectHandler::Do(h, buf);
					App::SelMenuItem(buf);
					EndDialog(h, TRUE);
				}
			}
		}
	};
}

void DelTypeSizeDlg::Do(HWND h)
{
	ParametersTableD t;
	t.items.get<NameParamD>().value = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	TemplDialog<ParametersBase, ParametersTableD, TL::MkTlst<OkBtnD, CancelBtn>::Result>(t).Do(h, L"Удалить типоразмер");
}
