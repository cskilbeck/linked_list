//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iterator>
#include <algorithm>

#define _CHS_LINKED_LIST_DONT_DEFINE_STL_ITERATORS_

#include "linked_list.h"

using chs::linked_list;
using chs::linked_list_node;

//////////////////////////////////////////////////////////////////////

struct foo: linked_list_node<foo>
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

	linked_list_node<foo> node1;
	linked_list_node<foo> node2;

	static linked_list<foo> lister;
};

//////////////////////////////////////////////////////////////////////

linked_list<foo> foo::lister;

linked_list<foo, &foo::node1> list1;
linked_list<foo, &foo::node2> list2;

//////////////////////////////////////////////////////////////////////

template <typename T> int sum_list(T &list)
{
	int sum = 0;
	for(T::pointer i = list.head(); i != list.done(); i = list.next(i))
//	for(auto i = list.crbegin(); i != list.crend(); ++i)
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

	//for(auto i = list.head(); i != list.done(); i = list.next(i))
	//{
	//	printf("%s%d", sep, i->p);
	//	sep = ",";
	//}

	for(auto i = list.begin(); i != list.end(); ++i)
	{
		printf("%s%d", sep, i->p);
		sep = ",";
	}

	//for(auto i: list)
	//{
	//	printf("%s%d", sep, i.p);
	//	sep = ",";
	//}

	//std::for_each(list.begin(), list.end(), [&] (T::ref i) {
	//	printf("%s%d", sep, i.p);
	//	sep = ",";
	//});

	printf("] = %d\n", sum_list(list));
}

//////////////////////////////////////////////////////////////////////

int main(int, char **)
{
	foo a(1);
	foo b(2);
	foo c(3);

	int t = sum_list(list1);

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

	list1.pop_back();
	list2.pop_front();

	print_list("!", foo::lister);

	getchar();
	return 0;
}
