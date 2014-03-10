//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

struct list_node
{
	list_node *next;
	list_node *prev;
};

//////////////////////////////////////////////////////////////////////

template <typename T, list_node T::*NODE, bool is_member> struct list_base
{
};

template <typename T, list_node T::*NODE>
struct list_base<T, NODE, true>
{
};

template <typename T, list_node T::*NODE>
struct list_base<T, NODE, false>
{
};

template <typename T, list_node T::*NODE = nullptr> struct mylist : list_base<T, NODE, NODE == nullptr>
{
};

template<typename T, list_node T::* NODE, bool is_member = true> struct linked_list
{
public:

	linked_list()
	{
		root.next = root.prev = &root;
	}

	//////////////////////////////////////////////////////////////////////

	void push_front(T *obj)
	{
		list_node &node = get_node(obj);
		root.next->prev = &node;
		node.next = root.next;
		node.prev = &root;
		root.next = &node;
	}

	//////////////////////////////////////////////////////////////////////

	void push_front(T &obj)
	{
		list_node &node = get_node(&obj);
		root.next->prev = &node;
		node.next = root.next;
		node.prev = &root;
		root.next = &node;
	}

	//////////////////////////////////////////////////////////////////////

	void push_back(T *obj)
	{
		list_node &node = get_node(obj);
		node.prev = root.prev;
		node.next = &root;
		root.prev->next = &node;
		root.prev = &node;
	}

	//////////////////////////////////////////////////////////////////////

	void push_back(T &obj)
	{
		list_node &node = get_node(&obj);
		node.prev = root.prev;
		node.next = &root;
		root.prev->next = &node;
		root.prev = &node;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_before(T *pos, T *obj)
	{
		list_node &node = get_node(obj);
		list_node &posn = get_node(pos);
		posn.prev->next = &node;
		node.prev = posn.prev;
		posn.prev = &node;
		node.next = &posn;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_before(T &pos, T &obj)
	{
		list_node &node = get_node(&obj);
		list_node &posn = get_node(&pos);
		posn.prev->next = &node;
		node.prev = posn.prev;
		posn.prev = &node;
		node.next = &posn;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_after(T *pos, T *obj)
	{
		list_node &node = get_node(obj);
		list_node &posn = get_node(pos);
		posn.next->prev = node;
		node.next = posn.next;
		posn.next = &node;
		node.prev = &posn;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_after(T &pos, T &obj)
	{
		list_node &node = get_node(&obj);
		list_node &posn = get_node(&pos);
		posn.next->prev = &node;
		node.next = posn.next;
		posn.next = &node;
		node.prev = &posn;
	}

	//////////////////////////////////////////////////////////////////////

	void remove(T *obj)
	{
		list_node &node = get_node(obj);
		node.prev->next = node.next;
		node.next->prev = node.prev;
	}

	//////////////////////////////////////////////////////////////////////

	void remove(T &obj)
	{
		list_node &node = get_node(&obj);
		node.prev->next = node.next;
		node.next->prev = node.prev;
	}

	//////////////////////////////////////////////////////////////////////

	T *pop_back()
	{
		list_node * const node = root.prev;
		node->prev->next = node->next;
		node->next->prev = node->prev;
		return get_object(node);
	}

	//////////////////////////////////////////////////////////////////////

	T *pop_front()
	{
		list_node * const node = root.next;
		node->next->prev = node->prev;
		node->prev->next = node->next;
		return get_object(node);
	}

	//////////////////////////////////////////////////////////////////////

	bool is_empty() const
	{
		return root.next == &root;
	}

	//////////////////////////////////////////////////////////////////////

	void clear()
	{
		root.next = root.prev = &root;
	}

	//////////////////////////////////////////////////////////////////////

	T * const head() const
	{
		return get_object(root.next);
	}

	//////////////////////////////////////////////////////////////////////

	T * const tail() const
	{
		return get_object(root.prev);
	}

	//////////////////////////////////////////////////////////////////////

	T const * const end()
	{
		return get_object(&root);
	}

	//////////////////////////////////////////////////////////////////////

	T * const next(T *i) const
	{
		return get_object(get_node(i).next);
	}

	//////////////////////////////////////////////////////////////////////

	T * const prev(T *i) const
	{
		return get_object(get_node(i).prev);
	}

	//////////////////////////////////////////////////////////////////////

	template <typename F> void remove_if(F &function)
	{
		for(T *i = head(); i != end(); i = next(i))
		{
			T *n = next(i);
			if(function(i))
			{
				remove(i);
			}
			i = n;
		}
	}

	//////////////////////////////////////////////////////////////////////

	template<typename F> void for_each(F &function)
	{
		for(T *i = head(); i != end(); i = next(i))
		{
			function(i);
		}
	}

	//////////////////////////////////////////////////////////////////////

	template<typename F> bool reverse_for_each(F &function)
	{
		for(T *i = tail(); i != end(); i = prev(i))
		{
			if(!function(i))
			{
				return false;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////

	template <typename F> T *find(F &function)
	{
		for(T *i = head(); i != end(); i = next(i))
		{
			if(function(i))
			{
				return i;
			}
		}
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////

	template <typename F> T *reverse_find(F &function)
	{
		for(T *i = tail(); i != end(); i = prev(i))
		{
			if(function(i))
			{
				return i;
			}
		}
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////

private:

	static list_node const &get_node(T const *o)
	{
		return *reinterpret_cast<list_node const *>
			(reinterpret_cast<char const *>(o)+offsetof(T, *NODE));
	}

	//////////////////////////////////////////////////////////////////////

	static list_node &get_node(T *o)
	{
		return *reinterpret_cast<list_node *>
			(reinterpret_cast<char *>(o)+offsetof(T, *NODE));
	}

	//////////////////////////////////////////////////////////////////////

	static T const *get_object(list_node const *n)
	{
		return reinterpret_cast<T const *>
			(reinterpret_cast<char const *>(n)-offsetof(T, *NODE));
	}

	//////////////////////////////////////////////////////////////////////

	static T *get_object(list_node *n)
	{
		return reinterpret_cast<T *>
			(reinterpret_cast<char *>(n)-offsetof(T, *NODE));
	}

	//////////////////////////////////////////////////////////////////////

	list_node root;

	//////////////////////////////////////////////////////////////////////
};

