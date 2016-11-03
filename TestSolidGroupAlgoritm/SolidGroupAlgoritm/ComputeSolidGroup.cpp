#include "stdafx.h"
#include "SolidGroupAlgoritm/ComputeSolidGroup.h"
#include <algorithm>
#include <math.h>
#include "SolidBase.h"
#include "tools_debug/DebugMess.h"

namespace
{
	template<class O, class P>struct __parameters_table__;
	template<int N, template<int>class W, class P>struct __parameters_table__<W<N>, P>
	{
		typedef W<N> O;
		void operator()(O &o, P &p)
		{
			p.persents[N] = o.value;
		}
	};
	template<class P>struct __parameters_table__<NameParam, P>
	{
		typedef NameParam O;
		void operator()(O &o, P &p)
		{
			p.typeSizeName = o.value;
		}
	};
	template<class P>struct __parameters_table__<FrequencySignal, P>
	{
		typedef FrequencySignal O;
		void operator()(O &o, P &p)
		{
			p.frequencySignal = o.value;
		}
	};
	template<class P>struct __parameters_table__<FrequencySamples, P>
	{
		typedef FrequencySamples O;
		void operator()(O &o, P &p)
		{
			p.frequencySamples = o.value;
		}
	};
//------------------------------------------------------------------------------------------------
	struct __sql_data__
	{
	   ComputeSolidGroup &self;
	   ComputeSolidGroup::SolidItem solidItem;
	  CBase &base;
	   __sql_data__(ComputeSolidGroup &self, CBase &base)
		   : self(self)
		   , base(base)
	   {}
	};
	template<class O, class P>struct __sql__;

	template<class P>struct __sql__<ID<GroupName>, P>
	{
		typedef ID<GroupName> O;
		void operator()(O &o, P &p)
		{
			try
			{
				p.solidItem.groupName = o.value;
				p.self.groupNameList.at(o.value);
			}
			catch(...)
			{
				GroupTable t;
				int id = Select<GroupTable>(p.base).ID(o.value).Execute(t);
				if(0 != id)
				{
					p.self.groupNameList[o.value].name = t.items.get<GroupName>().value;
					p.self.groupNameList[o.value].color = t.items.get<GroupColor>().value;
					p.self.groupNameList[o.value].added = false;
				}
			}
		}
	};

	template<class P>struct __sql__<ID<FileNameTable>, P>
	{
		typedef ID<FileNameTable> O;
		void operator()(O &o, P &p)
		{
			try
			{
				p.solidItem.solidFile = o.value;
				p.self.fileNameList.at(o.value);
			}
			catch(...)
			{
				FileNameTable t;
				int id = Select<FileNameTable>(p.base).ID(o.value).Execute(t);
				if(0 != id)
				{
					p.self.fileNameList[o.value].name = t.items.get<SolidFileName>().value;
					p.self.fileNameList[o.value].added = false;
				}
			}
		}
	};

	template<class P>struct __sql__<ID<SolidParametersTable>, P>
	{
		typedef ID<SolidParametersTable> O;
		void operator()(O &o, P &p)
		{}
	};

	template<class P>struct __sql__<Enable, P>
	{
		typedef Enable O;
		void operator()(O &o, P &p)
		{
			 p.solidItem.status = (ComputeSolidGroup::TStatus)o.value;
			 p.solidItem.changed = false;
		}
	};

	template<class P>struct __sql__<OffsetFile, P>
	{
		typedef Enable O;
		void operator()(O &o, P &p)
		{
			 p.solidItem.offset = o.value;
		}
	};

	template<int N, class P>struct __sql__<Point<N>, P>
	{
		typedef Point<N> O;
		void operator()(O &o, P &p)
		{
			 p.solidItem.points[N] = o.value;
		}
	};

	template<class O, class P>struct __get_thresholds__
	{
		bool operator()(int id, O &o, P &p)
		{	
			p.solidItem.ID = id;
			TL::foreach<typename O::items_list, __sql__>()(o.items, p);
			p.self.solidItems.push_back(p.solidItem);
			return false;
		}
	};
//-----------------------------------------------------------------------------------------------
}

