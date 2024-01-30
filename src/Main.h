#pragma once

#include "Array.h"
#include "EncryptionAlgorithm.h"
#include "PaddingAlgorithm.h"
#include "action.h"
#include "algorithm.h"
#include "block_cipher_mode.h"
#include "data_type.h"
#include "padding.h"

#include <string>

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
    int padding = padding::not_set;
    int append_newline_to_output = 0;
} parameters;

int main(int argc, char** argv);
void display_help();
void action_encrypt_no_padding(Array* data, EncryptionAlgorithm* algorithm);
void action_decrypt_no_padding(Array* data, EncryptionAlgorithm* algorithm);
void action_encrypt(Array* data, EncryptionAlgorithm* algorithm,
                    PaddingAlgorithm* padding_algorithm);
void action_decrypt(Array* data, EncryptionAlgorithm* algorithm,
                    PaddingAlgorithm* padding_algorithm);
void preprocess_and_write_base16(Array* data, std::ostream* stream);
void preprocess_and_write_base64(Array* data, std::ostream* stream);
void preprocess_and_write_raw(Array* data, std::ostream* stream);
