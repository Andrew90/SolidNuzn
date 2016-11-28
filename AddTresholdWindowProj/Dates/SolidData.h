#pragma once
template<class T>struct Singleton;
class SolidData
{
	friend Singleton<SolidData>;
	SolidData();
	SolidData &operator=(const SolidData &);
public:
	static const int MAX_ZONES_COUNT = 10000 * 120; 
	int currentOffset;
	int start;
	int stop; 
	double reference[MAX_ZONES_COUNT];
	double signal[MAX_ZONES_COUNT];
public:	
	void Clear();
	void SetData(double *data, int count);
};

