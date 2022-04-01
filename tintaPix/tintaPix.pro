#-------------------------------------------------
#
# Project created by QtCreator 2019-12-17T12:45:53
#
#-------------------------------------------------

QT       -= core gui

TARGET = tintaPix
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

INCLUDEPATH += src\
               ../\
               ../tintaCommon/src\


SOURCES += \
    src/tintaChildProc.cpp \
    src/tintaCommonImageFactory.cpp \
    src/tintaCompGeom.cpp \
    src/tintaConfigurator.cpp \
    src/tintaExecutingTask.cpp \
    src/tintaExecutingUnit.cpp \
    src/tintaFunctions.cpp \
    src/tintaGeomCatRom.cpp \
    src/tintaGeomSet.cpp \
    src/tintaGeomTypes.cpp \
    src/tintaImgCanvas.cpp \
    src/tintaImgStack.cpp \
    src/tintaInteractHandlerImpl.cpp \
    src/tintaInteractPool.cpp \
    src/tintaInteractUnit.cpp \
    src/tintaInteractUnitsSet.cpp \
    src/tintaPrecompile.cpp \
    src/tintaRandGen.cpp \
    src/tintaScriptTaskExecutor.cpp \
    src/tintaTaskQueue.cpp \
    src/tintaTexSpringMain.cpp \
    src/tintaUnitsSet.cpp

HEADERS += \
    src/tintaAlgorithm.h \
    src/tintaChildProc.h \
    src/tintaCommonImageFactory.h \
    src/tintaCompGeom.h \
    src/tintaConfigurator.h \
    src/tintaExecutingTask.h \
    src/tintaExecutingUnit.h \
    src/tintaFunctions.h \
    src/tintaGeomCatRom.h \
    src/tintaGeomFactory.h \
    src/tintaGeomSet.h \
    src/tintaGeomTypes.h \
    src/tintaIImgWindow.h \
    src/tintaImgCanvas.h \
    src/tintaImgStack.h \
    src/tintaInteractHandlerImpl.h \
    src/tintaInteractPool.h \
    src/tintaInteractUnit.h \
    src/tintaInteractUnitsCallback.h \
    src/tintaInteractUnitsSet.h \
    src/tintaPrecompile.h \
    src/tintaPrimitives.h \
    src/tintaRandGen.h \
    src/tintaScriptTaskExecutor.h \
    src/tintaTaskQueue.h \
    src/tintaTexSpringMain.h \
    src/tintaUnitsSet.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    src/tintapixver
