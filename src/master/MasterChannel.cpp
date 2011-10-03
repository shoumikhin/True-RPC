#include "MasterChannel.h"

#include <memory>
#include <fcntl.h>

//==================================================================================================
MasterChannel::MasterChannel(int readDescriptor, int writeDescriptor)
: _readDescriptor(readDescriptor)
, _writeDescriptor(writeDescriptor)
{}
//--------------------------------------------------------------------------------------------------
MasterChannel::~MasterChannel()
{}
//--------------------------------------------------------------------------------------------------
size_t MasterChannel::read(char **buffer, unsigned timeout)
{
    boost::mutex::scoped_lock l(_readMutex);

    fd_set readfds;
    struct timeval stimeout;

    stimeout.tv_sec = timeout / 1000;
    stimeout.tv_usec = timeout % 1000 * 1000;

    FD_ZERO(&readfds);
    FD_SET(_readDescriptor, &readfds);

    switch (::select(_readDescriptor + 1, &readfds, 0, 0, &stimeout))
    {
        case -1 :

            throw EChannelRead("select() failed!");

        case 0 :

            throw EChannelReadTimeout("read() timedout!");
    }

    ssize_t count;
    size_t amount = 0;

    count = ::read(_readDescriptor, &amount, sizeof(amount));

    if (count < 1)
        throw EChannelRead("read() failed!");

    if (sizeof(amount) != count)
        throw EChannelProtocol("Read less bytes then protocol specified!");

    std::auto_ptr<char> ret;

    try
    {
        ret.reset(new char[amount]);
    }
    catch (std::bad_alloc)
    {
        throw EChannelProtocol("Too big response!");
    }

    size_t remainder = amount;
    char *pointer = (char *)ret.get();

    for (; remainder; remainder -= count, pointer += count)
    {
        count = ::read(_readDescriptor, pointer, remainder);

        if (count < 1)
            throw EChannelRead("read() failed!");
    }

    *buffer = ret.release();

    return amount;
}
//--------------------------------------------------------------------------------------------------
size_t MasterChannel::read(char *buffer, size_t size, size_t *remainder, unsigned timeout)
{
    boost::mutex::scoped_lock l(_readMutex);

    fd_set readfds;
    struct timeval stimeout;

    stimeout.tv_sec = timeout / 1000;
    stimeout.tv_usec = timeout % 1000 * 1000;

    FD_ZERO(&readfds);
    FD_SET(_readDescriptor, &readfds);

    switch (::select(_readDescriptor + 1, &readfds, 0, 0, &stimeout))
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
        count = ::read(_readDescriptor, &amount, sizeof(amount));

        if (count < 1)
            throw EChannelRead("read() failed!");

        if (sizeof(amount) != count)
            throw EChannelProtocol("Read less bytes then protocol specified!");
    }

    if (amount < size)
        count = ::read(_readDescriptor, buffer, amount);
    else
        count = ::read(_readDescriptor, buffer, size);

    if (count < 1)
        throw EChannelRead("read() failed!");

    *remainder = amount - count;

    return count;
}
//--------------------------------------------------------------------------------------------------
size_t MasterChannel::write(char const *buffer, size_t size)
{
    boost::mutex::scoped_lock l(_writeMutex);

    int count = 0;
    size_t remainder = size;
    char *pointer = (char *)buffer;

    count = ::write(_writeDescriptor, &size, sizeof(size));

    if (count < 1)
        throw EChannelWrite("write() failed!");

    for (; remainder; remainder -= count, pointer += count)
    {
        count = ::write(_writeDescriptor, pointer, remainder);

        if (count < 1)
            throw EChannelWrite("write() failed!");
    }

    return count;
}
//==================================================================================================
