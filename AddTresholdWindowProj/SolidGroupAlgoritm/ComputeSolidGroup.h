#pragma once
#include <string>
#include <vector>
#include <map>

struct GroupNameItem
{
	std::wstring name;
	unsigned color;
	bool added;
};

struct FileNameItem
{
   std::wstring name;
   bool added;
};

template<class>struct Singleton;

class ComputeSolidGroup
{
	friend Singleton<ComputeSolidGroup>;
	ComputeSolidGroup(){}
	ComputeSolidGroup &operator =(const ComputeSolidGroup &);
public:
	typedef	enum {enabled, disabled, deleted, new_item} TStatus;
	static const int count_points = 8;
	struct SolidItem
	{
		int ID;
		int groupName;
		int solidFile;
		int offset;
		double corelation;
		TStatus status;
		double root;
		double points[count_points];
		bool changed;
	};
public:
	std::map<int, GroupNameItem> groupNameList;
	std::map<int, FileNameItem> fileNameList;
	std::vector<SolidItem> solidItems;
public:
	std::wstring typeSizeName;	
	int persents[count_points];

	int frequencySamples;
	int frequencySignal;

	bool changeTresholds;
	std::wstring currentTypesize;
	void Clear();
	bool Load(wchar_t *);
	void Load();
	void Save();

	void AddThreshold(wchar_t *groupName, wchar_t *solidFile, int offset, double(&points)[count_points]);
	void DelThreshold(int id);

	void SetGroupName(int, wchar_t *);
	const wchar_t *GetGroupName(int);

	void UpdateTresholds();

	bool AddSolidGroup(wchar_t *);
	bool DelSolidGroup(wchar_t *);

	bool OneFrame(double (&)[count_points], double &, wchar_t *&, unsigned &);
	bool Frames(double* signal, double *reference, int start, int stop, double &, wchar_t *&, unsigned &);
private:
	void InitRootAll();
};





