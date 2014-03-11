//////////////////////////////////////////////////////////////////////

#pragma once

#include <cstddef>

//////////////////////////////////////////////////////////////////////

template <typename T> struct list_node_base
{
	T *next;
	T *prev;
};

//////////////////////////////////////////////////////////////////////

template <typename T> struct list_node
{
	list_node_base<T> node_pointers;
};

//////////////////////////////////////////////////////////////////////

template <typename T, list_node<T> T::*NODE, bool is_member> struct list_base
{
};

//////////////////////////////////////////////////////////////////////

template <typename T, list_node<T> T::*NODE> struct list_base<T, NODE, true>
{
protected:

	list_node<T> root_node;

	T &			root_object()		{ return *reinterpret_cast<T *>			(reinterpret_cast<char *>		(&root_node) - offsetof(T, *NODE)); }
	T const &	root_object() const	{ return *reinterpret_cast<T const *>	(reinterpret_cast<char const *>	(&root_node) - offsetof(T, *NODE)); }

	list_node_base<T> &get_node(T *obj) { return (obj->*NODE).node_pointers; }
	list_node_base<T> &get_node(T &obj) { return (obj.*NODE).node_pointers; }

	list_node_base<T> const &get_node(T const *obj) const { return (obj->*NODE).node_pointers; }
	list_node_base<T> const &get_node(T const &obj) const { return (obj.*NODE).node_pointers; }
};

//////////////////////////////////////////////////////////////////////

template <typename T, list_node<T> T::*NODE> struct list_base<T, NODE, false>
{
protected:

	list_node<T> root_node;

	T &			root_object()		{ return *reinterpret_cast<T *>			(reinterpret_cast<char *>		(&root_node) - offsetof(T, node_pointers)); }
	T const &	root_object() const	{ return *reinterpret_cast<T const *>	(reinterpret_cast<char const *>	(&root_node) - offsetof(T, node_pointers)); }

	list_node_base<T> &get_node(T *obj) const { return obj->node_pointers; }
	list_node_base<T> &get_node(T &obj) const { return obj.node_pointers; }

	list_node_base<T> const &get_node(T const *obj) const { return obj->node_pointers; }
	list_node_base<T> const &get_node(T const &obj) const { return obj.node_pointers; }
};

//////////////////////////////////////////////////////////////////////

template <typename T, list_node<T> T::*NODE = nullptr> struct linked_list: list_base<T, NODE, NODE>
{
	typedef T *			pointer;
	typedef T const *	const_pointer;
	typedef T &			ref;
	typedef T const &	const_ref;

	void clear()
	{
		ref root = root_object();
		list_node_base<T> &node = get_node(root);
		node.next = &root;
		node.prev = &root;
	}

	void insert_before(pointer obj_before, pointer obj)
	{
		pointer &p = get_node(obj_before).prev;
		list_node_base<T> &n = get_node(obj);
		get_node(p).next = obj;
		n.prev = p;
		p = obj;
		n.next = obj_before;
	}

	void insert_after(pointer obj_after, pointer obj)
	{
		pointer &n = get_node(obj_after).next;
		list_node_base<T> &p = get_node(obj);
		get_node(n).prev = obj;
		p.next = n;
		n = obj;
		p.prev = obj_after;
	}

	pointer remove(pointer obj)
	{
		pointer p = prev(obj);
		pointer n = next(obj);
		get_node(p).next = n;
		get_node(n).prev = p;
		return obj;
	}

					linked_list()							{ clear(); }

	bool			is_empty() const						{ return root_node.node_pointers.next == &root_object(); }

	pointer			head()									{ return root_node.node_pointers.next; }
	const_pointer	head() const							{ return root_node.node_pointers.next; }

	pointer			tail()									{ return root_node.node_pointers.prev; }
	const_pointer	tail() const							{ return root_node.node_pointers.prev; }

	pointer			next(pointer obj)						{ return get_node(obj).next; }
	const_pointer	next(const_pointer obj) const			{ return get_node(obj).next; }

	pointer			prev(pointer obj)						{ return get_node(obj).prev; }
	const_pointer	prev(const_pointer obj) const			{ return get_node(obj).prev; }

	const_pointer	end() const								{ return &root_object(); }

	void			insert_before(ref obj_before, ref obj)	{ insert_before(&obj_before, &obj); }
	void			insert_after(ref obj_after, ref obj)	{ insert_after(&obj_after, &obj); }

	pointer			remove(ref obj)							{ return remove(&obj); }

	void			push_back(ref obj)						{ insert_before(root_object(), obj); }
	void			push_back(pointer obj)					{ insert_before(root_object(), *obj); }

	void			push_front(ref obj)						{ insert_after(root_object(), obj); }
	void			push_front(pointer obj)					{ insert_after(root_object(), *obj); }
		
	pointer			pop_front()								{ return !is_empty() ? remove(head()) : nullptr; }
	pointer			pop_back()								{ return !is_empty() ? remove(tail()) : nullptr; }
};
