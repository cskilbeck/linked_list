//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

template <typename T>
class list_node_base
{
public:
	T *next;
	T *prev;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
class linked_list_node
{
public:
	list_node_base<T> list_node;
};

//////////////////////////////////////////////////////////////////////

template <typename T, linked_list_node<T> T::*NODE, bool is_member>
class list_base
{
};

//////////////////////////////////////////////////////////////////////

template <typename T, linked_list_node<T> T::*NODE>
class list_base<T, NODE, true>
	: protected linked_list_node<T>
{
protected:
	static size_t const offset()
	{
		return (size_t)&reinterpret_cast<const volatile char &>(((T *)0)->*NODE);
	}
};

//////////////////////////////////////////////////////////////////////

template <typename T, linked_list_node<T> T::*NODE>
class list_base<T, NODE, false>
	: protected linked_list_node<T>
{
protected:
	static size_t const offset()
	{
		return (size_t)&reinterpret_cast<const volatile char &>(((T *)0)->list_node);
	}
};

//////////////////////////////////////////////////////////////////////

template <typename T, linked_list_node<T> T::*NODE = nullptr>
class linked_list
	: protected list_base<T, NODE, NODE>
{
public:

	typedef T *						pointer;
	typedef T const *				const_pointer;
	typedef T &						reference;
	typedef T const &				const_reference;

	// !! #include <iterator> before linked_list.h to get std::iterators defined (but not for push/pop operators)
	#if defined(_ITERATOR_)

	class iterator : std::iterator<std::bidirectional_iterator_tag, T>
	{
	public:
		iterator() : p(nullptr) { }
		iterator(T *t) : p(t) { }
		iterator(iterator const &o) : p(o.p) { }
		iterator const &operator=(iterator const &o) { p = o.p; return o; }
		iterator &operator++() { p = node(p).next; return *this; }
		iterator &operator--() { p = node(p).prev; return *this; }
		iterator operator++(int) { iterator t(*this); p = node(p).next; return t; }
		iterator operator--(int) { iterator t(*this); p = node(p).prev; return t; }
		bool operator==(iterator const &o) { return p == o.p; }
		bool operator!=(iterator const &o) { return p != o.p; }
		T &operator *() { return *p; }
		T *operator->() { return  p; }
	private:
		T *p;
		friend class const_iterator;
	};

	class const_iterator : std::iterator<std::bidirectional_iterator_tag, T>
	{
	public:
		const_iterator() : p(nullptr) { }
		const_iterator(T const *t) : p(t) { }
		const_iterator(const_iterator const &o) : p(o.p) { }
		const_iterator const &operator=(const_iterator const &o) { p = o.p; return o; }
		const_iterator const &operator=(iterator const &o) { p = o.p; return o; }
		const_iterator &operator++() { p = node(p).next; return *this; }
		const_iterator &operator--() { p = node(p).prev; return *this; }
		const_iterator operator++(int) { iterator t(*this); p = node(p).next; return t; }
		const_iterator operator--(int) { iterator t(*this); p = node(p).prev; return t; }
		bool operator==(const_iterator const &o) { return p == o.p; }
		bool operator!=(const_iterator const &o) { return p != o.p; }
		T const &operator *() { return *p; }
		T const *operator->() { return  p; }
	private:
		T const *p;
	};

	class reverse_iterator : std::iterator<std::bidirectional_iterator_tag, T>
	{
	public:
		reverse_iterator() : p(nullptr) { }
		reverse_iterator(T *t) : p(t) { }
		reverse_iterator(reverse_iterator const &o) : p(o.p) { }
		reverse_iterator const &operator=(reverse_iterator const &o) { p = o.p; return o; }
		reverse_iterator &operator++() { p = node(p).next; return *this; }
		reverse_iterator &operator--() { p = node(p).prev; return *this; }
		reverse_iterator operator++(int) { reverse_iterator t(*this); p = node(p).prev; return t; }
		reverse_iterator operator--(int) { reverse_iterator t(*this); p = node(p).next; return t; }
		bool operator==(reverse_iterator const &o) { return p == o.p; }
		bool operator!=(reverse_iterator const &o) { return p != o.p; }
		T &operator *() { return *p; }
		T *operator->() { return  p; }
	private:
		T *p;
		friend class const_reverse_iterator;
	};

	class const_reverse_iterator : std::iterator<std::bidirectional_iterator_tag, T>
	{
	public:
		const_reverse_iterator() : p(nullptr) { }
		const_reverse_iterator(T const *t) : p(t) { }
		const_reverse_iterator(const_reverse_iterator const &o) : p(o.p) { }
		const_reverse_iterator const &operator=(const_iterator const &o) { p = o.p; return o; }
		const_reverse_iterator const &operator=(iterator const &o) { p = o.p; return o; }
		const_reverse_iterator &operator++() { p = node(p).next; return *this; }
		const_reverse_iterator &operator--() { p = node(p).prev; return *this; }
		const_reverse_iterator operator++(int) { iterator t(*this); p = node(p).prev; return t; }
		const_reverse_iterator operator--(int) { iterator t(*this); p = node(p).next; return t; }
		bool operator==(const_reverse_iterator const &o) { return p == o.p; }
		bool operator!=(const_reverse_iterator const &o) { return p != o.p; }
		T const &operator *() { return *p; }
		T const *operator->() { return  p; }
	private:
		T const *p;
	};

	#endif //defined(_ITERATOR_)

private:

	typedef list_node_base<T>		node_t;
	typedef node_t *				node_pointer;
	typedef node_t &				node_ref;
	typedef list_node_base<T> const	const_node_t;
	typedef const_node_t *			const_node_pointer;
	typedef const_node_t &			const_node_ref;

	static node_ref node(pointer obj)
	{
		return *reinterpret_cast<node_pointer>
			(reinterpret_cast<char *>(obj) + offset());
	}

	static const_node_ref node(const_pointer obj)
	{
		return *reinterpret_cast<const_node_pointer>
			(reinterpret_cast<char const *>(obj) + offset());
	}

	pointer root()
	{
		return reinterpret_cast<pointer>
			(reinterpret_cast<char *>(&list_node) - offset());
	}

	const_pointer root() const
	{
		return reinterpret_cast<const_pointer>
			(reinterpret_cast<char const *>(&list_node) - offset());
	}

public:

	void clear()
	{
		list_node.next = root();
		list_node.prev = root();
	}

	void insert_before(pointer obj_before, pointer obj)
	{
		pointer &p = node(obj_before).prev;
		node_ref n = node(obj);
		node(p).next = obj;
		n.prev = p;
		p = obj;
		n.next = obj_before;
	}

	void insert_after(pointer obj_after, pointer obj)
	{
		pointer &n = node(obj_after).next;
		node_ref p = node(obj);
		node(n).prev = obj;
		p.next = n;
		n = obj;
		p.prev = obj_after;
	}

	pointer remove(pointer obj)
	{
		pointer p = prev(obj);
		pointer n = next(obj);
		node(p).next = n;
		node(n).prev = p;
		return obj;
	}

						linked_list()			{ clear(); }

	bool				empty() const			{ return list_node.next == root(); }

	#if defined(_ITERATOR_)

	iterator			begin()	const			{ return iterator(list_node.next); }
	const_iterator		cbegin() const			{ return const_iterator(list_node.next); }
	reverse_iterator	rbegin() const			{ return reverse_iterator(list_node.next); }
	const_reverse_iterator crbegin() const		{ return const_reverse_iterator(list_node.next); }
	iterator			end()					{ return iterator(root()); }
	const_iterator		cend() const			{ return const_iterator(root()); }
	iterator			rend()					{ return reverse_iterator(root()); }
	const_reverse_iterator crend() const		{ return const_reverse_iterator(root()); }

	#endif //defined(_ITERATOR_)

	pointer				head()					{ return list_node.next; }
	pointer				tail()					{ return list_node.prev; }

	const_pointer		c_head() const			{ return list_node.next; }
	const_pointer		c_tail() const			{ return list_node.prev; }

	pointer				next(pointer obj)		{ return node(obj).next; }
	pointer				prev(pointer obj)		{ return node(obj).prev; }

	const_pointer		c_next(const_pointer obj) const	{ return node(obj).next; }
	const_pointer		c_prev(const_pointer obj) const	{ return node(obj).prev; }

	pointer				done()					{ return root(); }
	const_pointer		c_done() const			{ return root(); }

	pointer				remove(reference obj)	{ return remove(&obj); }

	void				push_back(reference obj){ insert_before(*root(), obj); }
	void				push_back(pointer obj)	{ insert_before(root(), obj); }

	void				push_front(reference obj){ insert_after(*root(), obj); }
	void				push_front(pointer obj)	{ insert_after(root(), *obj); }

	pointer				pop_front()				{ return !empty() ? remove(head()) : nullptr; }
	pointer				pop_back()				{ return !empty() ? remove(tail()) : nullptr; }

	void				insert_before(reference obj_before, reference obj)
												{ insert_before(&obj_before, &obj); }

	void				insert_after(reference obj_after, reference obj)
												{ insert_after(&obj_after, &obj); }

};

//////////////////////////////////////////////////////////////////////
