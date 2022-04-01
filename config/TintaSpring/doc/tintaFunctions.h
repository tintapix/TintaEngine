/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_FUNCTIONS_H_
#define _TINTA_FUNCTIONS_H_

#include "tintaPredefine.h"
#include <tintaScriptContext.h>
#include "tintaExecutingUnit.h"

	namespace Tinta	{

            namespace TintaImageLua {

                /*
                    @name image.clearstains
                    @description Clears image from stains
                    @param UInt Max objects in group
                    @param Real[ 0.0, 1.0 ] color group threashold default 0.0 Factor for colors grouping.
                */
                _CoreExport int clearstains(SCRIPT_STATE *L);

                /*
                    @name image.getcolors
                    @description creates image with statistically collected colors
                    @param Colors quantity
                    @param Real[0,1] color group threashold default 0. Factor for colors grouping.
                    @return UInt index image with result pallete
                */
                _CoreExport int getcolors(SCRIPT_STATE *L);


                /*
                    @name image.priorcolor
                    @description Sets prior pixel value in the current image. Walks around every pixel in the image and sets prior value.
                    For window update call c_winupdate
                    @param Integer Threshold - if pixels quantity around( considering the factor ) equal or more then this value
                    pixel gets major pixel
                    @param Real[0,1] Coefficient of equivalence of the color ( by default 0.0 )
                    @param Integer passes - 1 by default
                    @param Boolean with alpha
                */
                _CoreExport int priorcolor(SCRIPT_STATE *L);


                /*
                    @name image.poster
                    @description    For window update call c_winupdate
                    pixel gets major pixel
                    @param Pallete image id
                */
                _CoreExport int poster(SCRIPT_STATE *L);

                /*
                    @name image.setpixelb
                    @description Draws point in selected image. For window update call c_winupdate
                    @param Int x coordinate
                    @param Int y coordinate
                    @param Byte r
                    @param Byte g
                    @param Byte b
                    @param Real[0,1] Alpha factor. For mixing 1- default
                */
                _CoreExport int setpixelb(SCRIPT_STATE *L);


                /*
                    @name image.setpixelf
                    @description Draws point in the selected image. For window update call c_winupdate
                    @param Int x coordinate
                    @param Int y coordinate
                    @param Real r
                    @param Real g
                    @param Real b
                    @param Real[0,1] Alpha factor. For mixing 1- default
                */
                _CoreExport int setpixelf(SCRIPT_STATE *L);


                /*
                    @name image.selected
                    @description Rerurns index of the selected image
                    @return index of the selected image
                    @return UInt index, if image was not selected, returns nil
                */
                _CoreExport int selected(SCRIPT_STATE *L);

                
                /*
                    @name image.select
                    @description Selecting image in the container,
                    @param UInt Image index
                    @return true - if image was selected, false otherwise
                */
                _CoreExport int select(SCRIPT_STATE *L);


                /*
                    @name image.save
                    @description Saves selected image in to the file. c_saveimg("png","\\\\server\\Shared\\user\\test.png")
                    @param String image type("png","jpeg" or "jpg")
                    @param String Full path to the output file ("d:/test.png")
                */
                _CoreExport int save(SCRIPT_STATE *L);

                /*
                    @name image.width
                    @description Returns the selected image width
                    @return Int Image width
                */
                _CoreExport int width(SCRIPT_STATE *L);

                /*
                    @name image.height
                    @description Returns the selected image height
                    @return Int Image height
                */
                _CoreExport int height(SCRIPT_STATE *L);

                /*
                    @name image.getpixelb
                    @description Returns pixel value
                    @param Int x coordinate value
                    @param Int y coordinate value
                    @return Byte r value
                    @return Byte g value
                    @return Byte b value
                    @return Byte alpha value
                */
                _CoreExport int getpixelb(SCRIPT_STATE *L);


                /*
                    @name image.getpixelf
                    @description Returns pixel value
                    @param Int x coordinate value
                    @param Int y coordinate value
                    @return Real r value
                    @return Real g value
                    @return Real b value
                    @return Real alpha value
                */
                _CoreExport int getpixelf(SCRIPT_STATE *L);


                /*
                    @name image.getindpixelb
                    @description Returns pixel value by index in the selected image
                    @param index value
                    @return r value in bytes
                    @return g value in bytes
                    @return b value in bytes
                    @return alpha value in bytes
                */
                _CoreExport int getindpixelb(SCRIPT_STATE *L);


                /*
                    @name image.getindpixelf
                    @description Returns pixel value by index in the selected image
                    @param UInt index value
                    @return Real r value
                    @return Real g value
                    @return Real b value
                    @return Real alpha value
                */
                _CoreExport int getindpixelf(SCRIPT_STATE *L);



                /*
                    @name image.fillb
                    @description Fills color channels in selected image with values. For window update call c_winupdate
                    @param Byte r value
                    @param Byte g value
                    @param Byte b value
                */
                _CoreExport int fillb(SCRIPT_STATE *L);

                /*
                    @name image.fillf
                    @description Fills color channels in selected image with values
                    @param Real r value
                    @param Real g value
                    @param Real b value
                */
                _CoreExport int fillf(SCRIPT_STATE *L);


                /*
                @name image.create
                @description Adds image in to the container
                @param UInt width
                @param UInt Height
                @param String  image types: "seamless", "basic"
                @param channels value 1 - 4
                @return UInt image created Index
                */
                _CoreExport int create(SCRIPT_STATE *L);

                /*
                @name image.read
                @description Reads the image from the file
                @param String Absolute path to the file ("d:/test.png")
                @param String image type("png","jpeg" or "jpg") - by default gets from path
                @param String new image type ("seamless", "common_image")  - by default common_image
                @return nil if the error occured,  otherwise image created index

                */
                _CoreExport int read(SCRIPT_STATE *L);

                /*
                @name image.fillchannelf
                @description Fills selected image channel with float values
                @param - (0,3) channel
                @param Real alpha value
                */
                _CoreExport int fillchannelf(SCRIPT_STATE *L);


                /*
                @name image.setchannelf
                @description Assigns channel value
                @param - x pos
                @param - y pos
                @param - (0,3) channel
                @param (0.0,1.0) value
                */
                _CoreExport int setchannelf(SCRIPT_STATE *L);

                /*
                @name image.setchannelb
                @description Assigns channel value
                @param - x pos
                @param - y pos
                @param - (0,3) channel
                @param Byte value
                */
                _CoreExport int setchannelb(SCRIPT_STATE *L);


                /*
                @name image.mix
                @description Interpolation between selected and second image
                @param - second image id
                @param - x pos in destination image
                @param - y pos in destination image
                @param - (0,1) factor
                */
                _CoreExport int mix(SCRIPT_STATE *L);

                /*
                @name image.mixchannel
                @description Mixing channels of selected and second image
                For window update call c_winupdate
                @param - second image id
                @param - x pos in destination image
                @param - y pos in destination image
                @param - channel (0,3)
                @param - (0,1) factor
                */
                _CoreExport int mixchannel(SCRIPT_STATE *L);

                /*
                @name image.erase
                @description Deletes image from the container
                @param UInt Image index
                */
                _CoreExport int erase(SCRIPT_STATE *L);

                /*
                @name image.eraseall
                @description Clears image container
                */
                _CoreExport int eraseall(SCRIPT_STATE *L);


                /*
                @name image.fillchannelb
                @description Fills selected image channel with byte values
                @param - (0,3) channel
                @param Byte alpha value
                */
                _CoreExport int fillchannelb(SCRIPT_STATE *L);


                /*
                @name image.images
                @description Returns images quantity
                @return quantity
                */
                _CoreExport int images(SCRIPT_STATE *L);

				/*
                @name image.channels
                @description Returns image channels quantity
                @return Channels quantity
                */
                _CoreExport int  channels(SCRIPT_STATE *L);
            }
        
            namespace tintaFontFunctions {


                /*
                @name font.add
                @description adds new font in stack and makes the font selected
                @param string font name
                @param real font x scale factor(new_size_x = x_size * x_factor) if nil, then default = 1.0
                @param real font y scale factor(new_size_y = y_size * y_factor) if nil, then default = 1.0
                @param int font color r channel or nil
                @param int font color g channel or nil
                @param int font color b channel or nil
                @return unsigned int font id
                */
                _CoreExport int add(SCRIPT_STATE *L);

                /*
                    @name font.copy
                    @description makes copy of hte font
                    @param unsigned int font id to copy
                    @return unsigned int new font id
                */
                _CoreExport int copy(SCRIPT_STATE *L);

                /*
                    @name font.select
                    @description select the font
                    @paramun usigned int font id
                */
                _CoreExport int select(SCRIPT_STATE *L);


                /*
                    @name font.setcolor
                    @description changes font color
                    @param unsigned int font id
                    @param int font color r channel
                    @param int font color g channel
                    @param int font color b channel
                */
                _CoreExport int setcolor(SCRIPT_STATE *L);

                /*
                    @name font.render
                    @description draw text with selected font in to the selected image
                    @param string text(\n as a new line)
                    @param int x position of the text
                    @param int y position of the text
                    @param int x space between symbols in pixels or nil
                    @param int y space between symbols in pixels or nil
                */
                _CoreExport int render(SCRIPT_STATE *L);

                /*
                    @name font.erase
                    @description removes the font from the stack
                    @param unsigned int font id
                */
                _CoreExport int erase(SCRIPT_STATE *L);


                /*
                    @name font.eraseall
                    @description removes all fonts from the stack
                */
                _CoreExport int eraseall(SCRIPT_STATE *L);
            }

        namespace tintaMainFunctions {

       

        /*
        @name main.printver
        @description Prints library version
        @return String version
        */
        _CoreExport int printver(SCRIPT_STATE *L);



        /*
        @name main.lassert
        @description Throws error with or without the message and terminates executing
        @param boolean expression throws if false
        @param String possible message
        */
        _CoreExport int lassert(SCRIPT_STATE *L);


		/*
            @name main.f
			@description Executs Script file on the first available unit. For window update call main.winupdate
			@param String path to the script. If Path is not absolute("NoName.lua"), tries to find Script using config paths
		*/
		_CoreExport int f(SCRIPT_STATE *L);

        /*
        @name main.fd
        @description Executs Script file ONLY on the children units
        @param String path to the script. If Path is not absolute("NoName.lua"), tries to find Script using config paths
        */
        _CoreExport int fd(SCRIPT_STATE *L);

        /*
        @name main.fl
        @description Executs Script file ONLY on the local unit
        @param String path to the script. If Path is not absolute("NoName.lua"), tries to find Script using config paths
        */
        _CoreExport int fl(SCRIPT_STATE *L);

        /*
        @name main.fu
        @description Sends script file for executing on any parent unit
        @param String path to the script. If Path is not absolute("NoName.lua"), tries to find Script using config paths
        */
        _CoreExport int fu(SCRIPT_STATE *L);
		

		/*
            @name main.getangle
			@description  
			@param Real x1
			@param Real y1
			@param Real x2
			@param Real y2		
			@return Real angle in degree
		*/
		_CoreExport int getangle(SCRIPT_STATE *L);	



        /*
            @name main.start
            @description Starts or resets executing timer
            @param String command
            @param UInt period in milliseconds
            @param String command to execute e.g. main.ed("c_start(\"c_f(\\\"c.lua\\\")",1000)")                       
        */
        _CoreExport int start(SCRIPT_STATE *L);

        

        /*
            @name main.stop
            @description Stops executing timer
        */
        _CoreExport int stop(SCRIPT_STATE *L);


		_CoreExport int test(SCRIPT_STATE *L);        

        	

        /*	
            @name main.rotpoint
			@description Rotates point around the other on angle 
			@param Int - xCenter
			@param Int - yCenter
			@param Int - x
			@param Int - y
			@param Real - angle in degree
			@return Int  x
			@return Int  y

		*/
		_CoreExport int rotpoint(SCRIPT_STATE *L);        

		/*
            @name main.setrandseed
			@description Initializes seed for random number generator with current time (for main.randint generator)
		*/
		_CoreExport int setrandseed(SCRIPT_STATE *L);

		/*
            @name main.randint
			@description Generates integer random number [min,max]
			@param random min		
			@param random max
			@return integer random number
		 */
		_CoreExport int randint(SCRIPT_STATE *L);

        /*
            @name main.randintex
            @description Generates large integer random number [min,max] using mersenne twister 
            @param random min
            @param random max
            @return integer random number
        */
        _CoreExport int randintex(SCRIPT_STATE *L);

               

		/*
            @name main.randfloat
			@description Generates Real random number [0,1]							
			@return Real random number
		 */
		_CoreExport int randfloat(SCRIPT_STATE *L);

		
		/*
            @name main.calchash
			@description Calculates hash from the string
			@param String to calculate
			@return - integer data
		 */
		_CoreExport int calchash(SCRIPT_STATE *L);

		      
		/*
            @name main.getname
			@description Returns app name
		*/
		_CoreExport int getname(SCRIPT_STATE *L);

		/*
            @name main.uniqid
			@description Generates uniqal id (hashedunitname_year:mounth:day:hour:minute:second:milliseconds_randnumber)
			@return - integer data
		 */
		_CoreExport int uniqid(SCRIPT_STATE *L);

        /*
            @name main.getseed
			@description  returns seed value for the random generator(generator main.randint)
			@return - integer data
		 */
		_CoreExport int getseed(SCRIPT_STATE *L);       
        		
        /*
        @name main.readfile
        @description Reads text from file in to the vector (type "strs_vector")
        @param path
        @param boolean if true - reads all lines in one string with spaces instead of wordwrap( vector size == 1), false - default
        @return id of the vector type 'strs_vector'
        */
        _CoreExport int readfile(SCRIPT_STATE *L);




       /*
 	   @name main.writetofile
        @description Writes text from String vector 'strs_box' into the file
        @param path
        @param id of the vector type 'strs_vector'
        */
        _CoreExport int writetofile(SCRIPT_STATE *L);




        /*	@name main.isserver
        @description Determines has app child units or not
        @name main.isserver
        @return mode,  true - if server mode
        */
        _CoreExport int isserver(SCRIPT_STATE *L);


        /*
        @name main.e
        @description executes buffer on the first available unit. e.g. main.e("c_delimg(10)") or main.e("script.lua",true)
        @param String - script data or path if second parameter is true
        @param Boolean - if true - reads buffer from path
        */
        _CoreExport int e(SCRIPT_STATE *L);

        /*
        @name main.el
        @description executes buffer ONLY on the local units. e.g. main.e("c_delimg(10)") or main.e("script.lua",true)
        @param String - script data or path if second parameter is true
        @param Boolean - if true - reads buffer from path
        */
        _CoreExport int el(SCRIPT_STATE *L);

        /*
        @name main.ed
        @description executes buffer ONLY on the children units. e.g. main.e("c_delimg(10)") or main.e("script.lua",true)
        @param String - script data or path if second parameter is true
        @param Boolean - if true - reads buffer from path
        */
        _CoreExport int ed(SCRIPT_STATE *L);

        /*
        @name main.eu
        @description Sends buffer for executing on parrent unit. e.g. main.e("c_delimg(10)") or main.e("script.lua",true)
        @param String - script data or path if second parameter is true
        @param Boolean - if true - reads buffer from path
        */
        _CoreExport int eu(SCRIPT_STATE *L);


        /*
        @name main.exit
        Closes app
        */
        _CoreExport int exit(SCRIPT_STATE *L);



        /*
        @name main.tasks
        @description Returns tasks quantity in the queue
        @return tasks quantity
        */
        _CoreExport int tasks(SCRIPT_STATE *L);


        /*
        @name main.winupdate
        @description Updates window image buffer. Should be avoided in lua "while" cycles
        @param Image id
        */
        _CoreExport int winupdate(SCRIPT_STATE *L);


        /*
        @name main.winhandler
        @description Handles window actions
        @param Handler type ("mouse_move","key")
        @param Script text to execute
        */
        _CoreExport int winhandler(SCRIPT_STATE *L);


        /*
        @name main.winsetimg
        @description Set image in the window to show
        @param UInt - image id to show
        */
        _CoreExport int winsetimg(SCRIPT_STATE *L);

        /*
        @name main.windelhandlers
        @description Deletes handler
        */
        _CoreExport int windelhandlers(SCRIPT_STATE *L);


        /*
        @name main.wincursor
        @description Returns cursor in window coordinates
        @return x pos
        @return y pos
        */
        _CoreExport int wincursor(SCRIPT_STATE *L);


        /*
        @name main.cursorimg
        @description Returns cursor in image coordinates -1 -1 by default, if no image selected
        @return x pos
        @return y pos
        */
        _CoreExport int cursorimg(SCRIPT_STATE *L);


        /*
        @name main.keystate
        @description Returns key state
        @param Key:
        ML_UNASSIGNED  = 0x00,
        ML_ESCAPE      = 0x01,
        ML_1           = 0x02,
        ML_2           = 0x03,
        ML_3           = 0x04,
        ML_4           = 0x05,
        ML_5           = 0x06,
        ML_6           = 0x07,
        ML_7           = 0x08,
        ML_8           = 0x09,
        ML_9           = 0x0A,
        ML_0           = 0x0B,
        ML_MINUS       = 0x0C,    // - on main keyboard
        ML_EQUALS      = 0x0D,
        ML_BACK        = 0x0E,    // backspace
        ML_TAB         = 0x0F,
        ML_Q           = 0x10,
        ML_W           = 0x11,
        ML_E           = 0x12,
        ML_R           = 0x13,
        ML_T           = 0x14,
        ML_Y           = 0x15,
        ML_U           = 0x16,
        ML_I           = 0x17,
        ML_O           = 0x18,
        ML_P           = 0x19,
        ML_LBRACKET    = 0x1A,
        ML_RBRACKET    = 0x1B,
        ML_RETURN      = 0x1C,    // Enter on main keyboard
        ML_LCONTROL    = 0x1D,
        ML_A           = 0x1E,
        ML_S           = 0x1F,
        ML_D           = 0x20,
        ML_F           = 0x21,
        ML_G           = 0x22,
        ML_H           = 0x23,
        ML_J           = 0x24,
        ML_K           = 0x25,
        ML_L           = 0x26,
        ML_SEMICOLON   = 0x27,
        ML_APOSTROPHE  = 0x28,
        ML_GRAVE       = 0x29,    // accent
        ML_LSHIFT      = 0x2A,
        ML_BACKSLASH   = 0x2B,
        ML_Z           = 0x2C,
        ML_X           = 0x2D,
        ML_C           = 0x2E,
        ML_V           = 0x2F,
        ML_B           = 0x30,
        ML_N           = 0x31,
        ML_M           = 0x32,
        ML_COMMA       = 0x33,
        ML_PERIOD      = 0x34,    // . on main keyboard
        ML_SLASH       = 0x35,    // / on main keyboard
        ML_RSHIFT      = 0x36,
        ML_MULTIPLY    = 0x37,    // * on numeric keypad
        ML_LMENU       = 0x38,    // left Alt
        ML_SPACE       = 0x39,
        ML_CAPITAL     = 0x3A,
        ML_F1          = 0x3B,
        ML_F2          = 0x3C,
        ML_F3          = 0x3D,
        ML_F4          = 0x3E,
        ML_F5          = 0x3F,
        ML_F6          = 0x40,
        ML_F7          = 0x41,
        ML_F8          = 0x42,
        ML_F9          = 0x43,
        ML_F10         = 0x44,
        ML_NUMLOCK     = 0x45,
        ML_SCROLL      = 0x46,    // Scroll Lock
        ML_NUMPAD7     = 0x47,
        ML_NUMPAD8     = 0x48,
        ML_NUMPAD9     = 0x49,
        ML_SUBTRACT    = 0x4A,    // - on numeric keypad
        ML_NUMPAD4     = 0x4B,
        ML_NUMPAD5     = 0x4C,
        ML_NUMPAD6     = 0x4D,
        ML_ADD         = 0x4E,    // + on numeric keypad
        ML_NUMPAD1     = 0x4F,
        ML_NUMPAD2     = 0x50,
        ML_NUMPAD3     = 0x51,
        ML_NUMPAD0     = 0x52,
        ML_DECIMAL     = 0x53,    // . on numeric keypad
        ML_OEM_102     = 0x56,    // < > | on UK/Germany keyboards
        ML_F11         = 0x57,
        ML_F12         = 0x58,
        ML_F13         = 0x64,    //                     (NEC PC98)
        ML_F14         = 0x65,    //                     (NEC PC98)
        ML_F15         = 0x66,    //                     (NEC PC98)
        ML_KANA        = 0x70,    // (Japanese keyboard)
        ML_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards
        ML_CONVERT     = 0x79,    // (Japanese keyboard)
        ML_NOCONVERT   = 0x7B,    // (Japanese keyboard)
        ML_YEN         = 0x7D,    // (Japanese keyboard)
        ML_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
        ML_NUMPADEQUALS= 0x8D,    // = on numeric keypad (NEC PC98)
        ML_PREVTRACK   = 0x90,    // Previous Track (ML_CIRCUMFLEX on Japanese keyboard)
        ML_AT          = 0x91,    //                     (NEC PC98)
        ML_COLON       = 0x92,    //                     (NEC PC98)
        ML_UNDERLINE   = 0x93,    //                     (NEC PC98)
        ML_KANJI       = 0x94,    // (Japanese keyboard)
        ML_STOP        = 0x95,    //                     (NEC PC98)
        ML_AX          = 0x96,    //                     (Japan AX)
        ML_UNLABELED   = 0x97,    //                        (J3100)
        ML_NEXTTRACK   = 0x99,    // Next Track
        ML_NUMPADENTER = 0x9C,    // Enter on numeric keypad
        ML_RCONTROL    = 0x9D,
        ML_MUTE        = 0xA0,    // Mute
        ML_CALCULATOR  = 0xA1,    // Calculator
        ML_PLAYPAUSE   = 0xA2,    // Play / Pause
        ML_MEDIASTOP   = 0xA4,    // Media Stop
        ML_VOLUMEDOWN  = 0xAE,    // Volume -
        ML_VOLUMEUP    = 0xB0,    // Volume +
        ML_WEBHOME     = 0xB2,    // Web home
        ML_NUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)
        ML_DIVIDE      = 0xB5,    // / on numeric keypad
        ML_SYSRQ       = 0xB7,
        ML_RMENU       = 0xB8,    // right Alt
        ML_PAUSE       = 0xC5,    // Pause
        ML_HOME        = 0xC7,    // Home on arrow keypad
        ML_UP          = 0xC8,    // UpArrow on arrow keypad
        ML_PGUP        = 0xC9,    // PgUp on arrow keypad
        ML_LEFT        = 0xCB,    // LeftArrow on arrow keypad
        ML_RIGHT       = 0xCD,    // RightArrow on arrow keypad
        ML_END         = 0xCF,    // End on arrow keypad
        ML_DOWN        = 0xD0,    // DownArrow on arrow keypad
        ML_PGDOWN      = 0xD1,    // PgDn on arrow keypad
        ML_INSERT      = 0xD2,    // Insert on arrow keypad
        ML_DELETE      = 0xD3,    // Delete on arrow keypad
        ML_LWIN        = 0xDB,    // Left Windows key
        ML_RWIN        = 0xDC,    // Right Windows key
        ML_APPS        = 0xDD,    // AppMenu key
        ML_POWER       = 0xDE,    // System Power
        ML_SLEEP       = 0xDF,    // System Sleep
        ML_WAKE        = 0xE3,    // System Wake
        ML_WEBSEARCH   = 0xE5,    // Web Search
        ML_WEBFAVORITES= 0xE6,    // Web Favorites
        ML_WEBREFRESH  = 0xE7,    // Web Refresh
        ML_WEBSTOP     = 0xE8,    // Web Stop
        ML_WEBFORWARD  = 0xE9,    // Web Forward
        ML_WEBBACK     = 0xEA,    // Web Back
        ML_MYCOMPUTER  = 0xEB,    // My Computer
        ML_MAIL        = 0xEC,    // Mail
        ML_MEDIASELECT = 0xED,     // Media Select
        ML_MOUSE_LEFT  = 0xEE,
        ML_MOUSE_MIDDLE = 0xEF,
        ML_MOUSE_RIGHT  = 0xF0,
        ML_MOUSE_WEEL  = 0xF1,
        @return true - if key`s  pressed
        */
        _CoreExport int keystate(SCRIPT_STATE *L);


        /*
        @name main.executeshell
        @description Executes shell command. Path in script set "C:\\foto\\" and "\\\\user\\user\\test"
        @param  - string to execute
        */
        _CoreExport int executeshell(SCRIPT_STATE *L);

                
          

         /*
         @name main.regextoken
         @description Finds tokens using regular expressions, taking data from string parameter
         for symbol "\" use "\\"
         demo:
         local regex = "(\\d{2})/(\\d{2})/(\\d{4})"
         local vec = main.regextoken( regex, "01/02/2003 blahblah 04/23/1999 blahblah 11/13/1981", "2", 1	)
         if vec ~= nil then
         local q = main.boxelements( vec )
         util.c_msg( "size: " , q )
         for k = 0, q - 1 do
         util.c_msg( main.getvalboxs( vec,k ) )
         end
         main.delbox( vec )
         end
         @param string regular expression
         @param string text
         @param string submatch options like "0,1" see http://www.cplusplus.com/reference/regex/regex_token_iterator/regex_token_iterator/
         0 by default
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
         @return string vector id or zero_id == 0
         */
         _CoreExport int regextoken(SCRIPT_STATE *L);

         /*
         @name main.regextokenex
         @description Finds tokens using regular expressions, taking data from string vector
         demo:
         function s_getStringArrayId( pathIn, code, bInOneLine )
         local idBox = main.readfile(pathIn, code, bInOneLine )
         return idBox
         end
         local regex = "(\\d{2})/(\\d{2})/(\\d{4})"
         local textBox = s_getStringArrayId( "D:/text.txt", "utf8", true )
         util.c_msg( "...Read ....")
         local vec = main.regextokenex( regex, textBox, 0, 1 , "0,1", 1 )
         @param string regular expression
         @param int string vector id with text
         @param int pos in vector
         @param string submatch options like "0,-1" see http://www.cplusplus.com/reference/regex/regex_token_iterator/regex_token_iterator/
         0 by default
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
         @return string vector id or zero_id == 0
         */
         _CoreExport int regextokenex(SCRIPT_STATE *L);


         /*
         @name main.regexmatch
         @description	Finds matches using regular expressions. Data takes from vector string box
         for symbol "\" use "\\"
         @param string regular expression
         @param int string vector id with text
         @param int pos in vector
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
         _CoreExport int regexmatch(SCRIPT_STATE *L);

        


        

         	      
         //////////// system not lua 

		 // outputs list of tasks
		 _CoreExport void tasks();

         // outputs list of units
		 _CoreExport void units();

         // outputs list of containers data
		 _CoreExport void containers();

         // outputs units states
		 _CoreExport void status();

         // totaly restarts unit with tasks deleting 
         _CoreExport void restart();

         // trying to refresh unit state
         _CoreExport void reset();

		 // aborts operation on unit(-s)
         _CoreExport void abort(); // const String &name, m_ulong32 idUnit);

         // recreates context on unit(-s)
         _CoreExport void cinit();


         // shows window with image by id
         _CoreExport void window();

		 // recreates GPU on unit(-s)
		 _CoreExport void ginit();

		 // requests states for child interact unit(-s)
		 _CoreExport void state();

         /* Removes all tasks (common and parallel)
         bParallelOnly - if true - clears only parallel tasks
         */
         _CoreExport void cleartasks( );

		 // state request for the concrete child interact unit
		 _CoreExport void state( const tintaExecutingUnit *unit );

         // clear screen
         _CoreExport void cls();

         // exit command
         _CoreExport void exit();        		 

         _CoreExport void registerEngineLua(SCRIPT_STATE *L);
	}			
    namespace TintaBoxLua {


        /*
        @name box.create
        @description Creates box - 2d or 1d(stack) array. Useful for gpu calculation.
        All images are 2d boxes with RGB or RGBA values.
        @param String -  box type:
        with 2d(x and y coordinates) : float32_box, int32_box, int16_box
        with 1d(stack only x coordinate) dimension or vectors: int16_vector, float32_vector, int32_vector, str_vector, vec3f_vector, uint8_vector
        @return - box id
        */
        _CoreExport int create(SCRIPT_STATE *L);


        /*
        @name box.erase
        @description Deletes box
        @param -  box id
        */
        _CoreExport int erase(SCRIPT_STATE *L);


        /*
        @name box.eraseall
        @description Deletes all boxes in the container
        */
        _CoreExport int eraseall(SCRIPT_STATE *L);

        /*
        @name box.getvalf
        @description Returns float value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @return -  value
        @return -  boolean result true - if coordinate located in boundary
        */
        _CoreExport int getvalf(SCRIPT_STATE *L);


        /*
        @name box.setvalf
        @description Assigns float value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @param -  value
        */
        _CoreExport int setvalf(SCRIPT_STATE *L);


        /*
        @name box.setval3f
        @description Assigns vector3f value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @param -  value
        */
        _CoreExport int setval3f(SCRIPT_STATE *L);

        /*
        @name box.getvali16
        @description Returns int16 value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @return -  value
        */
        _CoreExport int getvali16(SCRIPT_STATE *L);


        /*
        @name box.setvali16
        @description Assigns int16 value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @param -  value
        */
        _CoreExport int setvali16(SCRIPT_STATE *L);


        /*
        @name box.getvali32
        @description Returns int value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @return -  value
        */
        _CoreExport int getvali32(SCRIPT_STATE *L);


        /*
        @name box.getvalui8
        @description Returns byte value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @return -  value
        */
        _CoreExport int getvalui8(SCRIPT_STATE *L);

        /*
        @name box.getval3f
        @description Returns vector3f value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @return -  value
        */
        _CoreExport int getval3f(SCRIPT_STATE *L);

        /*
        @name box.getvals
        @description Returns String value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @return -  value
        */
        _CoreExport int getvals(SCRIPT_STATE *L);


        /*
        @name box.setvals
        @description Assigns String value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @param -  value
        */
        _CoreExport int setvals(SCRIPT_STATE *L);

        /*
        @name box.setvali32
        @description Sets int value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @param -  value
        */
        _CoreExport int setvali32(SCRIPT_STATE *L);

        /*
        @name box.setvalui8
        @description Sets byte value in the selected coordinate. In vector boxes 'y' coordinate is ignored
        @param -  box id
        @param -  x coordinate
        @param -  y coordinate discarding in vector box type
        @param -  value
        */
        _CoreExport int setvalui8(SCRIPT_STATE *L);

        /*
        @name box.resize
        @description Resizes box with saving values in the selected coordinate. In vector boxes 'xMax,yMin,yMax' parameters is ignored
        @param box id
        @param xMin or new size in vector boxes
        @param xMax is ignored in vector box type
        @param yMin	is ignored in vector box type
        @param yMax is ignored in vector box type
        */
        _CoreExport int resize(SCRIPT_STATE *L);

        /*
        @name box.pushbacki16
        @description Pushes the value to the end of the vector box. Allowed only for vector boxes
        @param box id
        @param int16 value
        */
        _CoreExport int pushbacki16(SCRIPT_STATE *L);

        /*
        @name box.pushbacks
        @description Pushes the value to the end of the vector box. Allowed only for vector boxes
        @param box id
        @param String value
        */
        _CoreExport int pushbacks(SCRIPT_STATE *L);


        /*
        @name box.pushbacki32
        @description Pushes the value to the end of the vector box. Allowed only for vector boxes
        @param box id
        @param int32 value
        */
        _CoreExport int pushbacki32(SCRIPT_STATE *L);

        /*
        @name box.pushbackui8
        @description Pushes the value to the end of the vector box. Allowed only for vector boxes
        @param box id
        @param uint8 value
        */
        _CoreExport int pushbackui8(SCRIPT_STATE *L);


        /*
        @name box.pushbackf
        @description Pushes the value to the end of the vector box. Allowed only for vector boxes
        @param box id
        @param Real value
        */
        _CoreExport int pushbackf(SCRIPT_STATE *L);

        /*
        @name box.pushback3f
        @description Pushes the value to the end of the vector box. Allowed only for vector boxes
        @param box id
        @param Real value
        */
        _CoreExport int pushback3f(SCRIPT_STATE *L);

        /*
        @name box.popbackf
        @description Removes last value from the float vector box.
        @param box id
        */
        _CoreExport int popbackf(SCRIPT_STATE *L);

        /*
        @name box.popbacks
        @description Removes last value from the string vector box.
        @param box id
        */
        _CoreExport int popbacks(SCRIPT_STATE *L);


        /*
        @name box.popback3f
        @description Removes last value from the vector3f vector box.
        @param box id
        */
        _CoreExport int popback3f(SCRIPT_STATE *L);

        /*
        @name box.popbacki32
        @description Removes last value from the integer32 vector box.
        @param box id
        */
        _CoreExport int popbacki32(SCRIPT_STATE *L);

        /*
        @name box.popbackui8
        @description Removes last value from the byte vector box.
        @param box id
        */
        _CoreExport int popbackui8(SCRIPT_STATE *L);

        /*
        @name box.popbacki16
        @description Removes last value from the integer vector box.
        @param box id
        */
        _CoreExport int popbacki16(SCRIPT_STATE *L);

        /*
        @name box.elements
        @description Returns elements quantity in the box(size in vector)
        @param box id
        @return width
        @return height(1 in vector)
        */
        _CoreExport int elements(SCRIPT_STATE *L);


        /*
        @name box.clear
        @description Nulling all values in box without resizing
        */
        _CoreExport int clear(SCRIPT_STATE *L);

        /*
        @name box.boxes
        @description Returns boxes quantity
        @return quantity
        */
        _CoreExport int boxes(SCRIPT_STATE *L);


    }
	namespace tintaOCLFunctions {

		/*
        @name ocl.clear
		@description Removes data from gpu program
		@param program name
		*/
		_CoreExport int clear(SCRIPT_STATE *L);

        /*
        @name ocl.create
        @description Creates gpu program
        @param program name or path, if program with this name already exists- overwrites the old version
        */
        _CoreExport int create(SCRIPT_STATE *L);


        /*
        @name ocl.exist
        @description Determines whether the program exists or not
        @param program name or path
        @return true if program exists
        */
        _CoreExport int exist(SCRIPT_STATE *L);


		/*
        @name ocl.platdescrip
		@description Returns GPU description		
		@return GPU platforms description string
		*/
		_CoreExport int platdescrip(SCRIPT_STATE *L);

		/*
        @name ocl.devdescrip
		@description Returns GPU description util.msg(ocl.devdescrip(0)) of all installed devices on the platform
        @param platform id
        @param device id or NILL
		@return GPU devices description string
		*/
		_CoreExport int devdescrip(SCRIPT_STATE *L);

        /*
        @name ocl.programdescrip
        @description Returns GPU description util.msg(ocl.programdescrip(0,"program_name")) of all installed devices on the platform
        @param platform id
        @param Program name
        @return Program description string
        */
        _CoreExport int programdescrip(SCRIPT_STATE *L);

		/*
        @name ocl.platforms
		@description Returns GPU platforms quantity		
		@return GPU platforms quantity
		*/
		_CoreExport int platforms(SCRIPT_STATE *L);

		/*
        @name ocl.enabled
		@description Returns GPU availability		
		@return true if GPU functions enabled
		*/
		_CoreExport int enabled(SCRIPT_STATE *L);

	

		/*
			@name ocl.setarray
			@description Assigns array argument
			@param Program name			
			@param uint box id 
			@param uint mem access mask
					enNoAccess = 0x00,
				    enReadWrite = 0x01,   //CL_MEM_READ_WRITE
					enWrite = 0x02,		  //CL_MEM_WRITE_ONLY
					enRead = 0x04,        //CL_MEM_READ_ONLY
					enHostPtr = 0x08,     //CL_MEM_USE_HOST_PTR
					enAllocHostPtr = 0x10,//CL_MEM_ALLOC_HOST_PTR
					enCopyHostPtr = 0x20  //CL_MEM_COPY_HOST_PTR
		*/
		_CoreExport int setarray(SCRIPT_STATE *L);


        /*
        @name ocl.setimage
        @description Assigns array argument
        @param Program name
        @param uint image id
        @param uint mem access mask
        enNoAccess = 0x00,
        enReadWrite = 0x01,   //CL_MEM_READ_WRITE
        enWrite = 0x02,		  //CL_MEM_WRITE_ONLY
        enRead = 0x04,        //CL_MEM_READ_ONLY
        enHostPtr = 0x08,     //CL_MEM_USE_HOST_PTR
        enAllocHostPtr = 0x10,//CL_MEM_ALLOC_HOST_PTR
        enCopyHostPtr = 0x20  //CL_MEM_COPY_HOST_PTR
        */
        _CoreExport int setimage(SCRIPT_STATE *L);

		/*
			@name ocl.setvalue
			@description Assigns value argument in to the gpu program
			@param Program name
			@param Value (bool,int8,uint8,int16,uint16,int32,uint32,int64,uint64,float)				
            @param String nil(int32 or float) or "bool","int8","uint8","int16","uint16","int32","uint32","int64","uint64","float"
		*/
		_CoreExport int setvalue (SCRIPT_STATE *L);				

		/*
			@name ocl.execute
			@description Execute gpu program
			@param String Program name		
			@param uint global width
			@param uint global height(2 - dimensional) or nil           
            @param uint global depth(3 - dimensional) or nil
			@param uint position of the output parameter from 0, depends of setvalue and setvalue
            @param uint grpup size x or nil default
            @param uint grpup size y(2 - dimensional) or nil default
            @param uint grpup size z(3 - dimensional) or nil default
		*/
		_CoreExport int execute(SCRIPT_STATE *L);




              
               
	}
			
}

#endif














