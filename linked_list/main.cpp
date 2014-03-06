#include <stdio.h>
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

struct jack
{
	virtual int get_j()
	{
		return 1;
	}
};

struct bill : bob, jack
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

	bob aa(100);

	bill d(a.i);
	bill e(b.i);
	bill f(c.i);

	bill dd(100);

	printf("a: %d\n", a.get_i());
	printf("b: %d\n", b.get_i());
	printf("c: %d\n", c.get_i());
	printf("aa: %d\n", aa.get_i());

	l.push_back(&a);
	l.push_back(&b);
	l.push_back(&c);

	l.insert_before(&a, &aa);

	l2.push_back(d);
	l2.push_back(e);
	l2.push_back(f);

	l2.insert_before(&d, &dd);

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

	printf("for_each:\n");
	l2.for_each([] (bill *b) { printf("%d\n", b->i); return true; });

	bob *s = l.find([] (bob *b) { return b->i == 100; });

	if(s != nullptr)
	{
		printf("Found: %d\n", s->i);
	}
	else
	{
		printf("Not found\n");
	}

	getchar();
	return 0;
}
