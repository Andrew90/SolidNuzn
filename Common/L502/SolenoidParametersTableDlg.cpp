#include "stdafx.h"
#include "SolenoidParametersTableDlg.h"
#include "AppBase.h"
#include "ParamDlg.h"
#include "L502SolidGroup.h"

template<>struct DlgItem<Frequency502>
{
	typedef Frequency502 T;
	T &value;
	HWND hWnd;
	template<class Z>DlgItem(Z &z) : value(z.get<T>()){}
	void Init(HWND h, int &width, int &dy)
	{
		value.value /= 2;
		hWnd = DlgSubItems<T, typename T::type_value>().Init(h, width, dy, value);
		value.value *= 2;
	}
};

namespace SolenoidParametersTableDlgSpace
{
PARAM_TITLE(Frequency502, L"Частота сбора одного канала(устанавливается при пусконаладке)")
PARAM_TITLE(FrequencyGenerator, L"Частота генератора(устанавливается при пусконаладке)")

MIN_EQUAL_VALUE(Frequency502, 500)
MAX_EQUAL_VALUE(Frequency502, 20000)



DO_NOT_CHECK(FrequencyGenerator)

PARAM_TITLE(InputRangeSignal, L"Диапазон входного сигнала В.(устанавливается при пусконаладке)")
PARAM_TITLE(RangeReferenceSignal, L"Диапазон опорного сигнала В.(устанавливается при пусконаладке)")
DO_NOT_CHECK(InputRangeSignal)
DO_NOT_CHECK(RangeReferenceSignal)

const wchar_t *SyncGainData[] ={L"10", L"5", L"2", L"1", L"0.5", L"0.2"};

template<>struct FillComboboxList<InputRangeSignal>
{
	void operator()(HWND h, InputRangeSignal &t)
	{
		for(int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			ComboBox_AddString(h, SyncGainData[i]);
		}
	}
};
template<>struct CurrentValue<InputRangeSignal>
{
	void operator()(HWND h, InputRangeSignal &t)
	{
		ComboBox_SetCurSel(h, Singleton<SolenoidParametersTable>::Instance().items.get<InputRangeSignal>().value);
	}
};
template<>struct FillComboboxList<RangeReferenceSignal>
{
	void operator()(HWND h, RangeReferenceSignal &t)
	{
		for(int i = 0; i < dimention_of(SyncGainData); ++i)
		{
			ComboBox_AddString(h, SyncGainData[i]);
		}
	}
};
template<>struct CurrentValue<RangeReferenceSignal>
{
	void operator()(HWND h, RangeReferenceSignal &t)
	{
		ComboBox_SetCurSel(h, Singleton<SolenoidParametersTable>::Instance().items.get<RangeReferenceSignal>().value);
	}
};

template<>struct DlgSubItems<InputRangeSignal, int>: ComboBoxSubItem<InputRangeSignal>{};
template<>struct DlgSubItems<RangeReferenceSignal, int>: ComboBoxSubItem<RangeReferenceSignal>{};





template<class P>struct __ok_btn__<DlgItem<RangeReferenceSignal>, P>
{
	typedef DlgItem<RangeReferenceSignal> O;
	void operator()(O *o, P *p)
	{
		dprint("%s", __FUNCTION__);
		wchar_t s[128];
	    GetWindowText(o->hWnd, s, dimention_of(s));
		int i = 0;
		for(; i < dimention_of(SolenoidParametersTableDlgSpace::SyncGainData); ++i)
		{
			if(0 == wcscmp(s, SolenoidParametersTableDlgSpace::SyncGainData[i])) break;
		}
        o->value.value =  i;
		p->update.set<typename TL::Inner<O>::Result>(o->value.value);
	}
};

template<class P>struct __ok_btn__<DlgItem<InputRangeSignal>, P>
{
	typedef DlgItem<InputRangeSignal> O;
	void operator()(O *o, P *p)
	{
		dprint("%s", __FUNCTION__);
		wchar_t s[128];
	    GetWindowText(o->hWnd, s, dimention_of(s));
		int i = 0;
		for(; i < dimention_of(SolenoidParametersTableDlgSpace::SyncGainData); ++i)
		{
			if(0 == wcscmp(s, SolenoidParametersTableDlgSpace::SyncGainData[i])) break;
		}
        o->value.value =  i;
		p->update.set<typename TL::Inner<O>::Result>(o->value.value);
	}
};

template<class P>struct __ok_btn__<DlgItem<Frequency502>, P>
{
	typedef DlgItem<Frequency502> O;
	void operator()(O *o, P *p)
	{
		dprint("%s", __FUNCTION__);
		o->value.value = 2 * __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(*o);
		p->update.set<typename TL::Inner<O>::Result>(o->value.value);
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
		if(TestPassword<Owner::Table>()(h))
		{
			if(__ok_table_btn__<
				Owner::Table
				, typename TL::SubListFromMultyList<ParametersBase::multy_type_list, Owner::Table>::Result
			>()(h, owner))
			{
				EndDialog(h, TRUE);
			}
		}
	}
};
}

void SolenoidParametersTableDlg::Do(HWND h)
{
	if(TemplDialog<SolenoidParametersTable, TL::MkTlst<SolenoidParametersTableDlgSpace::OkBtn, CancelBtn>::Result>(Singleton<SolenoidParametersTable>::Instance()).Do(h, L"Настройки генератора"))
	{
		if(Singleton<L502SolidGroup>::Instance().SetupParams())
		{
			MessageBox(0, L"Не могу инициировать плату L502", L"Ошибка!!!", MB_ICONERROR);
		}
	}
}