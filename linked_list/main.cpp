//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <Windows.h>
#include <list>
#include <string>
#include <algorithm>
#include "linked_list.h"

using std::string;

using chs::linked_list;
using chs::list_node;

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

struct foo
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
};

//////////////////////////////////////////////////////////////////////

typedef linked_list<foo, &foo::node1> list_1t;

list_1t list1;

//////////////////////////////////////////////////////////////////////

foo foos[5000000];

int __cdecl main(int, char **)
{
//	for(int l=1; l<636; ++l)
	{
//		printf("%d\n", l);
		list1.clear();
		for(int i=0; i<5000000; ++i)
		{
//			foos[i].p = l - i;
			list1.push_back(foos[i]);
		}
		size_t l1 = list1.size();
		{
			Timer t("sort");
			list1.sort();
		}
		size_t l2 = list1.size();
		if(l1 != l2)
		{
			printf("");
			printf("Size error !!");
		}
		int m = -1;
		for(auto &f: list1)
		{
			if(f.p < m)
			{
				printf("");
				printf("Sort error!\n");
			}
			m = f.p;
		}
	}

	printf("Press enter...");
	getchar();
    return 0;
}

