/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#include "tintaCommon.h"
#include "Math/tintaMatrix44.h"
#include "tintaLogger.h"
#include "tintaException.h"

namespace Tinta {



	tintaVector4f	colorBlack	= tintaVector4f( 0.00f, 0.00f, 0.00f, 1.00f );
	tintaVector4f	colorWhite	= tintaVector4f( 1.00f, 1.00f, 1.00f, 1.00f );
	tintaVector4f	colorRed	= tintaVector4f( 1.00f, 0.00f, 0.00f, 1.00f );
	tintaVector4f	colorGreen	= tintaVector4f( 0.00f, 1.00f, 0.00f, 1.00f );
	tintaVector4f	colorBlue	= tintaVector4f( 0.00f, 0.00f, 1.00f, 1.00f );
	tintaVector4f	colorYellow	= tintaVector4f( 1.00f, 1.00f, 0.00f, 1.00f );
	tintaVector4f	colorMagenta= tintaVector4f( 1.00f, 0.00f, 1.00f, 1.00f );
	tintaVector4f	colorCyan	= tintaVector4f( 0.00f, 1.00f, 1.00f, 1.00f );
	tintaVector4f	colorOrange	= tintaVector4f( 1.00f, 0.50f, 0.00f, 1.00f );
	tintaVector4f	colorPurple	= tintaVector4f( 0.60f, 0.00f, 0.60f, 1.00f );
	tintaVector4f	colorPink	= tintaVector4f( 0.73f, 0.40f, 0.48f, 1.00f );
	tintaVector4f	colorBrown	= tintaVector4f( 0.40f, 0.35f, 0.08f, 1.00f );



    const StringBasic digitsSymbols   = "1234567890";
    const StringBasic alphabetSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmonpqrstuvwxyz";
    const StringBasic specialSymbols  = " ~!@#$%^&*()_+-={}:\"|<>?/,.'][¹;`\\";
    //const StringBasic fullSymbols = digitsSymbols + alphabetSymbols + specialSymbols;
	
    //public static final byte[] UTF8_BOM = { (byte)0xEF, (byte)0xBB, (byte)0xBF };
    //public static final byte[] UTF16_BEBOM = { (byte)0xFE, (byte)0xFF };
    //public static final byte[] UTF16_LEBOM = { (byte)0xFF, (byte)0xFE };
    //public static final byte[] UTF32_BEBOM = {(byte)0x00,(byte)0x00, (byte)0xFF,(byte)0xFE}; 
    //public static final byte[] UTF32_LEBOM = { (byte)0xFF, (byte)0xFE, (byte)0x00, (byte)0x00 };
         
    
    String StringUtil::getTime(const String &delim) {
        StringStream msg;
        typedef std::chrono::system_clock Clock;
        auto now = Clock::now();

        auto sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
        auto fraction = now - sec;
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
        time_t ctTime = Clock::to_time_t(now);
        struct tm *pTime;
        pTime = localtime(&ctTime);
        msg << std::setw(2) << std::setfill(_M('0')) << pTime->tm_hour
            << delim << std::setw(2) << std::setfill(_M('0')) << pTime->tm_min
            << delim << std::setw(2) << std::setfill(_M('0')) << pTime->tm_sec
            << delim << std::setw(2) << std::setfill(_M('0')) << milliseconds.count();

        return msg.str();
    }

