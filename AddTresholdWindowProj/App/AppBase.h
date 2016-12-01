#pragma once
#include "Base\tables.hpp"
#include "Base\TablesDefine.h"
#include "templates\typelist.hpp"
#include "App\App.h"
//-------------------------------------------------------------------------------------------
DEFINE_PARAM(CurrentID, int, 1)

struct CurrentParametersTable
{
	typedef TL::MkTlst<
		CurrentID
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentParametersTable";}
};
//------------------------------------------------------------------------------------------
STR_PARAM(NameParam, 128, L"NONAME")
 struct ParametersTable
 {
	typedef TL::MkTlst<	
		 NameParam		
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };
//--------------------------------------------------------------------------------------------------------
DEFINE_PARAM(TubeInUnit  , unsigned, 1 << 0)
DEFINE_PARAM(UnitOn            , unsigned, 1 << 1)

struct InputBitTable
 {
	typedef TL::MkTlst<
		TubeInUnit
		, UnitOn          
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBitTable";}
 };

//DEFINE_PARAM(oWork            , unsigned, 1 << 0)
//DEFINE_PARAM(oToShiftThe      , unsigned, 1 << 1)
//DEFINE_PARAM(oResult1         , unsigned, 1 << 2)
//DEFINE_PARAM(oResult2         , unsigned, 1 << 3)
//DEFINE_PARAM(oPowerBM         , unsigned, 1 << 4)
//DEFINE_PARAM(oReserve         , unsigned, 1 << 5)
//
//struct OutputBitTable
// {
//	typedef TL::MkTlst<
//		oWork      
//		, oToShiftThe  //перекладка
//		, oResult1   
//		, oResult2   
//		, oPowerBM   
//		, oReserve   
//	>::Result items_list;
//	typedef TL::Factory<items_list> TItems;
//	TItems items;
//	const wchar_t *name(){return L"OutputBitTable";}
// };

// DEFINE_PARAM(Descriptor1730, unsigned, 0)
//struct Descriptor1730Table
// {
//	typedef TL::MkTlst<
//		Descriptor1730
//	>::Result items_list;
//	typedef TL::Factory<items_list> TItems;
//	TItems items;
//	const wchar_t *name(){return L"Descriptor1730Table";}
// };

DEFINE_PARAM(PrimarySignalMin , double, -10)
DEFINE_PARAM(PrimarySignalMax , double, 10)
DEFINE_PARAM(OffsetPointsMin  , double, -10)
DEFINE_PARAM(OffsetPointsMax  , double, 10)

DEFINE_PARAM(PrimarySignalOffset  , int, 300)
DEFINE_PARAM(PrimarySignalWidth  , int, 800)
struct GraphAxesTable
 {
	typedef TL::MkTlst<
		PrimarySignalMin
		, PrimarySignalMax
		, OffsetPointsMin 
		, OffsetPointsMax 
		, PrimarySignalOffset
		, PrimarySignalWidth
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GraphAxesTable";}
 };
//------------------------------------------------------------------------------------------------------------
DEFINE_PARAM(Frequency502, int, 4000)
DEFINE_PARAM(FrequencyGenerator, int, 6)
DEFINE_PARAM(InputRangeSignal, int, 0)
DEFINE_PARAM(RangeReferenceSignal, int, 0)
struct SolenoidParametersTable
{
	typedef TL::MkTlst<
		Frequency502
		, FrequencyGenerator
		, InputRangeSignal
		, RangeReferenceSignal
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"SolenoidParametersTable";}
};
//--------------------------------------------------------------
DEFINE_PARAM(PortTCP, int, 2030)
STR_PARAM(AddresTCP, 17, L"192.168.1.100")
struct TcpCommunications
{
	typedef TL::MkTlst<	
		PortTCP
		, AddresTCP
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TcpCommunications";}
};
//---------------------------------------------------------------
DEFINE_PARAM(CommunicationRemoveUnit, bool, false)
DEFINE_PARAM(TubesStored, bool, false)
DEFINE_PARAM(PaintMarker, bool, false)
DEFINE_PARAM(MessagePanelVisible  , bool, false)
DEFINE_PARAM(IOPortsVisible, bool, false)
STR_PARAM(DeviceDescription1730, 64, L"PCI-1730,BID#15")
struct DifferentOptionsTable
{
	typedef TL::MkTlst<
		CommunicationRemoveUnit
		, TubesStored
		, PaintMarker
		, MessagePanelVisible
		, IOPortsVisible
		, DeviceDescription1730
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DifferentOptionsTable";}
};
//---------------------------------------------------------------
 struct ParametersBase
 {
	 typedef TL::MkTlst<
		 InputBitTable
		 , GraphAxesTable
		 , SolenoidParametersTable
		 , TcpCommunications
		 , DifferentOptionsTable
		// , CommunicationTypeTable
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<		
		  CurrentParametersTable
		 , ParametersTable		
	 >::Result multy_row_table_list;

	 typedef TL::MkTlst<
		  multy_row_table_list
		 , one_row_table_list
	 >::Result multy_type_list; 

	 typedef TL::MultyListToList<multy_type_list>::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name();
 };

struct AppBase
{
	void Init();
	static void InitTypeSizeTables(CBase &);
};

template<class T>int CurrentId()
{
	  return Singleton<ParametersTable>::Instance().items.get<T>().value;
}

template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   ParametersTable &t = Singleton<ParametersTable>::Instance();
   t.items.get<T>().value = num;
   UpdateWhere<ParametersTable>(t, base).ID(current.items.get<CurrentID>().value).Execute();
}

template<class T>int CountId(CBase &base, int num)
{
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}
	return i;
}

class CopyTable
{
	template<class O, class P>struct loc
	{
		void operator()(O &o, P &p)
		{
			o.value = p.get<O>().value;
		}
	};
public:
	template<class T>void operator()(T &a, T &b)
	{
		TL::foreach<typename TL::Inner<T>::Result, loc>()(a, b);
	}
};

template<class T>struct CountRowsTable
{
	int operator()(CBase &base)
	{
	   int count = 0;
	   wchar_t buf[128];
	   wsprintf(buf, L"SELECT count(*) as C FROM %s", T().name());
	   CMD(base).CommandText(buf).Execute().GetValue(L"C", count);
	   return count;
	}
};








