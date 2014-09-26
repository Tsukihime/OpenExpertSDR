#-------------------------------------------------
#
# Project created by QtCreator 2014-07-28T23:01:34
#
#-------------------------------------------------

QT       -= gui

TARGET = SunSDR
TEMPLATE = lib

DEFINES += SUNSDR_LIBRARY

SOURCES += sunsdr.cpp \
    sunCtrl.cpp

HEADERS += sunsdr.h \
    sunCtrl.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
