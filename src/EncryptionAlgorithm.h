#pragma once

#include "Array.h"

#include <iostream>
#include <string>

class EncryptionAlgorithm
{
public:
    virtual uint8_t set_key(Array* K)
    {
        not_implemented();
        std::exit(1);
    }

    virtual uint8_t set_IV(Array* iv)
    {
        not_implemented();
        std::exit(1);
    }

    virtual uint8_t encrypt_file_CBC_PKCS7(std::string file_in, std::string file_out)
    {
        not_implemented();
        std::exit(1);
    }

    virtual uint8_t decrypt_file_CBC_PKCS7(std::string file_in, std::string file_out)
    {
        not_implemented();
        std::exit(1);
    }

    virtual Array* encrypt_CBC_PKCS7(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

    virtual Array* decrypt_CBC_PKCS7(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

    virtual uint8_t encrypt_CBC(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

    virtual uint8_t decrypt_CBC(Array* data)
    {
        not_implemented();
        std::exit(1);
    }

    virtual uint8_t get_required_input_alignment()
    {
        not_implemented();
        std::exit(1);
    }

    virtual ~EncryptionAlgorithm() {}

private:
    void not_implemented(void)
    {
        std::cerr << "Not implemented\n";
    }
};
