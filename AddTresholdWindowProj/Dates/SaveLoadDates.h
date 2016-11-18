#pragma once

struct SaveDateFile
{
	static void Do(HWND );
};

struct LoadDateFile
{
	static void Do(HWND );
};

void StoreDataFile(wchar_t *path);

bool FileExist(wchar_t *subDir, wchar_t *fileName);

wchar_t *CreateNameFile(wchar_t *subDir, wchar_t *typeSize, wchar_t *solidGroup, wchar_t (&)[1024]);