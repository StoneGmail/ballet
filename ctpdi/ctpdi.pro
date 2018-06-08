#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T06:09:12
#
#-------------------------------------------------

QT       -= core gui
CONFIG += c++14

TEMPLATE = lib

DEFINES += CTPDI_LIBRARY

SOURCES += ../src/ctpdi.cpp\
    ../src/datasaver.cpp \
    ../src/tick2kindle.cpp \
    ../src/ctpmdm.cpp


HEADERS += \
    ../include/ctpdi.h\
    ../include/ctpdi_global.h\
    ../include/datasaver.h \
    ../include/tick2kindle.h \
    ../include/ctpmem.h


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
        TARGET = ctpdid
        LIBS += -L$$OUT_PWD/../utility/ -lutilityd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = ctpdi
        LIBS += -L$$OUT_PWD/../utility/ -lutility
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
        TARGET = ctpdid
        LIBS += -L$$OUT_PWD/../utility/debug/ -lutilityd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = ctpdi
        LIBS += -L$$OUT_PWD/../utility/release/ -lutility
        message("Build for release version")
    }
}

DISTFILES += \
    pygom.py \
    pydi.py


INCLUDEPATH += $$PWD/../utility
DEPENDPATH += $$PWD/../utility
