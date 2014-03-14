//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iterator>
#include <string>
#include <algorithm>
#include "linked_list.h"

using chs::linked_list;
using chs::list_node;

//////////////////////////////////////////////////////////////////////

struct foo : list_node<foo>
{
    foo(char n) : p(n)
    {
    }
    ~foo()
    {
    }
    void knob()
    {
    }
    char p;

    list_node<foo> node1;
    list_node<foo> node2;
};

//////////////////////////////////////////////////////////////////////

linked_list<foo> list0;

linked_list<foo, &foo::node1> list1;
linked_list<foo, &foo::node2> list2;

using std::string;

template <typename T> string get(T const &list)
{
    string result;
    for(auto const &i: list)
    {
        result += i.p;
    }
    return result;
}

//////////////////////////////////////////////////////////////////////

template <typename T> void print_list(char const *h, T const &list)
{
    printf("$s [%s]\n", h, get(list).c_str());
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
    foo a('a');
    foo b('b');
    foo c('c');

    list1.push_back(a);
    list1.push_back(b);
    list1.push_back(c);

    print_list("1", list1);

    list1.remove(&a);
    list1.remove(&b);
    list1.remove(&c);

    print_list("~1", list1);

    getchar();
    return 0;
}

