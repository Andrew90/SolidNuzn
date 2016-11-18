#pragma once

namespace CounterTubes
{	
	void Load(wchar_t *typeName);
	void Inc(wchar_t *groupName);
	void Clear();
	int CountGroup();
	wchar_t *TypeName(unsigned);
	int CountType(unsigned);
	int CountTypeAll();
};