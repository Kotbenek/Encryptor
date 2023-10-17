#pragma once

#include <string>
#include <unordered_map>

namespace block_cipher_mode
{
    enum block_cipher_mode
    {
        not_set,
        ECB,
        CBC
    };

    static std::unordered_map<std::string, block_cipher_mode> const get_from_string = {
        {"ecb", block_cipher_mode::ECB},
        {"cbc", block_cipher_mode::CBC}
    };
} //namespace block_cipher_mode
