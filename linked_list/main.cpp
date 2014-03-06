#include <stdio.h>
#include <cstddef>
#include <random>
#include "linked_list.h"

struct bob
{
	list_node node;
	list_node node2;
	int i;

	bob(int _i) : i(_i)
	{
	}

	virtual int get_i()
	{
		return i;
	}
};

struct bill : bob
{
	bill(int _i): bob(_i)
	{
	}

	virtual int get_i()
	{
		return 0;
	}
};

typedef_linked_list(bob, node) bob_list;
typedef_linked_list(bill, node2) bob_list2;

bob_list l;
bob_list2 l2;

int main(int, char **)
{
	bob a(rand());
	bob b(rand());
	bob c(rand());

	bill d(a.i);
	bill e(b.i);
	bill f(c.i);

	printf("Added: %d\n", a.get_i());
	printf("Added: %d\n", b.get_i());
	printf("Added: %d\n", c.get_i());

	printf("%d\n", a.i + b.i + c.i);

	l.push_back(&a);
	l.push_back(&b);
	l.push_back(&c);

	l2.push_back(d);
	l2.push_back(e);
	l2.push_back(f);

	int total = 0;
	int total2 = 0;

	printf("l:\n");
	for(bob *i = l.head(); i != l.end(); i = l.next(i))
	{
		printf("  %d\n", i->get_i());
		total += i->get_i();
	}
	printf("total: %d\n", total);

	printf("l2:\n");
	for(bill *i = l2.head(); i != l2.end(); i = l2.next(i))
	{
		printf("  %d\n", i->i);
		total2 += i->i;
	}

	printf("total: %d\n", total2);

	getchar();
	return 0;
}
