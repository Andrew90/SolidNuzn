#pragma once

#include "templates/typelist.hpp"

struct Thick{};
struct Cross{};
struct Long{};

template<class T>struct Above;
template<class T>struct Below;

template<class T>struct Defect;
template<class T>struct Class2;

template<>struct Above<Thick>{};
template<>struct Below<Thick>{};

template<>struct Defect<Cross>{};
template<>struct Class2<Cross>{};

template<>struct Defect<Long>{};
template<>struct Class2<Long>{};

struct DeathZone{};
struct Norma{};
struct Undifined{};


typedef TL::MkTlst<
/*01*/	  DeathZone
/*02*/	, Norma
/*03*/	, Undifined	
/*04*/	, Defect<Cross>
/*05*/  , Defect<Long>
/*06*/  , Below<Thick>
/*07*/  , Class2<Cross>
/*08*/  , Class2<Long>
/*09*/  , Above<Thick>
>::Result result_list;

class SetMeshure
{
	template<class T>struct Bool
	{
		bool value;
	};
	typedef TL::ListToWapperList<result_list, Bool>::Result list;
	TL::Factory<list> items;
public:
	const int countMessages;
	SetMeshure();
	void Clear();
	void Set(int val);
	int Result();
	void Text(int mess, wchar_t *txt);
};