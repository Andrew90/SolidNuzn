#include "stdafx.h"
#include "SolidGroupAlgoritm/ComputeSolidGroup.h"
#include <algorithm>
#include <math.h>
#include "SolidBase.h"
#include "tools_debug/DebugMess.h"
#include "Dates\SaveLoadDates.h"

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
		typedef OffsetFile O;
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
			 p.solidItem.root += o.value * o.value;
		}
	};

	template<class O, class P>struct __get_thresholds__
	{
		bool operator()(int id, O &o, P &p)
		{	
			p.solidItem.ID = id;
			p.solidItem.root = 0;
			TL::foreach<typename O::items_list, __sql__>()(o.items, p);
			p.solidItem.root = sqrt(p.solidItem.root);
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

namespace
{
	template<class T, class D>struct __select_id__
	{
		bool operator()(T &t, D &d)
		{
			d.LoadItem(
				t.get<CommunicationTypeName>().value
				, t.get<CommunicationTypeID>().value
				);
			return false;
		}
	};
}

bool ComputeSolidGroup::Load(wchar_t *name)
{
	persentsChanged = false;
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
		typeSizeName = name;
		int id = Select<SolidParametersTable>(base).eq<NameParam>(nameParam.value).Execute(pt);

		communicationIDItems.Clear();
		//Select<CommunicationTypeTable>(base).eq<CurrentID>(id).ExecuteLoop<__select_id__>(communicationIDItems);
		if(0 == id)
		{
			pt.items.get<NameParam>().value = name;
			Insert_Into<SolidParametersTable>(pt, base).Execute();
			id = Select<SolidParametersTable>(base).eq<NameParam>(nameParam.value).Execute();
		}
		wchar_t *query =
			L"SELECT CommunicationTypeID, CommunicationTypeName"\
			L" FROM CommunicationTypeTable"\
			L" WHERE CurrentID=?"\
			L" ORDER BY CommunicationTypeName"
			;
		CMD(base).CommandText(query).Param(id)
			.ExecuteLoop<TL::MkTlst<CommunicationTypeID, CommunicationTypeName>::Result, __select_id__>(communicationIDItems);

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

		if(persentsChanged)
		{
			Update<SolidParametersTable>(base)
				.set<Offset<0>>(persents[0])
				.set<Offset<1>>(persents[1])
				.set<Offset<2>>(persents[2])
				.set<Offset<3>>(persents[3])
				.set<Offset<4>>(persents[4])
				.set<Offset<5>>(persents[5])
				.set<Offset<6>>(persents[6])
				.set<Offset<7>>(persents[7])
			.Where().ID(currentID).Execute();
			persentsChanged = false;
		}

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

//void ComputeSolidGroup::AddThreshold(wchar_t *groupName, wchar_t *solidFile, int offset, double(&points)[count_points])
//{
//	SolidItem s;
//
//	s.groupName = GetMapID(groupNameList, groupName);
//	s.solidFile = GetMapID(fileNameList, solidFile);
//
//	s.offset = offset;
//	memmove(s.points, points, sizeof(s.points));
//	s.status = new_item;
//	
//	changeTresholds = true;
//	double root = 0;
//	for(int i = 0; i < count_points; ++i)
//	{
//		double a = points[i];
//		root += a * a;
//	}
//	s.root = sqrt(root);
//
//	solidItems.insert(solidItems.begin(), s);
//}

void ComputeSolidGroup::AddThreshold()
{
	SolidItem s;
//	static int count = 0;
//	++count;
	//wchar_t buf[128];
	//wsprintf(buf, L"NONAME%2d", count);
	//todo сохранение порога

	wchar_t *solidFile = (wchar_t *)currentFile.c_str();
	wchar_t *solidGroup = L"NONAME";
	wchar_t *subDir = L"Config";

	if(!FileExist(subDir, solidFile))
	{
		wchar_t path[1024];
		GetModuleFileName(0, path, 1024);
		PathRemoveFileSpec(path);
		wsprintf(&path[wcslen(path)], L"\\%s", subDir);
		CreateDirectory(path, NULL);

		if('\0' == solidFile[0])
		{
			currentFile = solidFile = CreateNameFile(subDir
				, (wchar_t *)typeSizeName.c_str()
				, solidGroup
				, path
				);
		}
		else
		{
			wsprintf(&path[wcslen(path)], L"\\%s", solidFile);
		}
		StoreDataFile(path);
	}

	s.groupName = GetMapID(groupNameList, solidGroup);
	s.solidFile = GetMapID(fileNameList, solidFile);

	s.offset = start;
	memmove(s.points, points, sizeof(s.points));
	s.status = new_item;
	
	changeTresholds = true;
	double root = 0;
	for(int i = 0; i < count_points; ++i)
	{
		double a = points[i];
		root += a * a;
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

void ComputeSolidGroup::UpdateTresholds()
{
	//todo сделать смену точек
	changeTresholds = true;
	persentsChanged = true;
	double length = frequencySamples / (2 * 2 * frequencySignal);

	int offsets[count_points];

	for(int i = 0; i < count_points; ++i)
	{
	   offsets[i] = int(length * persents[i] / 100.0);
	}

	wchar_t path[1024];
	GetModuleFileName(0, path, 1024);
	PathRemoveFileSpec(path);
	int len = wcslen(path);
	wsprintf(&path[len], L"\\%s\\", L"Config");
	len = wcslen(path);


	for(auto i = solidItems.begin(); i != solidItems.end(); ++i)
	{
		wcscpy(&path[len], fileNameList[i->solidFile].name.c_str());
		FILE *f;
		f = _wfopen(path, L"rb");
		if(NULL != f)
		{
			int offset;
			fread(&offset, sizeof(int), 1, f);
			
			int i_offset = i->offset - 1;
			if(i_offset < 0) i_offset = 0;
			int ofs = i_offset * sizeof(double) + sizeof(int);
			fseek(f, ofs, SEEK_SET);

			double data[2];
			fread(data, sizeof(double), 2, f);
			double dY = data[0]/(data[1] - data[0]);
		    if(dY < 0) dY = -dY;

			for(int j = 0; j < count_points; ++j)
			{
//todo XZZZZZXXZXXZXXZXZXZ
				ofs = (i_offset + offsets[j] + offset) * sizeof(double) + sizeof(int);
				fseek(f, ofs, SEEK_SET);
				fread(data, sizeof(double), 2, f);
				double t = data[0] + dY * (data[1] - data[0]);

				ofs = (i_offset + offsets[j]) * sizeof(double) + sizeof(int);
				fseek(f, ofs, SEEK_SET);
				fread(data, sizeof(double), 2, f);
				t -= data[0] + dY * (data[1] - data[0]);
				i->points[j] = t;
			}
			dprint("%f %f %f %f %f %f %f %f\n"
				, i->points[0]
			    , i->points[1]
			    , i->points[2]
			    , i->points[3]
			    , i->points[4]
			    , i->points[5]
			    , i->points[6]
			    , i->points[7]
				);
			fclose(f);
			i->root = 0;
			for(int j = 0; j < count_points; ++j)
			{
				double x = i->points[j];
				i->root += x * x;
			}
			i->root = sqrt(i->root);
			i->changed = true;
		}
		else
		{
			i->status = deleted;
		}
	}	
}

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
			double a = points[k];
			t += a * a;
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
		int sumCount;
	};
}

bool ComputeSolidGroup::OneFrame(double (&points)[count_points], double &result, wchar_t *groupName, unsigned &color)
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
		res /= root * i->root;
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
			if(i->second.sumCorel > maxCorel)
			{
				maxCorel = i->second.sumCorel;
				maxOffs = i->first;
			}
			if(i->second.count > 1) i->second.sumCorel /= i->second.count;
		}
		result = corel[maxOffs].sumCorel;
		groupName = (wchar_t *)groupNameList[maxOffs].name.c_str();
		color	  =  groupNameList[maxOffs].color;
	}
	return b;
}

