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

INCLUDEPATH += src\
               ../\
               src/zlib\

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
    src/libjpeg/jcapimin.c \
    src/libjpeg/jcapistd.c \
    src/libjpeg/jccoefct.c \
    src/libjpeg/jccolor.c \
    src/libjpeg/jcdctmgr.c \
    src/libjpeg/jchuff.c \
    src/libjpeg/jcinit.c \
    src/libjpeg/jcmainct.c \
    src/libjpeg/jcmarker.c \
    src/libjpeg/jcmaster.c \
    src/libjpeg/jcomapi.c \
    src/libjpeg/jcparam.c \
    src/libjpeg/jcphuff.c \
    src/libjpeg/jcprepct.c \
    src/libjpeg/jcsample.c \
    src/libjpeg/jctrans.c \
    src/libjpeg/jdapimin.c \
    src/libjpeg/jdapistd.c \
    src/libjpeg/jdatadst.c \
    src/libjpeg/jdatasrc.c \
    src/libjpeg/jdcoefct.c \
    src/libjpeg/jdcolor.c \
    src/libjpeg/jddctmgr.c \
    src/libjpeg/jdhuff.c \
    src/libjpeg/jdinput.c \
    src/libjpeg/jdmainct.c \
    src/libjpeg/jdmarker.c \
    src/libjpeg/jdmaster.c \
    src/libjpeg/jdmerge.c \
    src/libjpeg/jdphuff.c \
    src/libjpeg/jdpostct.c \
    src/libjpeg/jdsample.c \
    src/libjpeg/jdtrans.c \
    src/libjpeg/jerror.c \
    src/libjpeg/jfdctflt.c \
    src/libjpeg/jfdctfst.c \
    src/libjpeg/jfdctint.c \
    src/libjpeg/jidctflt.c \
    src/libjpeg/jidctfst.c \
    src/libjpeg/jidctint.c \
    src/libjpeg/jidctred.c \
    src/libjpeg/jmemansi.c \
    src/libjpeg/jmemmgr.c \
    src/libjpeg/jmemname.c \
    src/libjpeg/jmemnobs.c \
    src/libjpeg/jquant1.c \
    src/libjpeg/jquant2.c \
    src/libjpeg/jutils.c \
    src/libpng/example.c \
    src/libpng/png.c \
    src/libpng/pngerror.c \
    src/libpng/pngget.c \
    src/libpng/pngmem.c \
    src/libpng/pngpread.c \
    src/libpng/pngread.c \
    src/libpng/pngrio.c \
    src/libpng/pngrtran.c \
    src/libpng/pngrutil.c \
    src/libpng/pngset.c \
    src/libpng/pngtest.c \
    src/libpng/pngtrans.c \
    src/libpng/pngwio.c \
    src/libpng/pngwrite.c \
    src/libpng/pngwtran.c \
    src/libpng/pngwutil.c \
    src/lua/lapi.c \
    src/lua/lauxlib.c \
    src/lua/lbaselib.c \
    src/lua/lbitlib.c \
    src/lua/lcode.c \
    src/lua/lcorolib.c \
    src/lua/lctype.c \
    src/lua/ldblib.c \
    src/lua/ldebug.c \
    src/lua/ldo.c \
    src/lua/ldump.c \
    src/lua/lfunc.c \
    src/lua/lgc.c \
    src/lua/linit.c \
    src/lua/liolib.c \
    src/lua/llex.c \
    src/lua/lmathlib.c \
    src/lua/lmem.c \
    src/lua/loadlib.c \
    src/lua/lobject.c \
    src/lua/lopcodes.c \
    src/lua/loslib.c \
    src/lua/lparser.c \
    src/lua/lstate.c \
    src/lua/lstring.c \
    src/lua/lstrlib.c \
    src/lua/ltable.c \
    src/lua/ltablib.c \
    src/lua/ltm.c \
    src/lua/lua.c \
    src/lua/luac.c \
    src/lua/lundump.c \
    src/lua/lutf8lib.c \
    src/lua/lvm.c \
    src/lua/lzio.c \
    src/zlib/adler32.c \
    src/zlib/compress.c \
    src/zlib/crc32.c \
    src/zlib/deflate.c \
    src/zlib/gzclose.c \
    src/zlib/gzlib.c \
    src/zlib/gzread.c \
    src/zlib/gzwrite.c \
    src/zlib/infback.c \
    src/zlib/inffast.c \
    src/zlib/inflate.c \
    src/zlib/inftrees.c \
    src/zlib/trees.c \
    src/zlib/uncompr.c \
    src/zlib/zutil.c \
    src/tintaHermSpline.cpp

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
    src/libjpeg/cderror.h \
    src/libjpeg/cdjpeg.h \
    src/libjpeg/jchuff.h \
    src/libjpeg/jconfig.h \
    src/libjpeg/jdct.h \
    src/libjpeg/jdhuff.h \
    src/libjpeg/jerror.h \
    src/libjpeg/jinclude.h \
    src/libjpeg/jmemsys.h \
    src/libjpeg/jmorecfg.h \
    src/libjpeg/jpegint.h \
    src/libjpeg/jpeglib.h \
    src/libjpeg/jversion.h \
    src/libjpeg/transupp.h \
    src/libpng/CHANGES \
    src/libpng/libpng.3 \
    src/libpng/libpngpf.3 \
    src/libpng/png.h \
    src/libpng/pngconf.h \
    src/libpng/pngdebug.h \
    src/libpng/pnginfo.h \
    src/libpng/pnglibconf.h \
    src/libpng/pngpriv.h \
    src/libpng/pngstruct.h \
    src/lua/lapi.h \
    src/lua/lauxlib.h \
    src/lua/lcode.h \
    src/lua/lctype.h \
    src/lua/ldebug.h \
    src/lua/ldo.h \
    src/lua/lfunc.h \
    src/lua/lgc.h \
    src/lua/llex.h \
    src/lua/llimits.h \
    src/lua/lmem.h \
    src/lua/lobject.h \
    src/lua/lopcodes.h \
    src/lua/lparser.h \
    src/lua/lprefix.h \
    src/lua/lstate.h \
    src/lua/lstring.h \
    src/lua/ltable.h \
    src/lua/ltm.h \
    src/lua/lua.h \
    src/lua/lua.hpp \
    src/lua/luaconf.h \
    src/lua/lualib.h \
    src/lua/lundump.h \
    src/lua/lvm.h \
    src/lua/lzio.h \
    src/zlib/crc32.h \
    src/zlib/deflate.h \
    src/zlib/gzguts.h \
    src/zlib/inffast.h \
    src/zlib/inffixed.h \
    src/zlib/inflate.h \
    src/zlib/inftrees.h \
    src/zlib/trees.h \
    src/zlib/zconf.h \
    src/zlib/zconf.h.cmakein \
    src/zlib/zconf.h.in \
    src/zlib/zlib.h \
    src/zlib/zutil.h \
    src/tintaHermSpline.h





unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    src/libpng/libpng-1.4.0-manual.pdf \
    src/libpng/libpng-config.in \
    src/libpng/pngbar.jpg \
    src/libpng/pngbar.png \
    src/libpng/pngnow.png \
    src/libpng/pngtest.png \
    src/libpng/ANNOUNCE \
    src/libpng/configure \
    src/libpng/libpng-manual.txt \
    src/libpng/libpng.pc.in \
    src/libpng/LICENSE \
    src/libpng/png.5 \
    src/libpng/TODO \
    src/libpng/CMakeLists.txt \
    src/libpng/INSTALL \
    src/libpng/README \
    src/lua/Makefile \
    src/zlib/zlib.3.pdf \
    src/zlib/zlib2ansi \
    src/zlib/configure \
    src/zlib/treebuild.xml \
    src/zlib/FAQ \
    src/zlib/INDEX \
    src/zlib/zlib.3 \
    src/zlib/zlib.map \
    src/zlib/zlib.pc.cmakein \
    src/zlib/zlib.pc.in \
    src/zlib/ChangeLog \
    src/zlib/CMakeLists.txt \
    src/zlib/Makefile \
    src/zlib/Makefile.in \
    src/zlib/README
