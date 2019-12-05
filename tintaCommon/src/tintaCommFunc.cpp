/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#include "tintaCommFunc.h"
#include "tintaPredefine.h"
#include "tintaLogger.h"
#include "tintaTreeConfig/tintaTreeConfig.h"

#include <tintaArray2d.h>

#if TINTA_LOCAL  ==  TINTA_RU
    #include "tintaUtilLocalRu.h"
#elif TINTA_LOCAL  ==  TINTA_EN
    #include "tintaUtilLocalEn.h"
#endif


namespace  Tinta {

    namespace tintaUtilFunc {


        /* 
            util. namespace functions
        */
        static const luaL_Reg util[] = {          
            { UTIL_getval, Tinta::tintaUtilFunc::getval },
            { UTIL_cpucores, Tinta::tintaUtilFunc::cpucores },
            { UTIL_msg, Tinta::tintaUtilFunc::msg },
            { UTIL_msgf, Tinta::tintaUtilFunc::msgf },
            { UTIL_bytetofloat, Tinta::tintaUtilFunc::bytetofloat },
            { UTIL_floattobyte, Tinta::tintaUtilFunc::floattobyte },
            { UTIL_linterp, Tinta::tintaUtilFunc::linterp },
            { UTIL_cosinterp, Tinta::tintaUtilFunc::cosinterp },
            { UTIL_cubeinterp, Tinta::tintaUtilFunc::cubeinterp },
            { UTIL_smoothstep, Tinta::tintaUtilFunc::smoothstep },
            { UTIL_sleep, Tinta::tintaUtilFunc::sleep },
            { UTIL_countfiles, Tinta::tintaUtilFunc::countfiles },
            { UTIL_getfile, Tinta::tintaUtilFunc::getfile },
            { UTIL_getdirectory, Tinta::tintaUtilFunc::getdirectory },
            { UTIL_countdirectories, Tinta::tintaUtilFunc::countdirectories },
            { UTIL_isdirectory, Tinta::tintaUtilFunc::isdirectory },
            { UTIL_error, Tinta::tintaUtilFunc::error },
            { UTIL_pow, Tinta::tintaUtilFunc::pow },
            { UTIL_packcolor, Tinta::tintaUtilFunc::packcolor },
            { UTIL_unpackcolor, Tinta::tintaUtilFunc::unpackcolor },
            { UTIL_rgbtohsl, Tinta::tintaUtilFunc::rgbtohsl },
            { UTIL_hsltorgb, Tinta::tintaUtilFunc::hsltorgb },
            { UTIL_fromdegtorad, Tinta::tintaUtilFunc::fromdegtorad },
            { UTIL_fromradtodeg, Tinta::tintaUtilFunc::fromradtodeg },
            { UTIL_regexmatch, Tinta::tintaUtilFunc::regexmatch },
            { UTIL_writetofile, Tinta::tintaUtilFunc::writetofile },
            { NULL, NULL } };


        int luaopen_util(lua_State *L) {
            luaL_newlib(L, util);
            return 1;
        }
        static const luaL_Reg loadedlibs[] = {
            { "_G", luaopen_base },
            { "util", luaopen_util },
            { NULL, NULL } };


        int getval(SCRIPT_STATE *L) {


            String path = GET_VAL_STRING_EX(L, 1);
            String request = GET_VAL_STRING_EX(L, 2);
            String type = GET_VAL_STRING_EX(L, 3);

            namespace mf = TreeConfig;
            mf::tintaTreeConfig conf;

            
            if ( !conf.parse( path ) ) {
                
                String err = conf.getError();
               
                if (Tinta::tintaLogger::getPtr())
                      Tinta::tintaLogger::getPtr()->logMsg(err, msg_error);
               
                return 0;
            }

            if ( type == _M("double") ) {
                double v(0.);
                if ( conf.getValue(request, v) ) {
                    PUSH_DOUBLE_ARG(L, v);
                    return 1;
                }
            }
            else if (type == _M("string")) {
                String v;
                if (conf.getValue(request, v)) {
                    PUSH_STRING_ARG(L, v.c_str());
                    return 1;
                }
            }
            else if (type == _M("bool")) {
                bool v;
                if (conf.getValue(request, v)) {
                    PUSH_BOOL_ARG(L, v);
                    return 1;
                }
            }                
            else {
                if ( Tinta::tintaLogger::getPtr() ) {

                    StringStream msg;
                    msg << _M("Wrong type: ");
                    msg << type;
                    msg << _M(", only \"double\",\"string\",\"bool\" permitted.");
                    if( Tinta::tintaLogger::getPtr() )
                        Tinta::tintaLogger::getPtr()->logMsg(msg.str(), msg_error);

                    
                }
            }           
            
            String err = conf.getError();

            if (err.length() > 0) {

                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(err, msg_error);

            }

            return 0;
            
        }
        int cpucores(SCRIPT_STATE *L) {    


            int cores = 1;

#define TEXSPRING_THREADS THREAD_HARDWARE_CONCURRENCY
            cores = TEXSPRING_THREADS;


            PUSH_INT_ARG(L, cores); //			
            return 1;
        }


