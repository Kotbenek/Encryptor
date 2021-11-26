#include "Main.h"

#include "Base64.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    //Debug
    cout << "f      " << Base64::data_to_base64("f", 1) << "\r\n";
    cout << "fo     " << Base64::data_to_base64("fo", 2) << "\r\n";
    cout << "foo    " << Base64::data_to_base64("foo", 3) << "\r\n";
    cout << "foob   " << Base64::data_to_base64("foob", 4) << "\r\n";
    cout << "fooba  " << Base64::data_to_base64("fooba", 5) << "\r\n";
    cout << "foobar " << Base64::data_to_base64("foobar", 6) << "\r\n";
    
    //Exit program
    return 0;
}
