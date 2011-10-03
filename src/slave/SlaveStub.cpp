#include "SlaveStub.h"

#include <gtk/gtk.h>

//==================================================================================================
SlaveStub *SlaveStub::s_this;
//==================================================================================================
SlaveStub::SlaveStub()
: _main(boost::this_thread::get_id())
{
    s_this = this;
}
//--------------------------------------------------------------------------------------------------
SlaveStub::~SlaveStub()
{}
//--------------------------------------------------------------------------------------------------
void SlaveStub::onMessage(Data const &response)
{
    //just some common response has arrived for stub, must not emit any signals on the current thread
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::onSignal(Data const &response)
{
    Data request;

    router()->beginSlot(response);

    switch (response.packet()->id)
    {
        case Packet4Id :

            processSlot1(response, request);

            break;

        case Packet5Id :

            processSlot2(response, request);

            break;
    }

    router()->endSlot(request);
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::onStart()
{

}
//--------------------------------------------------------------------------------------------------
void SlaveStub::onStop()
{
    ::gtk_main_quit();
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::slave_signal_1(int a, int b)
{
    Signal1 packet;
    size_t size = sizeof(Signal1); //should be serialized size

    packet.a = a;
    packet.b = b;

    //serialize if needed

    Data request(&packet, size);
    Data response;

    s_this->router()->signal(request, response);

    //process response if needed
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::slave_signal_2(char *c)
{
    Signal2 packet;
    size_t size = sizeof(Signal2); //should be serialized size

    packet.c = c;

    //serialize if needed

    Data request(&packet, size);
    Data response;

    s_this->router()->signal(request, response);

    //process response if needed
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::slave_signal_3(double d)
{
    Signal3 packet;
    size_t size = sizeof(Signal3); //should be serialized size

    packet.d = d;

    //serialize if needed

    Data request(packet, size);
    Data response;

    s_this->router()->signal(request, response);

    //process response if needed
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::slave_call_1(int h, char *i)
{
    Packet6 packet;
    size_t size = sizeof(Packet6); //should be serialized size

    packet.h = h;
    packet.i = i;

    //serialize if needed

    Data request(packet, size);
    Data response;

    s_this->router()->signal(request, response);

    //process response if needed
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::processSlot1(Data const &response, Data &request)
{
    //deserialize response to packet
    //make a real call based on packet's data
    //init request to answer
}
//--------------------------------------------------------------------------------------------------
void SlaveStub::processSlot2(Data const &response, Data &request)
{
    //deserialize response to packet
    //make a real call based on packet's data
    //init request to answer
}
//==================================================================================================