    String StringUtil::getDateTime(const String &delim) {
        StringStream msg;
        typedef std::chrono::system_clock Clock;
        auto now = Clock::now();

        auto sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
        auto fraction = now - sec;
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
        time_t ctTime = Clock::to_time_t(now);
        struct tm *pTime;
        pTime = localtime(&ctTime);
        
        msg << std::setw(2) << std::setfill(_M('0')) << pTime->tm_mday
            << delim << std::setw(2) << std::setfill(_M('0')) << pTime->tm_mon
            << delim << std::setw(2) << std::setfill(_M('0')) << pTime->tm_year
            << delim << std::setw(2) << std::setfill(_M('0')) << pTime->tm_hour
            << delim << std::setw(2) << std::setfill(_M('0')) << pTime->tm_min
            << delim << std::setw(2) << std::setfill(_M('0')) << pTime->tm_sec
            << delim << std::setw(2) << std::setfill(_M('0')) << milliseconds.count();

        return msg.str();
    }
    void StringUtil::toLower(String& str)
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
            tolower);
    }


    void StringUtil::toUpper(String& str)
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
            toupper);
    }

   

    StringVector StringUtil::split(const String &str, const StringVector &delimeters){

        StringVector result;

        if (delimeters.size() == 0)
            return result;

        size_t start, pos = str.size();
        start = 0;
        do {

            auto it = delimeters.begin();
            auto temp = str.size();
            auto found_pos = str.size();
            for (; it != delimeters.end(); it++){
            	temp = str.find_first_of(*it, start);
                if (temp != String::npos && temp < found_pos)   found_pos = temp;
            }

            if (found_pos <= str.size())
            				result.push_back(str.substr(start, found_pos - start));
            pos = found_pos;
            start = found_pos + 1;
        } while (pos < str.size());

        return std::move(result);
    }

    const String StringUtil::replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat){
        String result = source;
        String::size_type pos = 0;
        while (1)
        {
            pos = result.find(replaceWhat, pos);
            if (pos == String::npos) break;
            result.replace(pos, replaceWhat.size(), replaceWithWhat);
            pos += replaceWithWhat.size();
        }
        return std::move(result);
    }

    StringBasic StringUtil::genString( size_t length, const StringBasic &symbols, TintaMath::tintaRandomV2 *rand ) {

        StringBasic rez;
        rez.resize(length);
        TintaMath::tintaRandomV2 *gen = rand;
        TintaMath::tintaRandomV2 randGen((int)time(0));

        if ( !gen ) {
            gen = &randGen;
        }
        const StringBasic &alphabet = symbols;
        int maxPos = (int)alphabetSymbols.length() - 1;
        std::generate(rez.begin(), rez.end(), [&gen, alphabet, maxPos]() { return alphabet.at((*gen).randomInt(0, maxPos)); });

        return std::move(rez);
    }

    StringBasic StringUtil::genStringAlphabet(size_t length, TintaMath::tintaRandomV2 *rand) {
        return genString(length, alphabetSymbols, rand );
    }


    StringBasic StringUtil::genStringDigit(size_t length, TintaMath::tintaRandomV2 *rand) {
        return genString(length, digitsSymbols, rand );
    }

    StringBasic StringUtil::genStringSpecial(size_t length, TintaMath::tintaRandomV2 *rand) {
        return genString(length, specialSymbols, rand);
    }

    String StringUtil::getGetFilenameFromPath(const String &path) {
        String toParse = path;
        // abs path test
        if ( isPathValid(toParse) ) {
            StringVector r = StringUtil::tokenise(toParse, _M("/"), _M("//"), 10);
            if (r.size() > 0){                
                toParse = r.at(r.size() - 1);
            }
        }
        return std::move(toParse);
    }
    String StringUtil::getFileExt(const String &path){
               
        String rez;
        unsigned found = path.find_last_of(_M("."));

        if ( found != String::npos ){
            rez.insert( rez.begin(), path.begin() + found + 1, path.end() );
        }
        return std::move(rez);

    }

    void StringUtil::addLeaf(String &srcPath, const String &new_leaf){
        srcPath.append(_M("/"));
        srcPath.append(new_leaf);
    }



    void StringUtil::addLeafBefore(String &srcPath, const String &new_leaf) {       
       srcPath.insert(0, _M("/"));
       srcPath.insert(0, new_leaf);
    }


    String StringUtil::tintaConstrFileExec(const String &path){

        StringStream s;
        s << _M("c_f") << _M("(\"") << path << _M("\")");
        return s.str();
    }

    String StringUtil::tintayConstrFileExecCmd(const String &path){

        StringStream s;
        // using special symbol ^ in cmd commands 
        s << _M("c_executefile") << _M("(^") << path << _M("^)");
        return s.str();
    }

    char StringUtil::mlCharToLow(char c) {
        if (c <= 'Z' && c >= 'A') {
            return (c + ('a' - 'A'));
        }
        return c;
    }

    int StringUtil::hashStrCaseSens(const char *string) {
        int i, hash = 0;
        for (i = 0; *string != '\0'; i++) {
            hash += StringUtil::mlCharToLow(*string++) * (i + 119);
        }
        return hash;
    }

    bool StringUtil::haveFileExtension(const String &s){

        size_t n_found = s.rfind(_M('.'));
        if (n_found != String::npos)
            return true;
        return false;

    }

    String StringUtil::delPathEndDelim(const String &path, bool winDir){

        if (path.length() == 0)
            return String();
        String rez(path);
        String delim(_M("/\\"));
        std::size_t found = rez.find_last_not_of(delim);
        if (found != String::npos)
            rez.erase(found + 1);
        if (winDir && rez.length() > 0 && rez.at(rez.length() - 1) == _M(':'))
            rez.push_back(_M('/'));

        return std::move(rez);
    }

    String StringUtil::delLastLeaf(String &path, const String& delim, const String& delimDbl){

        if (path.length() == 0)
            return String();

        size_t delimPos = path.rfind(delim);

        size_t delimPosDbl = path.rfind(delimDbl);

        size_t assignPos = String::npos;
        if (delimPos != String::npos && delimPosDbl != String::npos){
            assignPos = std::max(delimPos, delimPosDbl);
        }
        else if (delimPos != String::npos){
            assignPos = delimPos;
        }
        else if (delimPosDbl != String::npos){
            assignPos = delimPosDbl;
        }
        else
        	return String();

        String temp;
        String leaf;
        temp.assign(path, 0, assignPos);
        leaf.assign(path, assignPos + 1, path.size());

        path.assign(temp);
        return std::move(leaf);
    }

    StringVector StringUtil::tokenise(const String& str, const String& singleDelims,
    								const String& doubleDelims, unsigned int maxParts) {
        StringVector ret;

        ret.reserve(maxParts ? maxParts + 1 : 10);

        unsigned int parts = 0;
        String delims = singleDelims + doubleDelims;

        size_t start;
        String::size_type pos;
        char_m delimDbl = 0;
        start = 0;
        do
        {
            if (delimDbl != 0){
                pos = str.find(delimDbl, start);
            }
            else{
                pos = str.find_first_of(delims, start);
            }

            if (pos == start){
                char_m curDelim = str.at(pos);
                if (doubleDelims.find_first_of(curDelim) != String::npos){
                	delimDbl = curDelim;
                }

                start = pos + 1;
            }
            else if (pos == String::npos || (maxParts && parts == maxParts)){
                if (delimDbl != 0){
                	// error
                }
                ret.push_back(str.substr(start));
                break;
            }
            else{
                if (delimDbl != 0){
                	delimDbl = 0;
                }
                ret.push_back(str.substr(start, pos - start));
                start = pos + 1;
            }
            if (delimDbl == 0){

                start = str.find_first_not_of(singleDelims, start);
            }

            ++parts;

        } while (pos != String::npos);

        return std::move(ret);
    }
