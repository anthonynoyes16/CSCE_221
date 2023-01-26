#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::range_error

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() {
        if (_capacity == 0) {
            _capacity += 1;
        }
        else {
            _capacity *= 2;
        }
        T* hold = new T[_capacity];
        for (int i = 0; i < _size; i++) {
            hold[i] = std::move(array[i]);
        }
        delete[] array;
        array = hold;
    }

public:
    Vector() noexcept {
        _capacity = 0;
        _size = 0;
        array = nullptr;
    }

    Vector(size_t count, const T& value) {
        _capacity = count;
        _size = count;
        array = new T[count];
        for (int i = 0; i < count; i++) {
            array[i] = value;
        }
    }

    explicit Vector(size_t count) {
        _capacity = count;
        _size = count;
        array = new T[count]();
    }

    Vector(const Vector& other) {
        _size = other._size;
        _capacity = other._capacity;
        array = new T[_capacity];
        for (int i = 0; i < _size; i++) {
            array[i] = other.array[i];
        }
    }

    Vector(Vector&& other) noexcept : array(other.array), _capacity(other._capacity), _size(other._size){
        other._size = 0;
        other._capacity = 0;
        other.array = nullptr;
    }

    ~Vector() {
        delete[] array;
    }

    // copy and move operators need to deallocate memory before copying over it
    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        _size = other._size;
        _capacity = other._capacity;
        delete[] array;
        array = new T[_capacity];
        for (int i = 0; i < _size; i++) {
            array[i] = other.array[i];
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] array;
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
        std::swap(array, other.array);
        other._size = 0;
        other._capacity = 0;
        other.array = nullptr;
        return *this;
    }

    iterator begin() noexcept {
        return &array[0];
    }
    iterator end() noexcept {
        return &array[_size];
    }

    [[nodiscard]] bool empty() const noexcept {
        if (_size == 0) {
            return true;
        }
        return false;
    }

    size_t size() const noexcept {
        return _size;
    }
    
    size_t capacity() const noexcept {
        return _capacity;
    }

    T& at(size_t pos) {
        if (pos >= _size) {
            throw std::out_of_range("Out of bounds access");
        }
        return array[pos];
    }
    const T& at(size_t pos) const {
        if (pos >= _size) {
            throw std::out_of_range("Out of bounds access");
        }
        return array[pos];
    }
    T& operator[](size_t pos) { // will be stored in a regular variable
        return array[pos];
    }
    
    const T& operator[](size_t pos) const { // will be stored in a constant variable
        return array[pos];
    }

    T& front() {
        return array[0];
    }
    const T& front() const {
        return array[0];
    }
    T& back() {
        return array[_size - 1];
    }
    const T& back() const {
        return array[_size - 1];
    }

    void push_back(const T& value) {
        if (_size == _capacity) {
            grow();
        }
        array[_size] = value;
        _size += 1; 
    }

    void push_back(T&& value) {
        if (_size == _capacity) {
            grow();
        }
        array[_size] = std::move(value);
        _size += 1;
    }

    void pop_back() {
        _size -= 1;
    }

    iterator insert(iterator pos, const T& value) {
        typename iterator::difference_type loc = pos - begin();
        if (loc >= _size) {
            throw std::out_of_range("");
        }
        if (_size == _capacity) {
            grow();
        }
        _size += 1;
        for (auto i = _size-1; i > loc; i--) {
            array[i] = std::move(array[i-1]);
        }
        array[loc] = value;
        pos = begin();
        pos += loc;
        return pos;
    }
    iterator insert(iterator pos, T&& value) {
        typename iterator::difference_type loc = pos - begin();
        if (loc >= _size) {
            throw std::out_of_range("");
        }
        if (_size == _capacity) {
            grow();
        }
        _size += 1;
        for (auto i = _size-1; i > loc; i--) {
            array[i] = std::move(array[i-1]);
        }
        array[loc] = std::move(value);
        pos = begin();
        pos += loc;
        return pos;
    }
    iterator insert(iterator pos, size_t count, const T& value) {
        for (auto i = 0; i < count; i++) {
            pos = insert(pos, value);
        }
        return pos;
    }
    iterator erase(iterator pos) {
        typename iterator::difference_type loc = pos - begin();
        if (loc >= _size) {
            throw std::out_of_range("");
        }
        for (auto i = loc; i < _size-1; i++) {
            array[i] = std::move(array[i+1]);
        }
        pop_back();
        return pos;
    }
    iterator erase(iterator first, iterator last) {
        typename iterator::difference_type len = last - first;
        typename iterator::difference_type start = first - begin();
        typename iterator::difference_type end = last - begin();
        for (auto i = 0; i < _size; i++) {
            if (end+i > _size-1) {
                break;
            }
            array[start+i] = std::move(array[end+i]);
        }
        _size -= len;
        iterator pos = begin();
        pos += start;
        return pos;
    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        pointer i_ptr;
        // Add your own data members here
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
    public:
        iterator() : i_ptr(nullptr) {

        }
        iterator(pointer ptr) : i_ptr(ptr) {
        
        }
        // Add any constructors that you may need

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept {
            return *i_ptr;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            return i_ptr;
        }

        // Prefix Increment: ++a
        iterator& operator++() noexcept {
            i_ptr++;
            return *this;
        }
        // Postfix Increment: a++
        iterator operator++(int) noexcept {
            iterator hold = *this;
            ++(*this);
            return hold;
        }
        // Prefix Decrement: --a
        iterator& operator--() noexcept {
            i_ptr--;
            return *this;
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept {
            iterator hold = *this;
            --(*this);
            return hold;
        }

        iterator& operator+=(difference_type offset) noexcept {
            i_ptr += offset;
            return *this;
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept {
            pointer q_ptr = i_ptr + offset;
            return q_ptr;
        }

        iterator& operator-=(difference_type offset) noexcept {
            i_ptr -= offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            pointer q_ptr = i_ptr - offset;
            return q_ptr;
        }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept {
            return i_ptr - rhs.i_ptr;
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept {
            return *(i_ptr + offset);
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept {
            if (i_ptr == rhs.i_ptr) {
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept {
            if (i_ptr == rhs.i_ptr) {
                return false;
            }
            return true;
        }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept {
           if (i_ptr < rhs.i_ptr) {
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept {
            if (i_ptr > rhs.i_ptr) {
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept {
            if (i_ptr <= rhs.i_ptr) {
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept {
            if (i_ptr >= rhs.i_ptr) {
                return true;
            }
            return false;
        }
    };


    void clear() noexcept {
        _size = 0;
    }
};

template <class T>
[[nodiscard]] typename Vector<T>::iterator operator+(typename Vector<T>::iterator::difference_type offset, typename Vector<T>::iterator iterator) noexcept {
    return iterator + offset;
}

#endif