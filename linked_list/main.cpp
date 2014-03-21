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

	bool operator < (foo const &a) const
    {
		return p < a.p;
    }

	bool operator == (int c)
	{
		return c == p;
	}
    int p;
};

//////////////////////////////////////////////////////////////////////

typedef linked_list<foo> list_t;

list_t list1;
list_t list2;

//////////////////////////////////////////////////////////////////////

foo foos[5000000];

void make_list(list_t &l, int *a, foo *foos, size_t s)
{
	for(size_t i = 0; i < s; ++i)
	{
		foos[i].p = a[i];
		l.push_back(foos[i]);
	}
}

void check_list(list_t &l)
{
	int min = INT_MIN;
	for(auto &f : l)
	{
		if(f.p < min)
		{
			DebugBreak();
		}
		min = f.p;
		printf("%d ", f.p);
	}
	printf("<--> ");
	int max = INT_MAX;
	for(foo *f = l.tail(); f != l.done(); f = l.prev(f))
	{
		if(f->p > max)
		{
			DebugBreak();
		}
		max = f->p;
		printf("%d ", f->p);
	}
	printf("\n");
}

void test_merge(int *a, size_t B, int *b, size_t C)
{
	foo *lf = new foo[B];
	foo *rf = new foo[C];
	list_t l;
	list_t r;
	make_list(l, a, lf, B);
	make_list(r, b, rf, C);
	check_list(l);
	check_list(r);
	printf("merge\n");
	list_t::ptr h = list_t::forward_merge(l.head(), B, r.head(), C);
	l.fixup_prev_pointers(h, B + C);
	check_list(l);
	printf("\n\n");
	delete[] lf;
	delete[] rf;
}

void test_sort(int *a, size_t B)
{
	foo *f1 = new foo[B];
	foo *f2 = new foo[B];
	list_t l1;
	list_t l2;
	make_list(l1, a, f1, B);
	make_list(l2, a, f2, B);
	l1.sort();
	l2.f_sort();
	check_list(l1);
	check_list(l2);
	delete[] f1;
	delete[] f2;
}

#define merge_test(x, y) test_merge(x, ARRAYSIZE(x), y, ARRAYSIZE(y))
#define sort_test(x) test_sort(x, ARRAYSIZE(x))

int __cdecl main(int, char **)
{
#if 1
	printf("MERGE:\n");

	int t1[] = { 3 };
	int t2[] = { 1, 2 };

	int u1[] = { 1,3 };
	int u2[] = { 2,4 };

	int v1[] = { 1,8,12,16,24 };
	int v2[] = { 0, 23, 25 };

	merge_test(t1, t2);
	merge_test(t2, t1);

	merge_test(v1, v2);
	merge_test(v2, v1);

	merge_test(u1, u2);
	merge_test(u2, u1);

	printf("\nSORT:\n");

	int knob4[] = { 4, 3, 2, 1 };
	int knob3[] = { 3, 2, 1 };
	int knob1[] = { 1, 2 };
	int knob2[] = { 2, 1 };
	int knob5[] = { 123,345,234,534,76,576,345,2345,564,576,576,345,234,1234,345,456,576,345,2345,234,2345,76,786,768,356 };

	sort_test(knob5);
	sort_test(knob4);
	sort_test(knob1);
	sort_test(knob2);
	sort_test(knob3);

#else
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
		{
			Timer t("f_sort");
			list1.f_sort();
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
#endif

	printf("Press enter...");
	getchar();
    return 0;
}