void ComputeSolidGroup::Clear()
{
	changeTresholds = false;
	groupNameList.clear();
	fileNameList.clear();
	solidItems.clear();
}

bool ComputeSolidGroup::Load(wchar_t *name)
{
	Clear();
	SolidBase sb;
	CBase base(
		sb.name()
		, CreateDataBase<SolidBase::type_list, SetDefault<SolidBase::type_list> >()
		, sb.tables
		);
	if(base.IsOpen())
	{
		SolidParametersTable pt;
		NameParam nameParam;
		nameParam.value = name;
		typeSizeName = name;
		int id = Select<SolidParametersTable>(base).eq<NameParam>(nameParam.value).Execute(pt);

		TL::foreach<SolidParametersTable::items_list, __parameters_table__>()(
			pt.items
			, *this
			);

		return Select<TresholdsTable>(base).eq<ID<SolidParametersTable>>(id).ExecuteLoop<__get_thresholds__>(__sql_data__(*this, base));
	}
	return false;
}

void ComputeSolidGroup::Load()
{
	//changeTresholds = false;
	Load((wchar_t *)typeSizeName.c_str());
}

namespace 
{
	template<class O, class P>struct __set_tresh__{void operator()(O &, P &){}};

#define SET_TRESH(type, val) template<class P>struct __set_tresh__<type, P>\
	{\
		void operator()(type &o, P &p)\
		{\
			o.value = p->val;\
		}\
	};

	SET_TRESH(ID<GroupName>, groupName)
	SET_TRESH(ID<FileNameTable>, solidFile)
	SET_TRESH(Enable, status)
	SET_TRESH(OffsetFile, offset)

	template<int N, class P>struct __set_tresh__<Point<N>, P>
	{
		void operator()(Point<N> &o, P &p)
		{
			 o.value = p->points[N];
		}
	};

#undef SET_TRESH
}

void ComputeSolidGroup::Save()
{
	CBase base(
		SolidBase().name()
		);
	changeTresholds = false;
	if(base.IsOpen())
	{
		NameParam::type_value val;
		val = (wchar_t *)typeSizeName.c_str();
		int currentID = Select<SolidParametersTable>(base).eq<NameParam>(val).Execute();

		{
			std::map<int, GroupNameItem> tmpGroupName;
			for(auto i = groupNameList.begin(); i != groupNameList.end();)
			{
				if(i->second.added)
				{
					GroupTable t;
					t.items.get<GroupName>().value = (wchar_t *)i->second.name.c_str();
					t.items.get<GroupColor>().value = i->second.color;
					int id = Select<GroupTable>(base).eq<GroupName>(t.items.get<GroupName>().value).Execute();
					if(0 == id)
					{						
						Insert_Into<GroupTable>(t, base).Execute();
						id = Select<GroupTable>(base).eq_all<GroupTable::items_list>(&t.items).Execute();
					}
					else
					{
						UpdateWhere<GroupTable>(t, base).ID(id).Execute();
					}
					
					int i_first = i->first;

					std::for_each(solidItems.begin(), solidItems.end()
						, [i_first, id](SolidItem &val){if(i_first == val.groupName)val.groupName = id;});

					i = groupNameList.erase(i);
					GroupNameItem g = {t.items.get<GroupName>().value, t.items.get<GroupColor>().value, false};
					tmpGroupName[id] = g;
				}
				else ++i;
			}
			if(tmpGroupName.size())
			{
				groupNameList.insert(tmpGroupName.begin(), tmpGroupName.end());
			}
		}
//-------------------------------------------------------------------------------
		{
			std::map<int, FileNameItem> tmpFileName;
			for(auto i = fileNameList.begin(); i != fileNameList.end();)
			{
				if(i->second.added)
				{
					FileNameTable t;
					t.items.get<SolidFileName>().value = (wchar_t *)i->second.name.c_str();
					Insert_Into<FileNameTable>(t, base).Execute();
		
					int id = Select<FileNameTable>(base).eq_all<FileNameTable::items_list>(&t.items).Execute();
					int i_first = i->first;
		
					std::for_each(solidItems.begin(), solidItems.end()
						, [i_first, id](SolidItem &val){if(i_first == val.solidFile)val.solidFile = id;});
		
					i = fileNameList.erase(i);
					FileNameItem g = {t.items.get<SolidFileName>().value, false};
					fileNameList[id] = g;
				}
				else ++i;
			}
			if(tmpFileName.size())
			{
				fileNameList.insert(tmpFileName.begin(), tmpFileName.end());
			}
		}
//-------------------------------------------------------------------------------
		for(auto i = solidItems.begin(); i != solidItems.end(); ++i)
		{
			if(deleted == i->status)
			{
				Delete<TresholdsTable>(base).ID(i->ID).Execute();
			}
			else if(new_item == i->status)
			{
				i->status = enabled;
				TresholdsTable t;
				TL::foreach<TresholdsTable::items_list, __set_tresh__>()(t.items, i);
				t.items.get<ID<SolidParametersTable>>().value = currentID;
				Insert_Into<TresholdsTable>(t, base).Execute();
				i->ID = Select<TresholdsTable>(base).eq_all<TresholdsTable::items_list>(&t.items).Execute();
			}
			else if(i->changed)
			{
				i->changed = false;
				TresholdsTable t;
				TL::foreach<TresholdsTable::items_list, __set_tresh__>()(t.items, i);
				t.items.get<ID<SolidParametersTable>>().value = currentID;
				UpdateWhere<TresholdsTable>(t, base).ID(i->ID).Execute();
			}
		}
		solidItems.erase(
			std::remove_if(solidItems.begin(), solidItems.end()
				, [](const SolidItem &val){return ComputeSolidGroup::deleted == val.status;})
					, solidItems.end()
		);
	}
}

