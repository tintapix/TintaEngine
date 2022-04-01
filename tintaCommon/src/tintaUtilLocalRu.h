/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_UTIL_LOCAL_RU_H_
#define _TINTA_UTIL_LOCAL_RU_H_


#include "tintaPredefine.h"


namespace Tinta {

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
#define UTIL_system "system"

        /*
        @name util.getval
        @description Returns value from tree config file
        @param String  - path
        @param String  - request
        @param String  - type return("double","string","bool")
        @return Real,String,bool
        */

#define UTIL_getval "getval"

        /*
        @name util.cpucores
        @description Returns CPU cores
        @return cpu avalible cores
        */

#define UTIL_cpucores "cpucores"

        /*
        @name util.msg
        @description	 Prints msg to the console and into the log file if used one
        @param Bool,String,Number - parameters comma separated  e.g.(  util.msg("test",100,true) )
        */

#define UTIL_msg "msg"

        /*
        @name util.msgf
        @description	Prints message into the log file
        @param Bool,String,Number parameters comma separated e.g.( util.msgf("test",100,true) )
        */

#define UTIL_msgf "msgf"


        /*
        @name util.timestamp
        @description Выводить время в сообщениях
        @param Bool true - будет выводить время
        */

#define UTIL_timestamp "timestamp"


        /*
        @name util.bytetofloat
        @description	converts byte value to the float
        @param byte value
        @return - Real data
        */
#define UTIL_bytetofloat "bytetofloat"        

        /*
        @name util.floattobyte
        @description	Converts float value to the byte
        @param Real value
        @return - byte data
        */
#define UTIL_floattobyte "floattobyte"           



        /*
        @name util.linterp
        @description	Implements linear interpolation
        @param Real param value
        @param Real param value
        @param Real param value[0,1]
        @return - Real
        */
#define UTIL_linterp "linterp"         

        /*
        @name util.cosinterp
        @description	Implements Cos interpolation
        @param Real param value
        @param Real param value
        @param Real param value[0,1]
        @return - Real
        */
#define UTIL_cosinterp "cosinterp"          

        /*
        @name util.cubeinterp
        @description	Implements Cubic interpolation
        @param param value
        @param param value
        @param param value
        @param param value
        @param param value[0,1]
        @return - Real
        */
#define UTIL_cubeinterp "cubeinterp"        


        /*
        @name util.smoothstep
        @description	Implements smooth step function
        @param Real param
        @param Real param
        @param Real param
        @return - Real
        */
#define UTIL_smoothstep "smoothstep"         


        /*
        @name util.sleep
        @description	Send to sleep executing thread
        @param UInt - time ms
        */
#define UTIL_sleep "sleep"


        /*
        @name util.fopened
        @description	Returns opened file in the another process flag
        @param String path to the file
        @return - Boolean true, if file is opened in the another process
        */
#define UTIL_fopened "fopened" 



        /*
        @name util.countfiles
        @description	Returns files quantity by the condition
        @param String - absolute or relative path to directory where to count
        @param String = "" - extension
        @return Int quantity
        */
#define UTIL_countfiles "countfiles"       


        /*
        @name util.getfile
        @description	Returns file by the condition in directory
        @param String -  absolute or relative path to directory where to find
        @param UInt - position of file in list for iterating using c_countfiles
        @param String = "" - extension
        @param Bool ;- if false returns full path to file, if true only filename
        @return path
        */
#define UTIL_getfile "getfile"           


        /*
        @name util.getdirectory
        @description	Returns directory by the condition
        @param - absolute or relative path to directory where to find
        @param - position of file in list for iterating using c_countfiles
        @param - if true returns full path to file, if false only full filename
        @return quantity
        */
#define UTIL_getdirectory "getdirectory"        


        /*
        @name util.countdirectories
        @description	Returns directories quantity by the condition
        @param - absolute or relative path to directory	where to count
        @return quantity
        */
#define UTIL_countdirectories "countdirectories"               


        /*
        @name util.isdirectory
        @description	Determine if the path is directory
        */
#define UTIL_isdirectory "isdirectory"    

