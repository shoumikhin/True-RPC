#define BOOST_TEST_MODULE TestMaster
#include <boost/test/unit_test.hpp>

#include <gtk/gth.h>
#include <msgpack.hpp>

#include <truerpc.h>
#include "UnixLocalChannel.h"

/*
   PART#1    |     master       |        TrueRPC        |       slave      |    PART#2
-------------+------------------+-----------------------+------------------+---------------
first()
{
           ===> first_signal() ===> call("first_slot") ===> first_slot() ===> first()
                                                                              {
  second() <=== second_slot() <=== call("second_slot") <=== second_signal() <=== second()
  {
    third() ===> third_signal() ===> call("third_slot") ===> third_slot() ===>     third()
                                                                                   {
                                                                                   }
            <--- third_signal() <---      reply()       <--- third_slot() <---
  }
            ---> second_slot()  --->      reply()       ---> second_signal() --->
                                                                              }
            <--- first_signal() <---      reply()       <--- first_slot() <---
}
*/

truerpc::server *rpc;

//==============================================================================
void second(char *string)
{
    third_signal();
}
//==============================================================================
int first_signal(int integer)
{
    //serialization
    msgpack::sbuffer buffer;

    msgpack::pack(buffer, integer);

    truerpc::serialized request(buffer.data(), buffer.size());
    truerpc::serialized response;

    //call
    response = rpc->call("first_slot", request);

    //deserialization
    msgpack::unpacked unpacked;
    msgpack::unpack(&unpacked, response.data(), response.size());

    int ret;

    unpacked.get().convert(&ret);

    return ret;
}
//==============================================================================
truerpc::serialized second_slot(truerpc::serialized const &request)
{
    //deserialization
    msgpack::unpacked unpacked;
    msgpack::unpack(&unpacked, request.data(), request.size());

    char *string;

    unpacked.get().convert(&string);

    //real call
    second(string);

    //serialization
    truerpc::serialized response;

    return response;
}
//==============================================================================
void third_signal()
{
    truerpc::serialized request;
    truerpc::serialized response;

    rpc->call("third_slot", request, response);
}
//==============================================================================
void call_on_main_thread(char const *name, truerpc::serialized const *request)
{
    if (!::strcmp("second_slot", name))
        g_idle_add_full(GTK_PRIORITY_HIGH, second_slot, request, 0);
}
//==============================================================================
BOOST_AUTO_TEST_CASE(RPC_run)
{
    truerpc::server rpc;

    rpc.subscribe("second_slot", second_slot);

    UnixLocalChannel channel("/tmp/truerpc-test");

    rpc.run(channel);
}
//==============================================================================
BOOST_AUTO_TEST_CASE(RPC_start)
{
    truerpc::server rpc;

    rpc.subscribe("second_slot", second_slot);

    UnixLocalChannel channel("/tmp/truerpc-test");

    rpc.start(channel, call_on_main_thread);

    ::gtk_main();

    rpc.stop();
}
//==============================================================================