#if    CORE_PLATFORM == CORE_PLATFORM_WIN32

	StringBasic StringUtil::createUTF8String(const tintaWString &inStr){
             
        StringBasic basic;
//#if CORE_PLATFORM == CORE_PLATFORM_WIN32
        // CHAR_DO
        std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv; 
       
        try {
            basic = utf8_conv.to_bytes(inStr);
        }
        catch (std::exception e){
#ifdef _DEBUG
            stream_out << _M("Convert in UTF8 error for string ");
#endif

        }
//#else
//        basic = boost::locale::conv::utf_to_utf<char>(inStr.c_str(), inStr.c_str() + inStr.length());
//#endif

        return basic;
    }
    
	tintaWString StringUtil::createUTF16String(const StringBasic &inStr) {
        
        
        tintaWString rez;
        // CHAR_DO
//#if CORE_PLATFORM == CORE_PLATFORM_WIN32
        std::wstring_convert< std::codecvt_utf8_utf16<wchar_t >> convertor;
        try {
            rez = convertor.from_bytes(inStr.c_str());
        }
        catch ( std::exception e ){    
#ifdef _DEBUG
            cout << "Convert in UTF16 error for string :"<< inStr;
#endif
        }
//#else
 //       rez = boost::locale::conv::utf_to_utf<wchar_t>(inStr.c_str(), inStr.c_str() + inStr.length());
//#endif
        return rez;
    }
  
