#include "stdafx.h"
#include "ResultItems.h"
#include <Windows.h>
#include <typeinfo>

namespace
{
	template<class List>struct SkipList;
	template<class Head, class Tail>struct SkipList<Tlst<Head, Tail>>
	{
		typedef typename SkipList<Tail>::Result Result;
	};
	template<class Tail, class T, template<class>class W>struct SkipList<Tlst<W<T>, Tail>>
	{
		typedef Tlst<W<T>, Tail> Result;
	};

	typedef SkipList<result_list>::Result sub_list;

	template<class List, int N>struct mk_sub_list;
	template<class Head, class Tail, int N>struct mk_sub_list<Tlst<Head, Tail>, N>
	{
		typedef typename mk_sub_list<Tail, N>::Result tmp;
		typedef typename TL::_if<0 != (N & (1 << TL::IndexOf<sub_list, Head>::value)), Tlst<Head, tmp>, tmp>::Result Result;
	};

	template<int N>struct mk_sub_list<NullType, N>
	{
		typedef NullType Result;
	};

	template<class List, class T>struct type_in_list;
	template<class Head, class Tail, class T>struct type_in_list<Tlst<Head, Tail>, T>
	{
		static const bool value = type_in_list<Tail, T>::value;
	};
	template<class Tail, class T>struct type_in_list<Tlst<T, Tail>, T>
	{
		static const bool value = true;
	};
	template<class T>struct type_in_list<NullType, T>
	{
		static const bool value = false;
	};

	template<class List>struct mk_sub_list_skip
	{
		static const bool value = 
			!(type_in_list<List, Defect<Cross>>::value && type_in_list<List, Class2<Cross>>::value
			|| type_in_list<List, Defect<Long>>::value && type_in_list<List, Class2<Long>>::value
			)
			&& 
			(TL::Length<List>::value > 1);
	};

	template<int N = 3, class tmp = NullType>struct mk_list
	{
		typedef typename mk_sub_list<sub_list, N>::Result sub;
		static const bool value = mk_sub_list_skip<sub>::value;
		typedef typename mk_list<N + 1
			, typename TL::Append<
			tmp
			, typename TL::_if<value, sub, NullType>::Result
			>::Result
		>::Result Result;
	};

	template<class List>struct __list_to_int__;

	template<class Head, class Tail>struct __list_to_int__<Tlst<Head, Tail> >
	{
		static const int value = (1 << TL::IndexOf<sub_list, Head>::value) | __list_to_int__<Tail>::value;
	};
	template<>struct __list_to_int__<NullType>
	{
		static const int value = 0;
	};

	template<class O, class P>struct __select_num__
	{
		bool operator()(P &p)
		{
			++p.res;
			return __list_to_int__<O>::value != p.data;
		}
	};

	struct __data__
	{
		unsigned data;
		unsigned res;
	};

	template<class tmp>struct mk_list<1 << TL::Length<sub_list>::value, tmp>
	{
		typedef tmp Result;
	};

	typedef mk_list<>::Result multy_result_list;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	typedef TL::AppendList<
		result_list
		, multy_result_list
	>::Result all_result_list;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool GetNumSubList(int num, int &res)
	{
		if(num < TL::Length<result_list>::value)
		{
			res = num;
			return true;
		}
		num -= TL::Length<result_list>::value;

		if(0 != (num & (1 << TL::IndexOf<sub_list, Defect<Cross>>::value))) num &=~(1 << TL::IndexOf<sub_list, Class2<Cross>>::value);
		if(0 != (num & (1 << TL::IndexOf<sub_list, Defect<Long>>::value))) num &=~(1 << TL::IndexOf<sub_list, Class2<Long>>::value);

		__data__ d = {num, 0};
		if(!TL::find<multy_result_list, __select_num__>()(d))
		{
			res = d.res + TL::Length<result_list>::value;   
			return true;
		}

		return false;
	}

	template<class List>struct NumMess;
	template<class Head, class Tail>struct NumMess<Tlst<Head, Tail>>
	{
		static const int value = (1 << TL::IndexOf<result_list, Head>::value) + NumMess<Tail>::value;
	};
	template<>struct NumMess<NullType>
	{
		static const int value = 0;
	};

	template<class T>struct MessageTxt;
	template<class T>struct ColorText;

	template<>struct MessageTxt<Above<Thick>>
	{
		wchar_t *operator()(){return L"толщина выше нормы";}
	};
	template<>struct MessageTxt<Below<Thick>>
	{
		wchar_t *operator()(){return L"толщина ниже нормы";}
	};
	template<>struct MessageTxt<Defect<Cross>>
	{
		wchar_t *operator()(){return L"поперечный дефект";}
	};
	template<>struct MessageTxt<Class2<Cross>>
	{
		wchar_t *operator()(){return L"поперечный 2 класс";}
	};
	template<>struct MessageTxt<Defect<Long>>
	{
		wchar_t *operator()(){return L"продольный дефект";}
	};
	template<>struct MessageTxt<Class2<Long>>
	{
		wchar_t *operator()(){return L"продольный 2 класс";}
	};

