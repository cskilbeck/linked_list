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

struct foo : list_node<foo>
{
	static int index;
    foo(int n) : p(n)
    {
		i = index++;
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
	int i;

    list_node<foo> node1;
    list_node<foo> node2;
};

int foo::index = 0;

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

template <typename T> void show_list(char const *t, T const &list)
{
	printf("%s\n", t);
	for(auto const &i: list)
	{
		printf("%d: %d\n", i.i, i.p);
	}
	printf("\n");
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

int __cdecl main(int, char **)
{
	if(0)
	{
		foo::index = 0;

		foo a(1);
		foo b(1);
		foo c(1);
		foo d(1);
		foo e(0);
		foo f(0);
		foo g(1);
		foo h(1);
		foo i(0);

		list1.push_back(a);
		list1.push_back(b);
		list1.push_back(c);
		list1.push_back(d);
		list1.push_back(e);
		list1.push_back(f);
		list1.push_back(g);
		list1.push_back(h);
		list1.push_back(i);

		show_list("1\\", list1);

		list1.sort();

		show_list("1\\", list1);

		list1.clear();
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

	printf(">\n");
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

	printf("<\n");
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

	printf("Press enter...");
	getchar();
    return 0;
}

