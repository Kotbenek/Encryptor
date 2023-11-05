#pragma once

#include "Array.h"

#include <cstdint>

namespace Padding
{
    class PKCS7
    {
    public:
        static void append(Array* data, uint8_t alignment);
        static bool check(Array* data);
        static void remove(Array* data);
    };

    class ANSI_X923
    {
    public:
        static void append(Array* data, uint8_t alignment);
        static bool check(Array* data);
        static void remove(Array* data);
    };
} //namespace Padding
