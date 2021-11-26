#pragma once

#include <cstdint>

using namespace std;

class Base64
{
public:
    static const char* data_to_base64(char* data, uint32_t size);
    static const char* base64_to_data(char* base64, uint32_t size);

private:
    static const char PADDING = '=';
    static const char DATA_TO_BASE64_LOOKUP_TABLE[];
};
