#pragma once
#include "window_tool/message.h"

class ColorPanel
{
	wchar_t buf[256];
	unsigned backColor;
	Gdiplus::Bitmap *backScreen;
	void __SetText__(wchar_t *, unsigned);
	void __Clear__();
public:
   HWND hWnd;
	ColorPanel();
	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	static void SetText(wchar_t *, unsigned);
	static void Clear();
};