#endif

	m_uint32	packColor4( const tintaVector4f &color ) {

		m_uint32 dw, dx, dy, dz;

		dx = floatToByte( color.mx );
		dy = floatToByte( color.my );
		dz = floatToByte( color.mz );
		dw = floatToByte( color.mw );

#if CORE_ENDIAN == CORE_ENDIAN_BIG //ARGB
		return ( dx << 0 ) | ( dy << 8 ) | ( dz << 16 ) | ( dw << 24 );
#else							      //BGRA
		return ( dx << 24 ) | ( dy << 16 ) | ( dz << 8 ) | ( dw << 0 );	
#endif

	}

	void	unpackColor4( const m_uint32 color, tintaVector4f &unpackedColor ) {

#if CORE_ENDIAN == CORE_ENDIAN_BIG
		unpackedColor.set( ( ( color >> 0 ) & 255 ) * ( 1.0f / 255.0f ),
			( ( color >> 8 ) & 255 ) * ( 1.0f / 255.0f ), 
			( ( color >> 16 ) & 255 ) * ( 1.0f / 255.0f ),
			( ( color >> 24 ) & 255 ) * ( 1.0f / 255.0f ) );
#else
		unpackedColor.set( ( ( color >> 24 ) & 255 ) * ( 1.0f / 255.0f ),
			( ( color >> 16 ) & 255 ) * ( 1.0f / 255.0f ), 
			( ( color >> 8 ) & 255 ) * ( 1.0f / 255.0f ),
			( ( color >> 0 ) & 255 ) * ( 1.0f / 255.0f ) );
#endif

	}

	m_uint32	packColor3( const tintaVector3f &color ) {

		m_uint32 dx, dy, dz;

		dx = floatToByte( color.mx );
		dy = floatToByte( color.my );
		dz = floatToByte( color.mz );

#if CORE_ENDIAN == CORE_ENDIAN_BIG //RGB
		return ( dx << 0 ) | ( dy << 8 ) | ( dz << 16 );
#else								  //BGR
		return ( dx << 16 ) | ( dy << 8 ) | ( dz << 0 );
#endif

	}
	void	unpackColor3( const m_uint32 color, tintaVector3f &unpackedColor ){

#if CORE_ENDIAN == CORE_ENDIAN_BIG
		unpackedColor.set( ( ( color >> 0 ) & 255 ) * ( 1.0f / 255.0f ),
			( ( color >> 8 ) & 255 ) * ( 1.0f / 255.0f ), 
			( ( color >> 16 ) & 255 ) * ( 1.0f / 255.0f ) );
#else
		unpackedColor.set( ( ( color >> 16 ) & 255 ) * ( 1.0f / 255.0f ),
			( ( color >> 8 ) & 255 ) * ( 1.0f / 255.0f ),
			( ( color >> 0 ) & 255 ) * ( 1.0f / 255.0f ) );
#endif

	}


	m_uint32	packColor3( m_uint8 r, m_uint8 g, m_uint8 b ) {


#if CORE_ENDIAN == CORE_ENDIAN_BIG //RGB
		return ( r << 0 ) | ( g << 8 ) | ( b << 16 );
#else							      //BGR
		return ( r << 16 ) | ( g << 8 ) | ( b << 0 );	
#endif
		
	}

    m_uint32	packColor4(m_uint8 r, m_uint8 g, m_uint8 b, m_uint8 a) {


#if CORE_ENDIAN == CORE_ENDIAN_BIG //RGB
        return (r << 0) | (g << 8) | (b << 16) | (a << 24);
#else							      //BGR
        return (r << 24) | (g << 16) | (b << 8) | (a << 0);
#endif

    }

	void	unpackColor3( const m_uint32 color, m_uint8 &r, m_uint8 &g, m_uint8 &b ) {

#if CORE_ENDIAN == CORE_ENDIAN_BIG
		r =  ( ( color >> 0 ) & 255 );
		g =  ( ( color >> 8 ) & 255 ); 
		b =  ( (color >> 16 ) & 255 );		
#else
		r =  ( (color >> 16 ) & 255 );
		g =  ( ( color >> 8 ) & 255 ); 
		b =  ( ( color >> 0 ) & 255 );	
#endif

	}

    void	unpackColor4(const m_uint32 color, m_uint8 &r, m_uint8 &g, m_uint8 &b, m_uint8 &a) {

#if CORE_ENDIAN == CORE_ENDIAN_BIG
        r = ((color >> 0) & 255);
        g = ((color >> 8) & 255);
        b = ((color >> 16) & 255);
        a = ((color >> 24) & 255);
#else
        r = ((color >> 24) & 255);
        g = ((color >> 16) & 255);
        b = ((color >> 8) & 255);
        a = ((color >> 0) & 255);
#endif

    }


    void	unpackColor4(const m_uint32 color, tintaVector4i &rez ) {

#if CORE_ENDIAN == CORE_ENDIAN_BIG
        r = ((color >> 0) & 255);
        g = ((color >> 8) & 255);
        b = ((color >> 16) & 255);
        a = ((color >> 24) & 255);
#else
        rez.mx = ((color >> 24) & 255);
        rez.my = ((color >> 16) & 255);
        rez.mz = ((color >> 8) & 255);
        rez.mw = ((color >> 0) & 255);
#endif

    }

m_ulong32 getProcessId(){
#if CORE_PLATFORM == CORE_PLATFORM_WIN32
	return GetCurrentProcessId();
#else
	return (m_ulong32)getpid();
#endif

}




bool  isPathValid(const String &path){
    String validate = StringUtil::delPathEndDelim(path);


#if CORE_PLATFORM == CORE_PLATFORM_WIN32


    struct _stat buf;
    int result;

    result = _stat(validate.c_str(), &buf);
    if (result == 0){
		return (( buf.st_mode & S_IFDIR ) != 0 
			|| (buf.st_mode & S_IFMT) != 0 
			|| (buf.st_mode & _S_IFREG) != 0);
    }
#else
    struct stat st;

    if (stat(validate.c_str(), &st) == 0){

        return ((st.st_mode & S_IFDIR) ||
            (st.st_mode & S_IFDIR_EX) ||
            (st.st_mode & S_IFMT) ||
            (st.st_mode & S_IFMT_EX));

    }

#endif
    return false;

}


// posBeg - first symbol to read from 0 
// posEnd - last symbol to read in the word for example: 'tree' posBeg = 0, posEnd = 2 reads 'tre'
bool readUTF8Text( const String &path, StringBasic &text ) {


/*#if CORE_PLATFORM == CORE_PLATFORM_WIN32
    std::wifstream fs8;
    fs8.open(path.c_str());
    tintaWString line;
#else*/
    std::ifstream fs8;
    fs8.open(path.c_str());
    String line;
//#endif

    if (!fs8.is_open()) {
        return false;
    }
    StringStream data;

    if (isUTF8WithBOM(path))
        fs8.seekg(UTF8BOM.length());


    while (getline(fs8, line)) {

/*#if CORE_PLATFORM == CORE_PLATFORM_WIN32
        //data text.push_back( StringUtil::createUTF8String(line) );
        data << StringUtil::createUTF8String(line);
#else*/
        data << line;
//#endif
        data << "\n";

    }
    //data.seekp(2, std::ios_base::end);
    text = data.str();

    return true;
}

