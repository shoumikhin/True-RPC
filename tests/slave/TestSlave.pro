QT          -=  core gui

TARGET      =   test_slave

TEMPLATE    =   app

CONFIG      +=  link_pkgconfig

PKGCONFIG   +=  gtk+-2.0

DEFINES     +=

INCLUDEPATH += \
                ../channels

HEADERS     += \
                UnixDomainChannel.h

SOURCES     += \
                slave.cpp\
                UnixDomainChannel.cpp
