/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_COMMON_H_
#define _TINTA_COMMON_H_



#include "tintaOSHeaders.h"
#include "tintaStdHeaders.h"
#include "Math/tintaCommonMath.h"
#include "Math/tintaVector2.h"
#include "Math/tintaVector3.h"
#include "Math/tintaVector4.h"
#include "tintaPredefine.h"



namespace Tinta
{

    /*
    ID_INLINE int idMath::FloatHash( const float *array, const int numFloats ) {
    int i, hash = 0;
    const int *ptr;

    ptr = reinterpret_cast<const int *>( array );
    for ( i = 0; i < numFloats; i++ ) {
    hash ^= ptr[i];
    }
    return hash;
    }
    */

    struct FileHandler{

        FileHandler() :mFp{ NULL_M }{}
        FileHandler(FILE *fp) 
            :mFp{ fp }{}
        ~FileHandler(){
            if ( mFp )
                fclose(mFp);
        }
        FILE *mFp;
    };

	struct getDirProp {

		getDirProp() :
		mOnlynames(false)
		, mPos(-1){}

		getDirProp(bool onlynames, int pos) :
			mOnlynames(onlynames)
			, mPos(pos){}

		// not full path in rezult
		bool mOnlynames;
		// index in list -1 - no position
		int mPos;
	};

	struct getFileProp
		: public getDirProp {
		getFileProp(){}
		getFileProp(bool onlynames, const String &ext, int pos) :
			getDirProp(onlynames, pos)
			, mExt(ext)
		{}
		// extension
		String  mExt;
	};




    class  NameGenerator
    {
    protected:
        String m_prefix;
        m_ulong32 m_number;

    public:
        NameGenerator(const NameGenerator& rhs)
            : m_prefix(rhs.m_prefix), m_number(rhs.m_number) {}

        NameGenerator() : m_prefix(_M("Name")), m_number(1) {}

        NameGenerator(const String& prefix) : m_prefix(prefix), m_number(1) {}        

        void setPrefix(const String &prefix) {
            m_prefix = prefix;
        }
        /// Generate a new name
        String generate(){
            StringStream s;
            s << m_prefix << m_number++;
            return s.str();
        }

        String generate(const String &dynam_prefix){
            StringStream s;
            s << dynam_prefix << m_number++;
            return s.str();
        }

        // from hash
        String generate(int dynam_prefix){
            StringStream s;
            s << dynam_prefix << m_number++;
            return s.str();
        }
        

        /// Generate a new name
        StringBasic generateBasic() {
            StringStreamBasic  s;
            s << m_number++;
            return s.str();
        }

        StringBasic generateBasic( const StringBasic &dynam_prefix ) {
            StringStreamBasic s;
            s << dynam_prefix << m_number++;
            return s.str();
        }

        // from hash
        StringBasic generateBasic( int dynam_prefix ) {
            StringStreamBasic s;
            s << dynam_prefix << m_number++;
            return s.str();
        }


        /// Reset the internal counter
        void reset() {
            m_number = 1ULL;
        }

        /// Manually set the internal counter (use caution)
        void set_next(m_ulong32 val) {
            m_number = val;
        }

        /// Get the internal counter
        m_ulong32 getNext() const {
            return m_number;
        }
    };

#if CORE_ENDIAN == CORE_ENDIAN_LITTLE
    const  StringBasic UTF8BOM = "\xEF\xBB\xBF";
#else
    const  StringBasic UTF8BOM = "\xBF\xBB\xEF";
#endif

    class _CoreExport StringUtil {
    public:
        typedef StringStream StrStreamType;
        
        
        static String getTime(const String &delim = ":");

        static String getDateTime(const String &delim = ":");

        static void toLower(String& str);

        static void toUpper(String& str);

        static StringVector split(const String &str, const StringVector &delimeters);

        static const String replaceAll(const String& source, const String& toFind, const String& pattern);        

        static  StringBasic genString(size_t length, const StringBasic &symbols, TintaMath::tintaRandomV2 *rand = NULL_M );

        static StringBasic genStringAlphabet(size_t length, TintaMath::tintaRandomV2 *rand = NULL_M);

        static StringBasic genStringDigit(size_t length, TintaMath::tintaRandomV2 *rand = NULL_M);

        static StringBasic genStringSpecial(size_t length, TintaMath::tintaRandomV2 *rand = NULL_M);

        /*
            Extracts string after '.' from the path string
        */
        static String getFileExt(const String &path);

        /*
            Adding leaf to the end of the path
        */
        static void addLeaf(String &srcPath, const String &new_leaf);

