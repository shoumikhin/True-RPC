#include "SlaveStub.h"
#include "SlaveChannel.h"
#include "../rpc/RPC.h"

#include <gtk/gtk.h>

//==================================================================================================
int main()
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

    return 0;
}
catch (std::exception &e)
{

}
//==================================================================================================
