#include "Base64.h"

#include "Array.h"

#include <cstdint>
#include <cstring>

const char* Base64::data_to_base64(Array* array)
{
    //Data is 8b/char, base64 is 6b/char; base64_size = 4 * ceiling(data_size / 3)
    //Ceiling: x / y + (x % y != 0)
    //Additional space for \0 char
    uint64_t buffer_size = 4 * (array->size() / 3 + (array->size() % 3 != 0)) + 1;

    char* base64_data = new char[buffer_size];
    base64_data[buffer_size - 1] = 0;

    uint64_t data_left = array->size();
    uint64_t data_pointer = 0;
    uint64_t base64_pointer = 0;
    while (data_left >= 3)
    {
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[array->data[data_pointer] >> 2];
        base64_data[base64_pointer++] =
            DATA_TO_BASE64_LOOKUP_TABLE[(array->data[data_pointer] << 4 & 0x3F) |
                                        (array->data[data_pointer + 1] >> 4)];
        base64_data[base64_pointer++] =
            DATA_TO_BASE64_LOOKUP_TABLE[(array->data[data_pointer + 1] << 2 & 0x3F) |
                                        (array->data[data_pointer + 2] >> 6)];
        base64_data[base64_pointer++] =
            DATA_TO_BASE64_LOOKUP_TABLE[array->data[data_pointer + 2] & 0x3F];

        data_left -= 3;
        data_pointer += 3;
    }
    if (data_left == 2)
    {
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[array->data[data_pointer] >> 2];
        base64_data[base64_pointer++] =
            DATA_TO_BASE64_LOOKUP_TABLE[(array->data[data_pointer] << 4 & 0x3F) |
                                        (array->data[data_pointer + 1] >> 4)];
        base64_data[base64_pointer++] =
            DATA_TO_BASE64_LOOKUP_TABLE[array->data[data_pointer + 1] << 2 & 0x3F];
        base64_data[base64_pointer++] = PADDING;
    }
    else if (data_left == 1)
    {
        base64_data[base64_pointer++] = DATA_TO_BASE64_LOOKUP_TABLE[array->data[data_pointer] >> 2];
        base64_data[base64_pointer++] =
            DATA_TO_BASE64_LOOKUP_TABLE[array->data[data_pointer] << 4 & 0x3F];
        base64_data[base64_pointer++] = PADDING;
        base64_data[base64_pointer++] = PADDING;
    }

    return base64_data;
}

Array* Base64::base64_to_data(char* base64)
{
    //Calculate base64 length
    uint64_t base64_length = strlen(base64);

    //Don't process invalid data
    if (base64_length % 4)
        return NULL;

    //Array for data
    Array* data = new Array((base64_length / 4 * 3) - (base64[base64_length - 1] == '=') -
                            (base64[base64_length - 2] == '='));

    //Decode base64
    uint64_t data_pointer = 0;

    for (uint64_t i = 0; i < base64_length - 4; i += 4)
    {
        //Check if data is valid
        if (!(is_base64(base64[i]) && is_base64(base64[i + 1]) && is_base64(base64[i + 2]) &&
              is_base64(base64[i + 3])))
        {
            //Invalid data - reject base64
            data->~Array();
            return NULL;
        }

        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[i]] << 2 |
                                     BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[i + 1]] >> 4;
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[i + 1]] << 4 |
                                     BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[i + 2]] >> 2;
        data->data[data_pointer++] = BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[i + 2]] << 6 |
                                     BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[i + 3]];
    }

    //Process last 4-character group
    if (is_base64(base64[base64_length - 4]) && is_base64(base64[base64_length - 3]) &&
        is_base64(base64[base64_length - 2]) && is_base64(base64[base64_length - 1]))
    {
        data->data[data_pointer++] =
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 4]] << 2 |
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 3]] >> 4;
        data->data[data_pointer++] =
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 3]] << 4 |
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 2]] >> 2;
        data->data[data_pointer++] =
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 2]] << 6 |
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 1]];
    }
    else if (is_base64(base64[base64_length - 4]) && is_base64(base64[base64_length - 3]) &&
             is_base64(base64[base64_length - 2]) && is_padding(base64[base64_length - 1]))
    {
        data->data[data_pointer++] =
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 4]] << 2 |
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 3]] >> 4;
        data->data[data_pointer++] =
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 3]] << 4 |
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 2]] >> 2;
    }
    else if (is_base64(base64[base64_length - 4]) && is_base64(base64[base64_length - 3]) &&
             is_padding(base64[base64_length - 2]) && is_padding(base64[base64_length - 1]))
    {
        data->data[data_pointer++] =
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 4]] << 2 |
            BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)base64[base64_length - 3]] >> 4;
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
    return BASE64_TO_DATA_LOOKUP_TABLE[(uint8_t)c] != 255;
}

bool Base64::is_padding(char c)
{
    return c == PADDING;
}

const char Base64::DATA_TO_BASE64_LOOKUP_TABLE[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

const uint8_t Base64::BASE64_TO_DATA_LOOKUP_TABLE[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //0 -  15
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //16 -  31
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,  255, 255, 255, 63,  //32 -  47
    52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255, 255, 255, 255, 255, //48 -  63
    255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  //64 -  79
    15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  255, 255, 255, 255, 255, //80 -  95
    255, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  //96 - 111
    41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  255, 255, 255, 255, 255, //112 - 127
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //128 - 143
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //144 - 159
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //160 - 175
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //176 - 191
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //192 - 207
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //208 - 223
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //224 - 239
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255  //240 - 255
};