        /*
            Adding leaf to the begining of the path
        */
        static void addLeafBefore(String &srcPath, const String &new_leaf);


        static String tintaConstrFileExec(const String &path);

        static String tintayConstrFileExecCmd(const String &path);

        static char mlCharToLow(char c);

        static int hashStrCaseSens(const char *string);

        /*
        Searches string after '.' in the path string
        */
        static bool haveFileExtension(const String &s);

        static String getGetFilenameFromPath(const String &path);

        /*
        Removes '/' and '\\' symbols in the end
        winDir - if true then after deleting adds '/' after ':' for path in WinOS
        */
        static String delPathEndDelim(const String &path, bool winDir = true);


        /*
        	Removes last leaf from path
        	param - path
        	param - first delimiter type (usually \)
        	param - second delimiter type(usually \\)
        	Returns deleted leaf
        */
        static String delLastLeaf(String &path, const String& delim, const String& delimDbl);

        static StringVector tokenise(const String& str, const String& singleDelims, const String& doubleDelims = _M(""), unsigned int maxSplits = 100);



#if CORE_PLATFORM == CORE_PLATFORM_WIN32
        /*
            from utf16 to utf8
        */
        static StringBasic  createUTF8String(const tintaWString &inStr);

        

        /*
            from utf8 to utf16
        */
        static tintaWString createUTF16String(const StringBasic &inStr);
#endif        
    };

#if (defined (CORE_UNICODE_SUPPORT)) && (CORE_PLATFORM  == CORE_PLATFORM_WIN32)
#define toNarrowChar(v) (StringUtil::createUTF8String((v)))
#else
#define toNarrowChar(v) (v)
#endif

#if (defined (CORE_UNICODE_SUPPORT)) && (CORE_PLATFORM  == CORE_PLATFORM_WIN32)
#define toWideChar(v) (StringUtil::createUTF16String((v)))
#else
#define toWideChar(v) (v)
#endif

	static const m_uint32 colorMask[2] = { 255, 0 };

    FORCEINL m_byte floatToByte(float c) { 
		return (m_byte)(((m_uint32)(c * 255.0f)) & colorMask[FLOATSIGNBITSET(c)]);
	}

    /*
	    function clamp(x,a,b) returns the value a when x is less than a, the value of x when x is between a and b, and the value b when x is greater than b.
	    min(x, b) == clamp(x, x, b)
	    max(x, a) == clamp(x, a, x)
    */
    FORCEINL float clamp(float x, float a, float b){

		return (x < a ? a : (x > b ? b : x));
	}

    FORCEINL float byteToFloat(m_byte val) {
		return clamp((float)((val & 0x0000ff) / 255.f), 0.0f, 1.0f);
	}

    /*
    function clamp(x,a,b) returns the value a when x is less than a, the value of x when x is between a and b, and the value b when x is greater than b.
    min(x, b) == clamp(x, x, b)
    max(x, a) == clamp(x, a, x)
    */
    FORCEINL float clampOffset(float x, float a, float a1, float b){
        return (x < a + a1 ? a : (x > b ? b : x));
    }

    FORCEINL  float step(float a, float x){
        return (float)(x >= a);
    }

    FORCEINL  float pulse(float a, float b, float x) {
        return step(a, x) - step(b, x);
    }

    FORCEINL float smoothstep(float a, float b, float x){
        if (x < a)
            return 0;
        if (x >= b)
            return 1;
        x = (x - a) / (b - a);
        return (x * x * (3 - 2 * x));
    }

    FORCEINL float mixValues(float val_new, float val_src, float mix_factor = 0.5f)
    {
        float ret_val = val_new * (1 - mix_factor) + val_src * mix_factor;
        float max_val = TintaMath::max(val_new, val_src);

        return ret_val > max_val ? ret_val : max_val;
    }

    

    

    /*
        return  from down left from upper left position and vice versa
    */
    FORCEINL tintaVector2f getInvertYpos(const tintaVector2f &pos, const tintaVector2f &size){
        return tintaVector2f(pos.mx, size.my - pos.my);
    }

    FORCEINL int getInvertYpos(int oldYpos, int ySize){
        return ySize - oldYpos;
    }

    


    FORCEINL void* mlMemcpy(void* p_dist, size_t dist_size, const void* p_src, size_t src_size){

#if CORE_PLATFORM == CORE_PLATFORM_WIN32
        errno_t error = memcpy_s(p_dist, dist_size, p_src, src_size);
        if (error == 0){
            return p_dist;
        }
        else{
            return 0;
        }
#else
        if (!p_dist || dist_size == 0 || !p_src || src_size == 0){
            return 0;
        }
        if (src_size > dist_size){
            return 0;
        }
        memcpy(p_dist, p_src, src_size);

        return p_dist;
#endif
    }

    



