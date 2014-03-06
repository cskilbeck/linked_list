#include <stdio.h>
#include <cstddef>
#include <random>
#include "linked_list.h"

struct bob
{
	int i;
	list_node node;
	list_node node2;

	bob(int _i) : i(_i)
	{

	}
};

linked_list_typedef(bob, node) bob_list;
linked_list_typedef(bob, node2) bob_list2;

bob_list l;
bob_list2 l2;

int main(int, char **)
{
	bob a(rand());
	bob b(rand());
	bob c(rand());

	bob d(100);

	printf("Added: %d\n", a.i);
	printf("Added: %d\n", b.i);
	printf("Added: %d\n", c.i);

	printf("%d\n", a.i + b.i + c.i);

	l.push_back(&a);
	l.push_back(&b);
	l.push_back(&c);

	l2.push_back(a);
	l2.push_back(b);
	l2.push_back(c);

	int total = 0;
	int total2 = 0;

	printf("l:\n");
	for(bob *i = l.head(); i != l.end(); i = l.next(i))
	{
		printf("  %d\n", i->i);
		total += i->i;
	}
	printf("total: %d\n", total);

	printf("l2:\n");
	for(bob *i = l2.head(); i != l2.end(); i = l2.next(i))
	{
		printf("  %d\n", i->i);
		total2 += i->i;
	}

	printf("total: %d\n", total2);

	getchar();
	return 0;
}
