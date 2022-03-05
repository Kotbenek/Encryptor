#include "Array.h"

#include <cstdint>

using namespace std;

uint64_t Array::size()
{
    return _size;
}

Array::Array(uint64_t size)
{
    data = new char[size];
    _size = size;
}

Array::Array(Array &a)
{
    _size = a.size();
    data = new char[_size];

    for (uint64_t i = 0; i < _size; i++)
    {
        data[i] = a.data[i];
    }
}

Array::~Array()
{
    delete[] data;
}