        int msg(SCRIPT_STATE *L) {
            
            int argc = GET_QUANTITY(L); //1 - int val - image index
            StringStreamBasic logname;
            for (int n = 1, i = 0; n <= argc; ++n, i++)
            {
                if ( IS_VAL_BOOL(L, n) )
                    logname << GET_VAL_BOOL(L, n);
                else if ( IS_VAL_STRING(L, n) ) {
//#ifdef _DEBUG
                   // StringBasic out = GET_VAL_STRING(L, n);
//#endif
                    logname << GET_VAL_STRING(L, n);
                }
                else if ( IS_VAL_REAL(L, n) )
                    logname << GET_VAL_DOUBLE(L, n);
            }
            StringBasic str( logname.str() );
            if ( Tinta::tintaLogger::getPtr() )
                Tinta::tintaLogger::getPtr()->logMsg(toWideChar(logname.str()), msgLevel::msg_info);

            return 0;

        }

         

        int writetofile( SCRIPT_STATE *L) {

            
            StringBasic path = GET_VAL_STRING(L, 1);
           
            String data = GET_VAL_STRING_EX(L, 2);

            if (path.length() == 0) {
                StringStreamBasic msg;
                msg << "Wrong path: " << path.c_str();
                if ( Tinta::tintaLogger::getPtr() )
                    Tinta::tintaLogger::getPtr()->logMsg(toWideChar(msg.str()));
                return 0;
            }
            bool rez = false;
            String realPath = toWideChar(path);
            
            rez = writeUTF8toFile(realPath, StringVector{ data }, true);
            

            return 0;
        }

        int msgf(SCRIPT_STATE *L) {
            

            int argc = GET_QUANTITY(L); //1 - int val - image index
            StringStreamBasic logname;
            for (int n = 1, i = 0; n <= argc; ++n, i++)
            {
                if (IS_VAL_BOOL(L, n))
                    logname << GET_VAL_BOOL(L, n);

                else if (IS_VAL_STRING(L, n))
                    logname << GET_VAL_STRING(L, n);


                else if (IS_VAL_REAL(L, n))
                    logname << GET_VAL_DOUBLE(L, n);

            }
            if ( Tinta::tintaLogger::getPtr() )
                Tinta::tintaLogger::getPtr()->logMsg(toWideChar(logname.str()), msg_info, false);

            return 0;

        }


        int bytetofloat(SCRIPT_STATE *L){           

            int argc = GET_QUANTITY(L); //1 - int val - image index

            if (argc != 1){
            
                return 0;
            }

            float rez = byteToFloat(GET_VAL_UBYTE(L, 1));

            PUSH_DOUBLE_ARG(L, (double)rez); // return value

            return 1;

        }
        int floattobyte(SCRIPT_STATE *L) {
            

            int argc = GET_QUANTITY(L); //1 - int val - image index

            if (argc != 1){
                
                return 0;
            }

            m_byte rez = floatToByte(GET_VAL_FLOAT(L, 1));
            PUSH_INT_ARG(L, (int)rez); // return value

            return 1;

        }


        int linterp(SCRIPT_STATE *L) {
            

            int argc = GET_QUANTITY(L);

            if (argc != 3){

                return 0;
            }
            float rez = TintaMath::lInterp<float>(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3));
            PUSH_FLOAT_ARG(L, (float)rez); // return value

            return 1;

        }

        int cosinterp(SCRIPT_STATE *L) {
           

            int argc = GET_QUANTITY(L);

            if (argc != 3){

                
                return 0;
            }

            float rez = TintaMath::cosInterp<float>(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3));

            PUSH_FLOAT_ARG(L, (float)rez); // return value

