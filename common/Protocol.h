#pragma once

#include "../rpc/Packet.h"

//==================================================================================================
enum PacketId
{
    Packet1Id,
    Packet2Id,
    Packet3Id,
    Packet4Id,
    Packet5Id,
    Packet6Id,
    Packet7Id
};
//==================================================================================================
struct Signal1 : Packet  //slave call
{
    int a;
    int b;

    Signal1()
    : Packet(Packet1Id)
    {}
};
//==================================================================================================
struct Signal2 : Packet  //slave call
{
    char *c;

    Signal2()
    : Packet(Packet2Id)
    {}
};
//==================================================================================================
struct Signal3 : Packet  //slave call
{
    double d;

    Signal3()
    : Packet(Packet3Id)
    {}
};
//==================================================================================================
struct Slot1 : Packet  //master call
{
    double e;
    char *f;

    Slot1()
    : Packet(Packet4Id)
    {}
};
//==================================================================================================
struct Slot2 : Packet  //master call
{
    int g;

    Slot2()
    : Packet(Packet5Id)
    {}
};
//==================================================================================================
struct Packet6 : Packet  //slave stub's call
{
    int h;
    char *i;

    Packet6()
    : Packet(Packet6Id)
    {}
};
//==================================================================================================
struct Packet7 : Packet //master response on slave stub's call
{
    bool j;

    Packet7()
    : Packet(Packet7Id)
    {}
};
//==================================================================================================
