#pragma once

#include "Array.h"
#include "PaddingAlgorithm.h"

#include <cstdint>

class PKCS7 : public PaddingAlgorithm
{
public:
    void append(Array* data, uint8_t alignment);
    bool check(Array* data);
    void remove(Array* data);
};
