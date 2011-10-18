#include "SlaveChannel.h"

#include <cstdio>
#include <fcntl.h>

//==================================================================================================
void SlaveChannel::read(std::auto_ptr<Data> &buffer, unsigned const timeout)
{
    boost::mutex::scoped_lock l(_readMutex);

    fd_set readfds;
    struct timeval stimeout;

    stimeout.tv_sec = timeout / 1000;
    stimeout.tv_usec = timeout % 1000 * 1000;

    FD_ZERO(&readfds);
    FD_SET(::fileno(_input), &readfds);

    switch (::select(::fileno(_input) + 1, &readfds, 0, 0, &stimeout))
    {
        case -1 :

            throw EChannelRead("select() failed!");

        case 0 :

            throw EChannelReadTimeout("read() timedout!");
    }

    ssize_t count;
    size_t amount = 0;

    count = ::read(::fileno(_input), &amount, sizeof(amount));

    if (count < 1)
        throw EChannelRead("read() failed!");

    if (sizeof(amount) != count)
        throw EChannelProtocol("Read less bytes then protocol specified!");

    try
    {
        buffer.reset(new Data(amount));
    }
    catch (std::bad_alloc)
    {
        throw EChannelProtocol("Too big response!");
    }

    size_t remainder = amount;
    char *pointer = &buffer->at(0);

    for (; remainder; remainder -= count, pointer += count)
    {
        count = ::read(::fileno(_input), pointer, remainder);

        if (count < 1)
            throw EChannelRead("read() failed!");
    }
}
//--------------------------------------------------------------------------------------------------
void SlaveChannel::read(Data &buffer, size_t *remainder, unsigned const timeout)
{
    boost::mutex::scoped_lock l(_readMutex);

    fd_set readfds;
    struct timeval stimeout;

    stimeout.tv_sec = timeout / 1000;
    stimeout.tv_usec = timeout % 1000 * 1000;

    FD_ZERO(&readfds);
    FD_SET(::fileno(_input), &readfds);

    switch (::select(::fileno(_input) + 1, &readfds, 0, 0, &stimeout))
    {
        case -1 :

            throw EChannelRead("select() failed!");

        case 0 :

            throw EChannelReadTimeout("read() timedout!");
    }

    ssize_t count;
    size_t amount = 0;

    if (*remainder)
        amount = *remainder;
    else
    {
        count = ::read(::fileno(_input), &amount, sizeof(amount));

        if (count < 1)
            throw EChannelRead("read() failed!");

        if (sizeof(amount) != count)
            throw EChannelProtocol("Read less bytes then protocol specified!");
    }

    if (amount < buffer.size())
        count = ::read(::fileno(_input), &buffer[0], amount);
    else
        count = ::read(::fileno(_input), &buffer[0], buffer.size());

    if (count < 1)
        throw EChannelRead("read() failed!");

    *remainder = amount - count;
}
//--------------------------------------------------------------------------------------------------
void SlaveChannel::write(Data const &buffer)
{
    boost::mutex::scoped_lock l(_writeMutex);

    int count = 0;
    size_t remainder = buffer.size();
    char const *pointer = &buffer.at(0);

    count = ::write(::fileno(_output), &remainder, sizeof(remainder));

    if (count < 1)
        throw EChannelWrite("write() failed!");

    for (; remainder; remainder -= count, pointer += count)
    {
        count = ::write(::fileno(_output), pointer, remainder);

        if (count < 1)
            throw EChannelWrite("write() failed!");
    }
}
//==================================================================================================