/*bool  readUTF8Text( const String &path, StringVector &text ){

	


#if CORE_PLATFORM == CORE_PLATFORM_WIN32
    std::wifstream fs8;
    fs8.open(path.c_str());
    tintaWString line;
#else
    std::ifstream fs8;
    fs8.open(path.c_str());
    String line;
#endif

    if (!fs8.is_open()) {
        return false;
    }
    unsigned line_count = 1;
    //StringBasic line;

    StringStream data;

    if (isUTF8WithBOM(path))
        fs8.seekg(UTF8BOM.length());

    while ( getline(fs8, line) ) {

#if CORE_PLATFORM == CORE_PLATFORM_WIN32
        text.push_back(StringUtil::createUTF8String(line));
#else
        text.push_back(line);
#endif
        line_count++;
    }

    return true;

}*/

bool  readUTF8Text(const String &path, StringVector &text) {
    
    std::ifstream fs8;
    fs8.open(path.c_str());
    String line;


    if (!fs8.is_open()) {
        return false;
    }
    unsigned line_count = 1;
    //StringBasic line;

    StringStream data;

    if (isUTF8WithBOM(path))
        fs8.seekg(UTF8BOM.length());

    while (getline(fs8, line)) {


        text.push_back(line);

        line_count++;
    }

    return true;

}



bool writeUTF8toFile(const String &path, const StringVector &data, bool end ){

    bool noFile = !isPathValid(path);

    //char fill = ' ';
//#if CORE_PLATFORM == CORE_PLATFORM_WIN32
    std::ofstream dest;
   // dest << UTF8BOM;
    
//#else
  //  std::ofstream dest;
//#endif
    setlocale(LC_ALL, "utf-8");

    if (noFile)
        dest.open(path.c_str(), std::ios::out);
    else {
        if (end)
            dest.open(path.c_str(), std::ios::out | std::ios::in | std::ios::app | std::ios::binary );
        else
            dest.open(path.c_str(), std::ios::out | std::ios::in | std::ofstream::trunc);
    }

    for (size_t i = 0; i < data.size(); i++) {
//#if CORE_PLATFORM == CORE_PLATFORM_WIN32
       // tintaWString v = StringUtil::createUTF16String(data[i]);
        dest << data[i] << std::endl;
//#else
  //      dest << data[i] << std::endl;
//#endif
        dest.flush();
    }
   
    dest.close();
    return true;
}


bool writeUTF8toFile( const String &path, const StringSet &data, bool end ){

    bool noFile = !isPathValid(path);

    //char fill = ' ';
//#if CORE_PLATFORM == CORE_PLATFORM_WIN32
//    std::wofstream dest;
//#else
    std::ofstream dest;
//#endif
    setlocale(LC_ALL, "utf-8");

    if (noFile)
        dest.open(path.c_str(), std::ios::out);
    else {
        if (end)
            dest.open(path.c_str(), std::ios::out | std::ios::in | std::ios::app);
        else
            dest.open(path.c_str(), std::ios::out | std::ios::in | std::ofstream::trunc);
    }

    StringSet::const_iterator i = data.begin();
    for (; i != data.end(); i++) {
//#if CORE_PLATFORM == CORE_PLATFORM_WIN32
  //      dest << StringUtil::createUTF16String(*i) << std::endl;
//#else
        dest << *i << std::endl;
//#endif
        dest.flush();
    }

    dest.close();
    return true;
}



tintaVector3f getHSVfromRGB(float r, float g, float b){

    /*
    RGB - common colors (0-1),
    H - colors hue (0-360), 0 - red
    S - saturation (0-1)
    V - brightness (0-1)
    */

    float hVal = 0.f;
    float sVal = 0.f;
    float vVal = TintaMath::max(r, g, b);

    float hTemp = TintaMath::min(r, g, b);

    if (TintaMath::isZero(vVal)){
        // ambiguity 
        return tintaVector3f::origin;
    }
    else
        sVal = (vVal - hTemp) / vVal;
    
    float Cr = (vVal - r) / (vVal - hTemp);
    float Cg = (vVal - g) / (vVal - hTemp);
    float Cb = (vVal - b) / (vVal - hTemp);

    /*between yellow and purple*/

    if (TintaMath::isZero(r - vVal))
        hVal = Cb - Cg;

    /*between blue and yellow */

    if (TintaMath::isZero(g - vVal))
        hVal = 2 + Cr - Cb;

    /*betwen purple and blue*/

    if (TintaMath::isZero(b - vVal))
        hVal = 4 + Cg - Cr;

    hVal = hVal * 60;    

    if (hVal < 0.f)
        hVal = hVal + 360.f;

    return 	tintaVector3f(hVal, sVal, vVal);
}

float HueToRGB(float v1, float v2, float vH) {
    if (vH < 0.f)
        vH += 1.f;

    if (vH > 1.f)
        vH -= 1.f;

    if ((6.f * vH) < 1.f)
        return (v1 + (v2 - v1) * 6.f * vH);

    if ((2.f * vH) < 1.f)
        return v2;

    if ((3.f * vH) < 2.f)
        return (v1 + (v2 - v1) * ((2.0f / 3.f) - vH) * 6.f);

    return v1;
}

