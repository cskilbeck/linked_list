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

using std::string;

template <typename T> string get(T const &list)
{
	string empty;
	string comma(",");
	string *sep = &empty;
	string result("[");
	for(auto const &i: list)
	{
		result += *sep + i.to_string();
		sep = &comma;
	}
	return result + "]";
}

//////////////////////////////////////////////////////////////////////

template <typename T> void print_list(char const *h, T const &list)
{
    printf("%s %s\n", h, get(list).c_str());
}

//////////////////////////////////////////////////////////////////////
// NODE, MEMBER
// ref              ptr
//
// push_back
// push_front
// insert_before
// insert_after
// remove
// pop_front
// pop_back
// 

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

    list1.push_back(d);
    list1.push_back(c);
    list1.push_back(b);
    list1.push_back(a);
    list1.push_back(i);
    list1.push_back(h);
    list1.push_back(g);
    list1.push_back(f);
    list1.push_back(e);

	list2 = list1.split(i);

    print_list("1", list1);
    print_list("2", list2);

	list1.clear();
	list2.clear();

    list1.push_back(d);
    list1.push_back(c);
    list1.push_back(b);
    list1.push_back(a);
    list1.push_back(i);
    list1.push_back(h);
    list1.push_back(g);
    list1.push_back(f);
    list1.push_back(e);

	list1.sort();

    print_list("1", list1);
    print_list("2", list2);

    getchar();
    return 0;
}

