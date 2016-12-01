#include "stdafx.h"
#include "Dates\StoreResultBase.h"
#include <Shlwapi.h>
#include <time.h>
#include "Base\TablesDefine.h"
#include "Base/tables.hpp"

DEFINE_PARAM(CurrentTubes, int, 0)
STR_PARAM(Diameters, 32, L"---")
STR_PARAM(GroupStrength, 32, L"Z");

struct CurrentTime
{
	typedef COleDateTime type_value;
	type_value value;
	const wchar_t *name(){return L"CurrentTime";}
	CurrentTime()  
	{
		value = COleDateTime::GetCurrentTime();
    }
};

struct TubesTable
{
	typedef TL::MkTlst<
		CurrentTubes
		, CurrentTime
		, Diameters
		, GroupStrength
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TubesTable";}
};

struct StoredDataBase
{
	typedef TL::MkTlst<
		TubesTable
	>::Result type_list;
	typedef TL::Factory<type_list> TTables;
	TTables tables;
	wchar_t path[512];

	const wchar_t *name()
	{
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\..\\Data");
		CreateDirectory(path, NULL);
		time_t t = time(0);
		struct tm *now = localtime( & t );
		wsprintf(&path[wcslen(path)], L"\\%02d%02d"
			, now->tm_year - 100
			, now->tm_mon + 1
			);
		return path;
	}
};

void StoreResultBase(int currentNum, wchar_t *typeSize, wchar_t *solidGroup)
{
	 TubesTable table;
	 table.items.get<CurrentTubes>().value = currentNum;
	 table.items.get<Diameters>().value = typeSize;
	 table.items.get<GroupStrength>().value = solidGroup;

	 StoredDataBase parameters;
	 CBase base(
		 parameters.name()
		 , CreateDataBase<StoredDataBase::type_list>()
		 , parameters.tables
		 );
	 if(base.IsOpen())
	 {
		 Insert_Into<TubesTable>(table, base).Execute();
	 }
};