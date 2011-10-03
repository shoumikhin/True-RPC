#pragma once

#include "../rpc/Channel.h"

#include <boost/utility.hpp>
#include <boost/thread/mutex.hpp>

//==================================================================================================
class SlaveChannel : public IChannel, boost::noncopyable
{
public :
    typedef FILE * FileHandle;

public :
    SlaveChannel(FileHandle &input, FileHandle &output)
    : _input(input)
    , _output(output)
    {}

    void read(std::auto_ptr<Data> &buffer, unsigned const timeout);
    void read(Data &buffer, size_t *remainder, unsigned const timeout);
    void write(Data const &buffer);

private :
    FileHandle _input;
    FileHandle _output;
    boost::mutex _readMutex;
    boost::mutex _writeMutex;
};
//==================================================================================================


