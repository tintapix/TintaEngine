/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_PLATFORM_H_
#define __TINTA_PLATFORM_H_

#include "tintaConfig.h"

namespace Tinta {


 // byte order
#define CORE_ENDIAN_LITTLE 0
#define CORE_ENDIAN_BIG 1

#define CORE_PLATFORM_WIN32 1
#define CORE_PLATFORM_LINUX 2


#define CORE_COMPILER_MSVC 1
#define CORE_COMPILER_GNUC 2

#define CORE_ARCHITECTURE_32 1
#define CORE_ARCHITECTURE_64 2

#define CORE_CPPx03 0
#define CORE_CPPx11 1
#define CORE_CPPx14 2

#if defined( _MSC_VER )
#   define CORE_COMPILER CORE_COMPILER_MSVC
#   define CORE_COMP_VER _MSC_VER
//_MSC_VER
// 	MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// 	MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
// 	MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// 	MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
// 	MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
// 	MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
// 	MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
// 	MSVC++ 7.0  _MSC_VER == 1300
// 	MSVC++ 6.0  _MSC_VER == 1200
// 	MSVC++ 5.0  _MSC_VER == 1100
// CPP standart in VSXXXX
// https://msdn.microsoft.com/ru-ru/library/hh567368.aspx	
#if _MSC_VER >=  1600 //&& _MSC_VER <  1900
#			define CORE_CPP_STANDART CORE_CPPx11 	
#else
#			define CORE_CPP_STANDART CORE_CPPx03 
#endif

#elif defined( __GNUC__ )
#   define CORE_COMPILER CORE_COMPILER_GNUC
#   define CORE_COMP_VER (((__GNUC__)*100) + \
	(__GNUC_MINOR__*10) + \
	__GNUC_PATCHLEVEL__)

#	define CPP_x14

#ifdef CPP_x11  // detecting CPPx11 standard
# 		define CORE_CPP_STANDART CORE_CPPx11
#endif

#ifdef CPP_x14 // detecting CPPx14 standard
# 		define CORE_CPP_STANDART CORE_CPPx14
#endif

#ifdef CPP_x03 // detecting CPPx03 standard
#		define CORE_CPP_STANDART CORE_CPPx03 
#endif  


#endif //#if defined( _MSC_VER )
	
#if CORE_COMPILER == CORE_COMPILER_MSVC
#       define FORCEINL _forceinline
#		define INL      __inline
#else
#   define FORCEINL __inline
#   define INL __inline
#endif


#if defined( __WIN32__ ) || defined( _WIN32 )
#   define CORE_PLATFORM CORE_PLATFORM_WIN32
#else
#   define CORE_PLATFORM CORE_PLATFORM_LINUX
#endif


#if defined(__x86_64__) || defined(_M_X64) || defined(_____LP64____)
#   define CORE_ARCH_TYPE CORE_ARCHITECTURE_64
#else
#   define CORE_ARCH_TYPE CORE_ARCHITECTURE_32
#endif

    // CHAR_DO
#if CORE_PLATFORM == CORE_PLATFORM_WIN32

#ifdef WINVER
#	undef WINVER
#endif
#ifdef _WIN32_WINNT
#	undef _WIN32_WINNT
#endif
#define WINVER 0x0501 
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501
#endif

#	if defined( STATIC_LIB )
#   	define _CoreExport
#   	define _CorePrivate
#   else
#   	if defined( TINTA_LIB )
#       	define _CoreExport __declspec( dllexport )
#   	else
#      	    define _CoreExport __declspec( dllimport )
#   	endif
#   	define _CorePrivate
#	endif

#   if defined(_DEBUG) || defined(DEBUG) || (RELEASE_TEST)
#       define DEBUG_MODE 1
#   else
#       define DEBUG_MODE 0
#   endif

#define CORE_ENDIAN CORE_ENDIAN_LITTLE


#endif // CORE_PLATFORM == CORE_PLATFORM_WIN32

// Linux Settings
#if CORE_PLATFORM == CORE_PLATFORM_LINUX 
	
#   if defined( CORE_GCC_VISIBILITY )
#       define _CoreExport  __attribute__ ((visibility("default")))
#       define _CorePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _CoreExport
#       define _CorePrivate
#   endif
	
#   define stricmp strcasecmp

#define CORE_ENDIAN CORE_ENDIAN_LITTLE	

#ifndef QT_NO_DEBUG
    #define DEBUG_MODE 1
#else
    #define DEBUG_MODE 0
#endif

#if defined(DEBUG) || defined(RELEASE_TEST)
#ifndef DEBUG_MODE
#       define DEBUG_MODE 1
#endif
#   else
#ifndef DEBUG_MODE
#       define DEBUG_MODE 0
#endif
#   endif
#endif

#if CORE_CPP_STANDART == CORE_CPPx11
#	define NULL_M	nullptr 
#else
#	define  NULL_M NULL
#endif

//
#if CORE_PLATFORM == CORE_PLATFORM_LINUX
#	define 	S_IFDIR_EX 16877
#	define 	S_IFMT_EX  33188
#else
#	define 	S_IFDIR_EX S_IFDIR
#	define 	S_IFMT_EX S_IFMT
#endif

#if CORE_PLATFORM == CORE_PLATFORM_LINUX
    #define FILESYSTEM_BOOST
#endif


}

#endif
