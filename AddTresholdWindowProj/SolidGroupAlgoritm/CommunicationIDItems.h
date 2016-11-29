#pragma once
#include <vector>
class CommunicationIDItems
{	
public:
	enum Status{eOk, eCancel, eAdd};
	struct type_value
	{
	   std::wstring name;
	   int id;
	   Status status;
	};
	void LoadItem(wchar_t *, int);
	void Add(wchar_t *, int);
	void Clear();
	void Del(wchar_t *);
	void Update(wchar_t *pred, wchar_t *current, int id);
	std::vector<type_value> items;
};