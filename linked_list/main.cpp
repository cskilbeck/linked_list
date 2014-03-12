#include <stdio.h>
#include <stddef.h>

template <typename T> struct list_node_base
{
	T *next;
	T *prev;
};

template <typename T>
struct linked_list_node
{
	list_node_base<T> list_node;
};

template <typename T, linked_list_node<T> T::*NODE, bool is_member>
struct list_base
{
	template <typename A, typename B>
	size_t offset_of(B A::*M)
	{
	    return reinterpret_cast<size_t>(&(((T*)0)->*M));
	};
};

template <typename T, linked_list_node<T> T::*NODE>
struct list_base<T, NODE, true> : linked_list_node<T>
{
	size_t offset()
	{
		return offset_of(&T::*NODE);
	}
};

template <typename T, linked_list_node<T> T::*NODE>
struct list_base<T, NODE, false> : linked_list_node<T>
{
	size_t offset()
	{
		return offset_of(&T::list_node);
	}
};

template <typename T, linked_list_node<T> T::*NODE = nullptr>
//struct linked_list : list_base<T, NODE, (linked_list_node<T> T::*)nullptr != NODE>
struct linked_list : list_base<T, NODE, NODE>
{
};

struct foo : linked_list_node<foo>
{
};

struct bar
{
	linked_list_node<bar> node;
};

linked_list<foo> foo_list;
linked_list<bar, &bar::node> bar_list;

int main(int, char **)
{
	return 0;
}