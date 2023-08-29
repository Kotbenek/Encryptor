#include "Base16.h"

#include "Array.h"

#include <cstddef>

const char* Base16::data_to_base16(Array* array)
{
    //Data is 8b/char, base16 is 4b/char
    //Additional space for \0 char
    uint64_t buffer_size = array->size() * 2 + 1;

    char* base16_data = new char[buffer_size];
    base16_data[buffer_size - 1] = 0;

    uint64_t data_pointer = 0;
    uint64_t base16_pointer = 0;

    while (array->size() - data_pointer)
    {
        base16_data[base16_pointer++] = DATA_TO_BASE16_LOOKUP_TABLE[array->data[data_pointer] >> 4];
        base16_data[base16_pointer++] =
            DATA_TO_BASE16_LOOKUP_TABLE[array->data[data_pointer] & 0x0F];

        data_pointer++;
    }

    return base16_data;
}

Array* Base16::base16_to_data(char* base16)
{
    //Calculate base16_length
    uint64_t base16_length = 0;
    while (base16[base16_length++])
        ;
    base16_length--;

    //Don't process invalid data
    if (base16_length % 2)
        return NULL;

    //Array for data
    Array* data = new Array(base16_length / 2);

    //Decode base16
    uint64_t data_pointer = 0;

    for (uint64_t i = 0; i < base16_length; i += 2)
    {
        //Check if data is valid
        if (!(is_base16(base16[i]) && is_base16(base16[i + 1])))
        {
            //Invalid data - reject base16
            data->~Array();
            return NULL;
        }

        data->data[data_pointer++] = BASE16_TO_DATA_LOOKUP_TABLE[(uint8_t)base16[i]] << 4 |
                                     BASE16_TO_DATA_LOOKUP_TABLE[(uint8_t)base16[i + 1]];
    }

    return data;
}

bool Base16::is_base16(char c)
{
    return BASE16_TO_DATA_LOOKUP_TABLE[(uint8_t)c] != 255;
}

const char Base16::DATA_TO_BASE16_LOOKUP_TABLE[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

const uint8_t Base16::BASE16_TO_DATA_LOOKUP_TABLE[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //0 -  15
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //16 -  31
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //32 -  47
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   255, 255, 255, 255, 255, 255, //48 -  63
    255, 10,  11,  12,  13,  14,  15,  255, 255, 255, 255, 255, 255, 255, 255, 255, //64 -  79
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //80 -  95
    255, 10,  11,  12,  13,  14,  15,  255, 255, 255, 255, 255, 255, 255, 255, 255, //96 - 111
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //112 - 127
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //128 - 143
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //144 - 159
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //160 - 175
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //176 - 191
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //192 - 207
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //208 - 223
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, //224 - 239
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255  //240 - 255
};
