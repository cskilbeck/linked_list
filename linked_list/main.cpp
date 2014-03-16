//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iterator>
#include <string>
#include <functional>
#include <algorithm>
#include "linked_list.h"

using chs::linked_list;
using chs::list_node;

//////////////////////////////////////////////////////////////////////

struct foo : list_node<foo>
{
    foo(int n) : p(n)
    {
    }
    ~foo()
    {
    }
	std::string to_string() const
	{
		char f[46];
		sprintf_s(f, "%d", p);
		return f;
	}
    bool operator < (foo const &a)
    {
		return p < a.p;
    }
	bool operator == (int c)
	{
		return c == p;
	}
    int p;

    list_node<foo> node1;
    list_node<foo> node2;
};

//////////////////////////////////////////////////////////////////////

linked_list<foo> list0;

linked_list<foo, &foo::node1> list1;
linked_list<foo, &foo::node2> list2;
linked_list<foo, &foo::node1> list3;

//////////////////////////////////////////////////////////////////////

template <typename T> void print_list(char const *h, T const &list)
{
    printf("%s [%s]\n", h, chs::to_string(list).c_str());
}

//////////////////////////////////////////////////////////////////////

int main(int, char **)
{
	for(int i=0; i<101; ++i)
	{
		foo *f = new foo(rand());
		list1.push_back(f);
	}

	print_list("1", list1);

	list1.sort();

	print_list("\nS1", list1);

	list1.copy_into(list2);

	foo *ff = list2.find_first_of(18467);
	if(ff != nullptr)
	{
		list2.remove(ff);
	}

	print_list("\n2", list2);

	list2.remove_if([] (foo &f)
	{
		return f.p < 20000;
	});

	print_list("\n2<", list2);

    getchar();
    return 0;
}

