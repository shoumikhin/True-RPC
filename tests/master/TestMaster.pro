QT          -=  core gui

TARGET      =   test_master

TEMPLATE    =   app

CONFIG      +=  link_pkgconfig

PKGCONFIG   +=  gtk+-2.0

DEFINES     +=

INCLUDEPATH += \
                ../channels

HEADERS     += \
                UnixDomainChannel.h

SOURCES     += \
                master.cpp\
                UnixDomainChannel.cpp
