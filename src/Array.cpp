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

Array::~Array()
{
    delete[] data;
}

