#pragma once

#include "Array.h"

#include <cstdint>

using namespace std;

class Base16
{
public:
    static const char* data_to_base16(Array* array);
    static Array* base16_to_data(char* base16);

private:
    static bool is_base16(char c);
    static const char DATA_TO_BASE16_LOOKUP_TABLE[];
    static const char BASE16_TO_DATA_LOOKUP_TABLE[];
};
