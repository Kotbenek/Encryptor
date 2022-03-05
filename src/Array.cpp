#include "Array.h"

#include <cstdint>
#include <algorithm>

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

    copy(&a.data[0], &a.data[a.size()], data);
}

Array::~Array()
{
    delete[] data;
}

