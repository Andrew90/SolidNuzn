#pragma once
#include "Dates\ChartData.h"
#include "1730\Device1730.h"

class App
{
	struct Impl;
public:
	static bool isStandard;
	static void Init();
	static void Destroy();
	static void PrintTopLabel(wchar_t *);
	static void UpdateMainWindow();
	static void UpdateLoopMainWindow(int delay);
	static void AddMenuItem(wchar_t *);
	static void DelMenuItem(wchar_t *);
	static void SelMenuItem(wchar_t *);

	static void ClearCounter();
	static void CheckMenuItem();
	static void UpdateGroupCounter();

	static HWND MainWindowHWND();
	static HWND MainWindowToolBarHWND();
};

template<class>struct Singleton;

class L502Signal   : public ChartData
{
	friend Singleton<L502Signal>;
	L502Signal(): ChartData(){}
	L502Signal &operator =(const L502Signal &);
};
class L502Reference: public ChartData
{
	friend Singleton<L502Reference>;
	L502Reference(): ChartData(){}
	L502Reference &operator =(const L502Reference &);
};