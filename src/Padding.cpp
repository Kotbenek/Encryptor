#include "Padding.h"

#include "Array.h"

#include <cstdint>

namespace Padding
{
    void PKCS7::append(Array* data, uint8_t alignment)
    {
        uint64_t old_size = data->size();
        uint8_t padding_value = alignment - (old_size % alignment);
        data->resize(old_size + padding_value);
        for (uint64_t i = old_size; i < data->size(); i++)
            data->data[i] = padding_value;
    }

    bool PKCS7::check(Array* data)
    {
        uint8_t padding_value = data->data[data->size() - 1];
        if (padding_value > data->size())
            return false;
        for (uint64_t i = data->size() - padding_value; i < data->size(); i++)
            if (data->data[i] != padding_value)
                return false;
        return true;
    }

    void PKCS7::remove(Array* data)
    {
        uint8_t padding_value = data->data[data->size() - 1];
        data->resize(data->size() - padding_value);
    }
} //namespace Padding