	template<>struct MessageTxt<DeathZone>
	{
		wchar_t *operator()(){return L"мёртвая зона";}
	};
	template<>struct MessageTxt<Norma>
	{
		wchar_t *operator()(){return L"норма";}
	};
	template<>struct MessageTxt<Undifined>
	{
		wchar_t *operator()(){return L"результат не определён";}
	};

	template<>struct ColorText<DeathZone>    {static const int value = 0xcccccc;};
	template<>struct ColorText<Norma>	     {static const int value = 0xff00;};
	template<>struct ColorText<Undifined>    {static const int value = 0x777777;};
	template<>struct ColorText<Defect<Cross>>{static const int value = 0xff0000;};
	template<>struct ColorText<Defect<Long>> {static const int value = 0xff0000;};
	template<>struct ColorText<Below<Thick>> {static const int value = 0xff0000;};
	template<>struct ColorText<Class2<Cross>>{static const int value = 0xffff00;};
	template<>struct ColorText<Class2<Long>> {static const int value = 0xffff00;};
	template<>struct ColorText<Above<Thick>> {static const int value = 0xff;};


	template<class O, class P>struct __Text__
	{
		void operator()(P &p)
		{
			wsprintf(p.txt, L"<%x>%s, ", ColorText<O>::value, MessageTxt<O>()());
			p.txt += wcslen(p.txt);
		}
	};

	template<class List, class tmp = NullType>struct __l__;
	template<class Head, class Tail, class tmp>struct __l__<Tlst<Head, Tail>, tmp>
	{
		template<class O>void operator()(O &o, int &d)
		{
			if(((Head &)o).value) 
			{
				__l__<Tail, TL::Append<tmp, typename TL::Inner<Head>::Result>::Result>()(o, d);

			}
			else
			{
				__l__<Tail, tmp>()(o, d);
			}
		}
	};

	template<class List, class T>struct OffsOf;
	template<class Head, class Tail, class T>struct OffsOf<Tlst<Head, Tail>, T>
	{
		static const int value = 1 + OffsOf<Tail, T>::value;
	};
	template<class Tail, class T>struct OffsOf<Tlst<T, Tail>, T>
	{
		static const int value = 0;
	};
	template<class T>struct OffsOf<NullType, T>
	{
		static const int value = TL::IndexOf<all_result_list, Undifined>::value;
	};

	template<class tmp>struct __l__<NullType, tmp>
	{
		template<class O>void operator()(O &, int &d)
		{
			printf("%s\n", typeid(tmp).name());
			d = OffsOf<all_result_list, tmp>::value;
		}
	};

	struct __text_data__
	{
		wchar_t *txt;
		int data;
	};
	template<class O>struct __print__
	{
		template<class Z>void operator()(Z &z)
		{
			wsprintf(z.txt, L"<%x>%s, ", ColorText<O>::value, MessageTxt<O>()());
		}
	};
	template<class Head, class Tail>struct __print__<Tlst<Head, Tail>>
	{
		typedef Tlst<Head, Tail>  O;
		template<class Z>void operator()(Z &z)
		{
			TL::foreach<O, __Text__>()(z);
		}
	};
	template<class O, class P>struct __txt__
	{
		bool operator()(P &p)
		{
			if(TL::IndexOf<all_result_list, O>::value == p.data)
			{
				__print__<O>()(p);
				return false;
			}
			return true;
		}
	};
}

SetMeshure::SetMeshure()
	: countMessages(TL::Length<all_result_list>::value)
{}

void SetMeshure::Clear()
{
	memset(&items, 0, sizeof(items));
}
void SetMeshure::Set(int val)
{
	if(val < TL::Length<result_list>::value) ((bool *)&items)[val] = true;
};

int SetMeshure::Result()
{
	if(items.get<Bool<DeathZone>>().value) return TL::IndexOf<all_result_list, DeathZone>::value;
	if(items.get<Bool<Defect<Cross>>>().value)	 items.get<Bool<Class2<Cross>>>().value = false;
	if(items.get<Bool<Defect<Long>>>().value)	 items.get<Bool<Class2<Long>>>().value = false;
	int res = TL::IndexOf<all_result_list, Undifined>::value; 
	__l__<TL::ListToWapperList<SkipList<result_list>::Result, Bool>::Result>()(items, res);
	if((res == TL::IndexOf<all_result_list, Undifined>::value) && items.get<Bool<Norma>>().value) return TL::IndexOf<all_result_list, Norma>::value;
	return res;
}

void SetMeshure::Text(int mess, wchar_t *txt)
{
	__text_data__ data = {txt, mess};
	TL::find<all_result_list, __txt__>()(data);
}