tintaVector3f rgbToHsl(const tintaVector3f &rgb) {

    float r = rgb.mx, g = rgb.my, b = rgb.mz;
    float max = TintaMath::max(r, g, b);
    float min = TintaMath::min(r, g, b);
    float h = 0, s = 0, l = (max + min) / 2.f;

    if (TintaMath::areEqual(max, min)) {
        h = s = 0.f;
    }
    else {
        float d = max - min;
        s = l > 0.5f ? d / (2 - max - min) : d / (max + min);
        if (TintaMath::areEqual(max, r)) {
            h = (g - b) / d + (g < b ? 6 : 0);
        }
        else if (TintaMath::areEqual(max, g)) {
            h = (b - r) / d + 2;
        }
        else if (TintaMath::areEqual(max, b)) {
            h = (r - g) / d + 4;
        }
        h /= 6.f;
    }
    return tintaVector3f(h, s, l);
}



tintaVector3f hslToRgb(const tintaVector3f &hsl) {
    float r, g, b;
    float h = hsl.mx, s = hsl.my, l = hsl.mz;
    if (TintaMath::isZero(s)) {
        r = g = b = l; // achromatic
    }
    else {
        float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        r = HueToRGB(p, q, h + 1.f / 3.f);
        g = HueToRGB(p, q, h);
        b = HueToRGB(p, q, h - 1.f / 3.f);
    }

    return tintaVector3f(r, g, b);
}
tintaVector3f getRGBfromHSV(const tintaVector3f &hsvVal){

    float      hh, p, q, t, ff;
    long        i;
    tintaVector3f  out;

    if (hsvVal.my <= 0.f) {       // < is bogus, just shuts up warnings
        out.mx = hsvVal.mx;
        out.my = hsvVal.my;
        out.mz = hsvVal.mz;
        return out;
    }

    hh = hsvVal.mx;

    if (hh >= 360.f)
        hh = 0.f;


    hh /= 60.f;

    i = (long)hh;

    ff = hh - i;
    p = hsvVal.mz * (1.f - hsvVal.my);
    q = hsvVal.mz * (1.f - (hsvVal.my * ff));
    t = hsvVal.mz * (1.f - (hsvVal.my * (1.f - ff)));

    switch (i) {
    case 0:
        out.mx = hsvVal.mz;
        out.my = t;
        out.mz = p;
        break;
    case 1:
        out.mx = q;
        out.my = hsvVal.mz;
        out.mz = p;
        break;
    case 2:
        out.mx = p;
        out.my = hsvVal.mz;
        out.mz = t;
        break;

    case 3:
        out.mx = p;
        out.my = q;
        out.mz = hsvVal.mz;
        break;
    case 4:
        out.mx = t;
        out.my = p;
        out.mz = hsvVal.mz;
        break;
    case 5:
    default:
        out.mx = hsvVal.mz;
        out.my = p;
        out.mz = q;
        break;
    }
    return out;


}

float spline(float x, int nknots, float *knot){
    int span;
    int nspans = nknots - 3;
    float cO, cl, c2, c3; /* coefficients of the cubic.*/
    if (nspans < 1)
        return 0;

    /* Find the appropriate 4-point span of the spline. */
    x = clamp(x, 0, 1) * nspans;
    span = (int)x;
    if (span >= nknots - 3)
        span = nknots - 3;
    x -= span;
    knot += span;
    /* Evaluate the span cubic at x using Horners rule. */
    c3 = tintaMatrix44::basis[0][0] * knot[0] + tintaMatrix44::basis[0][1] * knot[1] + tintaMatrix44::basis[0][2] * knot[2] + tintaMatrix44::basis[0][3] * knot[3];
    c2 = tintaMatrix44::basis[1][0] * knot[0] + tintaMatrix44::basis[1][1] * knot[1] + tintaMatrix44::basis[1][2] * knot[2] + tintaMatrix44::basis[1][3] * knot[3];
    cl = tintaMatrix44::basis[2][0] * knot[0] + tintaMatrix44::basis[2][1] * knot[1] + tintaMatrix44::basis[2][2] * knot[2] + tintaMatrix44::basis[2][3] * knot[3];
    cO = tintaMatrix44::basis[3][0] * knot[0] + tintaMatrix44::basis[3][1] * knot[1] + tintaMatrix44::basis[3][2] * knot[2] + tintaMatrix44::basis[3][3] * knot[3];

    return ((c3*x + c2)*x + cl)*x + cO;
}

size_t writeUTF8toFile( const String &path, const String &data ){

    bool noFile = !isPathValid(path);
    //char fill = ' ';

#if CORE_PLATFORM == CORE_PLATFORM_WIN32
    std::wofstream dest;
#else
    std::ofstream dest;
#endif

    setlocale(LC_ALL, "utf-8");
    if (noFile)
        dest.open(path.c_str(), std::ios::out);
    else
        dest.open(path.c_str(), std::ios::out | std::ios::in);

    if (!dest.is_open()) {
        return 0;
    }
#if CORE_PLATFORM == CORE_PLATFORM_WIN32
    dest << StringUtil::createUTF16String(data);
#else
    dest << data;
#endif

    size_t rez = (size_t)dest.tellp();
    dest.flush();
    dest.close();

    return rez;
}


