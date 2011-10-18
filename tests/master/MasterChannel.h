#pragma once

#include "../rpc/Channel.h"

#include <boost/utility.hpp>
#include <boost/thread/mutex.hpp>

//==================================================================================================
class MasterChannel : public IChannel, boost::noncopyable
{
public :
    MasterChannel(char const *readFilename, char const *writeFilename);
    MasterChannel(int readDescriptor, int writeDescriptor);
    ~MasterChannel();

    size_t read(char **buffer, unsigned timeout);
    size_t read(char *buffer, size_t size, size_t *remainder, unsigned timeout);
    size_t write(char const *buffer, size_t size);

private :
    int _readDescriptor;
    int _writeDescriptor;
    boost::mutex _readMutex;
    boost::mutex _writeMutex;
};
//==================================================================================================
struct EMasterChannel : std::runtime_error
{
explicit
    EMasterChannel(std::string const &message)
    : runtime_error(message)
    {}
};
//==================================================================================================
struct EMasterChannelProtocol : EMasterChannel
{
explicit
    EMasterChannelProtocol(std::string const &message)
    : EMasterChannel(message)
    {}
};
//==================================================================================================
struct EMasterChannelRead : EMasterChannel
{
explicit
    EMasterChannelRead(std::string const &message)
    : EMasterChannel(message)
    {}
};
//==================================================================================================
struct EMasterChannelReadTimeout : EMasterChannelRead
{
explicit
    EMasterChannelReadTimeout(std::string const &message)
    : EMasterChannelRead(message)
    {}
};
//==================================================================================================
struct EMasterChannelWrite : EMasterChannel
{
explicit
    EMasterChannelWrite(std::string const &message)
    : EMasterChannel(message)
    {}
};
//==================================================================================================


