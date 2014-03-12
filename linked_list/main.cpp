#include <stdio.h>

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

template <typename T, typename R, R T::*M>
size_t offset_of()
{
    return reinterpret_cast<size_t>(&(((T*)0)->*M));
}

template <typename T, linked_list_node<T> T::*NODE, bool is_member>
struct list_base
{
};

template <typename T, linked_list_node<T> T::*NODE>
struct list_base<T, NODE, true> : linked_list_node<T>
{
	static size_t offset()
	{
		return offset_of<T, linked_list_node<T>, NODE>();
	}
};

template <typename T, linked_list_node<T> T::*NODE>
struct list_base<T, NODE, false> : linked_list_node<T>
{
	static size_t offset()
	{
		return offset_of<T, list_node_base<T>, &T::list_node>();
	}
};

template <typename T, linked_list_node<T> T::*NODE = nullptr>
#if defined(_MSC_VER)
struct linked_list : list_base<T, NODE, NODE>
#elif defined(__GNUC__)
struct linked_list : list_base<T, NODE, (linked_list_node<T> T::*)nullptr != NODE>
#endif
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
	printf("%d\n", bar_list.offset());
	printf("%d\n", foo_list.offset());
	getchar();
	return 0;
}