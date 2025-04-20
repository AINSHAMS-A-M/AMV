#pragma once
#include <iostream>

template <typename T>
struct AMArray
{
private:
    T *_data;
    size_t _size;
    size_t _capacity;

public:
    // called when creating an object
    AMArray(int intial_size = 0)
    {
        if (intial_size < 0)
        {
            throw std::out_of_range("Size is smaller than zero");
        }

        _data = new T[intial_size];
        _size = intial_size;
        _capacity = std::min(intial_size, 1) * 2;
    }

    ~AMArray()
    {
        delete[] _data;
    }

    void push_back(T value)
    {
        if (_size == _capacity)
        {
            _capacity *= 2;

            int *new_data = new T[_capacity];

            for (size_t i = 0; i < _size; i++)
            {
                new_data[i] = _data[i];
            }

            delete[] _data;
            _data = new_data;
        }

        _data[_size++] = value;
    }

    void pop()
    {
        if (_size == 0)
        {
            throw std::out_of_range("Index out of bounds");
        }

        _size--;
    }

    T &at(size_t index)
    {
        if (index >= _size)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return _data[index];
    }

    std::size_t size() const { return _size; }
    std::size_t capacity() const { return _capacity; }

    T &operator[](int index) { return at(index); };
    const T &operator[](int index) const { return at(index); };

    void operator+=(T val)
    {
        push_back(val);
    }

    void operator=(AMArray val)
    {
        auto s = val.size();

        delete[] _data;
        _size = 0;
        _capacity = val.capacity();

        for (int i = 0; i < s; ++i)
        {
            push_back(val[i]);
        }
    }

    /// ITERATOR SETUP - Allows you to use "auto" syntax to loop ///
    struct Iterator
    {
    public:
        Iterator(T *ptr) : _ptr(ptr) {}

        T &operator*() const { return *_ptr; }
        T *operator->() const { return _ptr; }

        Iterator &operator++()
        {
            ++_ptr;
            return *this;
        }

        bool operator==(const Iterator &other) const { return _ptr == other._ptr; }
        bool operator!=(const Iterator &other) const { return _ptr != other._ptr; }

    private:
        T *_ptr; // Pointer to the current element
    };

    Iterator begin() { return Iterator(_data); }
    Iterator end() { return Iterator(_data + _size); }
};