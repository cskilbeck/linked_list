//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "linked_list.h"

//////////////////////////////////////////////////////////////////////

struct foo: list_node<foo>
{
	foo(int n) : p(n)
	{
		lister.push_back(this);
	}
	~foo()
	{
		lister.remove(this);
	}
	int p;
	list_node<foo> node;
	list_node<foo> node2;

	linked_list<foo> lister;
};

//////////////////////////////////////////////////////////////////////

linked_list<foo> list1;
linked_list<foo, &foo::node> list2;

//////////////////////////////////////////////////////////////////////

template <typename T> int sum_list(T const &list)
{
	int sum = 0;
	for(T::const_pointer i = list.head(); i != list.end(); i = list.next(i))
	{
		sum += i->p;
	}
	return sum;
}

//////////////////////////////////////////////////////////////////////

template <typename T> void print_list(char const *h, T &list)
{
	printf("%s[", h);
	char const *sep = "";
	for(T::pointer i = list.head(); i != list.end(); i = list.next(i))
	{
		printf("%s%d", sep, i->p);
		sep = ",";
	}
	printf("] = %d\n", sum_list(list));
}

//////////////////////////////////////////////////////////////////////

int main(int, char **)
{
	foo a(1);
	foo b(2);
	foo c(3);

	print_list("1", list1);
	print_list("2", list2);

	list1.push_front(a);
	list1.push_front(b);
	list1.push_front(c);
	list2.push_front(a);
	list2.push_front(b);
	list2.push_front(c);

	list1.remove(b);

	print_list("1", list1);
	print_list("2", list2);

	foo d(4);
	list1.insert_after(a, d);

	print_list("1", list1);
	print_list("2", list2);

	list1.remove(a);
	list1.remove(b);

	print_list("1", list1);
	print_list("2", list2);

	list1.remove(c);
	list2.remove(c);

	list1.insert_before(d, a);

	print_list("1", list1);
	print_list("2", list2);

	list1.clear();

	print_list("1", list1);
	print_list("2", list2);

	getchar();
	return 0;
}
