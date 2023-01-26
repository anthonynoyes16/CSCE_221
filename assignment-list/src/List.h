#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;
    public:
        basic_iterator() {
            node = nullptr;
        }
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}

        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

        reference operator*() const {
            return node->data;
        }
        pointer operator->() const {
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            basic_iterator hold = basic_iterator(node);
            node = node->next;
            return hold;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            basic_iterator hold = basic_iterator(node);
            node = node->prev;
            return hold;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            return (node == other.node);
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            return (node != other.node);
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() : head(), tail(), _size(0) {
        head.next = &tail;
        tail.prev = &head;
    }

    List(size_type count, const T& value    ) : head(), tail(), _size(0) { 
        head.next = &tail;
        tail.prev = &head;
        for (int i = 0; i < count; i++) {
            Node* new_node = new Node(value);
            head.next->prev = new_node;
            new_node->next = head.next;
            head.next = new_node;
            new_node->prev = &head;
            _size += 1;
        }
    }

    explicit List(size_type count) : head(), tail(), _size(0) {
        head.next = &tail;
        tail.prev = &head;
        for (size_type i = 0; i < count; i++) {
            Node* new_node = new Node(T());
            head.next->prev = new_node;
            new_node->next = head.next;
            head.next = new_node;
            new_node->prev = &head;
            _size += 1;
        }
    }

    List( const List& other ) : head(), tail(), _size(0){
        head.next = &tail;
        tail.prev = &head;
        iterator hold = iterator(&other.tail);
        for (size_type i = 0; i < other._size; i++) {
            --hold;
            Node* new_node = new Node(hold.node->data);
            head.next->prev = new_node;
            new_node->next = head.next;
            head.next = new_node;
            new_node->prev = &head;
            _size += 1;
        }
    }

    List( List&& other ) : head(), tail(), _size(0){
        if (other.empty()) {
            head.next = &tail;
            tail.prev = &head;
        }
        else {
            head.next = other.head.next;
            tail.prev = other.tail.prev;
            head.next->prev = &head;
            tail.prev->next = &tail;
            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            _size = other._size;
            other._size = 0;
        }
    }

    ~List() {
        while (head.next != &tail) {
            head.next = head.next->next;
            delete head.next->prev;
            head.next->prev = &head;
        }
        _size = 0;
        head.next = &tail;
        tail.prev = &head;
    }

    List& operator=(const List& other ) {
        if (const_iterator(head.next) == const_iterator(other.head.next)) {
            return *this;
        }
        this->clear();
        iterator hold = iterator(&other.tail);
        for (size_type i = 0; i < other._size; i++) {
            --hold;
            Node* new_node = new Node(hold.node->data);
            head.next->prev = new_node;
            new_node->next = head.next;
            head.next = new_node;
            new_node->prev = &head;
            _size += 1;
        }
        return *this;
    }

    List& operator=(List&& other ) noexcept {
        
        if (const_iterator(head.next) == const_iterator(other.head.next)) {
            return *this;
        }
        this->clear();
        if (other.empty()) {
            head.next = &tail;
            tail.prev = &head;
            _size = 0;
            return *this;
        }
        else {
            head.next = other.head.next;
            tail.prev = other.tail.prev;
            head.next->prev = &head;
            tail.prev->next = &tail;
            _size = other._size;
            other._size = 0;
            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            other.head.prev = nullptr;
            other.tail.next = nullptr;
        }
        return *this;
    }

    reference front() {
        return ((head.next)->data);
    }

    const_reference front() const {
        const_reference hold = ((head.next)->data);
        return hold;
    }
	
    reference back() {
        return ((tail.prev)->data);
    }

    const_reference back() const {
        const_reference hold = ((tail.prev)->data);
        return hold;
    }
	
    iterator begin() noexcept {
        return iterator(head.next);
    }

    const_iterator begin() const noexcept {
        const_iterator hold = const_iterator(head.next);
        return hold;
    }

    const_iterator cbegin() const noexcept {
        const_iterator hold = const_iterator(head.next);
        return hold;
    }

    iterator end() noexcept {
        return iterator(&tail);
    }

    const_iterator end() const noexcept {
        const_iterator hold = const_iterator(&tail);
        return hold;
    }

    const_iterator cend() const noexcept {
        const_iterator hold = const_iterator(&tail);
        return hold;
    }

    bool empty() const noexcept {
        return (_size == 0);
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        while (head.next != &tail) {
           head.next = head.next->next;
           delete head.next->prev;
           head.next->prev = &head;
       }
       head.next = &tail;
       tail.prev = &head;
       _size = 0;
    }

    iterator insert( const_iterator pos, const T& value ) {
        Node* new_node = new Node(value);
        new_node->next = pos.node;
        new_node->prev = pos.node->prev;
        pos.node->prev->next = new_node;
        pos.node->prev = new_node;
        _size += 1;
        return iterator(new_node);
    }
    iterator insert( const_iterator pos, T&& value ) {
        Node* new_node = new Node(std::move(value));
        new_node->next = pos.node;
        new_node->prev = pos.node->prev;
        pos.node->prev->next = new_node;
        pos.node->prev = new_node;
        _size += 1;
        return iterator(new_node);
    }

    iterator erase( const_iterator pos ) {
        pos.node->prev->next = pos.node->next;
        pos.node->next->prev = pos.node->prev;
        iterator hold = iterator(pos.node->next);
        delete pos.node;
        _size -= 1;
        return hold;
    }

    void push_back( const T& value ) {
        Node* new_node = new Node(value);
        tail.prev->next = new_node;
        new_node->prev = tail.prev;
        tail.prev = new_node;
        new_node->next = &tail;
        _size += 1;
    }
    void push_back( T&& value ) {
        Node* new_node = new Node(std::move(value));
        tail.prev->next = new_node;
        new_node->prev = tail.prev;
        tail.prev = new_node;
        new_node->next = &tail;
        _size += 1;
    }

    void pop_back() {
        tail.prev = tail.prev->prev;
        delete tail.prev->next;
        tail.prev->next = &tail;
        _size -= 1;
    }
	
    void push_front( const T& value ) {
        Node* new_node = new Node(value);
        head.next->prev = new_node;
        new_node->next = head.next;
        head.next = new_node;
        new_node->prev = &head;
        _size += 1;
    }
	void push_front( T&& value ) {
        Node* new_node = new Node(std::move(value));
        head.next->prev = new_node;
        new_node->next = head.next;
        head.next = new_node;
        new_node->prev = &head;
        _size += 1;
    }

    void pop_front() {
        head.next = head.next->next;
        delete head.next->prev;
        head.next->prev = &head;
        _size -= 1;
    }
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};

namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}