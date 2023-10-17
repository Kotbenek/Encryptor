#include "Main.h"

#include "AES.h"
#include "Array.h"
#include "Base16.h"
#include "Base64.h"
#include "EncryptionAlgorithm.h"
#include "Padding.h"
#include "action.h"
#include "algorithm.h"
#include "block_cipher_mode.h"
#include "data_type.h"
#include "membuf.h"
#include "opt.h"

#include <fstream>
#include <getopt.h>
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        display_help();
        return 0;
    }

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
                return 0;
            case '?':
                //Unknown argument
                return 1;
            default:
                abort();
        }
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
        case data_type::not_set:
            break;
        default:
            std::cerr << "Invalid IV type.\n";
            return 1;
    }
    if (iv && algorithm->set_IV(iv.get()))
    {
        std::cerr << "Invalid IV.\n";
        return 1;
    }

    std::unique_ptr<Array> input_data;
    std::unique_ptr<membuf> input_stream_membuf;
    std::unique_ptr<std::istream> input_stream;
    switch (parameters.input_data_type)
    {
        case data_type::base16:
            input_data =
                std::unique_ptr<Array>(Base16::base16_to_data((char*)parameters.input.c_str()));
            input_stream_membuf = std::unique_ptr<membuf>(
                new membuf(&input_data->data[0], &input_data->data[input_data->size()]));
            input_stream =
                std::unique_ptr<std::istream>(new std::istream(input_stream_membuf.get()));
            break;
        case data_type::base64:
            input_data =
                std::unique_ptr<Array>(Base64::base64_to_data((char*)parameters.input.c_str()));
            input_stream_membuf = std::unique_ptr<membuf>(
                new membuf(&input_data->data[0], &input_data->data[input_data->size()]));
            input_stream =
                std::unique_ptr<std::istream>(new std::istream(input_stream_membuf.get()));
            break;
        case data_type::file:
            input_stream = std::unique_ptr<std::istream>(new std::ifstream(parameters.input));
            if (!*input_stream)
            {
                std::cerr << "Could not open file \"" << parameters.input << "\".\n";
                return 1;
            }
            break;
        case data_type::raw:
            input_data = std::unique_ptr<Array>(new Array(parameters.input));
            input_stream_membuf = std::unique_ptr<membuf>(
                new membuf(&input_data->data[0], &input_data->data[input_data->size()]));
            input_stream =
                std::unique_ptr<std::istream>(new std::istream(input_stream_membuf.get()));
            break;
        default:
            std::cerr << "Invalid or missing input data type.\n";
            return 1;
    }

    switch (parameters.block_cipher_mode)
    {
        case block_cipher_mode::ECB:
            break;
        case block_cipher_mode::CBC:
            if (!iv)
            {
                std::cerr << "Missing IV.\n";
                return 1;
            }
            break;
        default:
            abort();
    }
    if (algorithm->set_block_cipher_mode(parameters.block_cipher_mode))
    {
        std::cerr << "Invalid or missing block cipher mode.\n";
        return 1;
    }

    void (*action)(Array * data, EncryptionAlgorithm * algorithm) = NULL;
    void (*action_no_padding)(Array * data, EncryptionAlgorithm * algorithm) = NULL;
    switch (parameters.action)
    {
        case action::encrypt:
            action = action_encrypt;
            action_no_padding = action_encrypt_no_padding;
            break;
        case action::decrypt:
            action = action_decrypt;
            action_no_padding = action_decrypt_no_padding;
            break;
        default:
            std::cerr << "Invalid or missing action.\n";
            return 1;
    }

    std::unique_ptr<std::ostream> output_stream;
    void (*preprocess_and_write)(Array * data, std::ostream * stream) = NULL;
    switch (parameters.output_data_type)
    {
        case data_type::base16:
            output_stream = std::unique_ptr<std::ostream>(new std::ostream(std::cout.rdbuf()));
            preprocess_and_write = preprocess_and_write_base16;
            break;
        case data_type::base64:
            output_stream = std::unique_ptr<std::ostream>(new std::ostream(std::cout.rdbuf()));
            preprocess_and_write = preprocess_and_write_base64;
            break;
        case data_type::file:
            output_stream = std::unique_ptr<std::ostream>(new std::ofstream(parameters.output));
            if (!*output_stream)
            {
                std::cerr << "Could not open file \"" << parameters.output << "\".\n";
                return 1;
            }
            preprocess_and_write = preprocess_and_write_raw;
            break;
        case data_type::raw:
            output_stream = std::unique_ptr<std::ostream>(new std::ostream(std::cout.rdbuf()));
            preprocess_and_write = preprocess_and_write_raw;
            break;
        default:
            std::cerr << "Invalid or missing output data type.\n";
            return 1;
    }

    std::unique_ptr<Array> buffer(new Array(4096));
    input_stream->read((char*)buffer->data, buffer->size());
    std::streamsize gcount = input_stream->gcount();
    while (input_stream->peek() != EOF)
    {
        action_no_padding(buffer.get(), algorithm.get());
        preprocess_and_write(buffer.get(), output_stream.get());
        input_stream->read((char*)buffer->data, buffer->size());
        gcount = input_stream->gcount();
    }

    buffer->resize(gcount);
    action(buffer.get(), algorithm.get());
    preprocess_and_write(buffer.get(), output_stream.get());

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
                 "    [--output <filename>]\n"
                 "  Algorithm:\n"
                 "    --algorithm <algorithm>\n"
                 "  Key:\n"
                 "    --key_data_type <data_type: (base16 | base64)>\n"
                 "    --key <data>\n"
                 "  IV:\n"
                 "    --iv_data_type <data_type: (base16 | base64)>\n"
                 "    --iv <data>\n"
                 "  Block cipher mode:\n"
                 "    --block_cipher_mode <block_cipher_mode>\n"
                 "\n"
                 "<data_type>:\n"
                 "  base16\n"
                 "  base64\n"
                 "  file\n"
                 "  raw\n"
                 "<algorithm>:\n"
                 "  aes128\n"
                 "  aes192\n"
                 "  aes256\n"
                 "<block_cipher_mode>:\n"
                 "  ecb\n"
                 "  cbc\n"
                 "\n"
                 "Help:\n"
                 "  --help\n"
                 "\n";
}

void action_encrypt_no_padding(Array* data, EncryptionAlgorithm* algorithm)
{
    algorithm->encrypt(data);
}

void action_decrypt_no_padding(Array* data, EncryptionAlgorithm* algorithm)
{
    algorithm->decrypt(data);
}

void action_encrypt(Array* data, EncryptionAlgorithm* algorithm)
{
    Padding::PKCS7::append(data, algorithm->get_required_input_alignment());
    algorithm->encrypt(data);
}

void action_decrypt(Array* data, EncryptionAlgorithm* algorithm)
{
    algorithm->decrypt(data);
    if (!Padding::PKCS7::check(data))
    {
        std::cerr << "Padding corrupted - decryption not successful.\n";
        std::exit(1);
    }
    Padding::PKCS7::remove(data);
}

void preprocess_and_write_base16(Array* data, std::ostream* stream)
{
    std::unique_ptr<char[]> preprocessed_data((char*)Base16::data_to_base16(data));
    *stream << preprocessed_data.get();
}

void preprocess_and_write_base64(Array* data, std::ostream* stream)
{
    std::unique_ptr<char[]> preprocessed_data((char*)Base64::data_to_base64(data));
    *stream << preprocessed_data.get();
}

void preprocess_and_write_raw(Array* data, std::ostream* stream)
{
    stream->write((char*)data->data, data->size());
}
