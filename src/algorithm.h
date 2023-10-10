#pragma once

#include <string>
#include <unordered_map>

namespace algorithm
{
    enum algorithm
    {
        not_set,
        AES128,
        AES192,
        AES256
    };

    static std::unordered_map<std::string, algorithm> const get_from_string = {
        {"aes128", algorithm::AES128},
        {"aes192", algorithm::AES192},
        {"aes256", algorithm::AES256}
    };
} //namespace algorithm
