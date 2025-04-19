#ifndef ESTL_ALGORITHM_HPP
#define ESTL_ALGORITHM_HPP

#include <cstddef>
#include "config.hpp"
#include "iterator.hpp"

namespace estl {

// Comparison operations
template<typename T>
struct less {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

template<typename T>
struct greater {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs > rhs;
    }
};

template<typename T>
struct equal_to {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs == rhs;
    }
};

// Non-modifying sequence operations
template<typename InputIt, typename UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (!p(*first)) {
            return false;
        }
    }
    return true;
}

template<typename InputIt, typename UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) {
            return true;
        }
    }
    return false;
}

template<typename InputIt, typename UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) {
            return false;
        }
    }
    return true;
}

template<typename InputIt, typename T>
InputIt find(InputIt first, InputIt last, const T& value) {
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}

template<typename InputIt, typename UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) {
            return first;
        }
    }
    return last;
}

template<typename InputIt, typename UnaryPredicate>
InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (!p(*first)) {
            return first;
        }
    }
    return last;
}

template<typename InputIt, typename T>
typename iterator_traits<InputIt>::difference_type
count(InputIt first, InputIt last, const T& value) {
    typename iterator_traits<InputIt>::difference_type result = 0;
    for (; first != last; ++first) {
        if (*first == value) {
            ++result;
        }
    }
    return result;
}

template<typename InputIt, typename UnaryPredicate>
typename iterator_traits<InputIt>::difference_type
count_if(InputIt first, InputIt last, UnaryPredicate p) {
    typename iterator_traits<InputIt>::difference_type result = 0;
    for (; first != last; ++first) {
        if (p(*first)) {
            ++result;
        }
    }
    return result;
}

// Modifying sequence operations
template<typename ForwardIt, typename T>
void fill(ForwardIt first, ForwardIt last, const T& value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

template<typename OutputIt, typename Size, typename T>
OutputIt fill_n(OutputIt first, Size count, const T& value) {
    for (Size i = 0; i < count; ++i) {
        *first = value;
        ++first;
    }
    return first;
}

template<typename InputIt, typename OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    for (; first != last; ++first, ++d_first) {
        *d_first = *first;
    }
    return d_first;
}

template<typename InputIt, typename OutputIt, typename UnaryPredicate>
OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred) {
    for (; first != last; ++first) {
        if (pred(*first)) {
            *d_first = *first;
            ++d_first;
        }
    }
    return d_first;
}

template<typename InputIt, typename OutputIt>
OutputIt move(InputIt first, InputIt last, OutputIt d_first) {
    for (; first != last; ++first, ++d_first) {
        *d_first = std::move(*first);
    }
    return d_first;
}

template<typename ForwardIt1, typename ForwardIt2>
ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        using std::swap;
        swap(*first1, *first2);
    }
    return first2;
}

template<typename ForwardIt, typename T>
void replace(ForwardIt first, ForwardIt last, const T& old_value, const T& new_value) {
    for (; first != last; ++first) {
        if (*first == old_value) {
            *first = new_value;
        }
    }
}

template<typename ForwardIt, typename UnaryPredicate, typename T>
void replace_if(ForwardIt first, ForwardIt last, UnaryPredicate p, const T& new_value) {
    for (; first != last; ++first) {
        if (p(*first)) {
            *first = new_value;
        }
    }
}

// Sorting and related operations
template<typename RandomIt>
void sort(RandomIt first, RandomIt last) {
    sort(first, last, less<typename iterator_traits<RandomIt>::value_type>());
}

template<typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare comp) {
    if (first == last || first + 1 == last) {
        return;
    }
    
    // Simple insertion sort for small arrays
    for (RandomIt it = first + 1; it != last; ++it) {
        auto key = std::move(*it);
        RandomIt j = it;
        
        while (j > first && comp(key, *(j - 1))) {
            *j = std::move(*(j - 1));
            --j;
        }
        
        *j = std::move(key);
    }
}

template<typename ForwardIt, typename T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value) {
    return lower_bound(first, last, value, less<T>());
}

template<typename ForwardIt, typename T, typename Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    typename iterator_traits<ForwardIt>::difference_type count = distance(first, last);
    
    while (count > 0) {
        typename iterator_traits<ForwardIt>::difference_type step = count / 2;
        ForwardIt it = first;
        advance(it, step);
        
        if (comp(*it, value)) {
            first = ++it;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    
    return first;
}

template<typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value) {
    return upper_bound(first, last, value, less<T>());
}

template<typename ForwardIt, typename T, typename Compare>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    typename iterator_traits<ForwardIt>::difference_type count = distance(first, last);
    
    while (count > 0) {
        typename iterator_traits<ForwardIt>::difference_type step = count / 2;
        ForwardIt it = first;
        advance(it, step);
        
        if (!comp(value, *it)) {
            first = ++it;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    
    return first;
}

template<typename ForwardIt, typename T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value) {
    first = lower_bound(first, last, value);
    return (first != last && !(value < *first));
}

template<typename ForwardIt, typename T, typename Compare>
bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    first = lower_bound(first, last, value, comp);
    return (first != last && !comp(value, *first));
}

// Min/max operations
template<typename T>
const T& min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

template<typename T, typename Compare>
const T& min(const T& a, const T& b, Compare comp) {
    return comp(b, a) ? b : a;
}

template<typename T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template<typename T, typename Compare>
const T& max(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? b : a;
}

template<typename ForwardIt>
ForwardIt min_element(ForwardIt first, ForwardIt last) {
    if (first == last) {
        return last;
    }
    
    ForwardIt smallest = first;
    ++first;
    
    for (; first != last; ++first) {
        if (*first < *smallest) {
            smallest = first;
        }
    }
    
    return smallest;
}

template<typename ForwardIt, typename Compare>
ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp) {
    if (first == last) {
        return last;
    }
    
    ForwardIt smallest = first;
    ++first;
    
    for (; first != last; ++first) {
        if (comp(*first, *smallest)) {
            smallest = first;
        }
    }
    
    return smallest;
}

template<typename ForwardIt>
ForwardIt max_element(ForwardIt first, ForwardIt last) {
    if (first == last) {
        return last;
    }
    
    ForwardIt largest = first;
    ++first;
    
    for (; first != last; ++first) {
        if (*largest < *first) {
            largest = first;
        }
    }
    
    return largest;
}

template<typename ForwardIt, typename Compare>
ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp) {
    if (first == last) {
        return last;
    }
    
    ForwardIt largest = first;
    ++first;
    
    for (; first != last; ++first) {
        if (comp(*largest, *first)) {
            largest = first;
        }
    }
    
    return largest;
}

// Lexicographical comparison
template<typename InputIt1, typename InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                            InputIt2 first2, InputIt2 last2) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) {
            return true;
        }
        if (*first2 < *first1) {
            return false;
        }
    }
    
    return (first1 == last1) && (first2 != last2);
}

template<typename InputIt1, typename InputIt2, typename Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                            InputIt2 first2, InputIt2 last2,
                            Compare comp) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2)) {
            return true;
        }
        if (comp(*first2, *first1)) {
            return false;
        }
    }
    
    return (first1 == last1) && (first2 != last2);
}

} // namespace estl

#endif // ESTL_ALGORITHM_HPP