namespace
{
	template<class T>int GetMapID(std::map<int, T> &m, wchar_t *name)
	{
		int maxIndex = 1;
		if(m.size())
		{
			for(auto i = m.begin(); i != m.end(); ++i)
			{
				if(0 == wcscmp((wchar_t *)i->second.name.c_str(), name)) return i->first;
			}
			maxIndex = m.rbegin()->first + 1;
		}
		
		m[maxIndex].name = name;
		m[maxIndex].added = true;
		return maxIndex;
	}
}

void ComputeSolidGroup::AddThreshold(wchar_t *groupName, wchar_t *solidFile, int offset, double(&points)[count_points])
{
	SolidItem s;

	s.groupName = GetMapID(groupNameList, groupName);
	s.solidFile = GetMapID(fileNameList, solidFile);

	s.offset = offset;
	memmove(s.points, points, sizeof(s.points));
	s.status = new_item;
	
	changeTresholds = true;
	double root = 0;
	for(int i = 0; i < count_points; ++i)
	{
		root += points[i];
	}
	s.root = sqrt(root);

	solidItems.insert(solidItems.begin(), s);
}

void ComputeSolidGroup::DelThreshold(int id)
{
	solidItems[id].status = deleted;
}

void ComputeSolidGroup::SetGroupName(int row, wchar_t *name)
{
	solidItems[row].groupName = GetMapID(groupNameList, name);
	solidItems[row].changed = true;
}

const wchar_t *ComputeSolidGroup::GetGroupName(int i)
{
	return groupNameList[solidItems[i].groupName].name.c_str();
}

void ComputeSolidGroup::UpdateTresholds(){}

namespace
{
	template<class O, class P>struct __new_group__;
	template<class P>struct __new_group__<NameParam, P>
	{
		typedef NameParam O;
		void operator()(O &o, P &p)
		{
			//o.value = (wchar_t *)p.typeSizeName.c_str();
		}
	};
	template<class P>struct __new_group__<FrequencySignal, P>
	{
		typedef FrequencySignal O;
		void operator()(O &o, P &p)
		{
			o.value = p.frequencySignal;
		}
	};
	template<class P>struct __new_group__<FrequencySamples, P>
	{
		typedef FrequencySamples O;
		void operator()(O &o, P &p)
		{
			o.value = p.frequencySamples;
		}
	};
	template<int N, class P>struct __new_group__<Offset<N>, P>
	{
		typedef Offset<N> O;
		void operator()(O &o, P &p)
		{
			o.value = p.persents[N];
		}
	};
}

