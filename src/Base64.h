#pragma once

#include "Array.h"

#include <cstdint>

using namespace std;

class Base64
{
public:
    static const char* data_to_base64(Array* array);
    static Array* base64_to_data(char* base64);
    
private:
    static bool is_base64(char c);
    static bool is_padding(char c);
    static const char PADDING = '=';
    static const char DATA_TO_BASE64_LOOKUP_TABLE[];
    static const char BASE64_TO_DATA_LOOKUP_TABLE[];
};
