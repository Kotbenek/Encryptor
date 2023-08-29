#include "Main.h"

#include "AES.h"
#include "Array.h"
#include "Base16.h"
#include "Base64.h"
#include "error_handler.h"

#include <iostream>

struct
{
    bool help = false;

    std::string input_type;
    std::string input_data;

    std::string output_type;
    std::string output_data;

    std::string action;

    std::string algorithm;

    std::string key_size;
    std::string block_cipher_mode;

    std::string key_type;
    std::string key_data;

    std::string iv_type;
    std::string iv_data;
} parameters;

int main(int argc, char** argv)
{
    //Parse command line arguments
    if (parse_args(argc, argv))
    {
        std::cerr << "An error occured during command line argument parsing.\n";
        return 1;
    }

    //Display help
    if (argc == 1 || parameters.help)
    {
        display_help();
        return 0;
    }

    //Run action and exit program
    return run_action();
}

void display_help()
{
    std::cout << "\n"
                 "Usage: encryptor input output action algorithm\n"
                 "\n"
                 "Options:\n"
                 "    input:\n"
                 "        -i b16 data_in_base16\n"
                 "        -i base16 data_in_base16\n"
                 "        --input b16 data_in_base16\n"
                 "        --input base16 data_in_base16\n"
                 "            Input data provided in base16 format\n"
                 "        -i b64 data_in_base64\n"
                 "        -i base64 data_in_base64\n"
                 "        --input b64 data_in_base64\n"
                 "        --input base64 data_in_base64\n"
                 "            Input data provided in base64 format\n"
                 "        -i f filename\n"
                 "        -i file filename\n"
                 "        --input f filename\n"
                 "        --input file filename\n"
                 "            Input data provided in specified file\n"
                 "    output:\n"
                 "        -o b16\n"
                 "        -o base16\n"
                 "        --output b16\n"
                 "        --output base16\n"
                 "            Print output data in base16 format\n"
                 "        -o b64\n"
                 "        -o base64\n"
                 "        --output b64\n"
                 "        --output base64\n"
                 "            Print output data in base64 format\n"
                 "        -o f filename\n"
                 "        -o file filename\n"
                 "        --output f filename\n"
                 "        --output file filename\n"
                 "            Write output data to specified file\n"
                 "    action:\n"
                 "        -e\n"
                 "        --encrypt\n"
                 "            Encrypt\n"
                 "        -d\n"
                 "        --decrypt\n"
                 "            Decrypt\n"
                 "    algorithm:\n"
                 "        -a algorithm_with_parameters\n"
                 "        --algorithm algorithm_with_parameters\n"
                 "Supported algorithms:\n"
                 "    AES: aes\n"
                 "        Requires:  key_size block_cipher_mode key iv\n"
                 "Parameters:\n"
                 "    key_size:\n"
                 "        -s size\n"
                 "        --key_size size\n"
                 "            size:\n"
                 "                128\n"
                 "                    128-bit key\n"
                 "                192\n"
                 "                    192-bit key\n"
                 "                256\n"
                 "                    256-bit key\n"
                 "    block_cipher_mode:\n"
                 "        -m mode\n"
                 "        --block_cipher_mode mode\n"
                 "            mode:\n"
                 "                cbc\n"
                 "                    Cipher block chaining\n"
                 "    key:\n"
                 "        -k b16 key_in_base16\n"
                 "        -k base16 key_in_base16\n"
                 "        --key b16 key_in_base16\n"
                 "        --key base16 key_in_base16\n"
                 "            Key provided in base16 format\n"
                 "        -k b64 key_in_base64\n"
                 "        -k base64 key_in_base64\n"
                 "        --key b64 key_in_base64\n"
                 "        --key base64 key_in_base64\n"
                 "            Key provided in base64 format\n"
                 "    iv:\n"
                 "        -v b16 iv_in_base16\n"
                 "        -v base16 iv_in_base16\n"
                 "        --iv b16 iv_in_base16\n"
                 "        --iv base16 iv_in_base16\n"
                 "            IV provided in base16 format\n"
                 "        -v b64 iv_in_base64\n"
                 "        -v base64 iv_in_base64\n"
                 "        --iv b64 iv_in_base64\n"
                 "        --iv base64 iv_in_base64\n"
                 "            IV provided in base64 format\n"
                 "\n";
}

