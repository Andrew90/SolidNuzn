#include "stdafx.h"
#include "ChangeStandard.h"
#include "templates/templates.hpp"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "App\AppBase.h"

#pragma warning(disable : 4996)

bool systemInStandardMode = false;

void ChangeStandard(bool isStandard)
{
	wchar_t st[] = L"_standard";
	wchar_t buf[128];
	wcscpy(buf, Singleton<ComputeSolidGroup>::Instance().typeSizeName.c_str());
	int len = wcslen(buf);

	int k =  len - (dimention_of(st) - 1);
	bool standardBool  = k > 0;
	if(standardBool)
	{
		wchar_t *s = &buf[k];
		standardBool = standardBool && 0 == wcsncmp(s, st, dimention_of(st) - 1);
	}

	ParametersTable &table = Singleton<ParametersTable>::Instance();

	if(isStandard && !standardBool)
	{
		wcscat(buf, st);		
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			ParametersTable typeSizeParam;
			table.items.get<NameParam>().value = buf;
			int id = Select<ParametersTable>(base).eq<NameParam>(table.items.get<NameParam>().value).Execute(typeSizeParam);
			if(id != 0)
			{
				CurrentParametersTable t;
				t.items.get<CurrentID>().value = id;
				UpdateWhere<CurrentParametersTable>(t, base).ID(1).Execute();
				AppBase::InitTypeSizeTables(base);
				Singleton<ComputeSolidGroup>::Instance().Load(buf);
				Singleton<ComputeSolidGroup>::Instance().typeSizeName = buf;
				systemInStandardMode = true;
				return;
			}
		}
	}

	if(systemInStandardMode)
	{
		systemInStandardMode = false;

		if(standardBool) if(k > 0)buf[k] = 0;
			
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			ParametersTable typeSizeParam;
			table.items.get<NameParam>().value = buf;
			int id = Select<ParametersTable>(base).eq<NameParam>(table.items.get<NameParam>().value).Execute(typeSizeParam);
			if(id != 0)
			{
				CurrentParametersTable t;
				t.items.get<CurrentID>().value = id;
				UpdateWhere<CurrentParametersTable>(t, base).ID(1).Execute();
				AppBase::InitTypeSizeTables(base);
				Singleton<ComputeSolidGroup>::Instance().Load(buf);
				Singleton<ComputeSolidGroup>::Instance().typeSizeName = buf;
			}
		}
	}
}