#pragma once

#include "../rpc/Stub.h"
#include "../common/Protocol.h"

#include <boost/utility.hpp>
#include <boost/thread.hpp>

//==================================================================================================
class SlaveStub : public IStub, boost::noncopyable
{
public :
    SlaveStub();
    ~SlaveStub();

    void onMessage(Data const &response);
    void onSignal(Data const &response);
    void onStart();
    void onStop();

private :
static void slave_signal_1(int a, int b);
static void slave_signal_2(char *c);
static void slave_signal_3(double d);
static void slave_call_1(int h, char *i);

private :
    void processSlot1(Data const &response, Data  &request);
    void processSlot2(Data  const &response, Data  &request);

private :
   boost::thread::id _main;

static SlaveStub *s_this;
};
//==================================================================================================

