/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_CL_CONFIG_H_
#define _TINTA_CL_CONFIG_H_


#include <tintaPredefine.h> 
#include <tintaConfig.h> 


#ifdef USING_GPUCL 

#include <CL\cl.h>

#define PLATFORM_TO_USE 0

#define TINTA_CL_DEVICE_TYPE CL_DEVICE_TYPE_DEFAULT 

#endif

#endif


