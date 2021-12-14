#pragma once // NOLINT
#include <iterator>
#include <limits>

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
        iterator(list_node *node) noexcept : node{node} {}
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

    class const_iterator {
        friend class list<T>;
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator() noexcept : node{nullptr} {}
        const_iterator(const iterator &it) noexcept : node{it.node} {}
        const_iterator(const const_iterator &it) noexcept : node{it.node} {}
        const_iterator(list_node *node) noexcept : node{node} {}
        const_iterator& operator=(const const_iterator&) noexcept;

        const_iterator& operator++();
        const_iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator--();
        const_iterator operator--(int);

        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;

     private:
        list_node *node;
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


    iterator begin() const noexcept;
    iterator end() const noexcept;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() const noexcept;

    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;


    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t max_size() const noexcept { return max_size_list; }
    void clear() noexcept;

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

 private:
    struct list_node {
     public:
        list_node() noexcept;
        list_node(T value) noexcept;
        list_node(const list_node &src_node) noexcept :
            data(src_node->data), prev(src_node->prev), next(src_node->next) {}
        ~list_node() = default;

        T data;
        list_node *prev;
        list_node *next;
    };

    list_node *first;
    list_node *last;

    void front_back_split(list &back, size_t size_list);

    size_t max_size_list = std::numeric_limits<size_t>::max() / sizeof(T);
};

template<class T>
list<T>::list_node::list_node() noexcept {
    prev = nullptr;
    next = nullptr;
    data = T{};
}

template<class T>
list<T>::list_node::list_node(T value) noexcept : data{value} {
    prev = nullptr;
    next = nullptr;
}

template<class T>
typename list<T>::iterator& list<T>::iterator::operator=(const iterator &it) noexcept {
    node = it.node;
    return *this;
}

template<class T>
typename list<T>::iterator& list<T>::iterator::operator++() {
    node = node->next;
    return *this;
}

template<class T>
typename list<T>::iterator list<T>::iterator::operator++(int) {
    iterator old_it = *this;
    node = node->next;
    return old_it;
}

template<class T>
typename list<T>::iterator::reference list<T>::iterator::operator*() const {
    return node->data;
}

template<class T>
typename list<T>::iterator::pointer list<T>::iterator::operator->() const {
    return &node->data;
}

template<class T>
typename list<T>::iterator& list<T>::iterator::operator--() {
    node = node->prev;
    return *this;
}

template<class T>
typename list<T>::iterator list<T>::iterator::operator--(int) {
    iterator old_it = *this;
    node = node->prev;
    return old_it;
}

template<class T>
bool list<T>::iterator::operator==(iterator other) const {
    return (node == other.node);
}

template<class T>
bool list<T>::iterator::operator!=(iterator other) const {
    return (node != other.node);
}

template<class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator=(const const_iterator &it) noexcept {
    node = it.node;
    return *this;
}

template<class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator++() {
    node = node->next;
    return *this;
}

template<class T>
typename list<T>::const_iterator list<T>::const_iterator::operator++(int) {
    const_iterator old_it = *this;
    node = node->next;
    return old_it;
}

template<class T>
typename list<T>::const_iterator::reference list<T>::const_iterator::operator*() const {
    return node->data;
}

template<class T>
typename list<T>::const_iterator::pointer list<T>::const_iterator::operator->() const {
    return &node->data;
}

template<class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator--() {
    node = node->prev;
    return *this;
}

template<class T>
typename list<T>::const_iterator list<T>::const_iterator::operator--(int) {
    const_iterator old_it = *this;
    node = node->prev;
    return old_it;
}

template<class T>
bool list<T>::const_iterator::operator==(const_iterator other) const {
    return (node == other.node);
}

template<class T>
bool list<T>::const_iterator::operator!=(const_iterator other) const {
    return (node != other.node);
}

template<class T>
list<T>::list(size_t count, const T& value) : list() {
    for (size_t i = 0; i < count; ++i) {
        push_front(value);
    }
}

