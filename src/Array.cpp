#include "Array.h"

#include <algorithm>

uint64_t Array::size()
{
    return _size;
}

void Array::resize(uint64_t new_size)
{
    data = (uint8_t*)realloc(data, new_size);
    _size = new_size;
}

Array::Array(uint64_t size)
{
    data = (uint8_t*)malloc(size);
    _size = size;
}

Array::Array(Array& a)
{
    _size = a.size();
    data = (uint8_t*)malloc(_size);

    std::copy(&a.data[0], &a.data[a.size()], data);
}

Array::~Array()
{
    free(data);
}
