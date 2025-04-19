#ifndef ESTL_VECTOR_HPP
#define ESTL_VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include "config.hpp"
#include "iterator.hpp"

namespace estl {

/**
 * @brief A dynamically sized array implementation for embedded systems
 * 
 * This vector implementation is designed for embedded systems with limited resources.
 * It provides most of the functionality of std::vector but with configurable storage
 * and no dynamic memory allocation.
 * 
 * @tparam T The type of elements
 * @tparam Capacity The maximum number of elements (static allocation)
 */
template <typename T, size_t Capacity>
class vector {
public:
    // Type definitions
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = estl::reverse_iterator<iterator>;
    using const_reverse_iterator = estl::reverse_iterator<const_iterator>;

    // Constructors
    vector() : m_size(0) {}

    vector(size_type count, const T& value) : m_size(0) {
        assign(count, value);
    }

    vector(std::initializer_list<T> init) : m_size(0) {
        assign(init);
    }

    // Copy constructor
    vector(const vector& other) : m_size(0) {
        assign(other.begin(), other.end());
    }

    // Assignment operators
    vector& operator=(const vector& other) {
        if (this != &other) {
            assign(other.begin(), other.end());
        }
        return *this;
    }

    vector& operator=(std::initializer_list<T> ilist) {
        assign(ilist);
        return *this;
    }

    // Element access
    reference at(size_type pos) {
        if (pos >= m_size) {
            // In a real implementation, we would throw an exception
            // but for embedded systems, we might want to handle this differently
            ESTL_ASSERT(pos < m_size);
        }
        return m_data[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= m_size) {
            ESTL_ASSERT(pos < m_size);
        }
        return m_data[pos];
    }

    reference operator[](size_type pos) {
        return m_data[pos];
    }

    const_reference operator[](size_type pos) const {
        return m_data[pos];
    }

    reference front() {
        return m_data[0];
    }

    const_reference front() const {
        return m_data[0];
    }

    reference back() {
        return m_data[m_size - 1];
    }

    const_reference back() const {
        return m_data[m_size - 1];
    }

    T* data() {
        return m_data;
    }

    const T* data() const {
        return m_data;
    }

    // Iterators
    iterator begin() {
        return m_data;
    }

    const_iterator begin() const {
        return m_data;
    }

    const_iterator cbegin() const {
        return m_data;
    }

    iterator end() {
        return m_data + m_size;
    }

    const_iterator end() const {
        return m_data + m_size;
    }

