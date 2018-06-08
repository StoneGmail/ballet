TEMPLATE = app
CONFIG += console
QMAKE_CXXFLAGS += -std=c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


INCLUDEPATH += ../include
INCLUDEPATH += $$PWD/../ctp-api/20160606_tradeapi_linux64
INCLUDEPATH += /usr/local/include

unix {
    DEFINES += _UNIX

    target.path = /usr/local/lib
    INSTALLS += target

}
