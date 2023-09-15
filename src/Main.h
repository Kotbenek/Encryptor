#pragma once

#include <string>
#include <unordered_map>

#define parse_parameter(parameter_type, parameter)                                                \
    {                                                                                             \
        if (auto it = parameter_type::get_from_string.find(optarg);                               \
            it != parameter_type::get_from_string.end())                                          \
        {                                                                                         \
            parameters.parameter = it->second;                                                    \
            break;                                                                                \
        }                                                                                         \
        std::cerr << "Invalid value \"" << optarg << "\" for parameter --" << #parameter << "\n"; \
        return 1;                                                                                 \
    }

namespace opt
{
    enum opt
    {
        flag_set = 0,
        input_data_type = 1000,
        input,
        output_data_type,
        output,
        algorithm,
        key_data_type,
        key,
        iv_data_type,
        iv,
        block_cipher_mode,
        help
    };
}

namespace action
{
    enum action
    {
        not_set,
        encrypt,
        decrypt
    };
}

namespace data_type
{
    enum data_type
    {
        not_set,
        base16,
        base64,
        file
    };

    static std::unordered_map<std::string, data_type> const get_from_string = {
        {"base16", data_type::base16},
        {"base64", data_type::base64},
        {"file",   data_type::file  }
    };
} //namespace data_type

namespace block_cipher_mode
{
    enum block_cipher_mode
    {
        not_set,
        CBC
    };

    static std::unordered_map<std::string, block_cipher_mode> const get_from_string = {
        {"cbc", block_cipher_mode::CBC}
    };
} //namespace block_cipher_mode

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

struct
{
    int action = action::not_set;
    int input_data_type = data_type::not_set;
    std::string input;
    int output_data_type = data_type::not_set;
    std::string output;
    int algorithm = algorithm::not_set;
    int key_data_type = data_type::not_set;
    std::string key;
    int iv_data_type = data_type::not_set;
    std::string iv;
    int block_cipher_mode = block_cipher_mode::not_set;
} parameters;

int main(int argc, char** argv);
void display_help();
