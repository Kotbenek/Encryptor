#include "Base64.h"

#include <cstddef>
#include <iostream>

const char* Base64::data_to_base64(char* data, uint32_t size)
{
    //Data is 8b/char, base64 is 6b/char; base64_size = ceiling(4/3 * data_size)
    //Ceiling: x / y + (x % y != 0)
    //Additional space for \0 char
    uint32_t buffer_size = size * 4 / 3 + (size * 4 % 3 != 0) + 1;
    
    char* base64_data = new char[buffer_size];
    base64_data[buffer_size - 1] = 0;
    
    uint32_t data_left = size;
    uint32_t data_pointer = 0;
    uint32_t base64_pointer = 0;
    while (data_left >= 3)
    {
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[data[data_pointer] >> 2];
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[((data[data_pointer] << 4) & 0x3F) | (data[data_pointer + 1] >> 4)];
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[((data[data_pointer + 1] << 2) & 0x3F) | (data[data_pointer + 2] >> 6)];
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[data[data_pointer + 2] & 0x3F];
        
        data_left -= 3;
        data_pointer += 3;
    }
    if (data_left == 2)
    {
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[data[data_pointer] >> 2];
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[((data[data_pointer] << 4) & 0x3F) | (data[data_pointer + 1] >> 4)];
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[((data[data_pointer + 1] << 2) & 0x3F)];
        base64_data[base64_pointer++] = Base64::PADDING;
    }
    else if (data_left == 1)
    {
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[data[data_pointer] >> 2];
        base64_data[base64_pointer++] = Base64::DATA_TO_BASE64_LOOKUP_TABLE[((data[data_pointer] << 4) & 0x3F)];
        base64_data[base64_pointer++] = Base64::PADDING;
        base64_data[base64_pointer++] = Base64::PADDING;
    }
    
    return base64_data;
}

const char* Base64::base64_to_data(char* base64, uint32_t size)
{
    //TODO
    return "Not yet implemented";
}

const char Base64::DATA_TO_BASE64_LOOKUP_TABLE[] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c' ,'d', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};
