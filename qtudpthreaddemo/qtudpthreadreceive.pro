QT += core network charts gui

greaterThan(QT_MAJOR_VERSION, 4.0) QT += widgets
QT -= gui

TARGET = COneOne
CONFIG +=  c++11
#CONFIG -= app_bundle
#QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += header
TEMPLATE = app

#DEFINES += -DNOMINMAX

SOURCES += src/main.cpp \
    src/udpthreadsender.cpp \
    src/udpreceiver.cpp \
    src/receivethread.cpp \
    src/mainwin.cpp

HEADERS += \
    header/udpthreadsender.h \
    header/udpreceiver.h \
    header/receivethread.h \
    header/mainwin.h

