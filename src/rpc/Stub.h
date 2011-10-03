#pragma once

#include "Responder.h"
#include "Requester.h"

//==================================================================================================
class IStub : public IResponder
{
public :
    IStub()
    : _router(0)
    {}

virtual void onStart() = 0;
virtual void onStop() = 0;

IRequester *router()
{
    return _router;
}

void setRouter(IRequester *router)
{
   _router = router;
}

private :
    IRequester *_router;
};
//==================================================================================================

