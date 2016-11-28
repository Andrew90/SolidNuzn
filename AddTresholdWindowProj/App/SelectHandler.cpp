#include "stdafx.h"
#include "SelectHandler.h"
#include "App\AppBase.h"
#include "SolidGroupAlgoritm\SolidBase.h"
#include "Base\tables.hpp"
#include "MainWindow\MainWindowMenu.hpp"
#include "Dates\CounterTubes.h"
#include "MainWindow\MainWindow.h"

namespace SelectHandler
{
	template<class T, class D>struct __sel__
	{
		bool operator()(T &t, D &d)
		{
			ComboBox_AddString(d, t.get<NameParam>().value);
			return false;
		}
	};

	void Init(HWND hWnd)
	{
#pragma message("делать выбор типоразмера")
		ParametersBase parameters;
		CBase base(
			parameters.name()
			, CreateDataBase<ParametersBase::type_list, SetDefault<ParametersBase::type_list> >()
			, parameters.tables
			);

		if(base.IsOpen())
		{
			CMD(base).CommandText(L"SELECT NameParam FROM ParametersTable ORDER BY NameParam")
				.ExecuteLoop<TL::MkTlst<NameParam>::Result, __sel__>(hWnd);
		    int num = ComboBox_FindStringExact(hWnd, 0
				, Singleton<ParametersTable>::Instance().items.get<NameParam>().value
				);
	        if(CB_ERR != num) ComboBox_SetCurSel(hWnd, num);
		}

	}

	void Do(HWND h, wchar_t *buf)
	{
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{			
			NameParam::type_value name;
			name = buf;
			ParametersTable typeSizeParam;
			int id = Select<ParametersTable>(base).eq<NameParam>(name).Execute(typeSizeParam);
			if(id != 0)
			{
				CurrentParametersTable t;
				t.items.get<CurrentID>().value = id;
				UpdateWhere<CurrentParametersTable>(t, base).ID(1).Execute();
		
				AppBase::InitTypeSizeTables(base);
				Singleton<ComputeSolidGroup>::Instance().Load(buf);
				bool x = Singleton<DifferentOptionsTable>::Instance().items.get<MessagePanelVisible>().value;
				CheckMenu<MenuItem<MainWindowMenu::MessagePanel>>(h, x);
				CounterTubes::Load(name);
				Singleton<MainWindow>::Instance().gridCounterViewer.Update();
			}
		}
	}
}