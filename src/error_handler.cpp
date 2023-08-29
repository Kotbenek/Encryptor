#include "error_handler.h"

#include <iostream>

void illegal_state_handler(std::string file, int line)
{
    std::cerr << std::string("Illegal state. File ") + std::string(file) + std::string(", line ") +
                     std::to_string(line) + "\n";
    std::exit(1);
}
