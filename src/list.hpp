#pragma once // NOLINT
#include <iterator>


namespace task {


template<class T>
class list {
    struct list_node;
public:
    class iterator {
        friend class list<T>;
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() noexcept : node{nullptr} {}
        iterator(const iterator &it) noexcept : node{it.node} {}
        iterator(list_node &node) noexcept : node(node) {}
        iterator& operator=(const iterator&) noexcept;

        iterator& operator++();
        iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        iterator& operator--();
        iterator operator--(int);

        bool operator==(iterator other) const;
        bool operator!=(iterator other) const;

    private:

        list_node *node;

    };

    friend class iterator;

    class const_iterator {
        friend class list<T>;
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator() noexcept : node{nullptr} {}
        const_iterator(const_iterator &it) noexcept : node(it.node) {}
        const_iterator(list_node &node) noexcept : node(node) {}
        const_iterator& operator=(const_iterator&) noexcept;

        const_iterator& operator++();
        const_iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator--();
        const_iterator operator--(int);

        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;

    private:

        const list_node *node;
        friend class list<T>;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    list() : first(nullptr), last(nullptr) {}
    list(size_t count, const T& value);
    explicit list(size_t count);
    ~list();

    list(const list& other);
    list& operator=(const list& other);

    T& front() { return first->data; }
    const T& front() const { return first->data; }

    T& back() { return last->prev->data; }
    const T& back() const { return last->prev->data; }


    iterator begin() const;
    iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;


    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void clear();

    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, size_t count, const T& value);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);


    void push_back(const T& value);
    void pop_back();

    void push_front(const T& value);
    void pop_front();

    void resize(size_t count);
    void swap(list& other);


    void merge(list& other);
    void splice(const_iterator pos, list& other);
    void remove(const T& value);
    void reverse();
    void unique();
    void sort();

    // Your code goes here?..

private:

    struct list_node {

        friend class iterator;
        friend class list<T>;

    public:

        list_node() noexcept;
        list_node(T value) noexcept;
        list_node(list_node &src_node) noexcept : data(src_node->data), prev(src_node->prev), next(src_node->next) {}
        ~list_node() noexcept;

        T data;
        list_node *prev;
        list_node *next;

    };

    friend struct list_node;

    list_node *first;
    list_node *last;

};

template<class T>
typename list<T>::list_node<T>::list_node() noexcept
{
    prev = nullptr;
    next = nullptr;
    data = T{};
}

template<class T>
typename list<T>::list_node<T>::list_node(T value) noexcept : data{value} {
    prev = nullptr;
    next = nullptr;
}

template<class T>
typename list<T>::iterator& list<T>::iterator::operator=(const iterator &it) noexcept
{
    node = it.node;
    return *this;
}

template<class T>
typename list<T>::iterator& list<T>::iterator::operator++()
{
    //if (!node)
        //throw "something";
    //if (!node->next)
        //throw "end of list";
    node = node->next;
    return *this;
}

template<class T>
typename list<T>::iterator list<T>::iterator::operator++(int)
{
    //if (!node)
        //throw "something";
    //if (!node->next)
        //throw "end of list";
    iterator old_it = *this;
    node = node->next;
    return old_it;
}

template<class T>
typename list<T>::iterator<T>::reference list<T>::iterator::operator*() const
{
    //if (!node)
        //throw "something";
    return node->data;
}

template<class T>
/*T**/pointer list<T>::iterator::operator->() const
{
    //if (!node)
        //throw "something";
    return &node->data;
}

template<class T>
iterator& list<T>::iterator::operator--()
{
    //if (!node)
        //throw "something";
    //if (!node->prev)
        //throw "beginning of list";
    node = node->prev;
    return *this;
}

template<class T>
iterator list<T>::iterator::operator--(int)
{
    //if (!node)
        //throw "something";
    //if (!node->prev)
        //throw "end of list";
    iterator old_it = *this;
    node = node->prev;
    return old_it;
}

template<class T>
bool list<T>::iterator::operator==(iterator other) const
{
    return (node == other.node);
}

template<class T>
bool list<T>::iterator::operator!=(iterator other) const
{
    return (node != other.node);
}

//template<class T>
//typename list<T>::iterator::ptrdiff_t list<T>::iterator::distance(typename list<T>::iterator first, typename list<T>::iterator last) {
//    ptrdiff_t result = 0;
//    while (first != last) {
//        ++first;
//        ++result;
//    }
//    return result;
//}

//const_iterator

template<class T>
const_iterator& list<T>::const_iterator::operator=(const const_iterator &it) noexcept
{
    node = it.node;
    return *this;
}

