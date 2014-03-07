//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <functional>

//////////////////////////////////////////////////////////////////////

struct list_node
{
	list_node *next;
	list_node *prev;
};

	//////////////////////////////////////////////////////////////////////

	template<typename T, list_node T::* node> struct linked_list
	{
		//////////////////////////////////////////////////////////////////////

		static list_node const &get_node(T const *o)
		{
			return *reinterpret_cast<list_node const *>
				(reinterpret_cast<char const *>(o) + offsetof(T, *node));
		}

		//////////////////////////////////////////////////////////////////////

		static list_node &get_node(T *o)
		{
			return *reinterpret_cast<list_node *>
				(reinterpret_cast<char *>(o) + offsetof(T, *node));
		}

		//////////////////////////////////////////////////////////////////////

		static T const *get_object(list_node const *n)
		{
			return reinterpret_cast<T const *>
				(reinterpret_cast<char const *>(n) - offsetof(T, *node));
		}

		//////////////////////////////////////////////////////////////////////

		static T *get_object(list_node *n)
		{
			return reinterpret_cast<T *>
				(reinterpret_cast<char *>(n) - offsetof(T, *node));
		}

	//////////////////////////////////////////////////////////////////////

	list_node root;

	//////////////////////////////////////////////////////////////////////

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

	bool empty() const
	{
		return root.next == &root;
	}

	//////////////////////////////////////////////////////////////////////

	void clear()
	{
		root.next = root.prev = &root;
	}

	//////////////////////////////////////////////////////////////////////

	T *head() const
	{
		return get_object(root.next);
	}

	//////////////////////////////////////////////////////////////////////

	T *tail() const
	{
		return get_object(root.prev);
	}

	//////////////////////////////////////////////////////////////////////

	T const *end()
	{
		return get_object(&root);
	}

	//////////////////////////////////////////////////////////////////////

	T *next(T *i) const
	{
		return get_object(get_node(i).next);
	}

	//////////////////////////////////////////////////////////////////////

	T *prev(T *i) const
	{
		return get_object(get_node(i).prev);
	}

	//////////////////////////////////////////////////////////////////////

	void move_into(linked_list<T, node> &new_list, std::function<bool (T *)> func)
	{
		T *i = head();
		while(i != end())
		{
			T *n = next(i);
			if(func(i))
			{
				remove(i);
				new_list.push_back(i);
			}
			i = n;
		}
	}

	//////////////////////////////////////////////////////////////////////

	void remove_if(std::function<bool (T *)> func)
	{
		T *i = head();
		while(i != end())
		{
			T *n = next(i);
			if(func(i))
			{
				remove(i);
			}
			i = n;
		}
	}

	//////////////////////////////////////////////////////////////////////

	bool for_each(std::function<void (T *)> func)
	{
		for(T *i = head(); i != end(); i = next(i))
		{
			func(i);
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////

	bool reverse_for_each(std::function<bool (T *)> func)
	{
		for(T *i = tail(); i != end(); i = prev(i))
		{
			if(!func(i))
			{
				return false;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////

	T *find(std::function<bool (T *)> func)
	{
		for(T *i = head(); i != end(); i = next(i))
		{
			if(func(i))
			{
				return i;
			}
		}
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////

	T *reverse_find(std::function<bool (T *)> func)
	{
		for(T *i = tail(); i != end(); i = prev(i))
		{
			if(func(i))
			{
				return i;
			}
		}
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////
};
