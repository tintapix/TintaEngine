/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_OS_HEADERS_H_
#define _TINTA_OS_HEADERS_H_

#include "tintaPlatform.h"

#if CORE_PLATFORM == CORE_PLATFORM_WIN32
    #if !defined( __MINGW32__ )
    #ifndef WIN32_LEAN_AND_MEAN
    #   define WIN32_LEAN_AND_MEAN
    #endif
    #  ifndef NOMINMAX
    #	define NOMINMAX // required to stop windows.h messing up std::min
    #  endif
    #endif
    #	include <windows.h>
    #	include <Shlobj.h>
    #	include "Shlwapi.h"
    #   include <shellapi.h>
#endif
 	#if CORE_PLATFORM == CORE_PLATFORM_LINUX
 	extern "C" {

 	#include <unistd.h>
 	#include <dlfcn.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <dirent.h>
 	}
	#endif

#endif
