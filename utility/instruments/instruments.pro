TEMPLATE = app
CONFIG += console
QMAKE_CXXFLAGS += -std=c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    instgeter.cpp

HEADERS += instgeter.h



INCLUDEPATH += ../include


###################### unix ############################
unix {
    DEFINES += _UNIX

    message("Building for unix")
    INCLUDEPATH += ../ctp-api/20160606_tradeapi_linux64
    INCLUDEPATH += /usr/local/include
    LIBS += -lpthread -lrt

    target.path = /usr/local/bin/gmd
    INSTALLS += target

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = instrumentsd
        LIBS += -L$$OUT_PWD/../utility/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/ -lctpifd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = instruments
        LIBS += -L$$OUT_PWD/../utility/ -lutility
        LIBS += -L$$OUT_PWD/../ctpif/ -lctpif
        message("Build for release version")
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS

    message("Building for Windows")

    INCLUDEPATH += $$PWD/../ctp-api/20160606_tradeapi_win64
    INCLUDEPATH += $$PWD/../gtest
    # LIBS += -L$$PWD/../gtest/win64 -lgtest -lgtest_main
    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = instrumentsd
        LIBS += -L$$OUT_PWD/../utility/debug/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/debug/ -lctpifd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = instruments
        LIBS += -L$$OUT_PWD/../utility/release/ -lutility
        LIBS += -L$$OUT_PWD/../ctpif/release/ -lctpif
        message("Build for release version")
    }
}
