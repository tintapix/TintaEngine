/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef __TINTA_PREDEFINE__H_
#define __TINTA_PREDEFINE__H_

#include "tintaPlatform.h"

// win32 
#if CORE_COMPILER == CORE_COMPILER_MSVC
// std types export
#   pragma warning (disable : 4251)
// fopen vs fopen_s
#	pragma warning( disable: 4996) 


#endif

#include "tintaThread.h"

#ifdef  USING_BOOST

#   ifdef FILESYSTEM_BOOST
        #include <boost/filesystem.hpp>
        #include <boost/filesystem.hpp>
        #include <boost/filesystem/operations.hpp>
#   endif
#   ifdef LOCALE_BOOST
        #include <boost/locale.hpp>
#   endif
#endif

// configure memory tracking
#if DEBUG_MODE
#	ifdef CORE_MEMORY_TRACKER_DEBUG_MODE
#		define CORE_MEMORY_TRACKER 1
#	else
#		define CORE_MEMORY_TRACKER 0
#	endif
#else
#	ifdef CORE_MEMORY_TRACKER_RELEASE_MODE
#		define CORE_MEMORY_TRACKER 1
#	else
#		define CORE_MEMORY_TRACKER 0
#	endif
#endif

namespace Tinta {
      
    #if CORE_DOUBLE_PRECISION == 1		
        typedef double real;
    #else		
        typedef float  real;
    #endif


 	// pre-declaration 
	struct tintaImgStack;	
	class tintaImgCanvas;			
	class tintaScriptContext;
	class tintaLogger;		
	class tintaMatrix33;
	class tintaMatrix44;
	class tintaIImgCodec;		
	class tintaClObjectContainer;
	class tintaGPUExt;    
	
    enum {
        // common ID value for containers
        ZERO_ID = 0
    };

    enum CharSize : m_uint8 { ASCII_SIZE = 1, UTF8_SIZE = 1 };
	
    typedef m_uint8 channelByte;

    typedef float channelFloat;
    enum ImgChannels{
        ImgChannels_1 = 1, // Gray
        ImgChannels_2 = 2,
        ImgChannels_3 = 3, // RGB
        ImgChannels_4 = 4, // RGBA
        ImgChannels_5 = 6, // RGBAL
        ImgChannels_max = ImgChannels_5
    };

}	

#include "tintaStdHeaders.h"

namespace Tinta
{

typedef std::string utf8str_t;
typedef size_t sizetype_t;
typedef utf8str_t StringBasic;
typedef std::wstring tintaWString;
typedef std::basic_stringstream<char,std::char_traits<char>,std::allocator<char> > StringStreamBasic;	
typedef std::vector<StringBasic> tintaBasicStringVector;

// for common strings

typedef StringBasic StringBase;
typedef std::basic_stringstream<char,std::char_traits<char>,std::allocator<char> > StringStreamBase;	
#if CORE_PLATFORM == CORE_PLATFORM_WIN32
    #define _M(x)  u8 ## x
#else
    #define _M(x) x
#endif
	#define stream_out std::cout
	#define stream_in std::cin
	#define stream_fout std::ofstream
	#define stream_fin	std::ifstream
	#define char_m		char
	#define  open_file fopen_s
	#define	 atoi_m			atoi 
	#define  ml_strcopy		strcpy 
	#define stream_o std::ostream
    typedef std::regex regex_t;



    typedef StringBase String;
    typedef StringStreamBase StringStream;
	typedef StringStream stringstream;
    

	typedef std::vector<float> floatVec_t;
	typedef std::vector<int> intVec_t;
	typedef std::vector<m_uint32> uint32Vec_t;
	
	typedef std::vector<String> StringVector;
    typedef std::set<String>    StringSet;
    
    struct tintaINamed {
        virtual const StringBasic& getName() const = 0;
    };
}



//#include "tintaMemoryAllocator.h"


#endif


