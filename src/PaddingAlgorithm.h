#pragma once

#include "Array.h"

#include <cstdint>
#include <iostream>

class PaddingAlgorithm
{
public:
    static void append(Array* data, uint8_t alignment)
    {
        not_implemented();
        std::exit(1);
    }

    static bool check(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

    static void remove(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

private:
    static void not_implemented(void)
    {
        std::cerr << "Not implemented\n";
    }
};