    const_iterator cend() const {
        return m_data + m_size;
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

    size_type capacity() const {
        return Capacity;
    }

    // Modifiers
    void clear() {
        for (size_type i = 0; i < m_size; ++i) {
            m_data[i].~T();
        }
        m_size = 0;
    }

    iterator insert(const_iterator pos, const T& value) {
        size_type index = pos - begin();
        if (m_size < Capacity) {
            if (index < m_size) {
                // Move elements to make space
                for (size_type i = m_size; i > index; --i) {
                    new (&m_data[i]) T(m_data[i - 1]);
                    m_data[i - 1].~T();
                }
            }
            // Insert new element
            new (&m_data[index]) T(value);
            ++m_size;
        } else {
            // Handle capacity exceeded - in embedded systems we might want to assert here
            ESTL_ASSERT(m_size < Capacity);
        }
        return begin() + index;
    }

    iterator insert(const_iterator pos, T&& value) {
        size_type index = pos - begin();
        if (m_size < Capacity) {
            if (index < m_size) {
                // Move elements to make space
                for (size_type i = m_size; i > index; --i) {
                    new (&m_data[i]) T(std::move(m_data[i - 1]));
                    m_data[i - 1].~T();
                }
            }
            // Insert new element
            new (&m_data[index]) T(std::move(value));
            ++m_size;
        } else {
            // Handle capacity exceeded
            ESTL_ASSERT(m_size < Capacity);
        }
        return begin() + index;
    }

    iterator erase(const_iterator pos) {
        size_type index = pos - begin();
        if (index < m_size) {
            // Destroy the element at position
            m_data[index].~T();
            
            // Move subsequent elements
            for (size_type i = index; i < m_size - 1; ++i) {
                new (&m_data[i]) T(std::move(m_data[i + 1]));
                m_data[i + 1].~T();
            }
            
            --m_size;
        }
        return begin() + index;
    }

    void push_back(const T& value) {
        if (m_size < Capacity) {
            new (&m_data[m_size]) T(value);
            ++m_size;
        } else {
            // Handle capacity exceeded
            ESTL_ASSERT(m_size < Capacity);
        }
    }

    void push_back(T&& value) {
        if (m_size < Capacity) {
            new (&m_data[m_size]) T(std::move(value));
            ++m_size;
        } else {
            // Handle capacity exceeded
            ESTL_ASSERT(m_size < Capacity);
        }
    }

    void pop_back() {
        if (m_size > 0) {
            --m_size;
            m_data[m_size].~T();
        }
    }

    void resize(size_type count) {
        if (count > Capacity) {
            count = Capacity;  // Limit to capacity
        }
        
        if (count > m_size) {
            // Construct new elements
            for (size_type i = m_size; i < count; ++i) {
                new (&m_data[i]) T();
            }
        } else if (count < m_size) {
            // Destroy excess elements
            for (size_type i = count; i < m_size; ++i) {
                m_data[i].~T();
            }
        }
        
        m_size = count;
    }

    void resize(size_type count, const value_type& value) {
        if (count > Capacity) {
            count = Capacity;  // Limit to capacity
        }
        
        if (count > m_size) {
            // Construct new elements with value
            for (size_type i = m_size; i < count; ++i) {
                new (&m_data[i]) T(value);
            }
        } else if (count < m_size) {
            // Destroy excess elements
            for (size_type i = count; i < m_size; ++i) {
                m_data[i].~T();
            }
        }
        
        m_size = count;
    }

    template <class InputIt>
    void assign(InputIt first, InputIt last) {
        clear();
        while (first != last && m_size < Capacity) {
            push_back(*first);
            ++first;
        }
    }

    void assign(size_type count, const T& value) {
        clear();
        count = (count <= Capacity) ? count : Capacity;
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    void assign(std::initializer_list<T> ilist) {
        assign(ilist.begin(), ilist.end());
    }

    void swap(vector& other) {
        // Since we can't swap the arrays directly (fixed size),
        // we need to swap elements individually
        size_type min_size = (m_size < other.m_size) ? m_size : other.m_size;
        
        // Swap common elements
        for (size_type i = 0; i < min_size; ++i) {
            T temp = std::move(m_data[i]);
            m_data[i] = std::move(other.m_data[i]);
            other.m_data[i] = std::move(temp);
        }
        
        // Handle case where this vector is larger
        if (m_size > min_size) {
            for (size_type i = min_size; i < m_size; ++i) {
                new (&other.m_data[i]) T(std::move(m_data[i]));
                m_data[i].~T();
            }
        }
        // Handle case where other vector is larger
        else if (other.m_size > min_size) {
            for (size_type i = min_size; i < other.m_size; ++i) {
                new (&m_data[i]) T(std::move(other.m_data[i]));
                other.m_data[i].~T();
            }
        }
        
        // Swap sizes
        size_type temp_size = m_size;
        m_size = other.m_size;
        other.m_size = temp_size;
    }

private:
    // Storage for elements - using aligned storage to allow for proper construction/destruction
    alignas(T) unsigned char m_storage[sizeof(T) * Capacity];
    T* m_data = reinterpret_cast<T*>(m_storage);
    size_type m_size;
};

// Non-member functions
template <typename T, size_t Capacity>
bool operator==(const vector<T, Capacity>& lhs, const vector<T, Capacity>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (!(lhs[i] == rhs[i])) {
            return false;
        }
    }
    
    return true;
}

template <typename T, size_t Capacity>
bool operator!=(const vector<T, Capacity>& lhs, const vector<T, Capacity>& rhs) {
    return !(lhs == rhs);
}

template <typename T, size_t Capacity>
bool operator<(const vector<T, Capacity>& lhs, const vector<T, Capacity>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, size_t Capacity>
bool operator<=(const vector<T, Capacity>& lhs, const vector<T, Capacity>& rhs) {
    return !(rhs < lhs);
}

template <typename T, size_t Capacity>
bool operator>(const vector<T, Capacity>& lhs, const vector<T, Capacity>& rhs) {
    return rhs < lhs;
}

template <typename T, size_t Capacity>
bool operator>=(const vector<T, Capacity>& lhs, const vector<T, Capacity>& rhs) {
    return !(lhs < rhs);
}

template <typename T, size_t Capacity>
void swap(vector<T, Capacity>& lhs, vector<T, Capacity>& rhs) {
    lhs.swap(rhs);
}

} // namespace estl

#endif // ESTL_VECTOR_HPP
