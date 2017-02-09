#pragma once

#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "window_tool\Pass.h"

template<class Base, class Table>struct TestPassword
{
	bool operator()(HWND h)
	{
		static const int t = TL::TypeInMultyList<typename Base::multy_type_list, Table>::Result::value;
		return (0 == t)
			? TypesizePasswordDlg().Do(h)
			: OptionPasswordDlg().Do(h);
	}
};

