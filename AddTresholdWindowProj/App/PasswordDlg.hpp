#pragma once

#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "window_tool\Pass.h"

//template<class Base, class Table>struct TestPassword{bool operator()(HWND){return true;}};

template<class Base, class Table>struct TestPassword//<ParametersBase, Table>
{
	bool operator()(HWND h)
	{
		static const int t = TL::TypeInMultyList<typename Base::multy_type_list, Table>::Result::value;
		return (0 == t)
			? TypesizePasswordDlg().Do(h)
			: OptionPasswordDlg().Do(h);
	}
};

//template<class Base, class Table>struct __ok_table_btn__<Base, Table, typename Base::one_row_table_list>
//{
//	template<class T>bool operator()(HWND h, T &t)
//	{
//		if(!TL::find<T::list, __test__>()(&t.items, &h))return false;
//		CBase base(Base().name());
//		if(base.IsOpen())
//		{
//			__update_data__<Table> _data(base);
//			TL::foreach<T::list, __ok_btn__>()(&t.items, &_data);
//			_data.update.Where().ID(1).Execute();
//		}
//		return true;
//	}
//};