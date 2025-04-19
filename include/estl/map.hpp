#ifndef ESTL_MAP_HPP
#define ESTL_MAP_HPP

#include <cstddef>
#include <utility>
#include "config.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"

namespace estl {

/**
 * @brief A sorted associative container for embedded systems
 * 
 * This map implementation is designed for embedded systems with limited resources.
 * It provides most of the functionality of std::map but with configurable static storage
 * and no dynamic memory allocation.
 * 
 * @tparam Key The type of keys
 * @tparam T The type of mapped values
 * @tparam Compare The comparison function object type
 * @tparam Capacity The maximum number of elements (static allocation)
 */
template <
    typename Key,
    typename T,
    typename Compare = less<Key>,
    size_t Capacity = 16
>
class map {
public:
    // Type definitions
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using key_compare = Compare;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // Custom iterator implementation for map
    class iterator {
    public:
        using iterator_category = bidirectional_iterator_tag;
        using value_type = std::pair<const Key, T>;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        iterator() : m_ptr(nullptr), m_index(0) {}
        iterator(map* map_ptr, size_type index) : m_ptr(map_ptr), m_index(index) {}

        reference operator*() const {
            return m_ptr->m_data[m_index];
        }

        pointer operator->() const {
            return &(m_ptr->m_data[m_index]);
        }

