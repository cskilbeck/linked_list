//////////////////////////////////////////////////////////////////////

#pragma once

// Deal with MSVC internal compiler error
// this can be removed when a fix is available

#pragma push_macro("VC_WORKAROUND")
#undef VC_WORKAROUND
#if defined(_MSC_VER)
#define VC_WORKAROUND NODE
#else
#define VC_WORKAROUND (list_node<T> T::*)nullptr != NODE
#endif

//////////////////////////////////////////////////////////////////////

namespace chs
{

//////////////////////////////////////////////////////////////////////
// base list node class, 2 pointers

template <typename T> struct list_node_base
{
    T *next;
    T *prev;
};

//////////////////////////////////////////////////////////////////////
// base node is wrapped so we can get the offset to it

template <typename T> struct list_node
{
    list_node_base<T> node;
};

//////////////////////////////////////////////////////////////////////
// template base

template <typename T, list_node<T> T::*NODE, bool is_member> class list_base
{
};

//////////////////////////////////////////////////////////////////////
// specialization for instances using list_node as member field

template <typename T, list_node<T> T::*NODE> class list_base<T, NODE, true> : protected list_node<T>
{
protected:
    static size_t offset()
    {
        list_node<T> *b = &(((T *)0)->*NODE);
        return size_t(&b->node);
    }
};

//////////////////////////////////////////////////////////////////////
// specialization for instances deriving from list_node

template <typename T, list_node<T> T::*NODE> class list_base<T, NODE, false> : protected list_node<T>
{
    static_assert(!std::is_polymorphic<T>::value, "polymorphic! use the member-node version");
protected:
    static size_t offset()
    {
        list_node_base<T> T::*n = static_cast<list_node_base<T> T::*>(&T::node);
        return (size_t)(&(((T *)0)->*n));
    }
};

//////////////////////////////////////////////////////////////////////
// the actual list

template <typename T, list_node<T> T::*NODE = nullptr> class linked_list : protected list_base<T, NODE, VC_WORKAROUND>
{
public:

    using list_base<T, NODE, VC_WORKAROUND>::offset;
    using list_base<T, NODE, VC_WORKAROUND>::node;

    typedef T *            ptr;
    typedef T const *    const_ptr;
    typedef T &            ref;
    typedef T const &    const_ref;

    #if !defined(_CHS_LINKED_LIST_DONT_DEFINE_STL_ITERATORS_)

    typedef std::iterator<std::bidirectional_iterator_tag, T> iterator_base;

    struct const_iterator : iterator_base
    {
        const_iterator(const_ptr t) : p(t) {}
        const_iterator(const_iterator const &o) : p(o.p) {}
        const_iterator const &operator=(const_iterator const &o) { p = o.p; return *this; }
        const_iterator &operator++() { p = get_node(p).next; return *this; }
        const_iterator &operator--() { p = get_node(p).prev; return *this; }
        bool operator==(const_iterator const &o) { return p == o.p; }
        bool operator!=(const_iterator const &o) { return p != o.p; }
        const_ref operator *() { return *p; }
        const_ptr operator->() { return  p; }
    private:
        const_ptr p;
    };

    struct iterator : iterator_base
    {
        iterator(ptr t) : p(t) {}
        iterator(const_ptr *t) : p(t) {}
        iterator(iterator const &o) : p(o.p) {}
        iterator const &operator=(const_iterator const &o) { p = o.p; return *this; }
        iterator const &operator=(iterator const &o) { p = o.p; return *this; }
        iterator &operator++() { p = get_node(p).next; return *this; }
        iterator &operator--() { p = get_node(p).prev; return *this; }
        bool operator==(iterator const &o) { return p == o.p; }
        bool operator!=(iterator const &o) { return p != o.p; }
        ref operator *() { return *p; }
        ptr operator->() { return  p; }
    private:
        ptr p;
    };

    struct const_reverse_iterator : iterator_base
    {
        const_reverse_iterator(const_ptr t) : p(t) {}
        const_reverse_iterator(const_reverse_iterator const &o) : p(o.p) {}
        const_reverse_iterator const &operator=(const_reverse_iterator const &o) { p = o.p; return o; }
        const_reverse_iterator &operator++() { p = get_node(p).prev; return *this; }
        const_reverse_iterator &operator--() { p = get_node(p).next; return *this; }
        bool operator==(const_reverse_iterator const &o) { return p == o.p; }
        bool operator!=(const_reverse_iterator const &o) { return p != o.p; }
        const_ref operator *() { return *p; }
        const_ptr operator->() { return  p; }
    private:
        const_ptr p;
    };

    struct reverse_iterator : iterator_base
    {
        reverse_iterator(ptr t) : p(t) {}
        reverse_iterator(const_ptr t) : p(t) {}
        reverse_iterator(reverse_iterator const &o) : p(o.p) {}
        reverse_iterator const &operator=(const_reverse_iterator const &o) { p = o.p; return *this; }
        reverse_iterator const &operator=(reverse_iterator const &o) { p = o.p; return *this; }
        reverse_iterator &operator++() { p = get_node(p).prev; return *this; }
        reverse_iterator &operator--() { p = get_node(p).next; return *this; }
        bool operator==(reverse_iterator const &o) { return p == o.p; }
        bool operator!=(reverse_iterator const &o) { return p != o.p; }
        ref operator *() { return *p; }
        ptr operator->() { return  p; }
    private:
        ptr p;
    };

    iterator                  begin()         { return iterator(node.next); }
    const_iterator            begin() const   { return const_iterator(node.next); }

    iterator                  end()           { return iterator(root()); }
    const_iterator            end() const     { return const_iterator(const_root()); }

    const_iterator            cbegin() const  { return const_iterator(node.next); }
    const_iterator            cend() const    { return const_iterator(const_root()); }

    reverse_iterator          rbegin() const  { return reverse_iterator(node.prev); }
    reverse_iterator          rend() const    { return reverse_iterator(root()); }

    const_reverse_iterator    crbegin() const { return const_reverse_iterator(node.prev); }
    const_reverse_iterator    crend() const   { return const_reverse_iterator(const_root()); }

    #endif //!defined(_CHS_LINKED_LIST_DONT_DEFINE_STL_ITERATORS_)

private:

    typedef list_node_base<T>        node_t;
    typedef node_t *                 node_ptr;
    typedef node_t &                 node_ref;
    typedef list_node_base<T> const  const_node_t;
    typedef const_node_t *           const_node_ptr;
    typedef const_node_t &           const_node_ref;

    // get an object's node
    static node_ref get_node(ptr obj)
    {
        return *reinterpret_cast<node_ptr>(reinterpret_cast<char *>(obj) + offset());
    }

    static const_node_ref get_node(const_ptr obj)
    {
        return *reinterpret_cast<const_node_ptr>(reinterpret_cast<char const *>(obj) + offset());
    }

    ptr root()
    {
        return reinterpret_cast<ptr>(reinterpret_cast<char *>(&node) - offset());
    }

    const_ptr const_root() const
    {
        return reinterpret_cast<const_ptr>(reinterpret_cast<char const *>(&node) - offset());
    }

public:

    // basic operations

    // make the list empty
    void clear()
    {
        node.next = root();
        node.prev = root();
    }

    // insert an object before another object
    void insert_before(ptr obj_before, ptr obj)
    {
        ptr &p = get_node(obj_before).prev;
        node_ref n = get_node(obj);
        get_node(p).next = obj;
        n.prev = p;
        p = obj;
        n.next = obj_before;
    }

    // insert an object after another object
    void insert_after(ptr obj_after, ptr obj)
    {
        ptr &n = get_node(obj_after).next;
        node_ref p = get_node(obj);
        get_node(n).prev = obj;
        p.next = n;
        n = obj;
        p.prev = obj_after;
    }

    // remove an object from the list
    ptr remove(ptr obj)
    {
        ptr p = prev(obj);
        ptr n = next(obj);
        get_node(p).next = n;
        get_node(n).prev = p;
        return obj;
    }

    linked_list()
    {
        clear();
    }

    bool empty() const
    {
        return node.next == const_root();
    }

    // non-stl style iteration using T* and T const *

    ptr       head()                         { return node.next; }
    ptr       tail()                         { return node.prev; }

    const_ptr c_head() const                 { return node.next; }
    const_ptr c_tail() const                 { return node.prev; }

    ptr       next(ptr obj)                  { return get_node(obj).next; }
    ptr       prev(ptr obj)                  { return get_node(obj).prev; }

    const_ptr c_next(const_ptr obj) const    { return get_node(obj).next; }
    const_ptr c_prev(const_ptr obj) const    { return get_node(obj).prev; }

    ptr       done()                         { return const_root(); }
    const_ptr c_done() const                 { return const_root(); }

    // all these operations rely on the basic operations

    ptr       remove(ref obj)                 { return remove(&obj); }
                                                    
    void      push_back(ref obj)              { insert_before(*root(), obj); }
    void      push_back(ptr obj)              { insert_before(root(), obj); }
                                                    
    void      push_front(ref obj)             { insert_after(*root(), obj); }
    void      push_front(ptr obj)             { insert_after(root(), obj); }
                                                    
    ptr       pop_front()                     { return empty() ? nullptr : remove(head()) }
    ptr       pop_back()                      { return empty() ? nullptr : remove(tail()) }

    void      insert_before(ref bef, ref obj) { insert_before(&bef, &obj); }
    void      insert_after(ref aft, ref obj)  { insert_after(&aft, &obj); }

};

//////////////////////////////////////////////////////////////////////

} // chs

#pragma warning(push)
#pragma warning(disable: 4602)        // disable spurious warning
#pragma pop_macro("VC_WORKAROUND")
#pragma warning(pop)

