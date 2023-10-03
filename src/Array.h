#pragma once

#include <cstdint>
#include <string>

class Array
{
public:
    uint8_t* data;

    uint64_t size();
    void resize(uint64_t new_size);

    Array(uint64_t size);
    Array(Array& a);
    Array(std::string& s);
    ~Array();

private:
    uint64_t _size = 0;
};
