TEMPLATE = app
CONFIG += console
QMAKE_CXXFLAGS += -std=c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += ../include
DEPENDPATH += $$PWD/../utility


###################### unix ############################
unix {
    DEFINES += _UNIX

    message("Building for unix")
    INCLUDEPATH += /usr/local/include

    LIBS += -lpthread -lrt

    target.path = /usr/local/bin/gmd
    INSTALLS += target

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = tc2bd
        LIBS += -L$$OUT_PWD/../utility/ -lutilityd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = tc2b
        LIBS += -L$$OUT_PWD/../utility/ -lutility
        message("Build for release version")
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS

    message("Building for Windows")

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = savecdid
        LIBS += -L$$OUT_PWD/../utility/debug/ -lutilityd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = savecdi
        LIBS += -L$$OUT_PWD/../utility/release/ -lutility
        message("Build for release version")
    }
}
