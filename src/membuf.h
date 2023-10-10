#pragma once

#include <cstdint>
#include <streambuf>

struct membuf : std::streambuf
{
    membuf(uint8_t* gbeg, uint8_t* gend)
    {
        this->setg((char*)gbeg, (char*)gbeg, (char*)gend);
    }
};