    FORCEINL int openFile(FILE **stream, const String &path, const String &option){

#if CORE_PLATFORM == CORE_PLATFORM_WIN32

        return fopen_s(stream, path.c_str(), option.c_str());
#else

       *stream = fopen(path.c_str(), option.c_str());

        return 0;
#endif

    }
       

    /*
        Is path string valid either directory or file
    */
    bool _CoreExport isPathValid(const String &path);

	bool  _CoreExport  isUTF8WithBOM( const String &path );

	bool _CoreExport readUTF8Text(const String &path, StringBasic &text );

	//bool _CoreExport readUTF8Text(const String &path, StringVector &text);     

    bool _CoreExport readUTF8Text(const String &path, StringVector &text);
	/*
		writes text to the utf8 file and returns position		
	*/
	size_t _CoreExport writeUTF8toFile( const String &path, const String &data );


	/*
		writes vector of strings in to the file with utf8 
		end - if true writes data to the end of the file
	*/
    bool _CoreExport writeUTF8toFile(const String &path, const StringVector &data, bool end = true );


    /*
    writes vector of strings in to the file with utf8
    end - if true writes data to the end of the file
    */
    bool _CoreExport writeUTF8toFile(const String &path, const StringSet &data, bool end = true);

	

	m_ulong32 _CoreExport getProcessId();
	/*

	*/
	//long _CoreExport getNext( long cur );
    tintaVector3f _CoreExport getHSVfromRGB(float r, float g, float b);

    tintaVector3f _CoreExport rgbToHsl(const tintaVector3f &rgb);       

    tintaVector3f _CoreExport  hslToRgb(const tintaVector3f &hsl);

    tintaVector3f _CoreExport getRGBfromHSV(const tintaVector3f &hsvVal);


    
    
    float _CoreExport spline(float x, int nknots, float *knot);
    

    /*
        Validates directory path
    */
    bool _CoreExport isDirectory(const String &path);


    /*
        Validates file path
    */
    bool _CoreExport isFile(const String &path);

   
	/*
        Returns root path
    */
    String _CoreExport getRootPath();

	StringVector _CoreExport getFiles(const String &path, const getFileProp &prop, int *quantity = NULL);

	StringVector _CoreExport getDirs(const String &path, const getDirProp &prop, int *quantity = NULL );

    

	/*
        creats directory with parents
    */
    bool _CoreExport createFolder(const String &path);

    bool _CoreExport createProcess(const String &path_to_process, char_m *params = 0);

	// getting file size
	// return -1 if file was not found
    long _CoreExport getFileSize(const String &path);

	/*
        len - buffer len
        hashSoFar - offset
	*/ 
	m_uint32 _CoreExport FastHash(const char * data, int len, m_uint32 hashSoFar = 0);

	//// Colors 	

	extern	tintaVector4f color_black;
	extern	tintaVector4f color_white;
	extern	tintaVector4f color_red;
	extern	tintaVector4f color_green;
	extern	tintaVector4f color_blue;
	extern	tintaVector4f color_yellow;
	extern	tintaVector4f color_magenta;
	extern	tintaVector4f color_cyan;
	extern	tintaVector4f color_orange;
	extern	tintaVector4f color_purple;
	extern	tintaVector4f color_pink;
	extern	tintaVector4f color_brown;


	m_uint32	_CoreExport packColor4(const tintaVector4f &color);
	void		_CoreExport unpackColor4(const m_uint32 color, tintaVector4f &unpackedColor);
    void		_CoreExport unpackColor4(const m_uint32 color, tintaVector4i &unpackedColor);
	m_uint32	_CoreExport packColor3(const tintaVector3f &color);
	void		_CoreExport unpackColor3(const m_uint32 color, tintaVector3f &unpackedColor);
    

	m_uint32	_CoreExport packColor3( m_byte r, m_byte g, m_byte b );
	void		_CoreExport unpackColor3( const m_uint32 color, m_byte &r, m_byte &g, m_byte &b );

    m_uint32	_CoreExport packColor4(m_byte r, m_byte g, m_byte b, m_byte a);
    void		_CoreExport unpackColor4(const m_uint32 color, m_byte &r, m_byte &g, m_byte &b, m_byte &a);

     
    #define BOOL_RAND (((int)((float)rand() * (1.0 / ((float)RAND_MAX + 1.0))*2.0) - 1) == 0 )
}

#endif
