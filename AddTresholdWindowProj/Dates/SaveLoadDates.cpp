#include "stdafx.h"
#include "SaveLoadDates.h"
#include <stdio.h>
#include "window_tool/DlgFiles.h"
#include "SolidData.h"
#include <ShlWapi.h>
#include <time.h>
#include "templates\templates.hpp"
#include "App\AppBase.h"
//#include "Corel.h"
//#include "StoredData.h"

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
}

struct close_file
{
	FILE *f;
	close_file(FILE *f) : f(f){}
	~close_file(){if(NULL != f)fclose(f);}
};
#pragma warning(disable : 4996)
void SaveDateFile::Do(HWND h)
{
	SaveData o(h);
	bool b = false;
	if(solidData.currentOffset > 0 && o())
	{
		FILE *f= _wfopen(o.sFile, L"wb+");
		close_file c_f(f);
		if(NULL != f)
		{
			b = fwrite(&solidData.currentOffset, sizeof(solidData.currentOffset), 1, f)
		     && fwrite(solidData.reference, sizeof(double) * solidData.currentOffset, 1, f)
			 && fwrite(&solidData.signal, sizeof(double) * solidData.currentOffset, 1, f)
			 ;
		}
	}
	if(!b)
	{
		MessageBox(0, L"Файл не сохранён", L"Ошибка !!!", MB_ICONERROR);
	}
}
void LoadDateFile::Do(HWND h)
{
	OpenData o(h);
	bool b = false;
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"rb");
		close_file c_f(f);
		if(NULL != f)
		{
			//int len = wcslen(o.sFile);
			//wchar_t *s = o.sFile;
			//for(int i = len; i > 0; --i)
			//{
			//	if(s[i] == '\\' || s[i] == '/')
			//	{
			//		wcscpy(path, &s[i+1]);
			//		break;
			//	}
			//}
			if(fread(&solidData.currentOffset, sizeof(solidData.currentOffset), 1, f))
			{
				if(solidData.currentOffset < SolidData::MAX_ZONES_COUNT)
				{
					b = fread(solidData.reference, sizeof(double) * solidData.currentOffset, 1, f)
						&& fread(&solidData.signal, sizeof(double) * solidData.currentOffset, 1, f)
						;
				}
			}
		}
		if(!b)
		{
			MessageBox(0, L"Файл не загружен", L"Ошибка !!!", MB_ICONERROR);
		}
	}
}


void StoreDataFile()
{
	wchar_t path[1024];
	 GetModuleFileName(0, path, 1024);
	 PathRemoveFileSpec(path);
	 wcscat(path, L"\\Stored\\");
	 CreateDirectory(path, NULL);
	 time_t t = time(0);
	 struct tm *now = localtime( & t );
#pragma message("изменил формирование названия файла")
#pragma message("todo 2016 11 01")
	 wchar_t *nameParam = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	// wsprintf(&path[wcslen(path)], L"%s_%s_%02d%02d%02d_%02d%02d%02d.dat"
	//	 , nameParam
	//	 , corel.ClassTubeName()
	//	 , now->tm_year - 100
	//	 , now->tm_mon + 1
	//	 , now->tm_mday
	//	 , now->tm_hour
	//	 , now->tm_min
	//	 , now->tm_sec
	//	 );
	FILE *f= _wfopen(path, L"wb+");
	close_file c_f(f);
	if(NULL != f)
	{
		fwrite(&solidData.currentOffset, sizeof(solidData.currentOffset), 1, f)
			&& fwrite(solidData.reference, sizeof(double) * solidData.currentOffset, 1, f)
			&& fwrite(&solidData.signal, sizeof(double) * solidData.currentOffset, 1, f)
			;
	}
}