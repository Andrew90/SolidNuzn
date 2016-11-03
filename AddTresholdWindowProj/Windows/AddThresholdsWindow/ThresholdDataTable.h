#pragma once
#include <Windows.h>
#include "TableData.h"
#include <CommCtrl.h>

class TresholtDataTable	: public TableData
{
public:
	void Header(HWND );
	unsigned Count();
	const wchar_t *Cell(int, int, LV_DISPINFO *);



	void Init()			{}		   
	void Add(wchar_t *)	{}		   
	void Del(wchar_t *)	{}		   


};