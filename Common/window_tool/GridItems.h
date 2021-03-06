#pragma once
#pragma warning (disable : 4995)
#include <windows.h>
#include <commctrl.h>
#include "message.h"
//----------------------------------------------------------------------
template<class T>struct header_table;
#define HEADER_TABLE(T, width_, txt)template<>struct header_table<T>\
{\
	LPWSTR name(){return txt;}\
	static const int width = width_;\
};
//-----------------------------------------------------------------
void SetRow(HWND hWnd, int count);
void UpdateRow(HWND h);
void UpdateRowAll(HWND h);
//-----------------------------------------------------------------
struct GridHandlers
{
	//item.iItem - ������ 
	//item.iSubItem -�������
	virtual void SetDataToGrid(LV_DISPINFO *){}
	virtual void SetColorToGrid(NMLVCUSTOMDRAW *){}
	virtual void SetHeaderToGrid(HWND){}
	virtual void ItemChanged(HWND){}
	virtual void Timer(HWND){}
	virtual void LClick(LPNMITEMACTIVATE ){}
	virtual void RClick(LPNMITEMACTIVATE ){}
	virtual ~GridHandlers(){}
};
//----------------------------------------------------------------