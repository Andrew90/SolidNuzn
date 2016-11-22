#include "stdafx.h"
#include "Dialogs.h"
#include "App\AppBase.h"
#include "App\App.h"
#include "window_tool\WindowsPosition.h"
#include "MainWindow\MainWindow.h"

namespace
{
	 ParametersTable &params = Singleton<ParametersTable>::Instance();
}

int InitMessagePanel::Do()
{
	return Singleton<ParametersTable>::Instance().items.get<MessagePanelVisible>().value
			? MFS_CHECKED
			: MFS_UNCHECKED;
}

bool OpenMessagePanel::Do()
{
	try
	{
		bool &result = params.items.get<MessagePanelVisible>().value;
		result ^= true;
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			Update<ParametersTable>(base).set<MessagePanelVisible>(result)
				.Where().eq<NameParam>(
				params.items.get<NameParam>().value
				).Execute();
		}
		return result;
	}
	catch(...)
	{
		return false;
	}
};

void SaveWindowPositionDlg::Do(HWND h)
{
	WindowPositionDlg<MainWindow>::Do(h);
}


