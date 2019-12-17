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

SOURCES +=  src/tintaArrayBox.cpp\
            src/tintaAsyncTimer.cpp\
            src/tintaBoxContainer.cpp\
            src/tintaCommFunc.cpp\
            src/tintaControls.cpp\
            src/tintaIImgCodec.cpp\
            src/tintaImageFont.cpp\
            src/tintaProcessQueue.cpp\            
            src/tintaSignalHandler.cpp\
            src/tintaThreadPool.cpp\
            src/tintaTimer.cpp\
            src/tintaUInt8Image.cpp\
            src/tintaUInt8ImageSeamless.cpp\
            src/tintaCommon.cpp \
            src/MemTracker.cpp\
            src/tintaMemoryAllocator.cpp\
            src/tintaBufferIO.cpp\
            src/tintaLogger.cpp\
            src/tintaException.cpp\
            src/tintaScriptContext.cpp\
            src/tintaFont.cpp\
            src/OCL/tintaBox2dProgram.cpp\
            src/OCL/tintaClBaseImpl.cpp\
            src/OCL/tintaClObjectContainer.cpp\
            src/OCL/tintaGPUExt.cpp\
            src/tintaTreeConfig/tintaConfNode.cpp\
            src/tintaTreeConfig/tintaTreeConfig.cpp\
            src/Math/tintaCatRomSpline.cpp\
            src/Math/tintaMatrix33.cpp\
            src/Math/tintaMatrix44.cpp\
            src/Math/tintaQuatern.cpp\
            src/Math/tintaVector2.cpp\
            src/Math/tintaVector3.cpp\
            src/Math/tintaVector4.cpp\
            src/BoostAsioInteract/tintaAsioClientImpl.cpp\
            src/BoostAsioInteract/tintaAsioInteract.cpp\
            src/BoostAsioInteract/tintaInteractPool.cpp\
            src/BoostAsioInteract/tintaInteractFunction.cpp\
            src/BoostAsioInteract/tintaPacket.cpp\
            src/BoostAsioInteract/tintaServedPool.cpp\
            src/ImageCodecs/tintaJpgImgCodec.cpp\
            src/ImageCodecs/tintaPngImgCodec.cpp\

INCLUDEPATH += src
HEADERS +=  src/tintaArray2d.h\
            src/tintaArrayBox.h\
            src/tintaArrayBoxFactory.h\
            src/tintaAsyncTimer.h\
            src/tintaBoxContainer.h\
            src/tintaCommFunc.h\
            src/tintaControls.h\
            src/tintaCoord2.h\
            src/tintaDispatcher.h\
            src/tintaExtreme.h\
            src/tintaFactoryObj.h\
            src/tintaIImgCodec.h\
            src/tintaImageFactory.h\
            src/tintaImageFont.h\
            src/tintaISendEvents.h\
            src/tintaISerialisable.h\
            src/tintaISerialisableSet.h\
            src/tintaObjContainer.h\
            src/tintaObjCreator.h\
            src/tintaObserved.h\
            src/tintaPixel.h\
            src/tintaProcessQueue.h\
            src/tintaScriptContext.h\
            src/tintaScriptFunc.h\
            src/tintaSignalHandler.h\
            src/tintaThreadPool.h\
            src/tintaTimer.h\
            src/tintaUInt8Image.h\
            src/tintaUInt8ImageSeamless.h\
            src/tintaUtilLocalEn.h\
            src/tintaUtilLocalRu.h\
            src/tintaConnectionEvents.h\
            src/tintaCommon.h \
            src/tintaPlatform.h   \
            src/tintaPredefine.h  \
            src/tintaStdHeaders.h \
            src/tintaFont.h\
            src/tintaFontFactory.h\
            src/tintaGeomFuncs.h\
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
            src/tintaTreeConfig/tintaConfNode.h\
            src/tintaTreeConfig/tintaTreeConfig.h\
            src/OCL/tintaBox2dProgram.h\
            src/OCL/tintaClBaseImpl.h\
            src/OCL/tintaCLConfig.h\
            src/OCL/tintaClObjectContainer.h\
            src/OCL/tintaGPUExt.h\
            src/OCL/tintaIClBase.h\
            src/OCL/tintaLibsConfig.h\
            src/ImageCodecs/tintaJpgImgCodec.h\
            src/ImageCodecs/tintaPngImgCodec.h\
            src/BoostAsioInteract/tintaAsioClient.h\
            src/BoostAsioInteract/tintaAsioClientImpl.h\
            src/BoostAsioInteract/tintaAsioInteract.h\
            src/BoostAsioInteract/tintaIHandlerFactory.h\
            src/BoostAsioInteract/tintaIInteracted.h\
            src/BoostAsioInteract/tintaInteractFunction.h\
            src/BoostAsioInteract/tintaInteractHandler.h\
            src/BoostAsioInteract/tintaInteractPool.h\
            src/BoostAsioInteract/tintaPacket.h\
            src/BoostAsioInteract/tintaServedPool.h\
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
