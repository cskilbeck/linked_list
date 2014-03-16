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
    foo a(rand());
    foo b(rand());
    foo c(rand());
    foo d(rand());
    foo e(rand());
    foo f(rand());
    foo g(rand());
    foo h(rand());
    foo i(rand());
    foo j(rand());

    list1.push_back(a);
    list1.push_back(b);
    list1.push_back(c);
    list1.push_back(d);
    list1.push_back(e);
    list1.push_back(f);
    list1.push_back(g);
    list1.push_back(h);
    list1.push_back(i);
    list1.push_back(j);

    print_list("1", list1);

	list1.sort();

	print_list("S1", list1);

    getchar();
    return 0;
}

