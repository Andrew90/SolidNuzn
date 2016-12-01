#include "stdafx.h"
#include "Dialogs.h"
#include "App/AppBase.h"
#include "DlgTemplates\ParamDlg.h"
#include "Automat/Automat.h"

namespace
{
	PARAM_TITLE(DeviceDescription1730, L"Дескриптор")
	DO_NOT_CHECK(DeviceDescription1730)
}

void Descriptor1730Dlg::Do(HWND h)
{
	DeviceDescription1730::type_value &val = Singleton<DifferentOptionsTable>::Instance().items.get<DeviceDescription1730>().value;
	DeviceDescription1730::type_value last = val;
	if(TemplDialogList<ParametersBase, DifferentOptionsTable, TL::MkTlst<
		DeviceDescription1730
	>::Result>(Singleton<DifferentOptionsTable>::Instance()).Do(h, L"Дескриптор платы 1730"))
	{
		if(last != val)
		{
			if(Automat::Suspend())
			{
				Sleep(100);
				if(Automat::Init1730()) Automat::Resume();
			}
			else
			{
				if(Automat::Init1730()) Automat::Run();
			}
		}
	}
}