        /*
        @name util.error
        @description	Реализует функцию assert в lua. util.error( param , "Ошибочный параметр функции" )
        @param Булевая если true то ничего непроисходит, если false, прекратит выполнение скрипта и выведет сообщение
        @param Строка с сообщением - не обязательный параметр
        */
#define UTIL_error "error"  


        /*
        @name util.pow
        @description	pow function
        */
#define UTIL_pow "pow"                              

        /*
        @name util.packcolor
        @description	Packs from byte form R G B into the unsigned form 0x00000
        @param  byte R value
        @param  byte G value
        @param  byte B value
        @return  - unsigned packed result
        */
#define UTIL_packcolor "packcolor"         

        /*
        @name util.unpackcolor
        @description	Unpacks the unsigned form 0 in to the byte form R G B
        @param  - unsigned packed value
        @return  byte R result
        @return  byte G result
        @return  byte B result
        */
#define UTIL_unpackcolor "unpackcolor"                 


        /*
        @name util.rgbtohsl
        @description	Converts RGB to HSL
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
#define UTIL_rgbtohsl "rgbtohsl"                         

        /*
        @name util.hsltorgb
        @description	Converts HSL to RGB
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
#define UTIL_hsltorgb "hsltorgb"                                


        /*
        @name util.fromdegtorad
        @description	Converts angle from degree to radians
        @param Real angle
        */
#define UTIL_fromdegtorad "fromdegtorad"                                        

        /*
        @name util.fromradtodeg
        @description	Converts angle from radians to degree
        @param Real angle
        */
#define UTIL_fromradtodeg "fromradtodeg"       

        /*
        @name util.regexmatch
        @description	Finds matches using regular expressions
        @param string regular expression
        @param string text
        @param int match mask:
        match_default = 0x0000,
        match_not_bol = 0x0001,
        match_not_eol = 0x0002,
        match_not_bow = 0x0004,
        match_not_eow = 0x0008,
        match_any = 0x0010,
        match_not_null = 0x0020,
        match_continuous = 0x0040,
        _Match_partial = 0x0080,
        match_prev_avail = 0x0100,
        format_default = 0x0000,
        format_sed = 0x0400,
        format_no_copy = 0x0800,
        format_first_only = 0x1000,
        _Match_not_null = 0x2000
        @return int matches count
        */
#define UTIL_regexmatch "regexmatch"              

        /*	@name util.writetofile
            @description Writes text into the file
            @param path            
            @param string data string
        */
#define UTIL_writetofile "writetofile"  



        /*
            @name image.create
            @description Creates image
            @param UInt  - width
            @param UInt  - height
            @param UInt  - channels(1..6)
            @return UInt  - image id
        */
#define IMAGE_create "create"



        /*
            @name image.remove
            @description Deletes all image data and frees mamory
            @param UInt  - image id
        */
#define IMAGE_remove "remove"


        /*
            @name image.read
            @description Reads image from the file: jpg(jpeg) or png
            @param String - absolute path to the file  
            @return UInt  - image id
            @return UInt  - image width
            @return UInt  - image height
        */
#define IMAGE_read "read"



        /*
            @name image.save
            @description Saves image th the file: jpg(jpeg) or png
            @return UInt  - image id
            @param String - absolute path to save the file                        
        */
#define IMAGE_save "save"


        /*
            @name image.set
            @description Sets value the channel
            @param UInt  - image id
            @param UInt  -  position x(0,width-1)
            @param UInt  -  position y(0,height-1)
            @param Byte value
            @param UInt  -  channel(1..6)
        */
#define IMAGE_set "set"


        /*
            @name image.fill
            @description Fills image with values
            @param Byte value
            @param UInt  - image id
            @param UInt  -  channel(1..6)
        */
#define IMAGE_fill "fill"

        /*
            @name image.get
            @description Gets value the channel
            @param UInt  - image id
            @param UInt  -  position x(0,width-1)
            @param UInt  -  position y(0,height-1)
            @param UInt  -  channel(1..6)
            @return Byte value
        */
#define IMAGE_get "get"
    }
}
#endif