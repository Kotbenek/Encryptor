#pragma once

#include "Array.h"
#include "PaddingAlgorithm.h"

#include <cstdint>

class PKCS7 : public PaddingAlgorithm
{
public:
    static void append(Array* data, uint8_t alignment);
    static bool check(Array* data);
    static void remove(Array* data);
};
