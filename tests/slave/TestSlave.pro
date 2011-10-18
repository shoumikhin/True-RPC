QT          -=  core gui

TARGET      =   TestRPC

TEMPLATE    =   app

CONFIG      +=  link_pkgconfig

PKGCONFIG   +=  gtk+-2.0

DEFINES     +=

INCLUDEPATH +=\
                .

HEADERS     +=\
                common/Protocol.h\
                truerpc/Channel.h\
                truerpc/Data.h\
                truerpc/Packet.h\
                truerpc/Requester.h\
                truerpc/Responder.h\
                truerpc/Router.h\
                truerpc/Stub.h\
                truerpc/TrueRPC.h\
                slave/SlaveChannel.h\
                slave/SlaveStub.h\
                master/MasterChannel.h\
                master/MasterStub.h

SOURCES     += \
                truerpc/TrueRPC.cpp\
                master/MasterChannel.cpp\
                master/MasterStub.cpp\
                slave/SlaveChannel.cpp\
                slave/SlaveStub.cpp \
                tests/slave.cpp

unix\
{
LIBS        += \
                -lboost_thread-mt
}
