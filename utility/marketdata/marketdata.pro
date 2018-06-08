QT       -= gui

TEMPLATE = app
CONFIG += console
QMAKE_CXXFLAGS += -std=c++17

CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH +=  ../include

SOURCES += main.cpp



###################### unix ############################
unix {
    DEFINES += _UNIX

    message("Building for unix")
    INCLUDEPATH += ../ctp-api/20160606_tradeapi_linux64
    INCLUDEPATH += /usr/local/include


    # don't forget linke thost..so to libthost...so
    LIBS += -lrt -lpthread

    target.path = /usr/local/bin/gmd
    INSTALLS += target

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = marketdatad
        LIBS += -L$$OUT_PWD/../utility/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/ -lctpifd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = marketdata
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
    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = marketdatad
        LIBS += -L$$OUT_PWD/../utility/debug/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/debug/ -lctpifd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = marketdata
        LIBS += -L$$OUT_PWD/../utility/release/ -lutility
        LIBS += -L$$OUT_PWD/../ctpif/release/ -lctpif
        message("Build for release version")
    }
}

DISTFILES += \
    gmd-win.json \
    gmd-linux.json \
    inst-geter-linux.json \
    socp.sh \
    gmd-linux-allinst.json \
    instruments.json \
    all-instruments.json \
    gmd-znl.json \
    gmd-9.json \
    inst.json
