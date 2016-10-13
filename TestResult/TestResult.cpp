// TestResult.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ResultItems.h"
#include <clocale>

template<class O, class P>struct print
{
	void operator()()
	{
	   printf("%s\n", typeid(O).name());
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	 setlocale (LC_ALL,"");

	SetMeshure tt;

	tt.Clear();

	tt.Set(3);

	tt.Set(4);

	tt.Set(5);

	tt.Set(6);

	int res = tt.Result();

	printf("%d\n", res);

	wchar_t buf[1024];
	wchar_t *s = buf;
	tt.Text(res, s);
	wprintf(L"%s\n", buf);

	tt.Text(0, s);
	wprintf(L"%s\n", buf);

	wprintf(L"------------------------\n");

	for(int i = 0; i < tt.countMessages; ++i)
	{
		tt.Text(i, buf);
    	wprintf(L"%d  %s\n", i, buf);
	}

	getchar();
	return 0;
}

