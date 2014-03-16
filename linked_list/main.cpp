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
    foo(char const *n) : p(n)
    {
    }
    ~foo()
    {
    }
	std::string to_string() const
	{
		return std::string(p);
	}
    bool operator < (foo const &a)
    {
		return strcmp(p, a.p) < 0;
    }
    char const *p;

    list_node<foo> node1;
    list_node<foo> node2;
};

//////////////////////////////////////////////////////////////////////

linked_list<foo> list0;

linked_list<foo, &foo::node1> list1;
linked_list<foo, &foo::node1> list2;

//////////////////////////////////////////////////////////////////////

template <typename T> void print_list(char const *h, T const &list)
{
    printf("%s [%s]\n", h, chs::to_string(list).c_str());
}

//////////////////////////////////////////////////////////////////////

int main(int, char **)
{
    foo a("0");
    foo b("1");
    foo c("2");
    foo d("3");
    foo e("4");
    foo f("5");
    foo g("6");
    foo h("7");
    foo i("8");

    //list1.push_back(d);
    //list1.push_back(c);
    //list1.push_back(b);
    //list1.push_back(a);
    //list1.push_back(i);
    //list1.push_back(h);
    //list1.push_back(g);
    //list1.push_back(f);
    //list1.push_back(e);

    print_list("1", list1);
    print_list("2", list2);

	list1.sort();

    print_list("1", list1);
    print_list("2", list2);

    getchar();
    return 0;
}

