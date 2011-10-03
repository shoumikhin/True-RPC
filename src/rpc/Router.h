#pragma once

#include "Stub.h"
#include "Channel.h"

//==================================================================================================
class IRouter : public IRequester
{
public :
    IRouter(IStub *stub)
    : _stub(stub)
    , _channel(0)
    {
        if (_stub)
            _stub->setRouter(this);
    }

virtual void start(IChannel *channel) = 0;
virtual void stop() = 0;

protected :
    IStub *_stub;
    IChannel *_channel;
};
//==================================================================================================
