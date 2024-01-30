#pragma once

#include <string>
#include <unordered_map>

namespace padding
{
    enum padding
    {
        not_set,
        PKCS7,
        ANSI_X923,
        ISO7816
    };

    static std::unordered_map<std::string, padding> const get_from_string = {
        {"pkcs7",   padding::PKCS7    },
        {"x923",    padding::ANSI_X923},
        {"iso7816", padding::ISO7816  }
    };
} //namespace padding
