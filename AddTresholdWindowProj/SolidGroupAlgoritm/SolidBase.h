#pragma once
#include "SolidBase.h"
#include "Base\tables.hpp"
#include "Base\TablesDefine.h"
#include "templates\typelist.hpp"
#include "App\AppBase.h"

#include "ComputeSolidGroup.h"

//STR_PARAM(NameParam, 128, L"NONAME")

template<int>struct Offset;

DEFINE_PARAM_NUM(Offset, 0, int, 10)
DEFINE_PARAM_NUM(Offset, 1, int, 20)
DEFINE_PARAM_NUM(Offset, 2, int, 30)
DEFINE_PARAM_NUM(Offset, 3, int, 40)
DEFINE_PARAM_NUM(Offset, 4, int, 50)
DEFINE_PARAM_NUM(Offset, 5, int, 60)
DEFINE_PARAM_NUM(Offset, 6, int, 70)
DEFINE_PARAM_NUM(Offset, 7, int, 80)

DEFINE_PARAM(FrequencySignal, int, 6)
DEFINE_PARAM(FrequencySamples, int, 8000)

struct SolidParametersTable
 {
	 typedef TL::AppendList<	
		 TL::MkTlst<NameParam, FrequencySignal, FrequencySamples>::Result
		, TL::CreateNumList<Offset, 0, ComputeSolidGroup::count_points - 1>::Result
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"SolidParametersTable";}
 };

STR_PARAM(SolidFileName, 128, L"")
struct FileNameTable
{
	typedef TL::MkTlst<
		SolidFileName	
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"FileNameTable";}
};

STR_PARAM(GroupName, 32, L"DeleteIt")
DEFINE_PARAM(GroupColor, unsigned, 0xff00ff)
struct GroupTable
{
	typedef TL::MkTlst<
		GroupName
		, GroupColor
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GroupTable";}
};

template<int>struct Point;

DEFINE_PARAM_NUM(Point, 0, double, -6)
DEFINE_PARAM_NUM(Point, 1, double, -4)
DEFINE_PARAM_NUM(Point, 2, double, -2)
DEFINE_PARAM_NUM(Point, 3, double, 0)
DEFINE_PARAM_NUM(Point, 4, double, 2)
DEFINE_PARAM_NUM(Point, 5, double, 4)
DEFINE_PARAM_NUM(Point, 6, double, 6)
DEFINE_PARAM_NUM(Point, 7, double, 8)

DEFINE_PARAM_ID(GroupName, int, 1)
DEFINE_PARAM_ID(FileNameTable, int, 1)
DEFINE_PARAM_ID(SolidParametersTable, int, 1)

DEFINE_PARAM(Enable, unsigned, 0)
DEFINE_PARAM(OffsetFile, int, 0)

struct TresholdsTable
{
	typedef TL::AppendList<
		TL::MkTlst<
		ID<GroupName>
		, ID<FileNameTable>
		, ID<SolidParametersTable>
		, Enable
		, OffsetFile
		>::Result
		, TL::CreateNumList<Point, 0, ComputeSolidGroup::count_points - 1>::Result
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TresholdsTable";}
};
//---------------------------------------------------------------
DEFINE_PARAM(CommunicationTypeID, int, 0);
STR_PARAM(CommunicationTypeName, 32, L"DeleteIt")
struct CommunicationTypeTable
{
	typedef TL::MkTlst<
		CurrentID
		, CommunicationTypeID
		, CommunicationTypeName
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CommunicationTypeTable";}
};


struct SolidBase
 {
	 typedef TL::MkTlst<		
		 FileNameTable
		 , GroupTable
		 , TresholdsTable
		 , SolidParametersTable
		 , CommunicationTypeTable
	 >::Result type_list;

	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name();
 };



