#pragma once

class FrameWindowToolBar
{
public:
	HWND hWnd;
	HWND Init(HWND);
	void Size();
	static const int Width();
};