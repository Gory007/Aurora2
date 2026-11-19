TARGET = ru.example.PocRadio
TEMPLATE = app

QT += quick network
CONFIG += c++17 auroraapp

SOURCES += \
    src/main.cpp \
    src/audiocontroller.cpp \
    src/radiostate.cpp \
    src/signalingclient.cpp

HEADERS += \
    src/audiocontroller.h \
    src/radiostate.h \
    src/signalingclient.h

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

DISTFILES += \
    rpm/ru.example.PocRadio.spec \
    rpm/ru.example.PocRadio.changes \
    ru.example.PocRadio.desktop \
    qml/PocRadio.qml \
    qml/pages/MainPage.qml \
    qml/cover/DefaultCoverPage.qml \
    README.md \
    ARCHITECTURE.md \
    tests/mock_server.py \
    tests/tst_radiostate.cpp \
    tests/tst_radiostate.pro
