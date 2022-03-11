#include "Main.h"

#include "Base64.h"
#include "Base16.h"
#include "Array.h"
#include "AES.h"

#include <iostream>

int main(int argc, char** argv)
{
    //Debug
    
    Array* a;
    
    std::cout << "Data -> Base64" << "\r\n";
    
    a = new Array(1);
    a->data[0] = 'f';
    std::cout << "f            " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(2);
    a->data[0] = 'f';
    a->data[1] = 'o';
    std::cout << "fo           " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(3);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    std::cout << "foo          " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(4);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    std::cout << "foob         " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(5);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    a->data[4] = 'a';
    std::cout << "fooba        " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(6);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    a->data[4] = 'a';
    a->data[5] = 'r';
    std::cout << "foobar       " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    std::cout << "\r\n" << "Base64 -> Data" << "\r\n";
        
    a = Base64::base64_to_data((char*)"Zg==");
    std::cout << "Zg==         " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data((char*)"Zm8=");
    std::cout << "Zm8=         " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data((char*)"Zm9v");
    std::cout << "Zm9v         " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data((char*)"Zm9vYg==");
    std::cout << "Zm9vYg==     " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data((char*)"Zm9vYmE=");
    std::cout << "Zm9vYmE=     " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data((char*)"Zm9vYmFy");
    std::cout << "Zm9vYmFy     " << a->data << "\r\n";
    a->~Array();
    
    std::cout << "\r\n" << "Data -> Base16" << "\r\n";
    
    a = new Array(1);
    a->data[0] = 'f';
    std::cout << "f            " << Base16::data_to_base16(a) << "\r\n";
    a->~Array();
    
    a = new Array(2);
    a->data[0] = 'f';
    a->data[1] = 'o';
    std::cout << "fo           " << Base16::data_to_base16(a) << "\r\n";
    a->~Array();
    
    a = new Array(3);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    std::cout << "foo          " << Base16::data_to_base16(a) << "\r\n";
    a->~Array();
    
    a = new Array(4);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    std::cout << "foob         " << Base16::data_to_base16(a) << "\r\n";
    a->~Array();
    
    a = new Array(5);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    a->data[4] = 'a';
    std::cout << "fooba        " << Base16::data_to_base16(a) << "\r\n";
    a->~Array();
    
    a = new Array(6);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    a->data[4] = 'a';
    a->data[5] = 'r';
    std::cout << "foobar       " << Base16::data_to_base16(a) << "\r\n";
    a->~Array();
    
    std::cout << "\r\n" << "Base16 -> Data" << "\r\n";
    
    a = Base16::base16_to_data((char*)"66");
    std::cout << "66           " << a->data << "\r\n";
    a->~Array();
    
    a = Base16::base16_to_data((char*)"666F");
    std::cout << "666F         " << a->data << "\r\n";
    a->~Array();
    
    a = Base16::base16_to_data((char*)"666F6F");
    std::cout << "666F6F       " << a->data << "\r\n";
    a->~Array();
    
    a = Base16::base16_to_data((char*)"666F6F62");
    std::cout << "666F6F62     " << a->data << "\r\n";
    a->~Array();
    
    a = Base16::base16_to_data((char*)"666F6F6261");
    std::cout << "666F6F6261   " << a->data << "\r\n";
    a->~Array();
    
    a = Base16::base16_to_data((char*)"666F6F626172");
    std::cout << "666F6F626172 " << a->data << "\r\n";
    a->~Array();
    
    
    AES* aes;
    
    std::cout << "\r\n" << "AES -> Encryption" << "\r\n";
    
    aes = new AES(AES::key_length::AES256);
    
    a = Base16::base16_to_data((char*)"603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4");
    aes->set_key(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"000102030405060708090A0B0C0D0E0F");
    aes->set_IV(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"6BC1BEE22E409F96E93D7E117393172A");
    aes->encrypt_CBC(a);
    std::cout << Base16::data_to_base16(a) << "\r\n";
    std::cout << "F58C4C04D6E5F1BA779EABFB5F7BFBD6" << "\r\n";
    delete a;
    
    std::cout << "\r\n" << "AES -> Decryption" << "\r\n";
    
    a = Base16::base16_to_data((char*)"603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4");
    aes->set_key(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"000102030405060708090A0B0C0D0E0F");
    aes->set_IV(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"F58C4C04D6E5F1BA779EABFB5F7BFBD6");
    aes->decrypt_CBC(a);
    std::cout << Base16::data_to_base16(a) << "\r\n";
    std::cout << "6BC1BEE22E409F96E93D7E117393172A" << "\r\n";
    delete a;
    
    std::cout << "\r\n" << "AES -> Encryption" << "\r\n";
    
    a = Base16::base16_to_data((char*)"603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4");
    aes->set_key(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"000102030405060708090A0B0C0D0E0F");
    aes->set_IV(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"6BC1BEE22E409F96E93D7E117393172AAE2D8A571E03AC9C9EB76FAC45AF8E5130C81C46A35CE411E5FBC1191A0A52EFF69F2445DF4F9B17AD2B417BE66C3710");
    aes->encrypt_CBC(a);
    std::cout << Base16::data_to_base16(a) << "\r\n";
    std::cout << "F58C4C04D6E5F1BA779EABFB5F7BFBD69CFC4E967EDB808D679F777BC6702C7D39F23369A9D9BACFA530E26304231461B2EB05E2C39BE9FCDA6C19078C6A9D1B" << "\r\n";
    delete a;
    
    std::cout << "\r\n" << "AES -> Decryption" << "\r\n";
    
    a = Base16::base16_to_data((char*)"603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4");
    aes->set_key(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"000102030405060708090A0B0C0D0E0F");
    aes->set_IV(a);
    delete a;
    
    a = Base16::base16_to_data((char*)"F58C4C04D6E5F1BA779EABFB5F7BFBD69CFC4E967EDB808D679F777BC6702C7D39F23369A9D9BACFA530E26304231461B2EB05E2C39BE9FCDA6C19078C6A9D1B");
    aes->decrypt_CBC(a);
    std::cout << Base16::data_to_base16(a) << "\r\n";
    std::cout << "6BC1BEE22E409F96E93D7E117393172AAE2D8A571E03AC9C9EB76FAC45AF8E5130C81C46A35CE411E5FBC1191A0A52EFF69F2445DF4F9B17AD2B417BE66C3710" << "\r\n";
    delete a;
    
    delete aes;
    
    //Exit program
    return 0;
}
