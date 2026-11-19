TARGET = tst_radiostate
TEMPLATE = app

QT += core network testlib
CONFIG += console c++17 testcase
CONFIG -= app_bundle

INCLUDEPATH += ../src

SOURCES += \
    tst_radiostate.cpp \
    ../src/audiocontroller.cpp \
    ../src/radiostate.cpp \
    ../src/signalingclient.cpp

HEADERS += \
    ../src/audiocontroller.h \
    ../src/radiostate.h \
    ../src/signalingclient.h
