#include <stdio.h>
#include <random>
#include "linked_list.h"

struct foo
{
	foo(int n = 0) : i(n) { }

	list_node node1;	// would like it if these could be made private
	list_node node2;	// but the nasty macros need to see inside...
	list_node node3;	// getting rid of the macros would be even better

	int i;
};

linked_list<foo, &foo::node1> list1;
linked_list<foo, &foo::node2> list2;
linked_list<foo, &foo::node3> list3;

template <typename T> void print_list(T &list, char const *message)
{
	printf("%s\n", message);
	list.for_each([] (foo *p)
	{
		printf(" %d\n", p->i);
	});
	puts("");
}

int main(int, char **)
{
	foo foos[10];

	for(int i=0; i<10; ++i)
	{
		foos[i].i = i;
		list1.push_back(&foos[i]);
		list2.push_back(&foos[i]);
	}

	print_list(list1, "list1");

	list2.remove(foos[2]);

	print_list(list2, "list2");

	list2.remove_if([] (foo *p) { return p->i > 7; });

	print_list(list2, "list2");

	list3.push_back(foos[0]);
	list3.push_back(foos[1]);

	print_list(list3, "list3");

	linked_list<foo, &foo::node3> list3_a;
	list3.move_into(list3_a, [] (foo *p) { return (p->i % 2) == 0; });

	print_list(list3, "list3");
	print_list(list3_a, "list3_a");

	getchar();
	return 0;
}
