#include "Main.h"

#include "Base64.h"
#include "Base16.h"
#include "Array.h"
#include "AES.h"

#include <iostream>

struct
{
    std::string program_name;
    
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
        std::cerr << "An error occured during command line argument parsing." << "\r\n";
        return 1;
    }
    
    //Display help
    if (argc == 1 || parameters.help)
    {
        display_help(parameters.program_name);
        return 0;
    }
    
    //Run action and exit program
    return run_action();
}

void display_help(std::string program_name)
{
    std::cout << "\r\n"
              << "Usage: " << program_name << " input output action algorithm" << "\r\n"
              << "\r\n"
              << "Options:" << "\r\n"
              << "    input:" << "\r\n"
              << "        -i b16 data_in_base16" << "\r\n"
              << "        -i base16 data_in_base16" << "\r\n"
              << "        --input b16 data_in_base16" << "\r\n"
              << "        --input base16 data_in_base16" << "\r\n"
              << "            Input data provided in base16 format" << "\r\n"
              << "        -i b64 data_in_base64" << "\r\n"
              << "        -i base64 data_in_base64" << "\r\n"
              << "        --input b64 data_in_base64" << "\r\n"
              << "        --input base64 data_in_base64" << "\r\n"
              << "            Input data provided in base64 format" << "\r\n"
              << "        -i f filename" << "\r\n"
              << "        -i file filename" << "\r\n"
              << "        --input f filename" << "\r\n"
              << "        --input file filename" << "\r\n"
              << "            Input data provided in specified file" << "\r\n"
              << "    output:" << "\r\n"
              << "        -o b16" << "\r\n"
              << "        -o base16" << "\r\n"
              << "        --output b16" << "\r\n"
              << "        --output base16" << "\r\n"
              << "            Print output data in base16 format" << "\r\n"
              << "        -o b64" << "\r\n"
              << "        -o base64" << "\r\n"
              << "        --output b64" << "\r\n"
              << "        --output base64" << "\r\n"
              << "            Print output data in base64 format" << "\r\n"
              << "        -o f filename" << "\r\n"
              << "        -o file filename" << "\r\n"
              << "        --output f filename" << "\r\n"
              << "        --output file filename" << "\r\n"
              << "            Write output data to specified file" << "\r\n"
              << "    action:" << "\r\n"
              << "        -e" << "\r\n"
              << "        --encrypt" << "\r\n"
              << "            Encrypt" << "\r\n"
              << "        -d" << "\r\n"
              << "        --decrypt" << "\r\n"
              << "            Decrypt" << "\r\n"
              << "    algorithm:" << "\r\n"
              << "        -a algorithm_with_parameters" << "\r\n"
              << "        --algorithm algorithm_with_parameters" << "\r\n"
              << "Supported algorithms:" << "\r\n"
              << "    AES: aes" << "\r\n"
              << "        Requires:  key_size block_cipher_mode key iv" << "\r\n"
              << "Parameters:" << "\r\n"
              << "    key_size:" << "\r\n"
              << "        -s size" << "\r\n"
              << "        --key_size size" << "\r\n"
              << "            size:" << "\r\n"
              << "                128" << "\r\n"
              << "                    128-bit key" << "\r\n"
              << "                192" << "\r\n"
              << "                    192-bit key" << "\r\n"
              << "                256" << "\r\n"
              << "                    256-bit key" << "\r\n"
              << "    block_cipher_mode:" << "\r\n"
              << "        -m mode" << "\r\n"
              << "        --block_cipher_mode mode" << "\r\n"
              << "            mode:" << "\r\n"
              << "                cbc" << "\r\n"
              << "                    Cipher block chaining" << "\r\n"
              << "    key:" << "\r\n"
              << "        -k b16 key_in_base16" << "\r\n"
              << "        -k base16 key_in_base16" << "\r\n"
              << "        --key b16 key_in_base16" << "\r\n"
              << "        --key base16 key_in_base16" << "\r\n"
              << "            Key provided in base16 format" << "\r\n"
              << "        -k b64 key_in_base64" << "\r\n"
              << "        -k base64 key_in_base64" << "\r\n"
              << "        --key b64 key_in_base64" << "\r\n"
              << "        --key base64 key_in_base64" << "\r\n"
              << "            Key provided in base64 format" << "\r\n"
              << "    iv:" << "\r\n"
              << "        -v b16 iv_in_base16" << "\r\n"
              << "        -v base16 iv_in_base16" << "\r\n"
              << "        --iv b16 iv_in_base16" << "\r\n"
              << "        --iv base16 iv_in_base16" << "\r\n"
              << "            IV provided in base16 format" << "\r\n"
              << "        -v b64 iv_in_base64" << "\r\n"
              << "        -v base64 iv_in_base64" << "\r\n"
              << "        --iv b64 iv_in_base64" << "\r\n"
              << "        --iv base64 iv_in_base64" << "\r\n"
              << "            IV provided in base64 format" << "\r\n"
              << "\r\n";
}