int parse_args(int argc, char** argv)
{
    if (argc == 1)
        return 0;

    //Parse arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);

        if (arg == "-h" || arg == "--help")
        {
            parameters.help = true;
            return 0;
        }
        else if (arg == "-i" || arg == "--input")
        {
            if (check_if_enough_parameters_available(argc, i, 2))
            {
                std::string input_type(argv[++i]);
                std::string input_data(argv[++i]);

                if (input_type == "b16" || input_type == "base16")
                    parameters.input_type = "base16";
                else if (input_type == "b64" || input_type == "base64")
                    parameters.input_type = "base64";
                else if (input_type == "f" || input_type == "file")
                    parameters.input_type = "file";
                else
                {
                    std::cerr << "Unsupported input type: '" << input_type << "'.\n";
                    return 1;
                }

                if (input_data.length())
                    parameters.input_data = input_data;
                else
                {
                    std::cerr << "Input data empty.\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete input argument.\n";
                return 1;
            }
        }
        else if (arg == "-o" || arg == "--output")
        {
            if (check_if_enough_parameters_available(argc, i, 1))
            {
                std::string output_type(argv[++i]);

                if (output_type == "b16" || output_type == "base16")
                    parameters.output_type = "base16";
                else if (output_type == "b64" || output_type == "base64")
                    parameters.output_type = "base64";
                else if (output_type == "f" || output_type == "file")
                {
                    if (check_if_enough_parameters_available(argc, i, 1))
                    {
                        std::string output_data(argv[++i]);

                        parameters.output_type = "file";

                        if (output_data.length())
                            parameters.output_data = output_data;
                        else
                        {
                            std::cerr << "Output data empty.\n";
                            return 1;
                        }
                    }
                    else
                    {
                        std::cerr << "Incomplete output argument.\n";
                        return 1;
                    }
                }
                else
                {
                    std::cerr << "Unsupported output type: '" << output_type << "'.\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete output argument.\n";
                return 1;
            }
        }
        else if (arg == "-e" || arg == "--encrypt")
            parameters.action = "encrypt";
        else if (arg == "-d" || arg == "--decrypt")
            parameters.action = "decrypt";
        else if (arg == "-a" || arg == "--algorithm")
        {
            if (check_if_enough_parameters_available(argc, i, 1))
            {
                std::string algorithm(argv[++i]);

                if (algorithm == "aes")
                    parameters.algorithm = "aes";
                else
                {
                    std::cerr << "Unsupported algorithm: '" << algorithm << "'.\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete algorithm argument.\n";
                return 1;
            }
        }
        else if (arg == "-s" || arg == "--key_size")
        {
            if (check_if_enough_parameters_available(argc, i, 1))
            {
                std::string key_size(argv[++i]);

                if (key_size == "128")
                    parameters.key_size = "128";
                else if (key_size == "192")
                    parameters.key_size = "192";
                else if (key_size == "256")
                    parameters.key_size = "256";
                else
                {
                    std::cerr << "Unsupported key size: '" << key_size << "'.\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete key size argument.\n";
                return 1;
            }
        }
        else if (arg == "-m" || arg == "--block_cipher_mode")
        {
            if (check_if_enough_parameters_available(argc, i, 1))
            {
                std::string block_cipher_mode(argv[++i]);

                if (block_cipher_mode == "cbc")
                    parameters.block_cipher_mode = "cbc";
                else
                {
                    std::cerr << "Unsupported block cipher mode: '" << block_cipher_mode << "'.\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete block cipher mode argument.\n";
                return 1;
            }
        }
        else if (arg == "-k" || arg == "--key")
        {
            if (check_if_enough_parameters_available(argc, i, 2))
            {
                std::string key_type(argv[++i]);
                std::string key_data(argv[++i]);

                if (key_type == "b16" || key_type == "base16")
                    parameters.key_type = "base16";
                else if (key_type == "b64" || key_type == "base64")
                    parameters.key_type = "base64";
                else
                {
                    std::cerr << "Unsupported key type: '" << key_type << "'.\n";
                    return 1;
                }

                if (key_data.length())
                    parameters.key_data = key_data;
                else
                {
                    std::cerr << "Key data empty.\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete key argument.\n";
                return 1;
            }
        }
        else if (arg == "-v" || arg == "--iv")
        {
            if (check_if_enough_parameters_available(argc, i, 2))
            {
                std::string iv_type(argv[++i]);
                std::string iv_data(argv[++i]);

                if (iv_type == "b16" || iv_type == "base16")
                    parameters.iv_type = "base16";
                else if (iv_type == "b64" || iv_type == "base64")
                    parameters.iv_type = "base64";
                else
                {
                    std::cerr << "Unsupported IV type: '" << iv_type << "'.\n";
                    return 1;
                }

                if (iv_data.length())
                    parameters.iv_data = iv_data;
                else
                {
                    std::cerr << "IV data empty.\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete IV argument.\n";
                return 1;
            }
        }
        else
        {
            std::cerr << "Wrong argument: " << arg << "\n";
            return 1;
        }
    }

    //Check requirements

    if (!parameters.input_type.length() && !parameters.input_data.length())
    {
        std::cerr << "Input not specified.\n";
        return 1;
    }

    if (!parameters.output_type.length())
    {
        std::cerr << "Output not specified.\n";
        return 1;
    }

    if (!parameters.action.length())
    {
        std::cerr << "Action not specified.\n";
        return 1;
    }

    if (!parameters.algorithm.length())
    {
        std::cerr << "Algorithm not specified.\n";
        return 1;
    }

    if (parameters.algorithm == "aes")
    {
        if (!parameters.key_size.length())
        {
            std::cerr << "AES requires key size.\n";
            return 1;
        }
        if (!parameters.block_cipher_mode.length())
        {
            std::cerr << "AES requires block cipher mode.\n";
            return 1;
        }
        if (!parameters.key_type.length() && !parameters.key_data.length())
        {
            std::cerr << "AES requires key.\n";
            return 1;
        }
        if (!parameters.iv_type.length() && !parameters.iv_data.length())
        {
            std::cerr << "AES requires IV.\n";
            return 1;
        }
    }

    if (parameters.input_type == "file" && parameters.output_type != "file")
    {
        std::cerr << "File input requires file output.";
        return 1;
    }

    if (parameters.output_type == "file" && parameters.input_type != "file")
    {
        std::cerr << "File output requires file input.";
        return 1;
    }

    return 0;
}

bool check_if_enough_parameters_available(int argc, int current_index, int parameters_required)
{
    return (argc - current_index - 1 >= parameters_required);
}

int run_action()
{
    if (parameters.algorithm == "aes")
    {
        AES* aes = NULL;

        if (parameters.key_size == "128")
            aes = new AES(AES::key_length::AES128);
        else if (parameters.key_size == "192")
            aes = new AES(AES::key_length::AES192);
        else if (parameters.key_size == "256")
            aes = new AES(AES::key_length::AES256);
        else
        {
            //This should not happen
            illegal_state_handler(__FILE__, __LINE__);
        }

        Array* key = NULL;

        if (parameters.key_type == "base16")
            key = Base16::base16_to_data((char*)parameters.key_data.c_str());
        else if (parameters.key_type == "base64")
            key = Base64::base64_to_data((char*)parameters.key_data.c_str());
        else
        {
            //This should not happen
            illegal_state_handler(__FILE__, __LINE__);
        }

        if (!key || aes->set_key(key))
        {
            delete key;
            std::cerr << "Invalid key.\n";
            return 1;
        }
        delete key;

        Array* iv = NULL;

        if (parameters.iv_type == "base16")
            iv = Base16::base16_to_data((char*)parameters.iv_data.c_str());
        else if (parameters.iv_type == "base64")
            iv = Base64::base64_to_data((char*)parameters.iv_data.c_str());
        else
        {
            //This should not happen
            illegal_state_handler(__FILE__, __LINE__);
        }

        if (!iv || aes->set_IV(iv))
        {
            delete iv;
            std::cerr << "Invalid IV.\n";
            return 1;
        }

        Array* data = NULL;
        Array* processed_data = NULL;

        if (parameters.input_type == "base16")
            data = Base16::base16_to_data((char*)parameters.input_data.c_str());
        else if (parameters.input_type == "base64")
            data = Base64::base64_to_data((char*)parameters.input_data.c_str());
        else if (parameters.input_type == "file")
        {
            //No additional action required
        }
        else
        {
            //This should not happen
            illegal_state_handler(__FILE__, __LINE__);
        }

        if (parameters.block_cipher_mode == "cbc")
        {
            if (parameters.action == "encrypt")
            {
                if (parameters.input_type != "file")
                    processed_data = aes->encrypt_CBC_PKCS7(data);
                else
                    aes->encrypt_file_CBC_PKCS7(parameters.input_data, parameters.output_data);
            }
            else if (parameters.action == "decrypt")
            {
                if (parameters.input_type != "file")
                    processed_data = aes->decrypt_CBC_PKCS7(data);
                else
                    aes->decrypt_file_CBC_PKCS7(parameters.input_data, parameters.output_data);
            }
            else
            {
                //This should not happen
                illegal_state_handler(__FILE__, __LINE__);
            }
        }
        else
        {
            //This should not happen
            illegal_state_handler(__FILE__, __LINE__);
        }

        if (parameters.output_type == "base16")
            std::cout << Base16::data_to_base16(processed_data);
        else if (parameters.output_type == "base64")
            std::cout << Base64::data_to_base64(processed_data);
        else if (parameters.output_type == "file")
        {
            //No additional action required
        }
        else
        {
            //This should not happen
            illegal_state_handler(__FILE__, __LINE__);
        }

        delete processed_data;
        delete data;
        delete aes;
    }
    else
    {
        //This should not happen
        illegal_state_handler(__FILE__, __LINE__);
    }

    return 0;
}
