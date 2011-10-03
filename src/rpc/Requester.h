#pragma once

#include "Data.h"

//==================================================================================================
class IRequester
{
public :
virtual void message(Data const &request) = 0;
virtual void signal(Data const &request, Data &response) = 0;
virtual void beginSlot(Data const &response) = 0;
virtual void endSlot(Data const &request) = 0;
};
//==================================================================================================
