//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <Windows.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
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
		printf("%s begins\n", mName);
		SetProcessPriorityBoost(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		QueryPerformanceCounter((LARGE_INTEGER *)&mStart);
	}

	void Stop()
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&mEnd);
	}

	double ms()
	{
		LARGE_INTEGER resolution64;
		QueryPerformanceFrequency(&resolution64);
		volatile double total = (double)(mEnd.QuadPart - mStart.QuadPart);
		total *= 1000.0;
		total /= (double)resolution64.QuadPart;
		return total;
	}

	void Snapshot()
	{
		Stop();
		printf("%s took %fms\n", mName, ms());
	}

	~Timer()
	{
		SetProcessPriorityBoost(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	}

private:
	
	volatile LARGE_INTEGER mStart;
	volatile LARGE_INTEGER mEnd;
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

void make_list(list_t &l, int *a, foo *foos, size_t s)
{
	for(size_t i = 0; i < s; ++i)
	{
		foos[i].p = a[i];
		l.push_back(foos[i]);
	}
}

void check_list(list_t &l, size_t s)
{
	int min = INT_MIN;
	size_t cs = 0;
	for(auto &f : l)
	{
		if(f.p < min)
		{
			DebugBreak();
		}
		min = f.p;
		printf("%d ", f.p);
		++cs;
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
	if(cs != s)
	{
		printf("ERROR: Size mismatch!\n");
	}
}

void test_merge(int *a, size_t B, int *b, size_t C)
{
	printf("merge\n");
	foo *lf = new foo[B];
	foo *rf = new foo[C];
	list_t l;
	list_t r;
	make_list(l, a, lf, B);
	make_list(r, b, rf, C);
	l.merge_into(r);
	check_list(r, B + C);
	delete[] lf;
	delete[] rf;
}

void test_sort(int *a, size_t B)
{
	foo *f1 = new foo[B];
	list_t l1;
	make_list(l1, a, f1, B);
	l1.sort();
	check_list(l1, B);
	delete[] f1;
}

template <typename T, T a=0, T b=0> struct range
{
	struct iterator : std::iterator<std::forward_iterator_tag, int>
	{
		T value;

		iterator(T n)
		{
			value = n;
		}

		T operator ++()
		{
			return ++value;
		}

		bool operator != (iterator const &other)
		{
			return value != other.value;
		}

		T operator *()
		{
			return value;
		}
	};

	iterator begin()
	{
		return iterator(a);
	}

	iterator end()
	{
		return iterator(b+1);
	}
};

template <typename T, T a, T b> range<T, a, b> grange(T a, T b)
{
	return range<T, a, b>();
}

#define merge_test(x, y) test_merge(x, ARRAYSIZE(x), y, ARRAYSIZE(y))
#define sort_test(x) test_sort(x, ARRAYSIZE(x))

	int a1[] = { 1 };
	int a2[] = { 2 };

	int t1[] = { 1,2,3 };
	int t2[] = { 0,5,6 };

	int u1[] = { 1,3 };
	int u2[] = { 2,4 };

	int v2[] = { 1,8,12 };
	int v1[] = { 0, 23 };

	int r2[] = { 1,2,3,4,5,6,7,8 };
	int r1[] = { 1 };

	int knob1[] = { 1, 2 };
	int knob2[] = { 2, 1 };
	int knob3[] = { 3, 2, 1 };
	int knob4[] = { 1, 2, 3, 1 };
	int knob5[] = { 123,345,234,534,76,576,345,2345,564,576,576,345,234,1234,345,456,576,345,2345,234,2345,76,786,768,356 };

foo foos[5000001];

int __cdecl main(int, char **)
{
#if 0
	printf("MERGE:\n");

	merge_test(r1, r2);

	merge_test(a1, a2);
	merge_test(a2, a1);

	merge_test(r2, r1);

	merge_test(t2, t1);
	merge_test(t1, t2);

	merge_test(u2, u1);

	merge_test(v2, v1);

	merge_test(u1, u2);
	merge_test(u2, u1);

	printf("\nSORT:\n");

	sort_test(knob4);
	sort_test(knob1);
	sort_test(knob2);
	sort_test(knob3);
	sort_test(knob5);


#else

	std::vector<double> times;
	for (int i = 0; i < 20; ++i)
	{
		list1.clear();
		for(int i=0; i<5000001; ++i)
		{
			list1.push_back(foos[i]);
		}
		size_t l1 = list1.size();
		{
			Timer t("sort ");
			list1.sort();
			t.Stop();
			double ms = t.ms();
			times.push_back(ms);
			printf(" took %f\n", ms);
		}

		size_t l3 = list1.size();
		if(l1 != l3)
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
				break;
			}
			m = f.p;
		}
	}
	std::sort(times.begin(), times.end());
	printf("Times:\n");
	for (auto d : times)
	{
		printf("%f\n", d);
	}
#endif

	printf("Press enter...");
	getchar();
    return 0;
}

