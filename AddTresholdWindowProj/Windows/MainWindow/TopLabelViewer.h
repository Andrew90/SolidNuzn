#pragma once
#include "window_tool/message.h"
#include "Graphics/ColorLabel.h"

class TopLabelViewer
{	
	Gdiplus::Bitmap *backScreen;
public:
	ColorLabel label;
public:
	HWND hWnd;
	TopLabelViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TUser &);
	void SetMessage(wchar_t *);
};

//extern TopLabelViewer topLabelViewer;