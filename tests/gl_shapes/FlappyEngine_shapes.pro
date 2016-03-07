QT += core testlib

TARGET = test
CONFIG += console
CONFIG -= app_bundle
CONFIG += testcase

TEMPLATE = app

include("../src/FlappyCxx.pri")

QMAKE_CXXFLAGS += -std=c++1y

DEFINES += VIEW_TYPE=GL

SOURCES += main.cpp \
    glviewfactoryqt.cpp
HEADERS += \
    test_gobj.h \
    glviewfactoryqt.h

