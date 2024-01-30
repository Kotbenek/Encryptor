#pragma once

#include "Array.h"

#include <cstdint>
#include <iostream>

class PaddingAlgorithm
{
public:
    virtual void append(Array* data, uint8_t alignment)
    {
        not_implemented();
        std::exit(1);
    }

    virtual bool check(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

    virtual void remove(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

private:
    void not_implemented(void)
    {
        std::cerr << "Not implemented\n";
    }
};
