#include "Base64.h"

#include "Array.h"

#include <cstddef>
#include <iostream>

const char* Base64::data_to_base64(Array* array)
{
    //Data is 8b/char, base64 is 6b/char; base64_size = ceiling(4/3 * data_size)
    //Ceiling: x / y + (x % y != 0)
    //Additional space for \0 char
    uint64_t buffer_size = array->size() * 4 / 3 + (array->size() * 4 % 3 != 0) + 1;
    
    char* base64_data = new char[buffer_size];
    base64_data[buffer_size - 1] = 0;
    
    uint64_t data_left = array->size();
    uint64_t data_pointer = 0;
    uint64_t base64_pointer = 0;
    while (data_left >= 3)
    {
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)array->data[data_pointer] >> 2];
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)(((array->data[data_pointer] << 4) & 0x3F) | (array->data[data_pointer + 1] >> 4))];
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)(((array->data[data_pointer + 1] << 2) & 0x3F) | ((unsigned char)array->data[data_pointer + 2] >> 6))];
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)array->data[data_pointer + 2] & 0x3F];
        
        data_left -= 3;
        data_pointer += 3;
    }
    if (data_left == 2)
    {
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)array->data[data_pointer] >> 2];
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)(((array->data[data_pointer] << 4) & 0x3F) | ((unsigned char)array->data[data_pointer + 1] >> 4))];
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)((array->data[data_pointer + 1] << 2) & 0x3F)];
        base64_data[base64_pointer++] = PADDING;
    }
    else if (data_left == 1)
    {
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)array->data[data_pointer] >> 2];
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[(unsigned char)((array->data[data_pointer] << 4) & 0x3F)];
        base64_data[base64_pointer++] = PADDING;
        base64_data[base64_pointer++] = PADDING;
    }
    
    return base64_data;
}

Array* Base64::base64_to_data(char* base64)
{
    //Calculate base64 length
    uint64_t base64_length = 0;
    while (base64[base64_length++]);
    base64_length--;
    
    //Don't process invalid data
    if (base64_length % 4) return NULL;
    
    //Array for data
    Array* data = new Array((base64_length / 4 * 3) - (base64[base64_length - 1] == '=') - (base64[base64_length - 2] == '='));
    
    //Decode base64
    uint64_t data_pointer = 0;
    
    for (uint64_t i = 0; i < base64_length - 4; i += 4)
    {
        //Check if data is valid
        if (!(is_base64(base64[i]) && is_base64(base64[i + 1]) && is_base64(base64[i + 2]) && is_base64(base64[i + 3])))
        {
            //Invalid data - reject base64
            data->~Array();
            return NULL;
        }
        
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[i]] << 2 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[i + 1]] >> 4;
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[i + 1]] << 4 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[i + 2]] >> 2;
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[i + 2]] << 6 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[i + 3]];
        
    }
    
    //Process last 4-character group
    if (is_base64(base64[base64_length - 4]) && is_base64(base64[base64_length - 3]) && is_base64(base64[base64_length - 2]) && is_base64(base64[base64_length - 1]))
    {
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 4]] << 2 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 3]] >> 4;
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 3]] << 4 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 2]] >> 2;
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 2]] << 6 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 1]];
    }
    else if (is_base64(base64[base64_length - 4]) && is_base64(base64[base64_length - 3]) && is_base64(base64[base64_length - 2]) && is_padding(base64[base64_length - 1]))
    {
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 4]] << 2 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 3]] >> 4;
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 3]] << 4 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 2]] >> 2;
    }
    else if (is_base64(base64[base64_length - 4]) && is_base64(base64[base64_length - 3]) && is_padding(base64[base64_length - 2]) && is_padding(base64[base64_length - 1]))
    {
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 4]] << 2 | BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)base64[base64_length - 3]] >> 4;
    }
    else
    {
        //Invalid data - reject base64
        data->~Array();
        return NULL;
    }
    
    return data;
}

bool Base64::is_base64(char c)
{
    return BASE64_TO_DATA_LOOKUP_TABLE[(unsigned char)c] != -1;
}

bool Base64::is_padding(char c)
{
    return c == PADDING;
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

const char Base64::BASE64_TO_DATA_LOOKUP_TABLE[] =
{
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //  0 -  15
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 16 -  31
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  62,  -1,  -1,  -1,  63, // 32 -  47
    52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  -1,  -1,  -1,  -1,  -1,  -1, // 48 -  63
    -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14, // 64 -  79
    15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  -1,  -1,  -1,  -1,  -1, // 80 -  95
    -1,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40, // 96 - 111
    41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  -1,  -1,  -1,  -1,  -1, //112 - 127
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //128 - 143
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //144 - 159
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //160 - 175
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //176 - 191
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //192 - 207
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //208 - 223
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //224 - 239
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1  //240 - 255
};
