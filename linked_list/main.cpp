//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iterator>
#include <algorithm>
#include "linked_list.h"

using chs::linked_list;
using chs::linked_list_node;

//////////////////////////////////////////////////////////////////////

struct foo: linked_list_node<foo>
{
	foo(int n) : p(n)
	{
		list0.push_back(this);
	}
	~foo()
	{
		list0.remove(this);
	}
	int p;

	static linked_list<foo> list0;
	linked_list_node<foo> node1;
	linked_list_node<foo> node2;
};

//////////////////////////////////////////////////////////////////////

linked_list<foo> foo::list0;

linked_list<foo, &foo::node1> list1;
linked_list<foo, &foo::node2> list2;

//////////////////////////////////////////////////////////////////////

template <typename T> int sum_list(T &list)
{
	int sum = 0;
	for(auto &i : list)
	{
		sum += i.p;
	}
	return sum;
}

//////////////////////////////////////////////////////////////////////

template <typename T> void print_list(char const *h, T &list)
{
	printf("%s[", h);
	char const *sep = "";
	for(auto const &i : list)
	{
		printf("%s%d", sep, i.p);
		sep = ",";
	}
	printf("] = %d\n", sum_list(list));
}

//////////////////////////////////////////////////////////////////////

int main(int, char **)
{
	{
		foo a(1);
		foo b(2);
		foo c(3);

		print_list("0", foo::list0);

		list1.push_back(a);
		list1.push_back(b);
		list1.push_back(c);

		print_list("1", list1);

		list1.remove(&a);
		list1.remove(&b);
		list1.remove(&c);

		auto bob = std::begin(list1);
		if(bob != std::end(list1))
		{
			printf("!");
		}
	}

	print_list("~1", list1);
	print_list("~0", foo::list0);

	getchar();
	return 0;
}
