#include "stdafx.h"
#include "SelectHandler.h"
#include "App\AppBase.h"

namespace SelectHandler
{
	template<class T, class D>struct __sel__
	{
		bool operator()(unsigned, T &t, D &d)
		{
			ComboBox_AddString(d, t.items.get<NameParam>().value);
			return false;
		}
	};

	void Init(HWND hWnd)
	{
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			Select<ParametersTable>(base).ExecuteLoop<__sel__>(hWnd);
			int num = ComboBox_FindStringExact(hWnd, 0, Singleton<ParametersTable>::Instance().items.get<NameParam>().value);
			if(CB_ERR != num) ComboBox_SetCurSel(hWnd, num);
		}
	}

	void Do(wchar_t *buf)
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

			}
		}
	}
}