bool   isUTF8WithBOM( const String &path ) {

    std::ifstream fs8;

    fs8.open( path.c_str() );

    if (!fs8.is_open()) {
        return false;
    }
    String line;
    getline(fs8, line);
    bool rez = false;
    if (line.length() > 0
        && line.at(0) == UTF8BOM[0]
        && line.at(1) == UTF8BOM[1]
        && line.at(2) == UTF8BOM[2]) {

        rez = true;
    }

    fs8.close();

    return rez;
}


long getFileSize(const String &path){

    std::ifstream infile;

    infile.open(path.c_str());
    infile.seekg(0, infile.end);

    long length = (long)infile.tellg();

    return length;
}



bool createProcess(const String &path_to_process, char_m *params) {

#if CORE_PLATFORM == CORE_PLATFORM_WIN32

    //char* lpApplicationName = "C:/Windows/System32/cmd.exe"; /* The program to be executed */
    STARTUPINFO  lStartupInfo;
    PROCESS_INFORMATION  lProcessInfo;

    memset(&lStartupInfo, 0, sizeof(lStartupInfo));
    memset(&lProcessInfo, 0, sizeof(lProcessInfo));

    if (!CreateProcess(path_to_process.c_str(), params, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &lStartupInfo, &lProcessInfo))
        return false;

#endif

    return true;
}



	/** General hash function, derived from here
	http://www.azillionmonkeys.com/qed/hash.html
	Original by Paul Hsieh 
	*/

#if CORE_ENDIAN == CORE_ENDIAN_LITTLE
#  define GET16BITS(d) (*((const m_uint16 *) (d)))
#else
	// Cast to m_uint16 in little endian means first m_uint8 is least significant
	// replicate that here
