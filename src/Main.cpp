#include "Main.h"

#include "Base64.h"
#include "Base16.h"
#include "Array.h"
#include "AES.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::string program_name = argv[0];
    
    //Display help
    display_help(program_name);
    
    //Exit program
    return 0;
}

void display_help(std::string program_name)
{
    std::cout << "\r\n";
    std::cout << "Usage: " << program_name << " input output action algorithm" << "\r\n";
    std::cout << "\r\n";
    std::cout << "Options:" << "\r\n";
    std::cout << "    input:" << "\r\n";
    std::cout << "        -i b16 data_in_base16" << "\r\n";
    std::cout << "        --input base16 data_in_base16" << "\r\n";
    std::cout << "            Input data provided in base16 format" << "\r\n";
    std::cout << "        -i b64 data_in_base64" << "\r\n";
    std::cout << "        --input base64 data_in_base64" << "\r\n";
    std::cout << "            Input data provided in base64 format" << "\r\n";
    std::cout << "        -i f filename" << "\r\n";
    std::cout << "        --input file filename" << "\r\n";
    std::cout << "            Input data provided in specified file" << "\r\n";
    std::cout << "    output:" << "\r\n";
    std::cout << "        -o b16" << "\r\n";
    std::cout << "        --output base16" << "\r\n";
    std::cout << "            Print output data in base16 format" << "\r\n";
    std::cout << "        -o b64" << "\r\n";
    std::cout << "        --output base64" << "\r\n";
    std::cout << "            Print output data in base64 format" << "\r\n";
    std::cout << "        -o f filename" << "\r\n";
    std::cout << "        --output file filename" << "\r\n";
    std::cout << "            Write output data to specified file" << "\r\n";
    std::cout << "    action:" << "\r\n";
    std::cout << "        -e" << "\r\n";
    std::cout << "        --encrypt" << "\r\n";
    std::cout << "            Encrypt" << "\r\n";
    std::cout << "        -d" << "\r\n";
    std::cout << "        --decrypt" << "\r\n";
    std::cout << "            Decrypt" << "\r\n";
    std::cout << "    algorithm:" << "\r\n";
    std::cout << "        -a algorithm_with_parameters" << "\r\n";
    std::cout << "        --algorithm algorithm_with_parameters" << "\r\n";
    std::cout << "Supported algorithms:" << "\r\n";
    std::cout << "    AES: aes key_size block_cipher_mode key iv" << "\r\n";
    std::cout << "        key_size:" << "\r\n";
    std::cout << "            -s size" << "\r\n";
    std::cout << "            --key_size size" << "\r\n";
    std::cout << "                size:" << "\r\n";
    std::cout << "                    128" << "\r\n";
    std::cout << "                        128-bit key" << "\r\n";
    std::cout << "                    192" << "\r\n";
    std::cout << "                        192-bit key" << "\r\n";
    std::cout << "                    256" << "\r\n";
    std::cout << "                        256-bit key" << "\r\n";
    std::cout << "        block_cipher_mode:" << "\r\n";
    std::cout << "            -m mode" << "\r\n";
    std::cout << "            --block_cipher_mode mode" << "\r\n";
    std::cout << "                mode:" << "\r\n";
    std::cout << "                    cbc" << "\r\n";
    std::cout << "                        Cipher block chaining" << "\r\n";
    std::cout << "        key:" << "\r\n";
    std::cout << "            -k b16 key_in_base16" << "\r\n";
    std::cout << "            --key base16 key_in_base16" << "\r\n";
    std::cout << "                Key provided in base16 format" << "\r\n";
    std::cout << "            -k b64 key_in_base64" << "\r\n";
    std::cout << "            --key base64 key_in_base64" << "\r\n";
    std::cout << "                Key provided in base64 format" << "\r\n";
    std::cout << "        iv:" << "\r\n";
    std::cout << "            -v b16 iv_in_base16" << "\r\n";
    std::cout << "            --iv base16 iv_in_base16" << "\r\n";
    std::cout << "                IV provided in base16 format" << "\r\n";
    std::cout << "            -v b64 iv_in_base64" << "\r\n";
    std::cout << "            --iv base64 iv_in_base64" << "\r\n";
    std::cout << "                IV provided in base64 format" << "\r\n";
    std::cout << "\r\n";
}