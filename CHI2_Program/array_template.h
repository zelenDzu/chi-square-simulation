#pragma once

#include <cassert>
#include <iostream>

template <typename T>
class Array
{
private:
    T* list;
    size_t size;

public:
    explicit Array(size_t n);
    Array(const Array& data);
    Array(Array&& moved) noexcept;
    ~Array();

    size_t get_size() const;
    T get_max() const;

    const T& operator[](int i) const;
    T& operator[](int i);

    void set(size_t n, const T* data);
    void set(size_t i, T x);
    void print() const;

    Array& add(const Array& second);
    Array sum(const Array& second) const;
    Array& operator+=(const Array& second);

    Array& assign(const Array& second);
    Array& operator=(const Array& second);
    Array& operator=(Array&& second) noexcept;
};

template <typename T>
T Array<T>::get_max() const
{
    assert(size > 0);

    T max = list[0];
    for (int i = 1; i < size; i++)
    {
        if (list[i] > max)
            max = list[i];
    }

    return max;
}

template <typename T>
Array<T>::Array(size_t n) : size(n), list(nullptr)
{
    if (!n) return;
    list = new T[size];
    for (int i = 0; i < size; list[i++] = T()); // такой вызов int() = 0, double() = 0
}


template <typename T>
Array<T>::Array(const Array<T>& data) : size(data.size), list(nullptr)
{
    if (!size) return;
    list = new T[size];
    for (int i = 0; i < size; ++i)
    {
        list[i] = data.list[i];
    }
}

template <typename T>
Array<T>::Array(Array<T>&& moved) noexcept
{
    this->list = moved.list;
    this->size = moved.size;
    moved.list = nullptr;
    moved.size = 0;
}

template <typename T>
Array<T>::~Array()
{
    delete[] list;
}

template <typename T>
size_t Array<T>::get_size() const
{
    return size;
}

template <typename T>
const T& Array<T>::operator[](int i) const
{
    assert(i >= 0 && i < size);
    return list[i];
}

template <typename T>
T& Array<T>::operator[](int i)
{
    assert(i >= 0 && i < size);
    return list[i];
}

template <typename T>
void Array<T>::set(size_t n, const T* data)
{
    if (size != n)
    {
        size = n;
        delete[] list;
        list = new T[size];
    }
    for (int i = 0; i < size; ++i)
    {
        list[i] = data[i];
    }
}

template <typename T>
void Array<T>::set(size_t i, T x)
{
    assert(i >= 0 && i < size);
    list[i] = x;
}

template <typename T>
void Array<T>::print() const
{
    std::cout << "[";
    for (int i = 0; i < size; ++i)
    {
        std::cout << list[i];
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}


template <typename T>
Array<T>& Array<T>::add(const Array<T>& second)
{
    return (*this += second);
}

template <typename T>
Array<T> Array<T>::sum(const Array<T>& second) const
{
    Array<T> result = *this;
    result += second;
    return result;
}

template <typename T>
Array<T>& Array<T>::operator+=(const Array<T>& second)
{
    assert(size == second.size);
    for (int i = 0; i < size; ++i)
    {
        list[i] += second.list[i];
    }
    return *this;
}


template <typename T>
Array<T>& Array<T>::assign(const Array<T>& other)
{
    if (this != &other)
    {
        if (size != other.size)
        {
            delete[] list;
            size = other.size;
            list = list = new T[size];
        }

        for (size_t i = 0; i < size; ++i)
        {
            list[i] = other.list[i];
        }
    }

    return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& second)
{
    return assign(second);
}

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& second) noexcept
{
    if (this != &second)
    {
        delete[] this->list;
        this->list = second.list;
        this->size = second.size;
        second.list = nullptr;
        second.size = 0;

    }
    return *this;

}

template <typename T>
Array<T> operator+(Array<T> a, const Array<T>& b)
{
    a += b;
    return a;
}


