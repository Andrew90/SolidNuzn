#pragma once
#include <vector>
class CommunicationIDItems
{	
public:
	typedef std::pair<std::wstring, int> type_value;
	void Set(wchar_t *, int);
	int Get(wchar_t *);
	void Add(wchar_t *, int);
	void Clear();
	unsigned Count();
private:
	std::vector<type_value> items;
};