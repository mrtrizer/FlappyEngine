QT += core testlib

TARGET = test
CONFIG += console
CONFIG -= app_bundle
CONFIG += testcase

TEMPLATE = app

include("../../src/FlappyEngine.pri")

QMAKE_CXXFLAGS += -std=c++1y

DEFINES += VIEW_TYPE=GL

SOURCES += main.cpp
HEADERS += test_gobj.h

