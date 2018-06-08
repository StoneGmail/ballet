#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T06:21:26
#
#-------------------------------------------------
QT      -= core gui

CONFIG += -std=c++14
TEMPLATE = lib

DEFINES += UTILITY_LIBRARY

SOURCES += \
    ../src/utility.cpp \
    ../src/mylog.c \
    ../src/str.c \
    ../src/mem.cpp \
    ../src/dataset.cpp \
    ../src/tick_bin.cpp

HEADERS += \
    ../include/utility.h\
    ../include/mem.h \
    ../include/mderr.h \
    ../include/mylog.h \
    ../include/str.h \
    ../include/utility.h \
    ../include/utility_global.h \
    ../include/dataset.h \
    ../include/def.h \
    ../include/candle.h \
    ../include/tick_bin.h

INCLUDEPATH += ../include


###################### unix ############################
unix {
    DEFINES += _UNIX

    target.path = /usr/local/lib
    INSTALLS += target

    message("Building for unix")
    INCLUDEPATH += ../ctp-api/20160606_tradeapi_linux64
    INCLUDEPATH += /usr/local/include

    # don't forget linke thost..so to libthost...so
    LIBS += -lrt -lpthread

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = utilityd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = utility
        message("Build for release version")
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS

    message("Building for Windows")

    INCLUDEPATH += $$PWD/../ctp-api/20160606_tradeapi_win64
    INCLUDEPATH += "C:\Program Files\libuv\include"
    LIBS += -L"C:\Program Files\libuv" -llibuv
    LIBS += -lws2_32

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = utilityd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = utility
        message("Build for release version")
    }
}
