QT          -=  core gui

TARGET      =   test_master

TEMPLATE    =   app

CONFIG      +=  link_pkgconfig

PKGCONFIG   +=  gtk+-2.0

DEFINES     +=  BOOST_TEST_THREAD_SAFE

INCLUDEPATH += \
                ../..\
                ../channels

HEADERS     += \
                UnixLocalChannel.h

SOURCES     += \
                master.cpp\
                ../channels/UnixLocalChannel.cpp

LIBS        += -lboost_unit_test_framework -lmsgpack
