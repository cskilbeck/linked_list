//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

struct list_node
{
	list_node *next;
	list_node *prev;
};

//////////////////////////////////////////////////////////////////////

template<typename T, size_t offset> struct linked_list
{
	//////////////////////////////////////////////////////////////////////

	static list_node const *get_node(T const *o) throw()
	{
		return reinterpret_cast<list_node const *>(reinterpret_cast<char const *>(o) + offset);
	}

	//////////////////////////////////////////////////////////////////////

	static list_node *get_node(T *o) throw()
	{
		return reinterpret_cast<list_node *>(reinterpret_cast<char *>(o) + offset);
	}

	//////////////////////////////////////////////////////////////////////

	static T const *get_object(list_node const *node) throw()
	{
		return reinterpret_cast<T const *>(reinterpret_cast<char const *>(node) - offset);
	}

	//////////////////////////////////////////////////////////////////////

	static T *get_object(list_node *node) throw()
	{
		return reinterpret_cast<T *>(reinterpret_cast<char *>(node) - offset);
	}

	//////////////////////////////////////////////////////////////////////

	list_node root;

	//////////////////////////////////////////////////////////////////////

	linked_list() throw()
	{
		root.next = &root;
		root.prev = &root;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_before(T *pos, T *obj) throw()
	{
		list_node *node = get_node(obj);
		list_node *n = get_node(pos);
		n->prev->next = node;
		node->prev = n->prev;
		n->prev = node;
		node->next = n;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_before(T &pos, T &obj) throw()
	{
		list_node *node = get_node(&obj);
		list_node *n = get_node(&pos);
		n->prev->next = node;
		node->prev = n->prev;
		n->prev = node;
		node->next = n;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_after(T *pos, T *obj) throw()
	{
		list_node *node = get_node(obj);
		list_node *n = get_node(pos);
		n->next->prev = node;
		node->next = n->next;
		n->next = node;
		node->prev = n;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_after(T &pos, T &obj) throw()
	{
		list_node *node = get_node(&obj);
		list_node *n = get_node(&pos);
		n->next->prev = node;
		node->next = n->next;
		n->next = node;
		node->prev = n;
	}

	//////////////////////////////////////////////////////////////////////

	void push_back(T *obj) throw()
	{
		list_node *node = get_node(obj);
		node->prev = root.prev;
		node->next = &root;
		root.prev->next = node;
		root.prev = node;
	}

	//////////////////////////////////////////////////////////////////////

	void push_back(T &obj) throw()
	{
		list_node *node = get_node(&obj);
		node->prev = root.prev;
		node->next = &root;
		root.prev->next = node;
		root.prev = node;
	}

	//////////////////////////////////////////////////////////////////////

	void push_front(T *obj) throw()
	{
		list_node *node = get_node(obj);
		root.next->prev = node;
		node->next = root.next;
		node->prev = &root;
		root.next = node;
	}

	//////////////////////////////////////////////////////////////////////

	void push_front(T &obj) throw()
	{
		list_node *node = get_node(&obj);
		root.next->prev = node;
		node->next = root.next;
		node->prev = &root;
		root.next = node;
	}

	//////////////////////////////////////////////////////////////////////

	void remove(T *obj) throw()
	{
		list_node *node = get_node(obj);
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	//////////////////////////////////////////////////////////////////////

	void remove(T &obj) throw()
	{
		list_node *node = get_node(&obj);
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	//////////////////////////////////////////////////////////////////////

	T *pop_back() throw()
	{
		list_node *node = root.prev;
		node->prev->next = node->next;
		node->next->prev = node->prev;
		return get_object(node);
	}

	//////////////////////////////////////////////////////////////////////

	T *pop_front() throw()
	{
		list_node *node = root.next;
		node->next->prev = node->prev;
		node->prev->next = node->next;
		return get_object(node);
	}

	//////////////////////////////////////////////////////////////////////

	bool empty() const throw()
	{
		return root.next == &root;
	}

	//////////////////////////////////////////////////////////////////////

	void clear() throw()
	{
		root.next = root.prev = &root;
	}

	//////////////////////////////////////////////////////////////////////

	T *head() const throw()
	{
		return get_object(root.next);
	}

	//////////////////////////////////////////////////////////////////////

	T *tail() const throw()
	{
		return get_object(root.prev);
	}

	//////////////////////////////////////////////////////////////////////

	T const *end() throw()
	{
		return get_object(&root);
	}

	//////////////////////////////////////////////////////////////////////

	T *next(T *i) const throw()
	{
		return get_object(get_node(i)->next);
	}

	//////////////////////////////////////////////////////////////////////

	T *prev(T *i) const throw()
	{
		return get_object(get_node(i)->prev);
	}
};

#define declare_linked_list(type_name, node_name) linked_list<type_name, offsetof(type_name, node_name)>
#define typedef_linked_list(type_name, node_name) typedef declare_linked_list(type_name, node_name)
