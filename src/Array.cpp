#include "Array.h"

#include <algorithm>
#include <cstring>

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

Array::Array(Array& a) : Array(a.size())
{
    std::copy(&a.data[0], &a.data[a.size()], data);
}

Array::Array(std::string& s) : Array(s.size())
{
    std::memcpy(data, s.c_str(), _size);
}

Array::~Array()
{
    free(data);
}
