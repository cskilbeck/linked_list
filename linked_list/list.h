//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

template <typename T> class list_node
{
	template <typename, size_t> friend class linked_list;

	T *prev;
	T *next;
};

//////////////////////////////////////////////////////////////////////

template <typename T, size_t N = 1> class list_member
{
	template <typename, size_t> friend class linked_list;

	inline int node(size_t index) const throw()
	{
		return list_nodes[N];
	}

	list_node<T> list_nodes[N];
};

//////////////////////////////////////////////////////////////////////

template <typename T, size_t ID = 0> class linked_list
{
public:

	//////////////////////////////////////////////////////////////////////

	linked_list() : root(*(&root_node - ID))
	{
		static_assert(ID < sizeof(*arraysize(((T *)nullptr)->list_nodes)), "needs more nodes...");

		root.next = root.prev = reinterpret_cast<T *>(&root);
	}

	//////////////////////////////////////////////////////////////////////

	void push_front(T *obj) throw()
	{
		list_node<T> &node = obj->list_nodes[ID];
		root.next->list_nodes[ID].prev = obj;
		node.next = root_node.next;
		root.next = obj;
		node.prev = reinterpret_cast<T *>(&root_node);
	}

	//////////////////////////////////////////////////////////////////////

	void push_front(T &obj) throw()
	{
		list_node<T> &node = obj.list_nodes[ID];
		root.next->list_nodes[ID].prev = &obj;
		node.next = root_node.next;
		root.next = &obj;
		node.prev = reinterpret_cast<T *>(&root_node);
	}

	//////////////////////////////////////////////////////////////////////

	void push_back(T *obj) throw()
	{
		list_node<T> &node = obj->list_nodes[ID];
		root.prev->list_nodes[ID].next = obj;
		node.prev = root_node.prev;
		root.prev = obj;
		node.next = reinterpret_cast<T *>(&root_node.);
	}

	//////////////////////////////////////////////////////////////////////

	void push_back(T &obj) throw()
	{
		list_node<T> &node = obj.list_nodes[ID];
		root_node.prev->list_nodes[ID].next = &obj;
		node.prev = root_node.prev;
		root.prev = &obj;
		node.next = reinterpret_cast<T *>(&root_node);
	}

	//////////////////////////////////////////////////////////////////////

	void insert_before(T *pos, T *obj) throw()
	{
		assert(!empty());
		list_node<T> &node = obj->list_nodes[ID];
		list_node<T> &n = pos->list_nodes[ID];
		n.prev->list_nodes[ID].next = obj;
		node.prev = n.prev;
		n.prev = obj;
		node.next = pos;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_before(T &pos, T &obj) throw()
	{
		assert(!empty());
		list_node<T> &node = obj.list_nodes[ID];
		list_node<T> &n = pos.list_nodes[ID];
		n.prev->list_nodes[ID].next = &obj;
		node.prev = n.prev;
		n.prev = &obj;
		node.next = &pos;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_after(T *pos, T *obj) throw()
	{
		assert(!empty());
		list_node<T> &node = obj->list_nodes[ID];
		list_node<T> &n = pos->list_nodes[ID];
		n.next->list_nodes[ID].prev = obj;
		node.next = n->next;
		n.next = obj;
		node.prev = pos;
	}

	//////////////////////////////////////////////////////////////////////

	void insert_after(T &pos, T &obj) throw()
	{
		assert(!empty());
		list_node<T> &node = obj.list_nodes[ID];
		list_node<T> &n = pos.list_nodes[ID];
		n.next->list_nodes[ID].prev = &obj;
		node.next = n.next;
		n.next = &obj;
		node.prev = &pos;
	}

	//////////////////////////////////////////////////////////////////////

	void remove(T *obj) throw()
	{
		assert(!empty());
		list_node<T> &node = obj->list_nodes[ID];
		node.prev->list_nodes[ID].next = node.next;
		node.next->list_nodes[ID].prev = node.prev;
	}

	//////////////////////////////////////////////////////////////////////

	void remove(T &obj) throw()
	{
		assert(!empty());
		list_node<T> &node = obj.list_nodes[ID];
		node.prev->list_nodes[ID].next = node.next;
		node.next->list_nodes[ID].prev = node.prev;
	}

	//////////////////////////////////////////////////////////////////////

	T *pop_back() throw()
	{
		assert(!empty());
		T *o = root.prev;
		list_node<T> &node = o->list_nodes[ID];
		node.prev->list_nodes[ID].next = node.next;
		node.next->list_nodes[ID].prev = node.prev;
		return o;
	}

	//////////////////////////////////////////////////////////////////////

	T *pop_front() throw()
	{
		assert(!empty());
		T *o = root.next;
		list_node<T> &node = o->list_nodes[ID];
		node.prev->list_nodes[ID].next = node.next;
		node.next->list_nodes[ID].prev = node.prev;
		return o;
	}

	//////////////////////////////////////////////////////////////////////

	bool empty() const throw()
	{
		return root.next == reinterpret_cast<T const *>(&root);
	}

	//////////////////////////////////////////////////////////////////////

	T const *end() const throw()
	{
		return reinterpret_cast<T const *>(&root);
	}

	//////////////////////////////////////////////////////////////////////

	T *head() const throw()
	{
		return root.next;
	}

	//////////////////////////////////////////////////////////////////////

	T *tail() const throw()
	{
		return root.prev;
	}

	//////////////////////////////////////////////////////////////////////

	T *next(T *o) const throw()
	{
		return o->list_nodes[ID].next;
	}

	//////////////////////////////////////////////////////////////////////

	void clear() throw()
	{
		root.next = root.prev = reinterpret_cast<T *>(&root);
	}

	//////////////////////////////////////////////////////////////////////

	bool for_each(std::function<bool (T *)> func)
	{
		for(T *i = head(); i != end(); i = next(i))
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

private:

	//////////////////////////////////////////////////////////////////////

	list_node<T> root_node;
	list_node<T> &root;

	//////////////////////////////////////////////////////////////////////

	template <typename S, size_t C> char (*arraysize(S(&)[C]))[C];

	//////////////////////////////////////////////////////////////////////

};

