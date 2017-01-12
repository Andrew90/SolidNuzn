#pragma once
#include "window_tool\message.h"

namespace SelectHandler
{
	void Init(HWND);
	void Do(HWND, wchar_t *);

	void Restore();
}