bool ComputeSolidGroup::AddSolidGroup(wchar_t *name)
{
	int id = 0;
	bool newTypesize = false;

	{
		CBase base(
			SolidBase().name()
			);

		if(base.IsOpen())
		{
			SolidParametersTable pt;
			NameParam &nameParam = pt.items.get<NameParam>();
			nameParam.value = name;
			id = Select<SolidParametersTable>(base).eq<NameParam>(nameParam.value).Execute(pt);
			if(0 == id)
			{
				TL::foreach<SolidParametersTable::items_list, __new_group__>()(pt.items, *this); 
				Insert_Into<SolidParametersTable>(pt, base).Execute();
				id = Select<SolidParametersTable>(base).eq_all<SolidParametersTable::items_list>(&pt.items).Execute();
				newTypesize = true;
			}

		}
	}

	if(newTypesize)	return Load(name);
	return 0 != id;
}

bool ComputeSolidGroup::DelSolidGroup(wchar_t *name)
{
	CBase base(
		SolidBase().name()
		);

	if(base.IsOpen())
	{
		int count = CountRowsTable<SolidParametersTable>()(base);
		if(count > 1)
		{
			NameParam nameParam;
			nameParam.value = name;
			int id = Select<SolidParametersTable>(base).eq<NameParam>(nameParam.value).Execute();
			if(0 != id)
			{
				Delete<SolidParametersTable>(base).ID(id).Execute();

				wchar_t *sql0 = L"DELETE TresholdsTable"\
					L" FROM TresholdsTable LEFT JOIN SolidParametersTable"\
					L" ON TresholdsTable.SolidParametersTableID = SolidParametersTable.ID"\
					L" WHERE SolidParametersTable.ID IS NULL"
					;
				CMD(base).CommandText(sql0).Execute();

				wchar_t *sql1 = L"DELETE FileNameTable"\
					L" FROM FileNameTable LEFT JOIN TresholdsTable"\
					L" ON FileNameTable.ID = TresholdsTable.FileNameTableID"\
					L" WHERE TresholdsTable.FileNameTableID IS NULL"
					;
				CMD(base).CommandText(sql1).Execute();

				wchar_t *sql2 = L"DELETE GroupName"\
					L" FROM GroupName LEFT JOIN TresholdsTable"\
					L" ON GroupName.ID = TresholdsTable.GroupNameID"\
					L" WHERE TresholdsTable.GroupNameID IS NULL"
					;
				CMD(base).CommandText(sql2).Execute();
				return true;
			}
		}
	}
	return false;
}

void ComputeSolidGroup::InitRootAll()
{
	for(auto i = solidItems.begin(); i != solidItems.end(); ++i)
	{
		double t = 0;
		double *points = i->points;
		for(int k = 0; k < count_points; ++k)
		{
			t += points[k];
		}
		i->root = sqrt(t);
	}
}

namespace
{
	struct CountCorel
	{
		int count;
		double sumCorel;
	};
}

bool ComputeSolidGroup::Compute(double (&points)[count_points], double &result, const wchar_t *groupName, unsigned &color)
{
	std::map<int, CountCorel> corel;

	double root = 0;
	for(int i = 0; i < count_points; ++i)
	{
		root += points[i];
	}
	root = sqrt(root);

	for(auto i = solidItems.begin(); i != solidItems.end(); ++i)
	{
		double *a = i->points;
		double res = 0;
		for(int j = 0; j < count_points; ++j)
		{
		   res += a[j] * points[j];
		}
		res = res /(root * i->root);
		i->corelation = res;
		if(enabled == i->status)
		{
			++corel[i->groupName].count;
			corel[i->groupName].sumCorel += res;
		}
	}
	bool b = false;
	if(corel.size() > 0)
	{
		b = true;
		int maxOffs = 0;
		double maxCorel = 0;
		for(auto i = corel.begin();  i != corel.end(); ++i)
		{
			if(i->second.count > 1) i->second.sumCorel /= i->second.count;
			if(i->second.sumCorel > maxCorel)
			{
				maxCorel = i->second.sumCorel;
				maxOffs = i->first;
			}
		}
		result = corel[maxOffs].sumCorel;
		groupName = groupNameList[maxOffs].name.c_str();
		color	  =  groupNameList[maxOffs].color;
	}
	return b;
}
