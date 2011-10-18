QT          -=  core gui

TARGET      =   TestRPC

TEMPLATE    =   app

CONFIG      +=  link_pkgconfig

PKGCONFIG   +=  gtk+-2.0

DEFINES     +=

INCLUDEPATH +=\
                src/common\
                src/rpc

HEADERS     +=\
                Protocol.h\
                Channel.h\
                Data.h\
                Packet.h\
                Requester.h\
                Responder.h\
                Router.h\
                Stub.h\
                RPC.h\
                src/slave/SlaveChannel.h\
                src/slave/SlaveStub.h\
                src/master/MasterChannel.h\
                src/master/MasterStub.h

SOURCES     += \
                src/rpc/RPC.cpp\
                src/master/MasterChannel.cpp\
                src/master/MasterStub.cpp\
                src/slave/SlaveChannel.cpp\
                src/slave/SlaveStub.cpp\
                src/slave/GtkSlave.cpp

unix\
{
LIBS        += \
                -lboost_thread-mt
}
