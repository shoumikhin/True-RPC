#include <unittest++/UnitTest++.h>

#include "slave/SlaveStub.h"
#include "slave/SlaveChannel.h"
#include "truerpc/TrueRPC.h"

#include <gtk/gtk.h>

//==================================================================================================
TEST(SlaveGTK)
{
    try
    {
        SlaveChannel channel(::stdin, ::stdout);
        SlaveStub stub;
        RPC rpc(&stub);

        try
        {
            rpc.start(&channel);

            ::gtk_main();

            rpc.stop();
        }
        catch (ERPC)
        {

        }
    }
    catch (std::exception &e)
    {

    }
}
//==================================================================================================
int main()
{
    return UnitTest::RunAllTests();
}
//==================================================================================================
