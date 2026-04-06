#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <cstring> // для memcpy

// Простой итератор произвольного доступа
template<typename T>
class VectorIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    VectorIterator(pointer ptr = nullptr) : m_ptr(ptr) {}

    // Операторы разыменования
    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }

    // Инкремент / декремент
    VectorIterator& operator++() { ++m_ptr; return *this; }
    VectorIterator operator++(int) { VectorIterator tmp = *this; ++m_ptr; return tmp; }
    VectorIterator& operator--() { --m_ptr; return *this; }
    VectorIterator operator--(int) { VectorIterator tmp = *this; --m_ptr; return tmp; }

    // Арифметика
    VectorIterator operator+(difference_type n) const { return VectorIterator(m_ptr + n); }
    VectorIterator operator-(difference_type n) const { return VectorIterator(m_ptr - n); }
    difference_type operator-(const VectorIterator& other) const { return m_ptr - other.m_ptr; }
    VectorIterator& operator+=(difference_type n) { m_ptr += n; return *this; }
    VectorIterator& operator-=(difference_type n) { m_ptr -= n; return *this; }

    // Сравнение
    bool operator==(const VectorIterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const VectorIterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const VectorIterator& other) const { return m_ptr < other.m_ptr; }
    bool operator>(const VectorIterator& other) const { return m_ptr > other.m_ptr; }
    bool operator<=(const VectorIterator& other) const { return m_ptr <= other.m_ptr; }
    bool operator>=(const VectorIterator& other) const { return m_ptr >= other.m_ptr; }

private:
    pointer m_ptr;
};

// Константный итератор
template<typename T>
class VectorConstIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    VectorConstIterator(pointer ptr = nullptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }

    VectorConstIterator& operator++() { ++m_ptr; return *this; }
    VectorConstIterator operator++(int) { VectorConstIterator tmp = *this; ++m_ptr; return tmp; }
    VectorConstIterator& operator--() { --m_ptr; return *this; }
    VectorConstIterator operator--(int) { VectorConstIterator tmp = *this; --m_ptr; return tmp; }

    VectorConstIterator operator+(difference_type n) const { return VectorConstIterator(m_ptr + n); }
    VectorConstIterator operator-(difference_type n) const { return VectorConstIterator(m_ptr - n); }
    difference_type operator-(const VectorConstIterator& other) const { return m_ptr - other.m_ptr; }
    VectorConstIterator& operator+=(difference_type n) { m_ptr += n; return *this; }
    VectorConstIterator& operator-=(difference_type n) { m_ptr -= n; return *this; }

    bool operator==(const VectorConstIterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const VectorConstIterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const VectorConstIterator& other) const { return m_ptr < other.m_ptr; }
    bool operator>(const VectorConstIterator& other) const { return m_ptr > other.m_ptr; }
    bool operator<=(const VectorConstIterator& other) const { return m_ptr <= other.m_ptr; }
    bool operator>=(const VectorConstIterator& other) const { return m_ptr >= other.m_ptr; }

private:
    pointer m_ptr;
};

// Класс Vector
template<typename T>
class Vector {
public:
    using iterator = VectorIterator<T>;
    using const_iterator = VectorConstIterator<T>;
    using reverse_iterator = VectorIterator<T>;
    using const_reverse_iterator = VectorConstIterator<T>;