template<class T>
const_iterator& list<T>::const_iterator::operator++()
{
    //if (!node)
        //throw "something";
    //if (!node->next)
        //throw "end of list";
    node = node->next;
    return *this;
}

template<class T>
const_iterator list<T>::const_iterator::operator++(int)
{
    //if (!node)
        //throw "something";
    //if (!node->next)
        //throw "end of list";
    const_iterator old_it = *this;
    node = node->next;
    return old_it;
}

template<class T>
/*T&*/reference list<T>::const_iterator::operator*() const
{
    //if (!node)
        //throw "something";
    return node->data;
}

template<class T>
/*T**/pointer list<T>::const_iterator::operator->() const
{
    //if (!node)
        //throw "something";
    return &node->data;
}

template<class T>
const_iterator& list<T>::const_iterator::operator--()
{
    //if (!node)
        //throw "something";
    //if (!node->prev)
        //throw "beginning of list";
    const_node = node->prev;
    return *this;
}

template<class T>
const_iterator list<T>::const_iterator::operator--(int)
{
    //if (!node)
        //throw "something";
    //if (!node->prev)
        //throw "end of list";
    const_iterator old_it = *this;
    node = node->prev;
    return old_it;
}

template<class T>
bool list<T>::const_iterator::operator==(const_iterator other) const
{
    return (node == other.node);
}

template<class T>
bool list<T>::const_iterator::operator!=(const_iterator other) const
{
    return (node != other.node);
}

//template<class T>
//typename list<T>::const_iterator::ptrdiff_t list<T>::const_iterator::distance(typename list<T>::const_iterator first, typename list<T>::const_iterator last) {
//    ptrdiff_t result = 0;
//    while (first != last) {
//        ++first;
//        ++result;
//    }
//    return result;
//}

template<class T>
list<T>::list(size_t count, const T& value) : list() {
    if (count > 0) {
        first = new list_node(value);
        for (size_t i = 1, list_node *cur_node = first; i < count; ++i, cur_node = cur_node->next) {
            cur_node->next = new list_node(first);
            cur_node->next->prev = cur_node;
        }
        if (count == 0) {
            last = first;
        }
        else {
            last = new list_node(first);
        }
    }
}

template<class T>
explicit list<T>::list(size_t count) : list() {
    if (count > 0) {
        first = new list_node();
        for (size_t i = 1, list_node *cur_node = first; i < count; ++i, cur_node = cur_node->next) {
            cur_node->next = new list_node();
            cur_node->next->prev = cur_node;
        }
        if (count == 0) {
            last = first;
        }
        else {
            last = new list_node();
        }
    }
}

template<class T>
list<T>::~list() {
    for (list_node *cur_node = first; cur_node != last;) {
        list_node *next = cur_node->next;
        delete cur_node;
        cur_node = next;
    }
    delete last;
}

template<class T>
list<T>::list(const list& other) {
    first = new list_node(other.first);
    for (list_node *new_cur_node = first, list_node *cur_other_node = other.first; cur_other_node != other.last; cur_other_node = cur_other_node->next) {
        new_cur_node->next = new list_node(cur_other_node);
        new_cur_node->next->prev = new_cur_node;
    }
    last = new list_node(other.last);
}

template<class T>
list<T>::list& operator=(const list& other) {
    list<T> new_list(other);
    swap(new_list);
}

template<class T>
typename list<T>::iterator list<T>::begin() const noexcept {
    return iterator(first);
};

template<class T>
typename list<T>::iterator list<T>::end() const noexcept {
    return iterator(last);
};

template<class T>
typename list<T>::const_iterator list<T>::cbegin() const noexcept {
    return const_iterator(first);
}

template<class T>
typename list<T>::const_iterator list<T>::cend() const noexcept {
    return const_iterator(last);
}

template<class T>
typename list<T>::reverse_iterator list<T>::rbegin() const {
    return reverse_iterator(first);
}

template<class T>
typename list<T>::reverse_iterator list<T>::rend() const {
    return reverse_iterator(last);
}

template<class T>
typename list<T>::const_reverse_iterator list<T>::crbegin() const noexcept {
    return const_reverse_iterator(first);
}

template<class T>
typename list<T>::const_reverse_iterator list<T>::crend() const noexcept {
    return const_reverse_iterator(last);
}

template<class T>
bool list<T>::empty() const noexcept {
    return begin() == end();
}

