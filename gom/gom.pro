#-------------------------------------------------
#
# Project created by QtCreator 2016-10-27T08:58:35
#
#-------------------------------------------------

QT     -= core gui
CONFIG += c++14
TEMPLATE = lib

DEFINES += GOM_LIBRARY

SOURCES += \
    ../src/ctpifio.cpp \
    ../src/ctpmdspi.cpp \
    ../src/ctptdspi.cpp \
    ../src/gommem.cpp


HEADERS +=\
    ../include/ctpifio.h \
    ../include/ctpmdspi.h \
    ../include/ctptdspi.h \
    ../include/gom_global.h \
    ../include/gommem.h

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
    LIBS += -lthostmduserapi -lthosttraderapi -lrt -lpthread

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        LIBS += -L$$OUT_PWD/../utility/ -lutilityd
        TARGET = gomd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        message("Build for release version")
        TARGET = gom
        LIBS += -L$$OUT_PWD/../utility/ -lutility
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS

    message("Building for Windows")

    INCLUDEPATH += $$PWD/../ctp-api/20160606_tradeapi_win64
    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = gomd
        message("Build for Debug version")
        LIBS += -L$$PWD/../ctp-api/20160606_tradeapi_win64 -lthostmduserapi
        LIBS += -L$$PWD/../ctp-api/20160606_tradeapi_win64 -lthosttraderapi
        LIBS += -L$$OUT_PWD/../utility/debug/ -lutilityd
    }
    CONFIG(release, debug|release) {
        TARGET = gom
        message("Build for release version")
        LIBS += -L$$PWD/../ctp-api/20160606_tradeapi_win64 -lthostmduserapi
        LIBS += -L$$PWD/../ctp-api/20160606_tradeapi_win64 -lthosttraderapi
        LIBS += -L$$OUT_PWD/../utility/release/ -lutility
    }
}


INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

DISTFILES += \
    package.sh \
    install.sh
