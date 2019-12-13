#-------------------------------------------------
#
# Project created by QtCreator 2019-11-26T12:10:17
#
#-------------------------------------------------

QT       -= core gui

TARGET = tintaCommon
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=  src/tintaCommon.cpp \
            src/MemTracker.cpp\
            src/tintaMemoryAllocator.cpp\
            src/tintaBufferIO.cpp\
            src/tintaLogger.cpp\
            src/tintaException.cpp\
            src/tintaScriptContext.cpp\
            src/Math/tintaCatRomSpline.cpp\
            src/Math/tintaMatrix33.cpp\
            src/Math/tintaMatrix44.cpp\
            src/Math/tintaQuatern.cpp\
            src/Math/tintaVector2.cpp\
            src/Math/tintaVector3.cpp\
            src/Math/tintaVector4.cpp\

INCLUDEPATH += src
HEADERS +=  src/tintaCommon.h \
            src/tintaPlatform.h   \
            src/tintaPredefine.h  \
            src/tintaStdHeaders.h \
            src/utf8.h            \
            src/utf8/checked.h    \
            src/utf8/core.h       \
            src/utf8/unchecked.h  \
            src/tintaThread.h   \
            src/MemTracker.h    \
            src/tintaMemoryAllocator.h \
            src/tintaOSHeaders.h\
            src/tintaBufferIO.h\
            src/tintaLogger.h\
            src/tintaException.h\
            src/tintaConsoleOutput.h\
            src/tintaObserved.h\
            src/tintaSingleton.h\
            src/tintaScriptContext.h\
            src/Math/tintaBox3.h\
            src/Math/tintaCatRomSpline.h\
            src/Math/tintaCommonMath.h\
            src/Math/tintaMatrix44.h\
            src/Math/tintaQuatern.h\
            src/Math/tintaSphere3.h\
            src/Math/tintaVector2.h\
            src/Math/tintaVector3.h\
            src/Math/tintaVector4.h\





unix {
    target.path = /usr/lib
    INSTALLS += target
}
