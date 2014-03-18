//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <Windows.h>
#include <list>
#include <string>
#include <algorithm>

using std::string;

template <typename T> string to_string(T const &l, char const *separator = ",")
{
	string s;
	char const *sep = "";
	for(auto const &p: l)
	{
		s += sep;
		s += p.to_string();
		sep = separator;
	}
	sep = "\\";
	for(auto const &p: reverse(l))
	{
		s += sep;
		s += p.to_string();
		sep = separator;
	}
	return s;
}

template <typename T> void show_string(T const &l)
{
	string s("[");
	char const *sep = "";
	for(auto const &p : l)
	{
		s += sep;
		s += p.to_string();
		sep = ",";
	}
	OutputDebugString((s + "]\n").c_str());
}

#include "linked_list.h"

using chs::linked_list;
using chs::list_node;

//////////////////////////////////////////////////////////////////////

template<class T> struct in_reverse
{
    T &l;
    in_reverse(T &list) : l(list) {}

    auto begin() ->         decltype(l.rbegin())   { return l.rbegin(); } 
    auto begin() const ->   decltype(l.crbegin())  { return l.crbegin(); } 
    auto end() ->           decltype(l.rend())     { return l.rend(); } 
    auto end() const ->     decltype(l.crend())    { return l.crend(); } 
};

template<class T> in_reverse<T> reverse(T &l)
{
    return in_reverse<T>(l);
}

//////////////////////////////////////////////////////////////////////

struct foo : list_node<foo>
{
    foo(int n) : p(n)
    {
    }
	foo()
	{
		int n = rand();
		int n2 = rand();
		p = (n << 15) | n2;
	}
    ~foo()
    {
    }
	string to_string() const
	{
		char f[46];
		sprintf_s(f, "%d", p);
		return f;
	}
    bool operator < (foo const &a) const
    {
		return p < a.p;
    }
	bool operator == (int c)
	{
		return c == p;
	}
    int p;

    list_node<foo> node1;
    list_node<foo> node2;
};

//////////////////////////////////////////////////////////////////////

int gCopyConstructors = 0;
int gAssignmentOperators = 0;
int gMoveOperators = 0;

linked_list<foo> list0;

typedef linked_list<foo, &foo::node1> list_1t;

list_1t list1;
list_1t list2;
list_1t list3;

//////////////////////////////////////////////////////////////////////

template <typename T> void print_list(char const *h, T const &list)
{
    printf("%s (%s)\n", h, to_string(list).c_str());
}

//////////////////////////////////////////////////////////////////////

typedef unsigned __int64 uint64;

//////////////////////////////////////////////////////////////////////

class Timer
{
public:

	Timer(char const *name) : mName(name)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&mStart);
	}

	~Timer()
	{
		LARGE_INTEGER now;
		LARGE_INTEGER resolution64;
		QueryPerformanceCounter(&now);
		QueryPerformanceFrequency(&resolution64);
		volatile double total = (double)(now.QuadPart - mStart.QuadPart);
		total *= 1000.0;
		total /= (double)resolution64.QuadPart;
		printf("%s took %fms\n", mName, total);
	}

private:

	volatile LARGE_INTEGER mStart;
	char const *mName;
};

//////////////////////////////////////////////////////////////////////

foo foos[5000000];

int main(int, char **)
{
	if(1)
	{
		foo a(1);
		foo b(2);
		foo c(3);
		foo d(100);

		foo e(5);
		foo f(6);
		foo g(7);
		foo h(8);

		list1.push_back(a);
		list1.push_back(b);
		list1.push_back(c);
		list1.push_back(d);

		list2.push_back(e);
		list2.push_back(f);
		list2.push_back(g);
		list2.push_back(h);

		print_list("1", list1);
		print_list("2", list2);

		list_1t::merge(list1, list2);

//		list1.move_range_before(d, list2, e, e);

		print_list("1", list1);
		print_list("2", list2);

		list1.clear();
		list2.clear();
	}

	{
		for(int i=0; i<ARRAYSIZE(foos); ++i)
		{
			list1.push_back(foos[i]);
		}
	}

//	print_list("1", list1);

	{
		Timer t2("Sort list");
		list1.sort();
	}

	foo *f = list1.head();
	for(int i=0; i<25; ++i)
	{
		if(f == list1.done())
		{
			break;
		}
		printf("%d\n", f->p);
		f = list1.next(f);
	}

	f = list1.tail();
	for(int i=0; i<25; ++i)
	{
		if(f == list1.done())
		{
			break;
		}
		printf("%d\n", f->p);
		f = list1.prev(f);
	}




//	print_list("1", list1);

	getchar();
    return 0;
}

