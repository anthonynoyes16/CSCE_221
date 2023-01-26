#pragma once

#include <functional>
#include <utility>
#include <vector>

template <class T, class Container = std::vector<T>, class Compare = std::less<T>>
class PriorityQueue {
public:
    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;

private:
    Container c;
    value_compare comp;

    size_type parent(size_type index) { return (index - 1) / 2; }
    size_type left_child(size_type index) { return 2 * index + 1; }
    size_type right_child(size_type index) { return 2 * (index + 1); }
    bool is_internal(size_t index) { return left_child(index) < c.size(); }
    bool is_leaf(size_t index) { return left_child(index) >= c.size(); }

    
    /**
     * @brief Recursively (or Iteratively) swap the value at index up the heap until it is in the correct position.
     * 
     * @note This is a max heap, so assume Compare is less, but promote the larger value.
     * 
     * Used by push
     * 
     * O(log(size()))
     * 
     * @param index the current position to swap upwards
     */
    void upheap(size_type index) {
        while (index > 0 && comp(c.at(parent(index)), c.at(index))) {
            std::swap(c[index], c[parent(index)]);
            index = parent(index);
        }
    }
    
    /**
     * @brief Recursively swap the value at index down the heap until it is in the correct position.
     * 
     * @note This is a max heap, so assume Compare is less, so promote the larger value.
     * 
     * Used by pop
     * 
     * O(log(size()))
     * 
     * @param index the current position to swap downwards
     */
    void downheap(size_type index) {
        int max_child;
        if (c.size() <= 1) {
            return;
        }
        while (!is_leaf(index)) {
            if (c.size() <= 1) {
                return;
            }
            max_child = left_child(index);
            if (right_child(index) < c.size() && comp(c[left_child(index)], c[right_child(index)])) {
                max_child = right_child(index);
            }
            if (comp(c[index], c[max_child])) {
                std::swap(c.at(index), c.at(max_child));
                index = max_child;
            }
            else {
                break;
            }
        }
    }

public:
    PriorityQueue() = default;
    PriorityQueue( const PriorityQueue& other ) = default;
    PriorityQueue( PriorityQueue&& other ) = default;
    ~PriorityQueue() = default;
    PriorityQueue& operator=( const PriorityQueue& other ) = default;
    PriorityQueue& operator=( PriorityQueue&& other ) = default;

    const_reference top() const { return c.at(0); }

    bool empty() const {
        if (c.size() == 0) {
            return true;
        }
        return false;
    }

    size_type size() const { return c.size(); }
	
    /**
     * @brief Inserts element and sorts the underlying container, c.
     * 
     * Uses upheap
     * 
     * O(log(size())) 
     * 
     * @param value inserted by copying into c 
     */
    void push( const value_type& value ) {
        c.push_back(value);
        int index = c.size() - 1;
        upheap(index);
    }

    /**
     * @brief Inserts element and sorts the underlying container, c.
     * 
     * Uses upheap
     * 
     * O(log(size())) 
     * 
     * @param value inserted by moving into c 
     */
	void push( value_type&& value ) {
        c.push_back(std::move(value));
        int index = c.size() - 1;
        upheap(std::move(index));
    }

    /**
     * @brief Removes the top element.
     * 
     * Uses downheap
     * 
     * O(log(size())) 
     * 
     */
    void pop() {
        if (c.size() < 1) {
            return;
        }
        if (c.size() == 1) {
            c.pop_back();
            return;
        }
        std::swap(c.front(), c.back());
        c.pop_back();
        if (c.size() > 1) {
            downheap(0);
        }
    }
};