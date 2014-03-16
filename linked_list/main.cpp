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
	bool operator == (char const *c)
	{
		return _stricmp(p, c) == 0;
	}
    char const *p;

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
    foo a("0");
    foo b("1");
    foo c("2");
    foo d("3");
    foo e("4");
    foo f("5");
    foo g("6");
    foo h("7");
    foo i("8");
    foo j("9");

    list1.push_back(j);
    list1.push_back(d);
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

	list1.copy_into(list2);
	list1.sort();

	print_list("S1", list1);

	list3 = list1.select([] (foo &f) -> bool {
		return _stricmp(f.p, "4") < 0;
	});

	foo *p = chs::find_last_of(list3, "2");
	if(p != list3.done())
	{
		printf("Found: %s\n", p->p);
		list3.remove(p);
	}

	list3.for_each([] (foo &f) {
		if(f.p[0] == '2')
		{
			list3.remove(f);
		}
		printf("%s\n", f.p);
	});

	p = chs::find_first_of(list3, "1");
	if(p != list3.done())
	{
		list3.remove(p);
	}

    print_list("1", list1);
    print_list("2", list2);
    print_list("3", list3);

    getchar();
    return 0;
}