int parse_args(int argc, char** argv)
{
    //Program name
    parameters.program_name = argv[0];
    
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
                    std::cerr << "Unsupported input type: '" << input_type << "'." << "\r\n";
                    return 1;
                }
                
                if (input_data.length())
                    parameters.input_data = input_data;
                else
                {
                    std::cerr << "Input data empty." << "\r\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete input argument." << "\r\n";
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
                            std::cerr << "Output data empty." << "\r\n";
                            return 1;
                        }
                    }
                    else
                    {
                        std::cerr << "Incomplete output argument." << "\r\n";
                        return 1;
                    }
                }
                else
                {
                    std::cerr << "Unsupported output type: '" << output_type << "'." << "\r\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete output argument." << "\r\n";
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
                    std::cerr << "Unsupported algorithm: '" << algorithm << "'." << "\r\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete algorithm argument." << "\r\n";
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
                    std::cerr << "Unsupported key size: '" << key_size << "'." << "\r\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete key size argument." << "\r\n";
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
                    std::cerr << "Unsupported block cipher mode: '" << block_cipher_mode << "'."
                              << "\r\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete block cipher mode argument." << "\r\n";
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
                    std::cerr << "Unsupported key type: '" << key_type << "'." << "\r\n";
                    return 1;
                }
                
                if (key_data.length())
                    parameters.key_data = key_data;
                else
                {
                    std::cerr << "Key data empty." << "\r\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete key argument." << "\r\n";
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
                    std::cerr << "Unsupported IV type: '" << iv_type << "'." << "\r\n";
                    return 1;
                }
                
                if (iv_data.length())
                    parameters.iv_data = iv_data;
                else
                {
                    std::cerr << "IV data empty." << "\r\n";
                    return 1;
                }
            }
            else
            {
                std::cerr << "Incomplete IV argument." << "\r\n";
                return 1;
            }
        }
        else
        {
            std::cerr << "Wrong argument: " << arg << "\r\n";
            return 1;
        }
    }
    
    //Check requirements
    
    if (!parameters.input_type.length() && !parameters.input_data.length())
    {
        std::cerr << "Input not specified." << "\r\n";
        return 1;
    }
    
    if (!parameters.output_type.length())
    {
        std::cerr << "Output not specified." << "\r\n";
        return 1;
    }
    
    if (!parameters.action.length())
    {
        std::cerr << "Action not specified." << "\r\n";
        return 1;
    }
    
    if (!parameters.algorithm.length())
    {
        std::cerr << "Algorithm not specified." << "\r\n";
        return 1;
    }
    
    if (parameters.algorithm == "aes")
    {
        if (!parameters.key_size.length())
        {
            std::cerr << "AES requires key size." << "\r\n";
            return 1;
        }
        if (!parameters.block_cipher_mode.length())
        {
            std::cerr << "AES requires block cipher mode." << "\r\n";
            return 1;
        }
        if (!parameters.key_type.length() && !parameters.key_data.length())
        {
            std::cerr << "AES requires key." << "\r\n";
            return 1;
        }
        if (!parameters.iv_type.length() && !parameters.iv_data.length())
        {
            std::cerr << "AES requires IV." << "\r\n";
            return 1;
        }
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
            std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
        }
        
        Array* key = NULL;
        
        if (parameters.key_type == "base16")
        {
            key = Base16::base16_to_data((char*)parameters.key_data.c_str());
        }
        else if (parameters.key_type == "base64")
        {
            key = Base64::base64_to_data((char*)parameters.key_data.c_str());
        }
        else
        {
            //This should not happen
            std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
        }
        
        if (aes->set_key(key))
        {
            delete key;
            std::cerr << "Invalid key." << "\r\n";
            return 1;
        }
        delete key;
        
        Array* iv = NULL;
        
        if (parameters.iv_type == "base16")
        {
            iv = Base16::base16_to_data((char*)parameters.iv_data.c_str());
        }
        else if (parameters.iv_type == "base64")
        {
            iv = Base64::base64_to_data((char*)parameters.iv_data.c_str());
        }
        else
        {
            //This should not happen
            std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
        }
        
        if (aes->set_IV(iv))
        {
            delete iv;
            std::cerr << "Invalid IV." << "\r\n";
            return 1;
        }
        
        Array* data = NULL;
        
        if (parameters.input_type == "base16")
        {
            data = Base16::base16_to_data((char*)parameters.input_data.c_str());
        }
        else if (parameters.input_type == "base64")
        {
            data = Base64::base64_to_data((char*)parameters.input_data.c_str());
        }
        else if (parameters.input_type == "file")
        {
            std::cerr << "Not yet implemented." << "\r\n";
            return 1;
        }
        else
        {
            //This should not happen
            std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
        }
        
        if (parameters.block_cipher_mode == "cbc")
        {
            if (parameters.action == "encrypt")
            {
                aes->encrypt_CBC(data);
            }
            else if (parameters.action == "decrypt")
            {
                aes->decrypt_CBC(data);
            }
            else
            {
                //This should not happen
                std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
            }
        }
        else
        {
            //This should not happen
            std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
        }
        
        if (parameters.output_type == "base16")
        {
            std::cout << Base16::data_to_base16(data) << "\r\n";
        }
        else if (parameters.output_type == "base64")
        {
            std::cout << Base64::data_to_base64(data) << "\r\n";
        }
        else if (parameters.output_type == "file")
        {
            std::cerr << "Not yet implemented." << "\r\n";
            return 1;
        }
        else
        {
            //This should not happen
            std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
        }
        
        delete data;
        delete aes;
    }
    else
    {
        //This should not happen
        std::cerr << "Illegal state. File " << __FILE__ << ", line " << __LINE__ << "\r\n";
    }
    
    return 0;
}
