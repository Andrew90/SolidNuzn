#include "stdafx.h"
#include "Dialogs.h"
#include "App\AppBase.h"
#include "DlgTemplates\ParamDlg.hpp"
namespace ThresholdDlgN
{
	PARAM_TITLE(TubeInUnit, L"Труба в модуле")
	PARAM_TITLE(UnitOn , L"Модуль включён")
	
	template<class O, class P>struct __set__
	{
		void operator()(O &o, P &p)
		{
			unsigned i = o.value;
			unsigned k = 0;
			while(i >>= 1) ++k;
			p.get<O>().value = k;
		}
	};

	template<class O, class P>struct __get__
	{
		void operator()(O &o, P &p)
		{
			o.value = p.get<O>().value;
		}
	};

	template<class O>struct __ok_btn__<O, __update_data__<InputBitTable>>
	{
		typedef __update_data__<InputBitTable> P;
		void operator()(O *o, P *p)
		{
			o->value.value = 1 << __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(*o);
			p->update.set<typename TL::Inner<O>::Result>(o->value.value);
		}
	};

	DO_NOT_CHECK(TubeInUnit)
	DO_NOT_CHECK(UnitOn)

	template<>struct DlgSubItems<TubeInUnit, unsigned >: UpDownSubItem<TubeInUnit>{};
	template<>struct DlgSubItems<UnitOn, unsigned >: UpDownSubItem<UnitOn>{};
}



void InputBitDlg::Do(HWND h)
{
	InputBitTable t;
	TL::foreach<InputBitTable::items_list, ThresholdDlgN::__set__>()(Singleton<InputBitTable>::Instance().items, t.items);
	if(TemplDialog<ParametersBase
		, InputBitTable
	>(t).Do(h, L"Входные порты"))
	{
		TL::foreach<InputBitTable::items_list, ThresholdDlgN::__get__>()(Singleton<InputBitTable>::Instance().items, t.items);
	}
}
