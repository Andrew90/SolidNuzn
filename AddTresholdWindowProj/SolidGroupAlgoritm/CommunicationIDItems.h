#pragma once
#include <vector>
class CommunicationIDItems
{	
public:
	enum Status{eOk, eCancel, eAdd};
	//typedef std::pair<std::wstring, int> type_value;
	struct type_value
	{
	   std::wstring name;
	   int id;
	   Status status;
	};
	void LoadItem(wchar_t *, int);
	void Set(wchar_t *, int);
	int Get(wchar_t *);
	void Add(wchar_t *, int);
	void Clear();
	unsigned Count();
	int GetID(int);
	wchar_t *GetName(int);
	Status &GetStatus(int);
	void Del(wchar_t *);
	void Update(wchar_t *pred, wchar_t *current, int id);
//private:
	std::vector<type_value> items;
};