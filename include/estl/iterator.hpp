#ifndef ESTL_ITERATOR_HPP
#define ESTL_ITERATOR_HPP

#include <cstddef>
#include "config.hpp"

namespace estl {

// Iterator tags
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// Iterator traits
template<typename Iterator>
struct iterator_traits {
    using difference_type = typename Iterator::difference_type;
    using value_type = typename Iterator::value_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};

// Specialization for pointers
template<typename T>
struct iterator_traits<T*> {
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = random_access_iterator_tag;
};

// Specialization for const pointers
template<typename T>
struct iterator_traits<const T*> {
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = random_access_iterator_tag;
};

// Reverse iterator implementation
template<typename Iterator>
class reverse_iterator {
public:
    using iterator_type = Iterator;
    using iterator_category = typename iterator_traits<Iterator>::iterator_category;
    using value_type = typename iterator_traits<Iterator>::value_type;
    using difference_type = typename iterator_traits<Iterator>::difference_type;
    using pointer = typename iterator_traits<Iterator>::pointer;
    using reference = typename iterator_traits<Iterator>::reference;

    // Constructors
    reverse_iterator() : current() {}
    explicit reverse_iterator(iterator_type it) : current(it) {}
    
    template<typename Iter>
    reverse_iterator(const reverse_iterator<Iter>& other) : current(other.base()) {}

    // Access the underlying iterator
    iterator_type base() const {
        return current;
    }

    // Dereference
    reference operator*() const {
        iterator_type temp = current;
        return *--temp;
    }

    pointer operator->() const {
        return &(operator*());
    }

    // Subscript operator
    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    // Increment/decrement
    reverse_iterator& operator++() {
        --current;
        return *this;
    }

    reverse_iterator operator++(int) {
        reverse_iterator temp = *this;
        --current;
        return temp;
    }

    reverse_iterator& operator--() {
        ++current;
        return *this;
    }

    reverse_iterator operator--(int) {
        reverse_iterator temp = *this;
        ++current;
        return temp;
    }

    // Arithmetic operations
    reverse_iterator operator+(difference_type n) const {
        return reverse_iterator(current - n);
    }

    reverse_iterator& operator+=(difference_type n) {
        current -= n;
        return *this;
    }

    reverse_iterator operator-(difference_type n) const {
        return reverse_iterator(current + n);
    }

    reverse_iterator& operator-=(difference_type n) {
        current += n;
        return *this;
    }

private:
    iterator_type current;
};

// Non-member functions for reverse_iterator
template<typename Iterator1, typename Iterator2>
bool operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
    return lhs.base() == rhs.base();
}

template<typename Iterator1, typename Iterator2>
bool operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
    return lhs.base() != rhs.base();
}

template<typename Iterator1, typename Iterator2>
bool operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
    return lhs.base() > rhs.base();
}

template<typename Iterator1, typename Iterator2>
bool operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
    return lhs.base() >= rhs.base();
}

template<typename Iterator1, typename Iterator2>
bool operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
    return lhs.base() < rhs.base();
}

template<typename Iterator1, typename Iterator2>
bool operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
    return lhs.base() <= rhs.base();
}

template<typename Iterator>
reverse_iterator<Iterator> operator+(
    typename reverse_iterator<Iterator>::difference_type n,
    const reverse_iterator<Iterator>& it) {
    return reverse_iterator<Iterator>(it.base() - n);
}

template<typename Iterator1, typename Iterator2>
auto operator-(
    const reverse_iterator<Iterator1>& lhs,
    const reverse_iterator<Iterator2>& rhs) -> decltype(rhs.base() - lhs.base()) {
    return rhs.base() - lhs.base();
}

// Distance calculation
template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    using category = typename iterator_traits<InputIterator>::iterator_category;
    return distance_impl(first, last, category());
}

template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_impl(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type result = 0;
    while (first != last) {
        ++first;
        ++result;
    }
    return result;
}

template<typename RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type
distance_impl(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    return last - first;
}

// Advance implementation
template<typename InputIterator, typename Distance>
void advance(InputIterator& it, Distance n) {
    using category = typename iterator_traits<InputIterator>::iterator_category;
    advance_impl(it, n, category());
}

template<typename InputIterator, typename Distance>
void advance_impl(InputIterator& it, Distance n, input_iterator_tag) {
    while (n > 0) {
        ++it;
        --n;
    }
}

template<typename BidirectionalIterator, typename Distance>
void advance_impl(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n > 0) {
            ++it;
            --n;
        }
    } else {
        while (n < 0) {
            --it;
            ++n;
        }
    }
}

template<typename RandomAccessIterator, typename Distance>
void advance_impl(RandomAccessIterator& it, Distance n, random_access_iterator_tag) {
    it += n;
}

} // namespace estl

#endif // ESTL_ITERATOR_HPP
