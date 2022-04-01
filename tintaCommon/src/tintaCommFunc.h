/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_COMMON_FUNCS_H_
#define _TINTA_COMMON_FUNCS_H_

#include "tintaPredefine.h"
#include <tintaScriptContext.h>


	namespace Tinta	{

        /*
            Common scripts functions
        */
        namespace tintaUtilFunc {



            /*
            @name util.system
            @description Execute api system function system("command_text")
            @param String  - command                                    
            @return Integer - result
            */
            _CoreExport int system(SCRIPT_STATE *L);


            /*
            @name util.getval
            @description Return value from tree config file
            @param String  - path 
            @param String  - request
            @param String  - type return("double","string","bool")
            @return Real,String,bool 
            */
            _CoreExport int getval(SCRIPT_STATE *L);


            /*
            @name util.cpucores
            @description Return CPU cores	            
            @return cpu avalible cores
            */
            _CoreExport int cpucores(SCRIPT_STATE *L);


            /*
            @name util.msg
            @description	 Print msg to the console and into the log file if used one
            @param Bool,String,Number - parameters comma separated  e.g.(  util.msg("test",100,true) )
            */
            _CoreExport int msg(SCRIPT_STATE *L);
           

            /*
            @name util.msgf
            @description	Print message into the log file
            @param Bool,String,Number parameters comma separated e.g.( util.msgf("test",100,true) )
            */
            _CoreExport int msgf(SCRIPT_STATE *L);

            /*
            @name util.timestamp
            @description Log time stamps in log messages	
            @param Bool true - show time stamps in log
            */
            _CoreExport int timestamp(SCRIPT_STATE *L);
		


            /*
            @name util.bytetofloat
            @description	Convert byte value to the float
            @param byte value
            @return - Real data
            */
            _CoreExport int bytetofloat(SCRIPT_STATE *L);

            /*
            @name util.floattobyte
            @description	Convert float value to the byte
            @param Real value
            @return - byte data
            */
            _CoreExport int floattobyte(SCRIPT_STATE *L);



            /*
            @name util.linterp
            @description	Implement linear interpolation
            @param Real param value
            @param Real param value
            @param Real param value[0,1]
            @return - Real
            */
            _CoreExport int linterp(SCRIPT_STATE *L);

            /*
            @name util.c_cosinterp
            @description	Implement Cos interpolation
            @param Real param value
            @param Real param value
            @param Real param value[0,1]
            @return - Real
            */
            _CoreExport int cosinterp(SCRIPT_STATE *L);

            /*
            @name util.cubeinterp
            @description	Implement Cubic interpolation
            @param param value
            @param param value
            @param param value
            @param param value
            @param param value[0,1]
            @return - Real
            */
            _CoreExport int cubeinterp(SCRIPT_STATE *L);


            /*
            @name util.smoothstep
            @description	Implement smooth step function
            @param Real param
            @param Real param
            @param Real param
            @return - Real
            */
            _CoreExport int smoothstep(SCRIPT_STATE *L);


            /*
            @name util.sleep
            @description	Send to sleep executing thread
            @param UInt - time ms
            */
            _CoreExport int sleep(SCRIPT_STATE *L);    


            /*
            @name util.fopened
            @description	Return opened file in the another process flag
            @param String path to the file
            @return - Boolean
            */
            _CoreExport int fopened(SCRIPT_STATE *L);



            /*
            @name util.countfiles
            @description	Return files quantity by the condition
            @param String  absolute or relative path to directory where to count
            @param String = "" - extension
            @return Int quantity
            */
            _CoreExport int countfiles(SCRIPT_STATE *L);


            /*	
            @name util.getfile
            @description	Return file by the condition in directory
            @param String -  absolute or relative path to directory where to find
            @param UInt - position of file in list for iterating using c_countfiles
            @param String = "" - extension
            @param Bool ;- if false returns full path to file, if true only filename
            @return quantity

            */
            _CoreExport int getfile(SCRIPT_STATE *L);


            /*
            @name util.getdirectory
            @description	Return directory by the condition
            @param - absolute or relative path to directory where to find
            @param - position of file in list for iterating using c_countfiles
            @param - if true returns full path to file, if false only full filename
            @return quantity
            */
            _CoreExport int getdirectory(SCRIPT_STATE *L);


            /*
            @name util.countdirectories
            @description	Return directories quantity by the condition
            @param - absolute or relative path to directory	where to count
            @return quantity
            */
            _CoreExport int countdirectories(SCRIPT_STATE *L);


            /*
            @name util.isdirectory
            @description	Determine if the path is directory
            */
            _CoreExport int isdirectory(SCRIPT_STATE *L);


            _CoreExport int  error( SCRIPT_STATE *L );

            /*
            @name util.pow
            @description	pow function
            */
            _CoreExport int pow(SCRIPT_STATE *L);

            /*
            @name util.packcolor
            @description	Pack from byte form R G B into the unsigned form 0x00000
            @param  byte R value
            @param  byte G value
            @param  byte B value
            @return  - unsigned packed result
            */
            _CoreExport int packcolor(SCRIPT_STATE *L);

            /*
            @name util.unpackcolor
            @description	Unpack the unsigned form 0 in to the byte form R G B
            @param  - unsigned packed value
            @return  byte R result
            @return  byte G result
            @return  byte B result
            */
            _CoreExport int unpackcolor(SCRIPT_STATE *L);


            /*
            @name util.rgbtohsl
            @description	Convert RGB to HSL
            H - colors hue (0-360), 0 - red
            S - saturation (0-1)
            L - lightness (0-1)
            @param Real r
            @param Real g
            @param Real b
            @return Real h
            @return Real s
            @return Real l
            */
            _CoreExport int rgbtohsl(SCRIPT_STATE *L);

            /*
            @name util.hsltorgb
            @description	Convert HSL to RGB
            H - colors hue (0-360), 0 - red
            S - saturation (0-1)
            L - lightness (0-1)
            @param Real h
            @param Real s
            @param Real l
            @return Real r
            @return Real g
            @return Real b
            */
            _CoreExport int hsltorgb(SCRIPT_STATE *L);


            /*
            @name util.fromdegtorad
            @description	Convert angle from degree to radians
            @param Real angle
            */
            _CoreExport int fromdegtorad(SCRIPT_STATE *L);

            /*
            @name util.fromradtodeg
            @description	Convert angle from radians to degree
            @param Real angle
            */
            _CoreExport int fromradtodeg(SCRIPT_STATE *L);

            /*
            @name util.regexmatch
            @description	Find matches using regular expressions
                for symbol "\" use "\\"
            @param string regular expression
            @param string text
            @param int match mask:  match_default = 0x0000, match_not_bol = 0x0001, match_not_eol = 0x0002, match_not_bow = 0x0004, match_not_eow = 0x0008,      match_any = 0x0010,match_not_null = 0x0020,match_continuous = 0x0040,_Match_partial = 0x0080, match_prev_avail = 0x0100,format_default = 0x0000,      format_sed = 0x0400,format_no_copy = 0x0800,format_first_only = 0x1000,_Match_not_null = 0x2000
            @return int matches count
            */
            _CoreExport int regexmatch(SCRIPT_STATE *L);


            /*@name util.writetofile
            @description Write text in to the file
            @param path            
            @param string data string
            */
            _CoreExport int writetofile(SCRIPT_STATE *L);



            _CoreExport void registerUtilLua(SCRIPT_STATE *L); 


        /*
            @name image.create
            @description Create image
            @param UInt  - width
            @param UInt  - height
            @param UInt  - channels(1..6)            
        */
        _CoreExport int create(SCRIPT_STATE *L);


        /*
        @name image.remove
        @description Deletes all image data and frees mamory
        */
        _CoreExport int remove(SCRIPT_STATE *L);
        

        /*
            @name image.read
            @description Read image from the file: jpg(jpeg) or png
            @description String - absolute path to the input file            
        */
        _CoreExport int read(SCRIPT_STATE *L);

        /*
            @name image.save
            @description Save image: jpg(jpeg) or png
            @description String - absolute path to the output file                        
        */
        _CoreExport int save(SCRIPT_STATE *L);

        /*
            @name image.set
            @description Set channel value
            @description UInt  -  position x(0,width-1)
            @description UInt  -  position y(0,height-1)            
            @description Byte value
            @description UInt  -  channel(1..6)
        */
        _CoreExport int set(SCRIPT_STATE *L);

        /*
            @name image.fill
            @description Fill image with values
            @description Byte value
            @description UInt  -  channel(1..6)
        */
        _CoreExport int fill(SCRIPT_STATE *L);


        _CoreExport int get(SCRIPT_STATE *L);

        _CoreExport void registerImageLua(SCRIPT_STATE *L);
    }
			
}

#endif














