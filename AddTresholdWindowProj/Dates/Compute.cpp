#include "stdafx.h"
#include "Compute.h"
#include "SolidData.h"
#include "templates\templates.hpp"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "App\App.h"
#include "ColorPanel\ColorPanel.h"

namespace Compute
{
	SolidData &solidData = Singleton<SolidData>::Instance();
	ComputeSolidGroup &computeSolidGroup = Singleton<ComputeSolidGroup>::Instance();

	void Recalculation()
	{
		solidData.start = int(0.1 * solidData.currentOffset);
		solidData.stop = solidData.currentOffset - solidData.start;

		double result = 0;
		wchar_t *groupName = L"";
		unsigned color = 0;

		computeSolidGroup.Frames(
			solidData.signal
			, solidData.reference
			, solidData.start
			, solidData.stop
			, result
			, groupName
			, color
			);
		wchar_t buf[1024];

		wsprintf(buf, L"<ff00>%s<ff>√руппа прочности <%6x>%s <ff>коррел€ци€ <ffffff>%s"
			, computeSolidGroup.currentFile.c_str()
			, color
			, groupName
			, Wchar_from<double>(result)()
			);

		computeSolidGroup.currentGroupName = groupName;

		Singleton<L502Signal>::Instance().Set(solidData.signal, solidData.currentOffset);
		Singleton<L502Reference>::Instance().Set(solidData.reference, solidData.currentOffset);

		App::PrintTopLabel(buf);
		App::UpdateMainWindow();

		ColorPanel::SetText(groupName, color);
	}

	void Recalculation(double &result, wchar_t *&groupName, unsigned &color)
	{
		solidData.start = int(0.1 * solidData.currentOffset);
		solidData.stop = solidData.currentOffset - solidData.start;

		result = 0;
		groupName = L"";
		color = 0;

		computeSolidGroup.Frames(
			solidData.signal
			, solidData.reference
			, solidData.start
			, solidData.stop
			, result
			, groupName
			, color
			);
	}
}
