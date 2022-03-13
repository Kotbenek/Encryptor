#include "Array.h"

#include <algorithm>

uint64_t Array::size()
{
    return _size;
}

Array::Array(uint64_t size)
{
    data = new uint8_t[size];
    _size = size;
}

Array::Array(Array &a)
{
    _size = a.size();
    data = new uint8_t[_size];

    std::copy(&a.data[0], &a.data[a.size()], data);
}

Array::~Array()
{
    delete[] data;
}

