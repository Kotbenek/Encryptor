#pragma once

#include <cstdint>

using namespace std;

class Array
{
public:
    char* data;
    
    uint64_t size();
    
    Array(uint64_t size);
    ~Array();

private:
    uint64_t _size = 0;
};
