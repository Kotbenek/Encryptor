#include "Main.h"

#include "Base64.h"
#include "Array.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    //Debug
    Array* a;

    a = new Array(1);
    a->data[0] = 'f';
    cout << "f      " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(2);
    a->data[0] = 'f';
    a->data[1] = 'o';
    cout << "fo     " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(3);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    cout << "foo    " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(4);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    cout << "foob   " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(5);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    a->data[4] = 'a';
    cout << "fooba  " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
    
    a = new Array(6);
    a->data[0] = 'f';
    a->data[1] = 'o';
    a->data[2] = 'o';
    a->data[3] = 'b';
    a->data[4] = 'a';
    a->data[5] = 'r';
    cout << "foobar " << Base64::data_to_base64(a) << "\r\n";
    a->~Array();
        
    a = Base64::base64_to_data("Zg==");
    cout << "Zg==     " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data("Zm8=");
    cout << "Zm8=     " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data("Zm9v");
    cout << "Zm9v     " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data("Zm9vYg==");
    cout << "Zm9vYg== " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data("Zm9vYmE=");
    cout << "Zm9vYmE= " << a->data << "\r\n";
    a->~Array();
    
    a = Base64::base64_to_data("Zm9vYmFy");
    cout << "Zm9vYmFy " << a->data << "\r\n";
    a->~Array();
    
    //Exit program
    return 0;
}
