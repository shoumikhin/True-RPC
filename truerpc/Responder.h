#pragma once

#include "Data.h"

//==================================================================================================
class IResponder
{
public :
virtual void onMessage(Data const &response) = 0;
virtual void onSignal(Data const &response) = 0;
};
//==================================================================================================
