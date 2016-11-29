#include "stdafx.h"
#include "CommunicationIDItems.h"
#include <algorithm>

void CommunicationIDItems::LoadItem(wchar_t *name, int ind)
{
	type_value p;
	p.name = name;
	p.id = ind;
	p.status = eOk;
	items.push_back(p);
}

void CommunicationIDItems::Set(wchar_t *name, int ind)
{
	for(auto k = items.begin(); k != items.end(); ++k)
	{
		if(0 == wcscmp(k->name.c_str(), name))
		{
			k->id = ind;
			k->status = eAdd;
			return;
		}
		type_value p;
		p.name = name;
		p.id = ind;
		p.status = eAdd;
		items.push_back(p);
	}
}

int  CommunicationIDItems::Get(wchar_t *name)
{
	for(auto k = items.begin(); k != items.end(); ++k)
	{
		if(0 == wcscmp(k->name.c_str(), name))
		{
			return k->id;
		}
	}
	return 0;
}

void CommunicationIDItems::Add(wchar_t *name, int ind)
{
	for(auto k = items.begin(); k != items.end(); ++k)
	{
		if(0 == wcscmp(k->name.c_str(), name))
		{
			k->id = ind;
			k->status = eAdd;
			return;
		}
	}
	type_value p;
	p.name = name;
	p.id = ind;
	p.status = eAdd;
	items.insert(items.begin(), p);
}

void CommunicationIDItems::Clear()
{
	items.clear();
}

unsigned CommunicationIDItems::Count()
{
	return items.size();
}

int CommunicationIDItems::GetID(int ind)
{
	if((unsigned)ind < items.size())
	{
		return items[ind].id;
	}
	return 0;
}
wchar_t *CommunicationIDItems::GetName(int ind)
{
	if((unsigned)ind < items.size())
	{
		return (wchar_t *)items[ind].name.c_str();
	}
	return L"";
}

CommunicationIDItems::Status &CommunicationIDItems::GetStatus(int ind)
{
	if((unsigned)ind < items.size())
	{
		return items[ind].status;
	}
	static Status x;
	return x;
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