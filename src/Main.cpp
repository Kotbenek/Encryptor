#include "Main.h"

#include "AES.h"
#include "Array.h"
#include "Base16.h"
#include "Base64.h"
#include "EncryptionAlgorithm.h"

#include <getopt.h>
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
    int c;

    while (1)
    {
        static struct option long_options[] = {
            {"encrypt",           no_argument,       &parameters.action, action::encrypt       },
            {"decrypt",           no_argument,       &parameters.action, action::decrypt       },
            {"input_data_type",   required_argument, 0,                  opt::input_data_type  },
            {"input",             required_argument, 0,                  opt::input            },
            {"output_data_type",  required_argument, 0,                  opt::output_data_type },
            {"output",            required_argument, 0,                  opt::output           },
            {"algorithm",         required_argument, 0,                  opt::algorithm        },
            {"key_data_type",     required_argument, 0,                  opt::key_data_type    },
            {"key",               required_argument, 0,                  opt::key              },
            {"iv_data_type",      required_argument, 0,                  opt::iv_data_type     },
            {"iv",                required_argument, 0,                  opt::iv               },
            {"block_cipher_mode", required_argument, 0,                  opt::block_cipher_mode},
            {"help",              no_argument,       0,                  opt::help             },
            {0,                   0,                 0,                  0                     }
        };

        c = getopt_long(argc, argv, "", long_options, NULL);

        if (c == -1)
            break;

        if (c == opt::flag_set)
            continue;

        switch (c)
        {
            case opt::input_data_type:
                parse_parameter(data_type, input_data_type);
            case opt::input:
                parameters.input = optarg;
                break;
            case opt::output_data_type:
                parse_parameter(data_type, output_data_type);
            case opt::output:
                parameters.output = optarg;
                break;
            case opt::algorithm:
                parse_parameter(algorithm, algorithm);
            case opt::key_data_type:
                parse_parameter(data_type, key_data_type);
            case opt::key:
                parameters.key = optarg;
                break;
            case opt::iv_data_type:
                parse_parameter(data_type, iv_data_type);
            case opt::iv:
                parameters.iv = optarg;
                break;
            case opt::block_cipher_mode:
                parse_parameter(block_cipher_mode, block_cipher_mode);
            case opt::help:
                display_help();
                break;
            case '?':
                //Unknown argument
                return 1;
            default:
                abort();
        }
    }

    //For now, if input is a file, output also must be a file
    if (parameters.input_data_type == data_type::file &&
        parameters.output_data_type != data_type::file)
    {
        std::cerr << "File input requires file output.";
        return 1;
    }
    //For now, if output is a file, input also must be a file
    if (parameters.output_data_type == data_type::file &&
        parameters.input_data_type != data_type::file)
    {
        std::cerr << "File output requires file input.";
        return 1;
    }

    std::unique_ptr<EncryptionAlgorithm> algorithm;
    switch (parameters.algorithm)
    {
        case algorithm::AES128:
            algorithm = std::unique_ptr<EncryptionAlgorithm>(new AES(AES::key_length::AES128));
            break;
        case algorithm::AES192:
            algorithm = std::unique_ptr<EncryptionAlgorithm>(new AES(AES::key_length::AES192));
            break;
        case algorithm::AES256:
            algorithm = std::unique_ptr<EncryptionAlgorithm>(new AES(AES::key_length::AES256));
            break;
        default:
            std::cerr << "Invalid or missing algorithm.\n";
            return 1;
    }

    std::unique_ptr<Array> key;
    switch (parameters.key_data_type)
    {
        case data_type::base16:
            key = std::unique_ptr<Array>(Base16::base16_to_data((char*)parameters.key.c_str()));
            break;
        case data_type::base64:
            key = std::unique_ptr<Array>(Base64::base64_to_data((char*)parameters.key.c_str()));
            break;
        default:
            std::cerr << "Invalid or missing key type.\n";
            return 1;
    }
    if (!key || algorithm->set_key(key.get()))
    {
        std::cerr << "Invalid or missing key.\n";
        return 1;
    }

    std::unique_ptr<Array> iv;
    switch (parameters.iv_data_type)
    {
        case data_type::base16:
            iv = std::unique_ptr<Array>(Base16::base16_to_data((char*)parameters.iv.c_str()));
            break;
        case data_type::base64:
            iv = std::unique_ptr<Array>(Base64::base64_to_data((char*)parameters.iv.c_str()));
            break;
        default:
            std::cerr << "Invalid or missing IV type.\n";
            return 1;
    }
    if (!iv || algorithm->set_IV(iv.get()))
    {
        std::cerr << "Invalid or missing IV.\n";
        return 1;
    }

    std::unique_ptr<Array> input_data;
    switch (parameters.input_data_type)
    {
        case data_type::base16:
            input_data =
                std::unique_ptr<Array>(Base16::base16_to_data((char*)parameters.input.c_str()));
            break;
        case data_type::base64:
            input_data =
                std::unique_ptr<Array>(Base64::base64_to_data((char*)parameters.input.c_str()));
            break;
        case data_type::file:
            break;
        default:
            std::cerr << "Invalid or missing input data type.\n";
            return 1;
    }

    //TODO: Move things like block_cipher_mode to separate functions to avoid having functions named
    //like encrypt_file_CBC_PKCS7 that results in awkward calling convention that can be seen below

    std::unique_ptr<Array> output_data;

    switch (parameters.block_cipher_mode)
    {
        case block_cipher_mode::CBC:
            switch (parameters.action)
            {
                case action::encrypt:
                    if (parameters.input_data_type == data_type::file)
                        algorithm->encrypt_file_CBC_PKCS7(parameters.input, parameters.output);
                    else
                        output_data =
                            std::unique_ptr<Array>(algorithm->encrypt_CBC_PKCS7(input_data.get()));
                    break;
                case action::decrypt:
                    if (parameters.input_data_type == data_type::file)
                        algorithm->decrypt_file_CBC_PKCS7(parameters.input, parameters.output);
                    else
                        output_data =
                            std::unique_ptr<Array>(algorithm->decrypt_CBC_PKCS7(input_data.get()));
                    break;
                default:
                    std::cerr << "Invalid or missing action.\n";
                    return 1;
            }
            break;
        default:
            std::cerr << "Invalid or missing block cipher mode.\n";
            return 1;
    }

    //If there is no output data to print - exit
    //(input_data_type == output_data_type == data_type::file)
    if (!output_data)
        return 0;

    //Print output data
    std::unique_ptr<char> data_to_print;
    switch (parameters.output_data_type)
    {
        case data_type::base16:
            data_to_print = std::unique_ptr<char>((char*)Base16::data_to_base16(output_data.get()));
            std::cout << data_to_print.get();
            break;
        case data_type::base64:
            data_to_print = std::unique_ptr<char>((char*)Base64::data_to_base64(output_data.get()));
            std::cout << data_to_print.get();
            break;
        default:
            std::cerr << "Invalid or missing output data type.\n" << parameters.output_data_type;
            return 1;
    }

    return 0;
}

