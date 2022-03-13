#pragma once

#include <cstdint>

class Array
{
public:
    uint8_t* data;
    
    uint64_t size();
    
    Array(uint64_t size);
    Array(Array &a);
    ~Array();

private:
    uint64_t _size = 0;
};
