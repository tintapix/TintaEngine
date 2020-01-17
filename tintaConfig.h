/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef __TINTA_CONFIG_H_
#define __TINTA_CONFIG_H_


// compile with openCL support if defined
//#define USING_GPUCL



#define TINTA_PIX_COPYRIGNT "Tintapix(c) 2019"


/*
if the message has priority > LOG_LEVEL it will be logged

0 debug mode		        
1 EXCEPTIONs and etc.
2 script executing errors        
3 system messages
4 trivial messages
*/
#define LOG_LEVEL 0

// local setup
#define TINTA_EN 0
#define TINTA_RU 1
#define TINTA_LOCAL TINTA_RU


// base data types
typedef int m_int32;
typedef short m_int16;
typedef char m_int8;
typedef unsigned int m_uint32;
typedef unsigned long m_ulong32;
typedef unsigned char m_uint8;
typedef unsigned short m_uint16;
typedef float m_float32;
typedef double m_float64;

#endif
