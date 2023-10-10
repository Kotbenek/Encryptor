#pragma once

#include <string>
#include <unordered_map>

namespace data_type
{
    enum data_type
    {
        not_set,
        base16,
        base64,
        file,
        raw
    };

    static std::unordered_map<std::string, data_type> const get_from_string = {
        {"base16", data_type::base16},
        {"base64", data_type::base64},
        {"file",   data_type::file  },
        {"raw",    data_type::raw   }
    };
} //namespace data_type
