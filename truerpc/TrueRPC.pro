QT          -=  core gui

TARGET      =   TestRPC

TEMPLATE    =   lib

CONFIG      +=

DEFINES     +=

INCLUDEPATH +=\
                .

HEADERS     +=\
                truerpc/Channel.h\
                truerpc/Data.h\
                truerpc/Packet.h\
                truerpc/Requester.h\
                truerpc/Responder.h\
                truerpc/Router.h\
                truerpc/Stub.h\
                truerpc/TrueRPC.h

SOURCES     += \
                truerpc/TrueRPC.cpp

unix\
{
LIBS        += \
                -lboost_thread-mt
}
