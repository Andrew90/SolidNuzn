#include "stdafx.h"
#include "CommunicationIDItems.h"
#include <algorithm>

void CommunicationIDItems::LoadItem(wchar_t *name, int ind)
{
	type_value p;
	p.name = name;
	p.id = ind;
	items.push_back(p);
}

void CommunicationIDItems::Add(wchar_t *name, int ind)
{
	for(auto k = items.begin(); k != items.end(); ++k)
	{
		if(0 == wcscmp(k->name.c_str(), name))
		{
			k->id = ind;
			return;
		}
	}
	type_value p;
	p.name = name;
	p.id = ind;
	items.insert(items.begin(), p);
}

void CommunicationIDItems::Clear()
{
	items.clear();
}

void CommunicationIDItems::Del(wchar_t *name)
{
	for(auto i = items.begin(); i != items.end(); ++i)
	{
		if(0 == wcscmp(i->name.c_str(), name))
		{
			items.erase(i);
			break;
		}
	}
}

void CommunicationIDItems::Update(wchar_t *pred, wchar_t *current, int id)
{
	for(auto i = items.begin(); i != items.end(); ++i)
	{
		if(0 == wcscmp(i->name.c_str(), pred))
		{
			i->id = id;
			i->name = current;
			break;
		}
	}
}

unsigned CommunicationIDItems::GetID(wchar_t *name)
{
	for(auto i = items.begin(); i != items.end(); ++i)
	{
		if(0 == wcscmp(i->name.c_str(), name))
		{
			return i->id;
		}
	}
	return -1;
}