template<class T>
list<T>::list(size_t count) : list() {
    for (size_t i = 0; i < count; ++i) {
        push_front(T{});
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
list<T>::list(const list<T>& other) {
    first = new list_node(other.first->data);
    list_node *new_cur_node = first;
    for (list_node *cur_other_node = other.first->next; cur_other_node != other.last;
                                                    cur_other_node = cur_other_node->next) {
        new_cur_node->next = new list_node(cur_other_node->data);
        new_cur_node->next->prev = new_cur_node;
        new_cur_node = new_cur_node->next;
    }
    last = new list_node(other.last->data);
    last->prev = new_cur_node;
    new_cur_node->next = last;
}

template<class T>
list<T>& list<T>::operator=(const list<T>& other) {
    list<T> new_list(other);
    swap(new_list);
    new_list.clear();
    return *this;
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
typename list<T>::reverse_iterator list<T>::rbegin() const noexcept {
    return reverse_iterator(end());
}

template<class T>
typename list<T>::reverse_iterator list<T>::rend() const noexcept {
    return reverse_iterator(begin());
}

template<class T>
typename list<T>::const_reverse_iterator list<T>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template<class T>
typename list<T>::const_reverse_iterator list<T>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template<class T>
bool list<T>::empty() const noexcept {
    return begin() == end();
}

template<class T>
size_t list<T>::size() const noexcept {
    size_t result = 0;
    for (list_node *cur_node = first; cur_node != last; cur_node = cur_node->next) {
        result++;
    }
    return result;
}

template<class T>
void list<T>::clear() noexcept {
    erase(begin(), end());
}

template<class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, const T& value) {
    if (!last && !first) {
        first = new list_node(value);
        first->next = new list_node();
        last = first->next;
        last->prev = first;
        pos = end();
    } else if (pos == begin()) {
        pos.node->prev = new list_node(value);
        pos.node->prev->next = first;
        first = pos.node->prev;
    } else {
        pos.node->prev->next = new list_node(value);
        pos.node->prev->next->prev = pos.node->prev;
        pos.node->prev->next->next = pos.node;
        pos.node->prev = pos.node->prev->next;
    }
    return iterator((--pos).node);
}

template <class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, size_t count, const T& value) {
    if (!last && !first) {
        first = new list_node(value);
        first->next = new list_node();
        last = first->next;
        last->prev = first;
        for (size_t i = 1; i < count; ++i) {
            pos = insert(iterator(last), value);
        }
    } else {
        for (size_t i = 0; i < count; ++i) {
            pos = insert(pos, value);
        }
    }
    return iterator(pos.node);
}

template <class T>
typename list<T>::iterator list<T>::erase(const_iterator pos) {
    if (pos == begin()) {
        first = pos.node->next;
        delete pos.node;
        return first;
    }
    iterator next(pos.node->next);
    pos.node->prev->next = pos.node->next;
    pos.node->next->prev = pos.node->prev;
    delete pos.node;
    return next;
}

template <class T>
typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last) {
    iterator cur(first.node);
    for (; cur.node != last.node;) {
        cur = erase(cur);
    }
    return cur;
}

template<class T>
void list<T>::push_back(const T& value) {
    insert(end(), value);
}

template<class T>
void list<T>::pop_back() {
    if (!empty()) {
        erase(--end());
    }
}

template<class T>
void list<T>::push_front(const T& value) {
    insert(begin(), value);
}

template<class T>
void list<T>::pop_front() {
    if (!empty()) {
        erase(begin());
    }
}

template<class T>
void list<T>::resize(size_t count) {
    size_t size_list = size();
    if (size_list > count) {
        iterator it = begin();
        for (size_t i = 0; i < count; ++i, ++it) {}
        erase(it, end());
    } else if (size_list < count) {
        insert(const_iterator(last), count - size_list, T{});
    }
}

template<class T>
void list<T>::swap(list<T>& other) {
    list_node *other_first = other.first;
    list_node *other_last = other.last;
    other.first = first;
    other.last = last;
    first = other_first;
    last = other_last;
}

template<class T>
void list<T>::merge(list<T>& other) {
    if (first == other.first && last == other.last)
        return;
    list_node *cur = first;
    list_node *cur_other = other.first;
    if (first->data <= other.first->data) {
        cur = cur->next;
    } else {
        first = other.first;
        cur_other = cur_other->next;
    }
    list_node *new_cur = first;
    while (cur != last && cur_other != other.last) {
        if (cur->data <= cur_other->data) {
            new_cur->next = cur;
            cur->prev = new_cur;
            new_cur = new_cur->next;
            cur = cur->next;
        } else {
            new_cur->next = cur_other;
            cur_other->prev = new_cur;
            new_cur = new_cur->next;
            cur_other = cur_other->next;
        }
    }
    if (cur != last) {
        new_cur->next = cur;
        cur->prev = new_cur;
        delete other.last;
        other.first = nullptr;
        other.last = nullptr;
    }  else if (cur_other != last) {
        new_cur->next = cur_other;
        cur_other->prev = new_cur;
        delete last;
        last = other.last;
        other.first = nullptr;
        other.last = nullptr;
    }
}

template<class T>
void list<T>::splice(const_iterator pos, list<T>& other) {
    if (pos == begin()) {
        pos.node->prev = other.last->prev;
        other.last->prev->next = pos.node;
        first = other.first;
    } else {
        pos.node->prev->next = other.first;
        other.first->prev = pos.node->prev;
        pos.node->prev = other.last->prev;
        other.last->prev->next = pos.node;
    }

    other.first = other.last;
}

template<class T>
void list<T>::remove(const T& value) {
    for (iterator it = begin(); it != end(); ++it) {
        if (*it == value) {
            erase(it);
        }
    }
}

template<class T>
void list<T>::reverse() {
    if (first == last)
        return;
    for (list_node *cur = first; cur != last; cur = cur->prev) {
        list_node *prev = cur->prev;
        cur->prev = cur->next;
        cur->next = prev;
    }
    first->next = last;
    list_node *last_prev = last->prev;
    last->prev = first;
    first = last_prev;
}

template<class T>
void list<T>::unique() {
    for (iterator it = begin(); it != end(); ++it) {
        while (it.node->data == it.node->next->data) {
            it = erase(it);
        }
    }
}

template<class T>
void list<T>::front_back_split(list<T> &back, size_t size_list) {
    iterator it = begin();
    for (size_t i = 0; i < size_list / 2 + size_list % 2; ++i) {
        ++it;
    }
    back.last = last;
    it.node->prev->next = new list_node();
    last = it.node->prev->next;
    last->prev = it.node->prev;
    back.first = it.node;
    back.first->prev = nullptr;
}

template<class T>
void list<T>::sort() {
    size_t size_list = size();
    if (size_list == 0 || size_list == 1)
        return;
    list<T> right_head;
    front_back_split(right_head, size_list);
    sort();
    right_head.sort();
    merge(right_head);
}


}  // namespace task
