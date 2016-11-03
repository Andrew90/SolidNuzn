// TestSolidGroupAlgoritm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include "SolidGroupAlgoritm/SolidBase.h"
#include "templates\typelist.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	

	std::map<int, std::string> m;
	char buf[16];

	for(int i = 0; i < 10; ++i)
	{
		try
		{
			m.at(i);
		}
		catch(...)
		{
			m[i] = strcat(_itoa(i, buf, 10), "__");
		}
	}
	std::map<int, std::string> mm;
	for(int i = 0; i < 10; ++i)
	{
		try
		{
			mm.at(i);
		}
		catch(...)
		{
			mm[i] = strcat(_itoa(i, buf, 10), "__mm");
		}
	}

	std::for_each(m.begin(), m.end()
		, [](const std::pair<int, std::string> &i){std::cout << i.first << "  " << i.second << std::endl;});

	std::cout<< "----------------------------\n";

	

	

	for(auto i = m.begin(); i != m.end();)
	{
		if(i->first % 2 == 0) i = m.erase(i);
		else ++i;
	}

	std::for_each(m.begin(), m.end()
		, [](const std::pair<int, std::string> &i){std::cout << i.first << "  " << i.second << std::endl;});
	
	std::cout<< "----------------------------\n";
	m.insert(mm.begin(), mm.end());
	std::for_each(m.begin(), m.end()
		, [](const std::pair<int, std::string> &i){std::cout << i.first << "  " << i.second << std::endl;} );
   std::cout<< "----------------------------\n";
   std::cout << m.rbegin()->first << std::endl;
   std::cout<< "----------------------------\n";
	int t = 111111;
	
	int xx[] = {1,2,3};
	std::vector<int> v(xx, &xx[3]);

	 v.insert(v.begin(), t);

	 std::for_each(v.begin(), v.end()
		, [](const int &i){std::cout << i << std::endl;});
	
	
	//Initialize initialize;
	//
	//SolidBase().Init();
	//
	//ComputeSolidGroup &s =Singleton<ComputeSolidGroup>::Instance();
	//
	//double p[] = {4,2,3,4,5,6,7,8};
	//
	//s.AddThreshold(L"D", L"FileD", 5234, p);
	//
	//s.Save();

	getchar();
	return 0;
}