#  define GET16BITS(d) (*((const uint8 *) (d)) + (*((const uint8 *) (d+1))<<8))
#endif
	//hashSoFar - hash offset
	m_uint32 _CoreExport FastHash (const char * data, int len, m_uint32 hashSoFar)
	{
		m_uint32 hash;
		m_uint32 tmp;
		int rem;

		if (hashSoFar)
			hash = hashSoFar;
		else
			hash = len;

		if (len <= 0 || data == NULL) return 0;

		rem = len & 3;
		len >>= 2;

		/* Main loop */
		for (;len > 0; len--) {
			hash  += GET16BITS (data);
			tmp    = (GET16BITS (data+2) << 11) ^ hash;
			hash   = (hash << 16) ^ tmp;
			data  += 2*sizeof (m_uint16);
			hash  += hash >> 11;
		}

		/* Handle end cases */
		switch (rem) {
		case 3: hash += GET16BITS (data);
			hash ^= hash << 16;
			hash ^= data[sizeof (m_uint16)] << 18;
			hash += hash >> 11;
			break;
		case 2: hash += GET16BITS (data);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1: hash += *data;
			hash ^= hash << 10;
			hash += hash >> 1;
		}
		    
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}



    StringVector getFiles(const String &path, const getFileProp &prop, int *quantity  ) {



        String path_(path);
        if (path.length() > 0 && path.back() != _M('/'))
            path_.append(_M("/"));

        StringVector rez;
        rez.reserve(10);




    #	if 	CORE_PLATFORM == CORE_PLATFORM_WIN32
        String req(path_);
        req.append(_M("*"));

        WIN32_FIND_DATA FindFileData;
        HANDLE hf;
        hf = FindFirstFile(req.c_str(), &FindFileData);
        int pos{ 0 };

        if ( hf != INVALID_HANDLE_VALUE ){

            do
            {
                String r(FindFileData.cFileName);
                String ex(StringUtil::getFileExt(r));
                if ( ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0
                    && r != _M(".") && r != _M("..") 
                                && (ex == prop.mExt || prop.mExt.length() == 0 || prop.mExt == _M("*")) ){
                   
                    if ( !quantity ) {
                        
                        if ( prop.mPos == -1 || prop.mPos == pos ) {
                            if ( prop.mOnlynames )
                                rez.push_back(r);
                            else {
                                String rezStr{ path_ };
                                rezStr.append(r);
                                rez.push_back(rezStr);
                            }
                            if ( prop.mPos == pos )
                                return rez;
                        }
                        
                    }
                    pos++;
                }

            } while ( FindNextFile(hf, &FindFileData ) != 0 );

            if (quantity) 
                *quantity = pos;           
            else 
                std::sort( rez.begin(), rez.end() );       

            FindClose(hf);
        }


    #else

         DIR *dp;
         struct dirent *ep;
   
         dp = opendir ( path_.c_str() );
    

         if ( dp ) {
        	int pos{ 0 };
    		String exProp {prop.mExt};
    		StringUtil::toLower(exProp);

        	while ( ep = readdir (dp)  ){
        		if( ep->d_type == DT_REG ){
   
        			String r { ep->d_name };
    
        			String ex {StringUtil::getFileExt(r)};
        			StringUtil::toLower(ex);

        			if(exProp.length()== 0 || ex == exProp ){
    					if(prop.mPos == -1 || prop.mPos == pos){
    						if (prop.mOnlynames)
    							rez.push_back( r );
    						else {
    							String rezStr{ path_ };
    							rezStr.append(r);
    							rez.push_back(rezStr);
    						}

    					}
    					pos++;
        			}
        		}
        	}
        	if(quantity)
        			  *quantity = pos;

        	(void) closedir (dp);
          }

    #endif

            return rez;



    } 

    StringVector  getDirs(const String &path, const getDirProp &prop, int *quantity ) {


        String path_(path);
            if ( path.length() > 0 && path.back() != _M('/') )
                path_.append( _M("/") );

            StringVector rez;
            rez.reserve(10);


        #	if 	CORE_PLATFORM == CORE_PLATFORM_WIN32
            String req(path_);
            req.append(_M("*"));
            WIN32_FIND_DATA FindFileData;
            HANDLE hf;
            hf = FindFirstFile( req.c_str(), &FindFileData );
            int pos { 0 };

            if ( hf != INVALID_HANDLE_VALUE ){

                    do
                    {
                        String r( FindFileData.cFileName );
                        if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
                                            && r != _M(".") && r != _M("..") ){
                            if ( !quantity ) {
                                if ( prop.mPos == -1 || prop.mPos == pos ) {
                                    if ( prop.mOnlynames )
                                        rez.push_back(r);
                                    else {
                                        String rezStr{ path_ };
                                        rezStr.append(r);
                                        rez.push_back(rezStr);
                                    }
                                }
                            }
                            pos++;
                        }

                    } while ( FindNextFile(hf, &FindFileData ) != 0);

                    if ( quantity )
                        *quantity = pos;


                FindClose(hf);
            }


        #else

          DIR *dp;
          struct dirent *ep;
      
          dp = opendir ( path_.c_str() );
        

          if ( dp ) {
        	  int pos{ 0 };
        	  while ( (ep = readdir (dp))  ){
        			StringBasic d(ep->d_name);
        			if( ep->d_type == DT_DIR &&  d!= "." && d!= ".." ){
        				if( prop.mPos == -1 || prop.mPos == pos ){
        
        					String r {ep->d_name};
        
        					if ( prop.mOnlynames )
        						rez.push_back( r );
        					else {
        						String rezStr{ path_ };
        						rezStr.append(r);
        						rez.push_back(rezStr);
        					}
        				}
        				pos++;
        			}
        	  }
        	  if(quantity)
        		  *quantity = pos;

        	  (void) closedir (dp);
          }

        #endif

        return rez;
    }

    bool isDirectory( const String &path ){

        struct stat buf;

        int result;
     
        result = stat(path.c_str(), &buf);

        if (result == 0){
            return ((buf.st_mode & S_IFDIR) || (buf.st_mode & S_IFDIR_EX)) == 1;
        }
        return false;
    }

    bool isFile(const String &path){


        struct stat buf;

        int result;

        result = stat(path.c_str(), &buf);

        if (result == 0){
            return ((buf.st_mode & S_IFMT) || (buf.st_mode & S_IFMT_EX)) == 1;
        }
        return false;
    }
    
    

    String getRootPath(){

#if CORE_PLATFORM == CORE_PLATFORM_WIN32

        // Get the current working directory:
        char_m *buffer;

        buffer = _getcwd(NULL_M, 0);


 
        if (buffer){

            String rez(buffer);
            return rez;
        }

#else 
        char* buffer;
        buffer = getcwd(NULL, 0);
        if ( buffer ){
            StringBasic basicPath( buffer );
            
            String rez;
            rez = basicPath;

            return rez;
        }
#endif
        return String();

    }

    
     
   

    bool createFolder(const String &path){

        //return boost::filesystem::create_directory(path);
        String pathDir(path);
        bool rez = true;


        if (StringUtil::haveFileExtension(pathDir)){
            StringUtil::delLastLeaf(pathDir, _M("\\"), _M("/"));
        }

        // already exists or Windows drive
        if (isPathValid(pathDir))
            return rez;

        StringVector leaves;

        String path_to_fold_old;
        while ( !isPathValid( pathDir ) ){

            path_to_fold_old = pathDir;
#if CORE_PLATFORM == CORE_PLATFORM_WIN32

            int error = SHCreateDirectoryEx(0, pathDir.c_str(), 0);

            rez = error == 0;
#else

            rez = (mkdir(pathDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);

#endif
            if (rez)
                break;

            String leaf = StringUtil::delLastLeaf(pathDir, _M("\\"), _M("/"));

            if (leaf.length() > 0)
                leaves.push_back(leaf);

            if (leaves.size() == 0)
                break;
            // preventing  infinite looping
            if (path_to_fold_old == pathDir)
                break;
        }
        //creating all directories in path
        while (leaves.size()) {
            StringUtil::addLeaf(pathDir, leaves.back());
            leaves.pop_back();
#if CORE_PLATFORM == CORE_PLATFORM_WIN32

            int error = SHCreateDirectoryEx(0, pathDir.c_str(), 0);

            rez = rez && (error == 0);
#else

            rez = (mkdir(pathDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
#endif

        }

        return rez;
    }

    

}
