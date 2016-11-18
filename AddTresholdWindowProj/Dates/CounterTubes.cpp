#include "stdafx.h"
#include "Dates\CounterTubes.h"
#include <string>
#include <vector>
#include "Base\tables.hpp"
#include "Base\TablesDefine.h"
#include "templates\typelist.hpp"

namespace CounterTubes
{
struct Items
{
	int count;
	std::wstring typeName;
	Items(int c, wchar_t *n)
		: count(c)
		, typeName(n)
	{}
	Items(){}
};

std::vector<Items> vec;

std::wstring gTypeSizeName;

DEFINE_PARAM(CountTypeName, int, 1)
STR_PARAM(GroupName, 64, L"GroupName")
STR_PARAM(TypeSize, 64, L"TypeSize")

struct TypeSizeTable
{
	typedef TL::MkTlst<
		TypeSize
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TypeSizeTable";}
};

struct GroupNameTable
{
	typedef TL::MkTlst<
		GroupName 
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GroupNameTable";}
};

DEFINE_PARAM_ID(TypeSizeTable, int, 1)
DEFINE_PARAM_ID(GroupNameTable, int, 1)

struct CountTypeNameTable
{
	typedef TL::MkTlst<
		CountTypeName
		, ID<TypeSizeTable>
		, ID<GroupNameTable >
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CountTypeNameTable";}
};

struct CounterTubesBase
 {
	 typedef TL::MkTlst<
		TypeSizeTable
		, GroupNameTable
		, CountTypeNameTable
	 >::Result type_list; 
	
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name()
	 {
		 GetModuleFileName(0, path, dimention_of(path));
		 PathRemoveFileSpec(path);
		 wcscat(path, L"\\CounterTubesBase");
		 return path;
	 }
 };

template<class, class>struct __sub_sel__;
template<class P>struct __sub_sel__<CountTypeName, P>
{
	void operator()(CountTypeName &o, P &p)
	{
		p.count = o.value;
	}
};
template<class P>struct __sub_sel__<GroupName, P>
{
	void operator()(GroupName &o, P &p)
	{
		p.typeName = o.value;
	}
};

template<class T, class D>struct __select__;
template<class List, class D>struct __select__<TL::Factory<List>, D>
{
	bool operator()(TL::Factory<List> &t, D &d)
	{
		Items item;
		TL::foreach<List, __sub_sel__>()(t, item);
		d.push_back(item);
		return false;
	}
};

void Load(wchar_t *typeName)
{
	vec.clear();
	gTypeSizeName = typeName;
	CounterTubesBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<CounterTubesBase::type_list, SetDefault<CounterTubesBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		TypeSizeTable ts;
		ts.items.get<TypeSize>().value = (wchar_t *)gTypeSizeName.c_str();
		int idTs = Select<TypeSizeTable>(base).eq<TypeSize>(ts.items.get<TypeSize>().value).Execute();

		if(0 == idTs)
		{
			Insert_Into<TypeSizeTable>(ts, base).Execute();
			return;
		}
		
		CMD(base).CommandText( 
			L"SELECT CountTypeName, GroupName"\
            L" FROM (CountTypeNameTable INNER JOIN"\
            L" GroupNameTable ON CountTypeNameTable.GroupNameTableID = GroupNameTable.ID)"\
            L" WHERE(CountTypeNameTable.TypeSizeTableID =?)"
			).Param(idTs).ExecuteLoop<TL::MkTlst<CountTypeName, GroupName>::Result, __select__>(vec);
	}
}
void Inc(wchar_t *groupName)
{
	bool b = false;
	int data = 0;
	for(auto i = vec.begin(); i != vec.end(); ++i)
	{
		if(0 == wcscmp(groupName, i->typeName.c_str()))
		{
			data = ++i->count;
			b = true;
			break;
		}
	}

	CBase base(CounterTubesBase().name());

	if(base.IsOpen())
	{
		TypeSizeTable ts;
		ts.items.get<TypeSize>().value = (wchar_t *)gTypeSizeName.c_str();
		int idTs = Select<TypeSizeTable>(base).eq<TypeSize>(ts.items.get<TypeSize>().value).Execute();

		if(0 == idTs)
		{
			Insert_Into<TypeSizeTable>(ts, base).Execute();
			idTs = Select<TypeSizeTable>(base).eq<TypeSize>(ts.items.get<TypeSize>().value).Execute();
		}

		GroupNameTable gn;
		gn.items.get<GroupName>().value = groupName;
		int idGn = Select<GroupNameTable>(base).eq<GroupName>(gn.items.get<GroupName>().value).Execute();

		if(0 == idGn)
		{
			Insert_Into<GroupNameTable>(gn, base).Execute();
			idGn = Select<GroupNameTable>(base).eq<GroupName>(gn.items.get<GroupName>().value).Execute();
		}

		if(!b)
		{
			vec.push_back(Items(1, groupName));
			CountTypeNameTable ctn;
			ctn.items.get<ID<TypeSizeTable>>().value = idTs;
			ctn.items.get<ID<GroupNameTable>>().value = idGn;
			ctn.items.get<CountTypeName>().value = 1;
			Insert_Into<CountTypeNameTable>(ctn, base).Execute();
		}
		else
		{
			Update<CountTypeNameTable>(base).set<CountTypeName>(data).Where().eq<ID<TypeSizeTable>>(idTs).eq<ID<GroupNameTable>>(idGn).Execute();
		}
	}
}

void Clear()
{
	CBase base(CounterTubesBase().name());

	if(base.IsOpen())
	{
		TypeSizeTable ts;
		ts.items.get<TypeSize>().value = (wchar_t *)gTypeSizeName.c_str();
		int idTs = Select<TypeSizeTable>(base).eq<TypeSize>(ts.items.get<TypeSize>().value).Execute();
		if(0 != idTs)
		{
			Delete<CountTypeNameTable>(base).eq<ID<TypeSizeTable>>((ID<TypeSizeTable>::type_value &)idTs).Execute();
			vec.clear();
		}
	}
}

int CountGroup()
{
	return vec.size();
}
wchar_t *TypeName(unsigned i)
{
	if(i < vec.size()) return (wchar_t *)vec[i].typeName.c_str();
	return L"";
}
int CountType(unsigned i)
{
	if(i < vec.size()) return vec[i].count;
	return 0;
}
int CountTypeAll()
{
	int count = 0;
	for(auto i = vec.begin(); i != vec.end(); ++i)
	{
		count += i->count;
	}
	return count;
}
}
