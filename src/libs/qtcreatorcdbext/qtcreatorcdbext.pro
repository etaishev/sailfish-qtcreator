# Build the Qt Creator CDB extension
TEMPLATE = lib

include(../../../qtcreator.pri)
include(cdb_detect.pri)


VERSION =
CONFIG -= precompile_header
CONFIG += hide_symbols

# Switch to statically linked CRT. Note: There will be only one
# global state of the CRT, reconsider if other DLLs are required!
# TODO: No effect, currently?
QMAKE_CXXFLAGS_RELEASE    -= -MD
QMAKE_CXXFLAGS_DEBUG      -= -MDd
QMAKE_CXXFLAGS_RELEASE    += -MT
QMAKE_CXXFLAGS_DEBUG      += -MTd

BASENAME=qtcreatorcdbext

DEF_FILE=$$PWD/qtcreatorcdbext.def

# Find out whether we are _building_ 64/32bit and determine target
# directories accordingly.
#
# Check for VSCMD_ARG_TGT_ARCH (VS 17) or Platform=X64 (VS 13, 15)
# For older versions, fall back to hacky check on LIBPATH

ENV_TARGET_ARCH=$$(VSCMD_ARG_TGT_ARCH)
isEmpty(ENV_TARGET_ARCH):ENV_TARGET_ARCH = $$(Platform)
ENV_LIBPATH=$$(LIBPATH)

contains(ENV_TARGET_ARCH, .*64$) {
    DIRNAME=$${BASENAME}64
    CDB_PLATFORM=amd64

    exists($$CDB_PATH/lib/amd64) {
        LIBS+= -L$$CDB_PATH/lib/amd64 -ldbgeng
    } else {
        LIBS+= -L$$CDB_PATH/lib/x64 -ldbgeng
    }
} else:isEmpty(ENV_TARGET_ARCH):contains(ENV_LIBPATH, ^.*amd64.*$) {
    DIRNAME=$${BASENAME}64
    CDB_PLATFORM=amd64

    exists($$CDB_PATH/lib/amd64) {
        LIBS+= -L$$CDB_PATH/lib/amd64 -ldbgeng
    } else {
        LIBS+= -L$$CDB_PATH/lib/x64 -ldbgeng
    }
} else {
    DIRNAME=$${BASENAME}32
    CDB_PLATFORM=i386

    exists($$CDB_PATH/lib/i386}) {
        LIBS+= -L$$CDB_PATH/lib/i386 -ldbgeng
    } else {
        LIBS+= -L$$CDB_PATH/lib/x86 -ldbgeng
    }
}

LIBS+=-luser32

DESTDIR=$$IDE_BUILD_TREE/lib/$${DIRNAME}
TARGET = $$BASENAME

message("Compiling Qt Creator CDB extension $$TARGET $$DESTDIR for $$CDB_PLATFORM using $$CDB_PATH")

INCLUDEPATH += $$CDB_PATH/inc

CONFIG -= qt
QT -= gui
QT -= core

SOURCES += qtcreatorcdbextension.cpp \
    extensioncontext.cpp \
    eventcallback.cpp \
    symbolgroupnode.cpp \
    symbolgroup.cpp \
    common.cpp \
    stringutils.cpp \
    gdbmihelpers.cpp \
    outputcallback.cpp \
    symbolgroupvalue.cpp \
    containers.cpp

HEADERS += extensioncontext.h \
    common.h \
    iinterfacepointer.h \
    eventcallback.h \
    symbolgroup.h \
    stringutils.h \
    gdbmihelpers.h \
    outputcallback.h \
    symbolgroupvalue.h \
    containers.h \
    knowntype.h \
    symbolgroupnode.h

isEmpty(PYTHON_INSTALL_DIR):PYTHON_INSTALL_DIR=$$(PYTHON_INSTALL_DIR)
exists($$PYTHON_INSTALL_DIR) {
    DEFINES += WITH_PYTHON=1

    INCLUDEPATH += $$PYTHON_INSTALL_DIR/include
    DEPENDPATH += $$PYTHON_INSTALL_DIR/include

    SOURCES += \
        pycdbextmodule.cpp \
        pyfield.cpp \
        pystdoutredirect.cpp \
        pytype.cpp \
        pyvalue.cpp

    HEADERS += \
        pycdbextmodule.h \
        pyfield.h \
        pystdoutredirect.h \
        pytype.h \
        pyvalue.h

    #TODO: parse version number for a generic approach
    CONFIG(release, debug|release): LIBS += -L$$PYTHON_INSTALL_DIR/libs -lpython35
    else:CONFIG(debug, debug|release): LIBS += -L$$PYTHON_INSTALL_DIR/libs -lpython35_d
}

target.path = $$QTC_PREFIX/lib/$${DIRNAME} # TODO this should go to INSTALL_LIBRARY_PATH/$${DIRNAME}

INSTALLS += target
