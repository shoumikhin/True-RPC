#pragma once

#include "../rpc/Stub.h"

#include "../common/Protocol.h"

//==================================================================================================
class MasterStub : public IStub
{
public :
    MasterStub();
    ~MasterStub();

    void response(Data const &response);
    void responseSync(Data const &response);
    void onStart();
    void onStop();

private :
static void signal_1(int a, int b);
static void signal_2(char *c);
static void signal_3(double d);

private :
    void emitSignal1(Signal1 &packet);
    void emitSignal2(Signal2 &packet);
    void emitSignal3(Signal3 &packet);
    void processSlot1(Slot1 &packet);
    void processSlot2(Slot2 &packet);
};
//==================================================================================================