        iterator& operator++() {
            ++m_index;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator& operator--() {
            --m_index;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return m_ptr == other.m_ptr && m_index == other.m_index;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

    private:
        map* m_ptr;
        size_type m_index;
    };

    class const_iterator {
    public:
        using iterator_category = bidirectional_iterator_tag;
        using value_type = const std::pair<const Key, T>;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_iterator() : m_ptr(nullptr), m_index(0) {}
        const_iterator(const map* map_ptr, size_type index) : m_ptr(map_ptr), m_index(index) {}
        const_iterator(const iterator& it) : m_ptr(it.m_ptr), m_index(it.m_index) {}

        reference operator*() const {
            return m_ptr->m_data[m_index];
        }

        pointer operator->() const {
            return &(m_ptr->m_data[m_index]);
        }

        const_iterator& operator++() {
            ++m_index;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const_iterator& operator--() {
            --m_index;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const const_iterator& other) const {
            return m_ptr == other.m_ptr && m_index == other.m_index;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

    private:
        const map* m_ptr;
        size_type m_index;
    };

    using reverse_iterator = estl::reverse_iterator<iterator>;
    using const_reverse_iterator = estl::reverse_iterator<const_iterator>;

    // Value compare helper class
    class value_compare {
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    
    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return comp(lhs.first, rhs.first);
        }
    };

    // Constructors
    map() : m_size(0) {}

    map(const map& other) : m_size(0) {
        for (size_type i = 0; i < other.m_size; ++i) {
            insert(other.m_data[i]);
        }
    }

    // Assignment operator
    map& operator=(const map& other) {
        if (this != &other) {
            clear();
            for (size_type i = 0; i < other.m_size; ++i) {
                insert(other.m_data[i]);
            }
        }
        return *this;
    }

    // Element access
    T& at(const Key& key) {
        iterator it = find(key);
        if (it == end()) {
            // In a real implementation, we would throw an exception
            // but for embedded systems, we might want to handle this differently
            ESTL_ASSERT(it != end());
        }
        return it->second;
    }

    const T& at(const Key& key) const {
        const_iterator it = find(key);
        if (it == end()) {
            ESTL_ASSERT(it != end());
        }
        return it->second;
    }

    T& operator[](const Key& key) {
        iterator it = find(key);
        if (it != end()) {
            return it->second;
        }
        
        // Insert new element with default value
        return insert(std::make_pair(key, T())).first->second;
    }

    // Iterators
    iterator begin() {
        return iterator(this, 0);
    }

    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    const_iterator cbegin() const {
        return const_iterator(this, 0);
    }

    iterator end() {
        return iterator(this, m_size);
    }

    const_iterator end() const {
        return const_iterator(this, m_size);
    }

    const_iterator cend() const {
        return const_iterator(this, m_size);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }

    // Capacity
    bool empty() const {
        return m_size == 0;
    }

    size_type size() const {
        return m_size;
    }

    size_type max_size() const {
        return Capacity;
    }

    // Modifiers
    void clear() {
        for (size_type i = 0; i < m_size; ++i) {
            m_data[i].~value_type();
        }
        m_size = 0;
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        // Check if key already exists
        iterator it = find(value.first);
        if (it != end()) {
            return std::make_pair(it, false);
        }
        
        // Check if we have capacity
        if (m_size >= Capacity) {
            // Handle capacity exceeded
            ESTL_ASSERT(m_size < Capacity);
            return std::make_pair(end(), false);
        }
        
        // Find position to insert (keep sorted)
        size_type pos = m_size;
        for (size_type i = 0; i < m_size; ++i) {
            if (key_comp()(value.first, m_data[i].first)) {
                pos = i;
                break;
            }
        }
        
        // Shift elements to make space
        for (size_type i = m_size; i > pos; --i) {
            new (&m_data[i]) value_type(std::move(m_data[i-1]));
            m_data[i-1].~value_type();
        }
        
        // Insert new element
        new (&m_data[pos]) value_type(value);
        ++m_size;
        
        return std::make_pair(iterator(this, pos), true);
    }

    iterator erase(const_iterator pos) {
        size_type index = pos.m_index;
        
        // Destroy element
        m_data[index].~value_type();
        
        // Shift elements
        for (size_type i = index; i < m_size - 1; ++i) {
            new (&m_data[i]) value_type(std::move(m_data[i+1]));
            m_data[i+1].~value_type();
        }
        
        --m_size;
        return iterator(this, index);
    }

    size_type erase(const Key& key) {
        iterator it = find(key);
        if (it == end()) {
            return 0;
        }
        
        erase(it);
        return 1;
    }

    void swap(map& other) {
        // Since we can't swap the arrays directly (fixed size),
        // we need to swap elements individually
        size_type min_size = (m_size < other.m_size) ? m_size : other.m_size;
        
        // Swap common elements
        for (size_type i = 0; i < min_size; ++i) {
            value_type temp = std::move(m_data[i]);
            m_data[i] = std::move(other.m_data[i]);
            other.m_data[i] = std::move(temp);
        }
        
        // Handle case where this map is larger
        if (m_size > min_size) {
            for (size_type i = min_size; i < m_size; ++i) {
                new (&other.m_data[i]) value_type(std::move(m_data[i]));
                m_data[i].~value_type();
            }
        }
        // Handle case where other map is larger
        else if (other.m_size > min_size) {
            for (size_type i = min_size; i < other.m_size; ++i) {
                new (&m_data[i]) value_type(std::move(other.m_data[i]));
                other.m_data[i].~value_type();
            }
        }
        
        // Swap sizes
        size_type temp_size = m_size;
        m_size = other.m_size;
        other.m_size = temp_size;
    }

    // Lookup
    size_type count(const Key& key) const {
        return (find(key) != end()) ? 1 : 0;
    }

    iterator find(const Key& key) {
        for (size_type i = 0; i < m_size; ++i) {
            if (!key_comp()(m_data[i].first, key) && !key_comp()(key, m_data[i].first)) {
                return iterator(this, i);
            }
        }
        return end();
    }

    const_iterator find(const Key& key) const {
        for (size_type i = 0; i < m_size; ++i) {
            if (!key_comp()(m_data[i].first, key) && !key_comp()(key, m_data[i].first)) {
                return const_iterator(this, i);
            }
        }
        return end();
    }

    std::pair<iterator, iterator> equal_range(const Key& key) {
        iterator lower = lower_bound(key);
        iterator upper = upper_bound(key);
        return std::make_pair(lower, upper);
    }

    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
        const_iterator lower = lower_bound(key);
        const_iterator upper = upper_bound(key);
        return std::make_pair(lower, upper);
    }

    iterator lower_bound(const Key& key) {
        for (size_type i = 0; i < m_size; ++i) {
            if (!key_comp()(m_data[i].first, key)) {
                return iterator(this, i);
            }
        }
        return end();
    }

    const_iterator lower_bound(const Key& key) const {
        for (size_type i = 0; i < m_size; ++i) {
            if (!key_comp()(m_data[i].first, key)) {
                return const_iterator(this, i);
            }
        }
        return end();
    }

    iterator upper_bound(const Key& key) {
        for (size_type i = 0; i < m_size; ++i) {
            if (key_comp()(key, m_data[i].first)) {
                return iterator(this, i);
            }
        }
        return end();
    }

    const_iterator upper_bound(const Key& key) const {
        for (size_type i = 0; i < m_size; ++i) {
            if (key_comp()(key, m_data[i].first)) {
                return const_iterator(this, i);
            }
        }
        return end();
    }

    // Observers
    key_compare key_comp() const {
        return Compare();
    }

    value_compare value_comp() const {
        return value_compare(Compare());
    }

private:
    // Storage for elements - using aligned storage to allow for proper construction/destruction
    alignas(value_type) unsigned char m_storage[sizeof(value_type) * Capacity];
    value_type* m_data = reinterpret_cast<value_type*>(m_storage);
    size_type m_size;

    // Make iterators friends to access private members
    friend class iterator;
    friend class const_iterator;
};

// Non-member functions
template <typename Key, typename T, typename Compare, size_t Capacity>
bool operator==(const map<Key, T, Compare, Capacity>& lhs, const map<Key, T, Compare, Capacity>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    
    auto lit = lhs.begin();
    auto rit = rhs.begin();
    
    while (lit != lhs.end()) {
        if (*lit != *rit) {
            return false;
        }
        ++lit;
        ++rit;
    }
    
    return true;
}

template <typename Key, typename T, typename Compare, size_t Capacity>
bool operator!=(const map<Key, T, Compare, Capacity>& lhs, const map<Key, T, Compare, Capacity>& rhs) {
    return !(lhs == rhs);
}

template <typename Key, typename T, typename Compare, size_t Capacity>
bool operator<(const map<Key, T, Compare, Capacity>& lhs, const map<Key, T, Compare, Capacity>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Key, typename T, typename Compare, size_t Capacity>
bool operator<=(const map<Key, T, Compare, Capacity>& lhs, const map<Key, T, Compare, Capacity>& rhs) {
    return !(rhs < lhs);
}

template <typename Key, typename T, typename Compare, size_t Capacity>
bool operator>(const map<Key, T, Compare, Capacity>& lhs, const map<Key, T, Compare, Capacity>& rhs) {
    return rhs < lhs;
}

template <typename Key, typename T, typename Compare, size_t Capacity>
bool operator>=(const map<Key, T, Compare, Capacity>& lhs, const map<Key, T, Compare, Capacity>& rhs) {
    return !(lhs < rhs);
}

template <typename Key, typename T, typename Compare, size_t Capacity>
void swap(map<Key, T, Compare, Capacity>& lhs, map<Key, T, Compare, Capacity>& rhs) {
    lhs.swap(rhs);
}

} // namespace estl

#endif // ESTL_MAP_HPP
