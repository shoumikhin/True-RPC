QT          -=  core gui

TARGET      =   truerpc

TEMPLATE    =   lib

CONFIG      +=

DEFINES     +=

INCLUDEPATH +=\
                .

HEADERS     +=\
                Channel.h\
                Data.h\
                Packet.h\
                Requester.h\
                Responder.h\
                Router.h\
                Stub.h\
                truerpc.h

SOURCES     += \
                truerpc.cpp

unix\
{
LIBS        += \
                -lboost_thread-mt
}
