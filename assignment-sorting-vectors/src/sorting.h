#pragma once

#include <functional> // std::less
#include <iterator>

// This is C++ magic which will allows our function
// to default to using a < b if the comparator arg
// is unspecified. It uses defines std::less<T>
// for the iterator's value_type.
//
// For example: if you have a vector<float>, the 
// iterator's value type will be float. std::less 
// will select the < for sorting floats as the 
// default comparator.

namespace sort {
	template<typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template<typename T>
	void swap(T & a, T & b) noexcept {
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		
		difference_type n = end - begin;
		for (auto i = n-1; i >= 0; i--) {
			for (auto j = i; j < n - 1; j++) {
				if (!comp(*(begin + j),*(begin + j + 1))) {
					swap(*(begin + j), *(begin + j + 1));
				}
			}	
		}
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) { 
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		
		difference_type n = end - begin;
		for (auto i = 1; i < n; i++) {
			auto j = i - 1;
			auto hold = std::move(*(begin + i));
			while (j >= 0 && !comp(*(begin + j), hold)) {
				*(begin + j + 1) = std::move(*(begin + j));
				j--;
			}
			*(begin + j + 1) = std::move(hold);
		}
	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;

		difference_type n = end - begin;
		for (auto i = 0; i < n; i++) {
			auto lowest = i;
			for (auto j = i + 1; j < n; j++) {
				if (!comp(*(begin + lowest), *(begin + j))) {
					lowest = j;
				}
			}
			swap(*(begin + lowest), *(begin + i));
		}
	}
}
