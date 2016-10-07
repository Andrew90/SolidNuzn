#pragma once
#include "MenuApi.h"
class SelectTypeSizeList: public TEvent
{
public:
   HWND hWnd;
   void (*ptr)(wchar_t *);
public: 
	SelectTypeSizeList();
	void Create(HWND);
	void Size(int x, int y, int width);
	void Do(TCommand &);
	void AddMenuItem(wchar_t *);
	void DelMenuItem(wchar_t *);
	void SelMenuItem(wchar_t *);
};