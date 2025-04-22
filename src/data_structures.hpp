#pragma once

#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
struct MeshVector
{
private:
    T *_data;
    size_t _size;
    size_t _capacity;

public:
    // Constructor
    MeshVector(size_t initial_size = 0)
    {
        if (initial_size < 0)
        {
            throw std::out_of_range("Size is smaller than zero");
        }

        _capacity = std::max(initial_size, size_t(1)) * 2;
        _data = new T[_capacity];
        _size = initial_size;
    }

    // Destructor
    ~MeshVector()
    {
        delete[] _data;
    }

    // Copy constructor
    MeshVector(const MeshVector &other)
    {
        _capacity = other._capacity;
        _size = other._size;
        _data = new T[_capacity];

        for (size_t i = 0; i < _size; ++i)
        {
            _data[i] = other._data[i];
        }
    }

    // Copy assignment operator
    MeshVector &operator=(const MeshVector &other)
    {
        if (this == &other)
            return *this;

        delete[] _data;

        _capacity = other._capacity;
        _size = other._size;
        _data = new T[_capacity];

        for (size_t i = 0; i < _size; ++i)
        {
            _data[i] = other._data[i];
        }

        return *this;
    }

    // Move constructor
    MeshVector(MeshVector &&other) noexcept
    {
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    // Move assignment
    MeshVector &operator=(MeshVector &&other) noexcept
    {
        if (this == &other)
            return *this;

        delete[] _data;

        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;

        return *this;
    }

    // Add element
    void push_back(T value)
    {
        if (_size == _capacity)
        {
            _capacity *= 2;
            T *new_data = new T[_capacity];
            for (size_t i = 0; i < _size; i++)
            {
                new_data[i] = _data[i];
            }

            delete[] _data;
            _data = new_data;
        }

        _data[_size++] = value;
    }

    // Remove last element
    void pop()
    {
        if (_size == 0)
        {
            throw std::out_of_range("Index out of bounds");
        }

        _size--;
    }

    // Element access with bounds checking
    T &at(size_t index)
    {
        if (index >= _size)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return _data[index];
    }

    const T &at(size_t index) const
    {
        if (index >= _size)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return _data[index];
    }

    // Size and capacity access
    std::size_t size() const { return _size; }
    std::size_t capacity() const { return _capacity; }

    // Index operators
    T &operator[](int index) { return at(index); }
    const T &operator[](int index) const { return at(index); }

    // += operator to push_back
    void operator+=(T val)
    {
        push_back(val);
    }

    // Iterator support
    struct Iterator
    {
        T *ptr;
        Iterator(T *p) : ptr(p) {}
        T &operator*() const { return *ptr; }
        T *operator->() const { return ptr; }

        Iterator &operator++()
        {
            ++ptr;
            return *this;
        }

        bool operator==(const Iterator &other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator &other) const { return ptr != other.ptr; }
    };

    Iterator begin() { return Iterator(_data); }
    Iterator end() { return Iterator(_data + _size); }
};
