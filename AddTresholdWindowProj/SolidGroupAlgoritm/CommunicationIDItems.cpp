#include "stdafx.h"
#include "CommunicationIDItems.h"

void CommunicationIDItems::Set(wchar_t *name, int ind)
{
	for(auto k = items.begin(); k != items.end(); ++k)
	{
		if(0 == wcscmp(k->first.c_str(), name))
		{
			k->second = ind;
			return;
		}
	}
}

int  CommunicationIDItems::Get(wchar_t *name)
{
	for(auto k = items.begin(); k != items.end(); ++k)
	{
		if(0 == wcscmp(k->first.c_str(), name))
		{
			return k->second;
		}
	}
	return 0;
}

void CommunicationIDItems::Add(wchar_t *name, int ind)
{
	for(auto k = items.begin(); k != items.end(); ++k)
	{
		if(0 == wcscmp(k->first.c_str(), name))
		{
			k->second = ind;
			return;
		}
	}
	type_value p;
	p.first = name;
	p.second = ind;
	items.push_back(p);
}

void CommunicationIDItems::Clear()
{
	items.clear();
}

unsigned CommunicationIDItems::Count()
{
	return items.size();
}