    // Конструкторы и деструктор
    Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}
    explicit Vector(size_t count) : m_data(nullptr), m_size(0), m_capacity(0) {
        reserve(count);
        for (size_t i = 0; i < count; ++i)
            new (m_data + i) T();
        m_size = count;
    }
    Vector(const Vector& other) : m_data(nullptr), m_size(0), m_capacity(0) {
        assign(other.begin(), other.end());
    }
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            clear();
            assign(other.begin(), other.end());
        }
        return *this;
    }
    ~Vector() {
        clear();
        if (m_data)
            delete[] reinterpret_cast<char*>(m_data);
    }

    // Итераторы
    iterator begin() { return iterator(m_data); }
    iterator end() { return iterator(m_data + m_size); }
    const_iterator begin() const { return const_iterator(m_data); }
    const_iterator end() const { return const_iterator(m_data + m_size); }
    const_iterator cbegin() const { return const_iterator(m_data); }
    const_iterator cend() const { return const_iterator(m_data + m_size); }

    reverse_iterator rbegin() { return reverse_iterator(m_data + m_size - 1); }
    reverse_iterator rend() { return reverse_iterator(m_data - 1); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(m_data + m_size - 1); }
    const_reverse_iterator rend() const { return const_reverse_iterator(m_data - 1); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(m_data + m_size - 1); }
    const_reverse_iterator crend() const { return const_reverse_iterator(m_data - 1); }

    // Ёмкость
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }
    size_t max_size() const { return static_cast<size_t>(-1) / sizeof(T); }
    void reserve(size_t new_cap) {
        if (new_cap <= m_capacity) return;
        T* new_data = reinterpret_cast<T*>(new char[new_cap * sizeof(T)]);
        for (size_t i = 0; i < m_size; ++i) {
            new (new_data + i) T(std::move(m_data[i]));
            m_data[i].~T();
        }
        delete[] reinterpret_cast<char*>(m_data);
        m_data = new_data;
        m_capacity = new_cap;
    }
    void resize(size_t count) {
        if (count < m_size) {
            for (size_t i = count; i < m_size; ++i)
                m_data[i].~T();
        } else if (count > m_size) {
            if (count > m_capacity) reserve(count);
            for (size_t i = m_size; i < count; ++i)
                new (m_data + i) T();
        }
        m_size = count;
    }

    // Доступ к элементам
    T& at(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("Vector::at: index out of range");
        return m_data[pos];
    }
    const T& at(size_t pos) const {
        if (pos >= m_size) throw std::out_of_range("Vector::at: index out of range");
        return m_data[pos];
    }
    T& operator[](size_t pos) { return m_data[pos]; }
    const T& operator[](size_t pos) const { return m_data[pos]; }
    T& front() { return m_data[0]; }
    const T& front() const { return m_data[0]; }
    T& back() { return m_data[m_size - 1]; }
    const T& back() const { return m_data[m_size - 1]; }
    T* data() { return m_data; }
    const T* data() const { return m_data; }

    // Модификаторы
    void assign(size_t count, const T& value) {
        clear();
        reserve(count);
        for (size_t i = 0; i < count; ++i)
            new (m_data + i) T(value);
        m_size = count;
    }
    template<typename InputIt>
    void assign(InputIt first, InputIt last) {
        clear();
        size_t count = static_cast<size_t>(last - first);
        reserve(count);
        for (size_t i = 0; first != last; ++first, ++i)
            new (m_data + i) T(*first);
        m_size = count;
    }
    void push_back(const T& value) {
        if (m_size >= m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        new (m_data + m_size) T(value);
        ++m_size;
    }
    void push_back(T&& value) {
        if (m_size >= m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        new (m_data + m_size) T(std::move(value));
        ++m_size;
    }
    void pop_back() {
        if (m_size > 0) {
            m_data[--m_size].~T();
        }
    }
    void clear() {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i].~T();
        m_size = 0;
    }
    iterator insert(const_iterator pos, const T& value) {
        size_t index = static_cast<size_t>(pos - cbegin());
        if (m_size >= m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        // Сдвигаем элементы вправо
        for (size_t i = m_size; i > index; --i) {
            new (m_data + i) T(std::move(m_data[i-1]));
            m_data[i-1].~T();
        }
        new (m_data + index) T(value);
        ++m_size;
        return iterator(m_data + index);
    }
    iterator erase(const_iterator pos) {
        size_t index = static_cast<size_t>(pos - cbegin());
        m_data[index].~T();
        for (size_t i = index + 1; i < m_size; ++i) {
            new (m_data + i - 1) T(std::move(m_data[i]));
            m_data[i].~T();
        }
        --m_size;
        return iterator(m_data + index);
    }
    iterator erase(const_iterator first, const_iterator last) {
        size_t start = static_cast<size_t>(first - cbegin());
        size_t end = static_cast<size_t>(last - cbegin());
        size_t count = end - start;
        for (size_t i = start; i < end; ++i)
            m_data[i].~T();
        for (size_t i = end; i < m_size; ++i) {
            new (m_data + i - count) T(std::move(m_data[i]));
            m_data[i].~T();
        }
        m_size -= count;
        return iterator(m_data + start);
    }
    void swap(Vector& other) {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }
    // emplace, emplace_back – для простоты оставляем как push_back
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_t index = static_cast<size_t>(pos - cbegin());
        if (m_size >= m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        for (size_t i = m_size; i > index; --i) {
            new (m_data + i) T(std::move(m_data[i-1]));
            m_data[i-1].~T();
        }
        new (m_data + index) T(std::forward<Args>(args)...);
        ++m_size;
        return iterator(m_data + index);
    }
    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (m_size >= m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        new (m_data + m_size) T(std::forward<Args>(args)...);
        ++m_size;
    }

private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;
};

#endif // VECTOR_H