            return 1;

        }
        int cubeinterp(SCRIPT_STATE *L) {
           

            int argc = GET_QUANTITY(L);

            if (argc != 5){

                
                return 0;
            }
            float rez = TintaMath::cubeInterp<float>(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3), GET_VAL_FLOAT(L, 4), GET_VAL_FLOAT(L, 5));

            PUSH_FLOAT_ARG(L, (float)rez); // return value

            return 1;

        }

        int smoothstep(SCRIPT_STATE *L){
           

            int argc = GET_QUANTITY(L);

            if (argc != 3){
                
                return 0;
            }
            float rez = Tinta::smoothstep(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2), GET_VAL_FLOAT(L, 3));
            PUSH_DOUBLE_ARG(L, (double)rez); // return value

            return 1;

        }


        int sleep(SCRIPT_STATE *L){

            unsigned imgId = GET_VAL_UINT(L, 1);
            THREAD_SLEEP(imgId);

            return 0;
        }


        /*
            Returns files quantity by the condition
            param - absolute or relative path to directory
            param - extension
            return quantity
        */
        int countfiles(SCRIPT_STATE *L){



            String ext = IS_VAL_STRING(L, 2) ? toWideChar(GET_VAL_STRING(L, 2)) : _M("");

            if (ext == _M("*"))
                ext = String();

            StringUtil::toLower(ext);

            String ext_{ ext };
#ifdef FILESYSTEM_BOOST
            // adding '.' in front
            if (ext_.length()  > 0 && ext_.at(0) != _M('.')){
                ext_.clear();
                ext_.append(_M("."));
                ext_.append(ext);
            }
#endif

            getFileProp propFiles(true, ext_, 0);
            int rez = 0;

            String fillPath(toWideChar(GET_VAL_STRING(L, 1)));

            if (!isDirectory(fillPath)){

                StringStreamBasic msg;
                msg << "Wrong path: ";
                msg << GET_VAL_STRING(L, 1);
                //TROW_ERR_FUNC_CLEAR(L, msg.str().c_str());
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(toWideChar(msg.str()));
                return 0;
            }

            getFiles(fillPath, propFiles, &rez);

            PUSH_INT_ARG(L, rez);

            return 1;
        }


        /*
        Returns file by the condition
        param - absolute or relative path to directory
        param - position of file in list for iterating using countfiles
        param - extension
        param - if false returns full path to file, if true only full filename
        return path
        */
        int getfile(SCRIPT_STATE *L){

           

            String dir = toWideChar(GET_VAL_STRING(L, 1));

            if ( !isDirectory( dir ) ){

                StringStreamBasic msg;
                msg << "Wrong path: ";
                msg << GET_VAL_STRING(L, 1);
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(toWideChar(msg.str()));
                return 0;
            }

            String ext = IS_VAL_STRING(L, 3) ? toWideChar(GET_VAL_STRING(L, 3)) : _M("");

            if (ext == _M("*"))
                ext = String();
            StringUtil::toLower(ext);
            String ext_(ext);
#ifdef FILESYSTEM_BOOST
            if (ext_.length()  > 0 && ext_.at(0) != _M('.')){
                ext_.clear();
                ext_.append(_M("."));
                ext_.append(ext);
            }
#endif
            //StringUtil::toLower(ext_);

            getFileProp propFiles(GET_VAL_BOOL(L, 4), ext_, GET_VAL_INT(L, 2));

            StringVector rez = getFiles(dir, propFiles, NULL);
            
            if ( rez.size() == 0 )
                return 0;

            PUSH_STRING_ARG(L, rez[0].c_str());
            return 1;

        }

        /*
        Returns directory by the condition
        param - absolute or relative path to directory where to find
        param - position of file in list for iterating using countdirectories
        param - if true returns full path to file, if false only full filename
        return path
        */
        _CoreExport int getdirectory(SCRIPT_STATE *L) {
           

            String dir = toWideChar(GET_VAL_STRING(L, 1));

            if (!isDirectory(dir)){

                StringStreamBasic msg;
                msg << "Wrong path: ";
                msg << GET_VAL_STRING(L, 1);
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(toWideChar(msg.str()));
                return 0;
            }

            getDirProp prop(GET_VAL_BOOL(L, 3), GET_VAL_INT(L, 2));

            StringVector rez = getDirs(dir, prop, NULL);

            if (rez.size() == 0)
                return 0;

            PUSH_STRING_ARG(L, rez[0].c_str());
            return 1;

        }

        /*
        Returns directories quantity by the condition
        param  absolute or relative path to directory
        return quantity
        */
        int countdirectories(SCRIPT_STATE *L) {
          

            String dir = toWideChar(GET_VAL_STRING(L, 1));

            if ( !isDirectory( dir ) ){

                StringStreamBasic msg;
                msg << "Wrong path: ";
                msg << GET_VAL_STRING(L, 1);
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(toWideChar(msg.str()));
                return 0;
            }

            getDirProp propDir;
            int rez = 0;
            getDirs(dir, propDir, &rez);

            PUSH_INT_ARG(L, rez);

            return 1;
        }

        /*
        Determine is absolute path is directory
        */
        int isdirectory(SCRIPT_STATE *L) {

            String param = toWideChar(GET_VAL_STRING(L, 1));
            PUSH_BOOL_ARG(L, isDirectory(param));
            return 1;
        }

        /*
        pow function
        */
        int pow(SCRIPT_STATE *L){
            
            PUSH_FLOAT_ARG(L, TintaMath::pow<m_float32>(GET_VAL_FLOAT(L, 1), GET_VAL_FLOAT(L, 2)));
            return 1;

        }

        int error( SCRIPT_STATE *L ) {

            bool vExp = GET_VAL_BOOL(L, 1);
            if( !vExp ){
                StringBasic msg = GET_VAL_STRING( L, 2 );
                if ( msg.size() > 0 ) 
                    TROW_ERR_FUNC(L, msg.c_str());                
                else
                    TROW_ERR_FUNC(L, "Assert has occurred!");                
            }
            return 0;
        }

        /*
        Packs from byte form R G B into the unsigned form 0x00000
        @param  byte R value
        @param  byte G value
        @param  byte B value
        @return  - unsigned packed result
        */
        int packcolor(SCRIPT_STATE *L){
            

            m_byte rv = GET_VAL_UBYTE(L, 1);
            m_byte gv = GET_VAL_UBYTE(L, 2);
            m_byte bv = GET_VAL_UBYTE(L, 3);

            //const tintaVector3f &color
            m_uint32 r = packColor3(rv, gv, bv);

            PUSH_UINT_ARG(L, r);

            return 1;
        }

        /*
        Unpacks the unsigned form 0x00000  in to the byte form R G B
        @param  - unsigned packed value
        @return  byte R result
        @return  byte G result
        @return  byte B result
        */
        int unpackcolor(SCRIPT_STATE *L){

           

            m_uint32 v = GET_VAL_UINT(L, 1);

            m_byte r(0), g(0), b(0);

            //const tintaVector3f &color
            unpackColor3(v, r, g, b);

            PUSH_UINT_ARG(L, r);
            PUSH_UINT_ARG(L, g);
            PUSH_UINT_ARG(L, b);

            return 3;
        }

        int rgbtohsl(SCRIPT_STATE *L) {
            
            tintaVector3f r = rgbToHsl(tintaVector3f(GET_VAL_FLOAT(L, 1),
                GET_VAL_FLOAT(L, 2),
                GET_VAL_FLOAT(L, 3)));

            PUSH_FLOAT_ARG(L, r.mx);
            PUSH_FLOAT_ARG(L, r.my);
            PUSH_FLOAT_ARG(L, r.mz);

            return 3;
        }


        int hsltorgb(SCRIPT_STATE *L) {
            

            tintaVector3f r = hslToRgb(tintaVector3f(GET_VAL_FLOAT(L, 1),
                GET_VAL_FLOAT(L, 2),
                GET_VAL_FLOAT(L, 3)));

            PUSH_FLOAT_ARG(L, r.mx);
            PUSH_FLOAT_ARG(L, r.my);
            PUSH_FLOAT_ARG(L, r.mz);

            return 3;
        }

        int fromdegtorad(SCRIPT_STATE *L){
            

            PUSH_FLOAT_ARG(L, TintaMath::toRad(GET_VAL_FLOAT(L, 1)));
            return 1;
        }

        int fromradtodeg(SCRIPT_STATE *L) {            

            PUSH_FLOAT_ARG(L, TintaMath::toDeg(GET_VAL_FLOAT(L, 1)));
            return 1;
        }

        int regexmatch(SCRIPT_STATE *L) {
#if         	CORE_COMPILER == CORE_COMPILER_MSVC
            try {
                regex_t regex(toWideChar(GET_VAL_STRING(L, 1)));

                String text = toWideChar(GET_VAL_STRING(L, 2));
                m_uint32 mask = regex_constants::match_default; // def 0
				if (IS_VAL_INTEGER(L, 3))
                    mask = GET_VAL_UINT(L, 3);

                int rez = std::regex_match(text, regex, (regex_constants::match_flag_type)mask);

                PUSH_INT_ARG(L, rez);
                return 1;
            }
            catch (...){
                if (Tinta::tintaLogger::getPtr())
                    Tinta::tintaLogger::getPtr()->logMsg(_M("Regex error!"));
            }
#endif
            return 0;
        }



        void registerUtilLua(SCRIPT_STATE *L) {

            //luaL_newlib(L, util);           

            const luaL_Reg *lib;
            /* "require" functions from 'loadedlibs' and set results to global table */
            for (lib = loadedlibs; lib->func; lib++) {
                luaL_requiref(L, lib->name, lib->func, 1);
                lua_pop(L, 1);  /* remove lib */
            }
        }



	}


}
