#include "ISO7816.h"

#include <cstdint>

void ISO7816::append(Array* data, uint8_t alignment)
{
    uint64_t old_size = data->size();
    uint8_t padding_length = alignment - (old_size % alignment);
    data->resize(old_size + padding_length);
    data->data[old_size] = 0x80;
    for (uint64_t i = old_size + 1; i < data->size(); i++)
        data->data[i] = 0;
}

bool ISO7816::check(Array* data)
{
    uint64_t i = data->size();
    while (i-- > 0)
        if (data->data[i] != 0)
        {
            if (data->data[i] == 0x80)
                return true;
            return false;
        }
    return false;
}

void ISO7816::remove(Array* data)
{
    uint64_t i = data->size();
    while (i-- > 0)
        if (data->data[i] == 0x80)
            break;
    data->resize(i);
}
