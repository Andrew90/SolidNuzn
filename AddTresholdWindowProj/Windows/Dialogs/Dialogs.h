#pragma once
#include <windows.h>
#include "templates\typelist.hpp"
//-----------------------------------------------------------------
class CopyFactory
{
template<class O, class P>struct loc
{
	void operator ()(O *o, P *p)
	{
		o->value = p->get<O>().value;
	}
};
public:
	template<class List>static void Do(TL::Factory<List> &to, TL::Factory<List> &from)
	{
		TL::foreach<List, loc>()(&to, &from);
	}
};
//-------------------------------------------------------------------
#define DLG(n)struct n{static void Do(HWND);};

DLG(AddTypeSizeDlg)
DLG(DelTypeSizeDlg)
DLG(AboutWindow)

DLG(ThresholdDlg)
DLG(PrimarySignalDlg)

struct InitMessagePanel
{
	static int Do();
};

struct OpenMessagePanel
{
	static bool Do();
};

DLG(SaveWindowPositionDlg)
DLG(SolenoidParametersTableDlg)

DLG(TcpCommunicationsDlg)
DLG(InputBitDlg)

#undef DLG