bool ComputeSolidGroup::Frames(double* signal, double *reference, int start, int stop
	, double &result, wchar_t *&groupName, unsigned &color)
{	
	double length = frequencySamples / (2 * 2 * frequencySignal);
	int minL = int(0.9 * length);
	int maxL = int(1.1 * length);

	int offsets[count_points];

	for(int i = 0; i < count_points; ++i)
	{
	   offsets[i] = int(length * persents[i] / 100.0);
	}

	double points[count_points];

	std::for_each(solidItems.begin(), solidItems.end(), [](SolidItem &i){i.sumCorrelation = 0;i.numberMatches = 0;});

	int numberPasses = 0;

	for(int i = start; i < stop; ++i)
	{
		while(reference[i] > 0 && i < stop) ++i;
		int startPeriod = i;

		if(reference[i - 1] < 0) continue;
       
		double dY = reference[i - 1]/(reference[i] - reference[i - 1]);
		if(dY < 0) dY = -dY;

		while(reference[i] < 0 && i < stop) ++i;
		int stopPeriod = i;

		int tLen = stopPeriod - startPeriod;
		if(tLen < minL || tLen > maxL) continue;

		for(int j = 0; j < count_points; ++j)
		{
			int offs = offsets[j];
			double y0 = signal[offs + startPeriod - 1];
			double y1 = signal[offs + startPeriod];
			points[j] = y0 + dY * (y1 - y0);

			y0 = reference[offs + startPeriod - 1];
			y1 = reference[offs + startPeriod];
			points[j] -= y0 + dY * (y1 - y0);
		}

		double root = 0;
		for(int j = 0; j < count_points; ++j)
		{
			double a = points[j];
			root += a * a;
		}
		root = sqrt(root);
		int maxCorelItem = -1;
		int ind = 0;
		double maxCor = 0;
		for(auto j = solidItems.begin(); j != solidItems.end(); ++j)
		{
			double *a = j->points;
			double res = 0;
			for(int k = 0; k < count_points; ++k)
			{
				res += a[k] * points[k];
			}
			res /= root * j->root;
			j->corelation = res;
			j->sumCorrelation += res;
			

			if(enabled == j->status && res >= maxCor)
			{
				maxCorelItem = ind;
				maxCor = res;
			}
			++ind;
		}
		++numberPasses;
		if(-1 != maxCorelItem) ++solidItems[maxCorelItem].numberMatches;		
	}

	if(0 == numberPasses) numberPasses = 1;
	std::for_each(solidItems.begin(), solidItems.end(), [numberPasses](SolidItem &i){i.corelation = i.sumCorrelation / numberPasses;});

	std::map<int, CountCorel> corel;
	for(auto j = solidItems.begin(); j != solidItems.end(); ++j)
	{
		corel[j->groupName].count += j->numberMatches;
		corel[j->groupName].sumCorel += j->corelation;		  //todo 1
		++corel[j->groupName].sumCount;
	}
	int ind = 0;
	int countItems = 0;
	double maxCorel = 0;
	int first = -1;
	for(auto j = corel.begin(); j != corel.end(); ++j)
	{
		if(j->second.count > 0)
		{
			j->second.sumCorel /= j->second.sumCount;  //todo 2
			if(j->second.count > countItems)
			{
				maxCorel = ind;
				countItems = j->second.count;
				maxCorel = j->second.sumCorel;
				first = j->first;
			}
			else if(j->second.count == countItems)
			{
				maxCorel = ind;
				maxCorel = j->second.sumCorel;
				first = j->first;
			}
			++ind;
		}
	}

	if(-1 != first)
	{
		result = 0;
		for(auto j = solidItems.begin(); j != solidItems.end(); ++j)
		{
			if(j->groupName == first && j->corelation > result) result = j->corelation;
		}
		groupName = (wchar_t *)groupNameList[first].name.c_str();
		color	  =  groupNameList[first].color;
		return true;
	}
	return false;
}
//Скопипастено см. ComputeSolidGroup::Frames
bool ComputeSolidGroup::FramesOne(double(&points)[count_points], int(&offsets)[count_points],double *signal, double *reference
	, int &start, int stop, double &result, wchar_t *&groupName, unsigned &color)
{
	double length = frequencySamples / (2 * 2 * frequencySignal);
	int minL = int(0.9 * length);
	int maxL = int(1.1 * length);

	//int offsets[count_points];

	for(int i = 0; i < count_points; ++i)
	{
	   offsets[i] = int(length * persents[i] / 100.0);
	}

	//double points[count_points];

	std::for_each(solidItems.begin(), solidItems.end(), [](SolidItem &i){i.sumCorrelation = 0;i.numberMatches = 0;});

	int numberPasses = 0;

	for(int i = start; i < stop; ++i)
	{
		while(reference[i] > 0 && i < stop) ++i;
		int startPeriod = i;

		if(reference[i - 1] < 0) continue;
       
		double dY = reference[i - 1]/(reference[i] - reference[i - 1]);
		if(dY < 0) dY = -dY;

		while(reference[i] < 0 && i < stop) ++i;
		int stopPeriod = i;

		int tLen = stopPeriod - startPeriod;
		if(tLen < minL || tLen > maxL) continue;

		for(int j = 0; j < count_points; ++j)
		{
			int offs = offsets[j] + startPeriod;
			double y0 = signal[offs - 1];
			double y1 = signal[offs];
			points[j] = y0 + dY * (y1 - y0);

			y0 = reference[offs - 1];
			y1 = reference[offs];
			points[j] -= y0 + dY * (y1 - y0);
		}

		double root = 0;
		for(int j = 0; j < count_points; ++j)
		{
			double a = points[j];
			root += a * a;
		}
		root = sqrt(root);
		int maxCorelItem = -1;
		int ind = 0;
		double maxCor = 0;
		for(auto j = solidItems.begin(); j != solidItems.end(); ++j)
		{
			double *a = j->points;
			double res = 0;
			for(int k = 0; k < count_points; ++k)
			{
				res += a[k] * points[k];
			}
			res /= root * j->root;
			j->corelation = res;
			j->sumCorrelation += res;
			

			if((enabled == j->status || new_item == j->status) && res >= maxCor)
			{
				maxCorelItem = ind;
				maxCor = res;
			}
			++ind;
		}
		++numberPasses;
		if(-1 != maxCorelItem) ++solidItems[maxCorelItem].numberMatches;
		start = startPeriod;
		break;
	}

	if(0 == numberPasses) numberPasses = 1;
	std::for_each(solidItems.begin(), solidItems.end(), [numberPasses](SolidItem &i){i.corelation = i.sumCorrelation / numberPasses;});

	std::map<int, CountCorel> corel;
	for(auto j = solidItems.begin(); j != solidItems.end(); ++j)
	{
		corel[j->groupName].count += j->numberMatches;
		corel[j->groupName].sumCorel += j->corelation;
		++corel[j->groupName].sumCount;
	}
	int ind = 0;
	int countItems = 0;
	double maxCorel = 0;
	int first = -1;
	for(auto j = corel.begin(); j != corel.end(); ++j)
	{
		if(j->second.count > 0)
		{
			j->second.sumCorel /= j->second.sumCount;
			if(j->second.count > countItems)
			{
				maxCorel = ind;
				countItems = j->second.count;
				maxCorel = j->second.sumCorel;
				first = j->first;
			}
			else if(j->second.count == countItems)
			{
				maxCorel = ind;
				maxCorel = j->second.sumCorel;
				first = j->first;
			}
			++ind;
		}
	}

	if(-1 != result)
	{
		result = 0;
		for(auto j = solidItems.begin(); j != solidItems.end(); ++j)
		{
			if(j->groupName == first && j->corelation > result) result = j->corelation;
		}
		groupName = (wchar_t *)groupNameList[first].name.c_str();
		color	  =  groupNameList[first].color;
		return true;
	}
	return false;	
}