void display_help()
{
    std::cout << "\n"
                 "Usage: encryptor [OPTIONS]\n"
                 "\n"
                 "Options:\n"
                 "  Action:\n"
                 "    --encrypt\n"
                 "    --decrypt\n"
                 "  Input:\n"
                 "    --input_data_type <data_type>\n"
                 "    --input (<data> | <filename>)\n"
                 "  Output:\n"
                 "    --output_data_type <data_type>\n"
                 "    --output (<data> | <filename>)\n"
                 "  Algorithm:\n"
                 "    --algorithm <algorithm>\n"
                 "  Key:\n"
                 "    --key_data_type <data_type>\n"
                 "    --key <data>\n"
                 "  IV:\n"
                 "    --iv_data_type <data_type>\n"
                 "    --iv <data>\n"
                 "  Block cipher mode:\n"
                 "    --block_cipher_mode <block_cipher_mode>\n"
                 "\n"
                 "<data_type>:\n"
                 "  base16\n"
                 "  base64\n"
                 "  file\n"
                 "<algorithm>:\n"
                 "  aes128\n"
                 "  aes192\n"
                 "  aes256\n"
                 "<block_cipher_mode>:\n"
                 "  cbc\n"
                 "\n"
                 "Help:\n"
                 "  --help\n"
                 "\n";
}
