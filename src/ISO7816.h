#pragma once

#include "Array.h"
#include "PaddingAlgorithm.h"

#include <cstdint>

class ISO7816 : public PaddingAlgorithm
{
public:
    static void append(Array* data, uint8_t alignment);
    static bool check(Array* data);
    static void remove(Array* data);
};