template<class T>
size_t list<T>::size() const noescept {
    size_t result = 0;
    for (list_node *cur_node = first; cur_node != last; cur_node = cur_node->next) {
        result++;
    }
}

template<class T>
size_t list<T>::max_size() const noexcept {
    return std::numeric_limits<size_t>::max() / sizeof(T);
}

void clear() noexcept {
    if(empty()){
        //TODO
    }
    erase(begin(), end());
}

template<class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, const T& value) {
    if (!last && !first) {
        first = new list_node(value);
        first->next = new list_node();
        last = first->next;
        last->prev = first;
    } else {
        pos.node->prev->next = new list_node(value);
        pos.node->prev->next->prev = pos.node->prev;
        pos.node->prev->next->next = pos.node;
        pos.node->prev = pos.node->prev->next;
    }
    return --pos;
}

iterator insert(const_iterator pos, size_t count, const T& value) {
    if (!last && !first) {
        first = new list_node(value);
        first->next = new list_node();
        last = first->next;
        last->prev = first;
        for (size_t i = 1; i < count; ++i) {
            insert(iterator(last), value);
        }
    } else {
        for (size_t i = 0; i < count; ++i) {
            pos = insert(pos, value);
        }
    }
    return pos;
}

iterator erase(const_iterator pos) {
    iterator next(pos.node->next);
    pos.node->prev->next = pos.node->next;
    pos.node->next->prev = pos.node->prev;
    delete pos.node;
    return next;
}

iterator erase(const_iterator first, const_iterator last) {
    iterator cur(first.node);
    for (; cur.node != last.node;) {
        cur = erase(cur);
    }
    return cur;
}


void push_back(const T& value) {
    insert(end(), value);
}

void pop_back() {
    if (empty()) {
        //TODO
    }
    erase(--end());
}

void push_front(const T& value) {
    insert(begin(), value);
}

void pop_front() {
    if (empty()) {
        //TODO
    }
    erase(begin());
}

void resize(size_t count) {
    size_t size_list = size();
    if (size_list > count) {
        for (size_t i = 0, iterator it; i < count; ++i, ++it) {
            ;
        }
        erase(it, last);
    }
    else if (size_list < count) {
        insert(const_iterator (last->prev), count - size_list, T());
    }
}

void swap(list& other) {
    list_node *other_first = other.first;
    list_node *other_last = other.last;
    other.first = first;
    other.last = last;
    first = other_first;
    last = other_last;
}

void merge(list& other) {
    list_node *cur = first;
    list_node *cur_other = other.first;
    if (first->data < other.first->data)
    {
        cur = cur->next;
    }
    else
    {
        first = other.first;
        cur_other = cur_other->next;
    }
    list_node *new_cur = first;
    while (cur != last && cur_other != other.last)
    {
        if (cur->data < cur_other->data)
        {
            new_cur->next = cur;
            cur->prev = new_cur;
            new_nur = new_cur->next;
            cur = cur->next;
        }
        else
        {
            new_cur->next = cur_other;
            cur_other->prev = new_cur;
            new_nur = new_cur->next;
            cur_other = cur_other->next;
        }
    }
    if (cur != last)
    {
        new_cur->next = cur;
        cur->prev = new_cur;
        delete other.last;
    }
    else if (cur_other != last)
    {
        new_cur->next = cur_other;
        cur_other->prev = new_cur;
        delete last;
        last = other.last;
    }
}

void splice(const_iterator pos, list& other) {
    for (iterator it = other.begin(); it != other.end(); ++it) {
        pos = insert(pos, it->node->data);
        pos = pos->next;
    }
}

void remove(const T& value) {
    for(iterator it = begin(); it != end(); ++it){
        if (*it == value) {
            erase(it);
        }
    }
}

void reverse() {
    list<T> new_list = *this;
    clear();
    for(iterator it = new_list.begin(); it != new_list.end(); ++it) {
        this->push_front(*it);
    }
}

void unique() {
    for (iterator it = begin(); it != end(); ++it) {
        while (it->node->data == it->node->next->data) {
            erase(it);
            it = it->next;
        }
    }
}

void front_back_split(list &back)
{
    size_t size_list = size();
    iterator it = begin();
    for (int i = 0; i < size_list / 2 + size_list % 2; ++i)
    {
        ++it;
    }
    back.last = last;
    it.node->prev->next = new list_node();
    last = it.node->prev->next;
    last->prev = it.node->prev;
    back.first = it.node;
    back.first->prev = nullptr;
}

void sort() {
    list right_head();
    front_back_split(right_head);
    sort();
    right_head.sort();
    merge(right_head);
}


}  // namespace task
