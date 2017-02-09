#include "stdafx.h"
#include "SelectTypeSizeList.h"

SelectTypeSizeList::SelectTypeSizeList(): ptr(NULL){}

void SelectTypeSizeList::Create(HWND h)
{
	hWnd = CreateWindow(L"COMBOBOX", NULL
		, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST|CBS_AUTOHSCROLL |WS_VSCROLL 
		, 0, 0, 0, 0, h, 0, (HINSTANCE)::GetModuleHandle(NULL), 0
		);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
	HFONT font = CreateFont(40,0,0,90,900,TRUE,0,0,ANSI_CHARSET,0,0,0,0, L"Times New Roman"); 
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, (LPARAM)0);
}

void SelectTypeSizeList::Size(int x, int y, int width)
{	
	MoveWindow(hWnd, x, y, width, 525, TRUE);
}

void SelectTypeSizeList::Do(TCommand &m)
{
	if(1 == m.isAcselerator)
	{
		wchar_t buf[128];
		GetWindowText(m.hControl, buf, dimention_of(buf));
		if(NULL != ptr) (*ptr)(m.hwnd, buf);
		SetFocus(m.hwnd);
	}
}

void SelectTypeSizeList::AddMenuItem(wchar_t *buf)
{
	ComboBox_AddString(hWnd, buf);
	int num = ComboBox_FindStringExact(hWnd, 0, buf);
	if(CB_ERR != num) ComboBox_SetCurSel(hWnd, num);
}

void SelectTypeSizeList::DelMenuItem(wchar_t *buf)
{
	int num = ComboBox_FindStringExact(hWnd, 0, buf);
	if(CB_ERR != num)
	{
		ComboBox_DeleteString(hWnd, num);
		ComboBox_SetCurSel(hWnd, 0);
		ComboBox_GetText(hWnd, buf, 128);
	}
	else
	{
		buf[0] = 0;
	}
}

void SelectTypeSizeList::SelMenuItem(wchar_t *buf)
{
	int num = ComboBox_FindStringExact(hWnd, 0, buf);
	if(CB_ERR != num) ComboBox_SetCurSel(hWnd, num);
}