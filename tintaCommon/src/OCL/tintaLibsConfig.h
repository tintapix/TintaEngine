/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_LIBS_CONFIG_H_
#define _TINTA_LIBS_CONFIG_H_

#include  "tintaPredefine.h"

#ifdef USING_GPUCL 


#if CORE_ARCH_TYPE == CORE_ARCHITECTURE_64
	//#pragma comment(lib, "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v6.0\\lib\\x64\\OpenCL.lib")
#endif
#if CORE_ARCH_TYPE == CORE_ARCHITECTURE_32
	//#pragma	comment(lib, "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v6.0\\lib\\Win32\\OpenCL.lib")
#endif


#endif /*USING_GPUCL*/



#endif