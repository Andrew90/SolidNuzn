#pragma once
#include "window_tool\message.h"
#include  "MainWindow\MainWindowToolBar.h"
#include "window_tool\SelectTypeSizeList.h"
#include "window_tool\CheckBoxWidget.h"
#include "App\AppBase.h"
#include "Viewers\CrossViewer.h"
#include "Viewers\LongViewer.h"
#include "Viewers\ThickViewer.h"
#include "Viewers\ResultViewer.h"
#include "TopLabel\TopLabelViewer.h"

void CheckBoxStateStoreInBase();

template<class T>struct OnTheJobCheckBox
{
protected:
	void Command(TCommand &m, bool b)
	{
		Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<T> >().value = b;
		CheckBoxStateStoreInBase();
	}
	bool Init(HWND h)
	{		
		return Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<T> >().value;
	}
};

class MainWindow
{
public:
	typedef TL::MkTlst<CrossViewer, LongViewer, ThickViewer, ResultViewer>::Result viewers_list;
	HWND hWnd;
	HWND hStatusWindow;
	MainWindowToolBar toolBar;
	SelectTypeSizeList &select;
	CheckBoxWidget<OnTheJobCheckBox<Cross> > crossCheckBox;
	CheckBoxWidget<OnTheJobCheckBox<Long > > longCheckBox;
	CheckBoxWidget<OnTheJobCheckBox<Thick> > thickCheckBox;
	TL::Factory<viewers_list> viewers;
	TopLabelViewer topLabel;
	MainWindow();
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);

	void operator()(TUser &);
	void operator()(TClose &);
	void operator()(TMouseWell &);

	void operator()(TGetMinMaxInfo &);
};