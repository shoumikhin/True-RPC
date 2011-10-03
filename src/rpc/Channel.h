#pragma once

#include "Data.h"

#include <memory>
#include <stdexcept>

//==================================================================================================
class IChannel
{
public :  //timeout should be in milliseconds
virtual void read(std::auto_ptr<Data> &buffer, unsigned const timeout) = 0;  //should throw EChannelProtocol on bad data, EChannelRead on bad channel, EChannelReadTimeout on timeout
virtual void read(Data &buffer, size_t *remainder, unsigned const timeout) = 0;  //should throw EChannelProtocol on bad data, EChannelRead on bad channel, EChannelReadTimeout on timeout
virtual void write(Data const &buffer) = 0;  //should throw EChannelWrite on bad channel
};
//==================================================================================================
struct EChannel : std::runtime_error
{
explicit
    EChannel(std::string const &message)
    : runtime_error(message)
    {}
};
//==================================================================================================
struct EChannelProtocol : EChannel
{
explicit
    EChannelProtocol(std::string const &message)
    : EChannel(message)
    {}
};
//==================================================================================================
struct EChannelRead : EChannel
{
explicit
    EChannelRead(std::string const &message)
    : EChannel(message)
    {}
};
//==================================================================================================
struct EChannelReadTimeout : EChannelRead
{
explicit
    EChannelReadTimeout(std::string const &message)
    : EChannelRead(message)
    {}
};
//==================================================================================================
struct EChannelWrite : EChannel
{
explicit
    EChannelWrite(std::string const &message)
    : EChannel(message)
    {}
};
//==================================================================================================
