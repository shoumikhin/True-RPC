#pragma once

#include "Packet.h"

#include <cstddef>
#include <cstring>
#include <vector>

//==================================================================================================
struct Sync
{
mutable void *sender;
mutable void *receiver;
};
//==================================================================================================
struct Data : std::vector<char>
{
    Data()
    {}

    Data(size_t size)
    : std::vector<char>(size)
    {}

    Data(size_t size, char value)
    : std::vector<char>(size, value)
    {}

    Data(Packet const &packet, size_t size)
    {
        resize(size + sizeof(Sync));
        ::memcpy(&this->at(0) + sizeof(Sync), &packet, size);
        ::memset(&this->at(0), 0, sizeof(Sync));
    }

    Data(Packet const *packet, size_t size)
    {
        resize(size + sizeof(Sync));
        ::memcpy(&this->at(0) + sizeof(Sync), packet, size);
        ::memset(&this->at(0), 0, sizeof(Sync));
    }

    Packet const *packet() const
    {
        return reinterpret_cast<Packet const *>(&this->at(0) + sizeof(Sync));
    }

    Packet *packet()
    {
        return reinterpret_cast<Packet *>(&this->at(0) + sizeof(Sync));
    }
};
//==================================================================================================
