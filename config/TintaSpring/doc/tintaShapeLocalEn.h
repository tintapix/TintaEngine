/*  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */


#ifndef _TINTA_SHAPE_LOCAL_EN_H_
#define _TINTA_SHAPE_LOCAL_EN_H_

    /*
    @name point.add
    @description Adds points layer
    @param name - points layer name
    @param x - position(window coordinates)
    @param y - position(window coordinates)
    @param r - 0..1.0 color R value
    @param g - 0..1.0 color G value
    @param b - 0..1.0 color B value
    @param size - in window coorinates
    */
#define POINT_add "add"

    /*
    @name point.move
    @description Moves point to the position
    @param name - points layer name
    @param index - point index
    @param x
    @param y
    */
#define POINT_move "move"

    /*
    @name point.getpos
    @description Gets point position
    @param name - points layer name
    @param index - point index
    @return x
    @return y
    */
#define POINT_getpos "getpos"


    /*
    @name point.setcolor
    @description Sets point color
    @param name - points layer name
    @param index - point index
    @param r
    @param g
    @param b
    */
#define POINT_setcolor "setcolor"


    /*
    @name point.getcolor
    @description Gets point color
    @param name - points layer name
    @param index - point index
    @return r
    @return g
    @return b
    */
#define POINT_getcolor "getcolor"


    /*
    @name point.setsize
    @description Sets points size
    @param name - points layer name   
    @param size
    */
#define POINT_setsize "setsize"

    /*
    @name point.getsize
    @description Gets points size
    @param name - points layer name    
    @return size
    */
#define POINT_getsize "getsize"

    /*
    @name point.del
    @description Deletes point from the layer(use d3.find to get index)
    @param name - points layer name
    @param index - point index
    */
#define POINT_del "del"

    /*
    @name point.find
    @description Trying to find point in position
    @param name - points layer name
    @param x - position(window coordinates)
    @param y - position(window coordinates)
    @return Uint point index or nil
    */
#define POINT_find "find"



    /*
    @name point.points
    @description Returns points quantity
    @param name - points layer name
    @return Uint points quantity
    */


#define POINT_points "points"

    /*
    @name point.addlayer
    @description Adds points layer
    @param name - points layer name
    @param name - point size
    @param type - points layer primitive type("point","line","linestrip","polygon")
    */
#define POINT_addlayer "addlayer"


    /*
    @name point.findlayer
    @description Find layer by coordinate
    @param x - position
    @param y - position
    */
#define POINT_findlayer "findlayer"


    /*
    @name point.filllayer
    @description Fill layer with points in 0,0 coordinate and color
    @param name - points layer name
    @param Quantity - points quantity
    @param r - channel
    @param g - channel
    @param b - channel

    */
#define POINT_filllayer "filllayer"


    /*
    @name point.dellayer
    @description Deletes points layer
    @param name - points layer name
    */
#define POINT_dellayer "dellayer"


    /*
    @name point.dellayer
    @description Deletes all point layers
    */
#define POINT_dellayers "dellayers"

    /*
    @name point.visible
    @description Returns points layer visibility
    */
#define POINT_visible "visible"

    /*
    @name point.setvisible
    @description Sets layer visibility 
    */
#define POINT_setvisible "setvisible"


    /*
    @name point.dellayer
    @description Deletes all point layers
    */
#define POINT_dellayers "dellayers"

    /*
    @name point.movelayer
    @description Shift points layer
    @param name - points layer name
    @param Float x shift
    @param Float y shift
    */
#define POINT_movelayer "movelayer"

    /*
    @name point.arraypos
    @description Gets positions from array
    @param name - points layer name
    @param Id - array ID
    */
#define POINT_arraypos "arraypos"


/// points end




/*
@name d3.getpos
@description Returns entity center coordinates
@param Object type: "back", "scene", "front","camera"
@param name
@return Real x
@return Real y
@return Real z
*/

#define D3_getpos "getpos"

/*
@name d3.setmaterial
@description Adds material to the entity(if need) and move it front
@param Object type: "back", "scene", "front"
@param Object name
@param Material name
*/

#define D3_setmaterial "setmaterial"


/*
@name d3.crosspoint
@description Determine intersection of the point with object(by boundary type -  Sphere default) in the world space
@param Object type: "back", "scene", "front"
@param Object name
@param x coordinate 
@param y coordinate 
@param z coordinate 
@return Boolean if contains true otherwise false
*/

#define D3_crosspoint "crosspoint"

/*
@name d3.setvisible
@description Makes object and all children visible or not
@param Object type: "back", "scene", "front"
@param Object name
@param Boolean visible
*/

#define D3_setvisible "setvisible"



/*
@name d3.mousepos
@description Returns mouse position in window coordinates
@return x coordinate
@return y coordinate
*/

#define D3_mousepos "mousepos"




/*
@name d3.setvariable
@description Assing a new value for "manual" material variable
@param Object type: "back", "scene", "front"
@param Object name
@param Material name
@param Variable name
@param Depends of variable type: 
matBool,
matInt,
matUInt,
matFloat,
matDouble,
matbVec2,
matbVec3,
matbVec4,
matiVec2,
matiVec3,
matiVec4,
matuVec2,
matuVec3,
matuVec4,
matfVec2,
matfVec3,
matfVec4,
matdVec2,
matdVec3,
matdVec4,
matfMat2,
matdMat2,
matfMat3,
matdMat3,
matfMat4,
matdMat4,
matTexture,
*/

#define D3_setvariable "setvariable"

/*
@name d3.setkey
@description Binds handler to the key
@param Key code:
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
ML_MOUSE_WEEL  = 0xF1
@param String Data to execute
@param Bool true - button down(mouse weel forward), false - button up (mouse weel backward)
*/
#define D3_setkey "setkey"

/*
@name d3.keypressed
@description Returnds key state
@param Key code
@return true - key pressed
*/
#define D3_keypressed "keypressed"


/*
@name d3.exists
@description Determines is there an object or layer in the scene
@param @param Object type: "back", "scene", "front"
@param Name object, layer
@return Bool true - exists
*/
#define D3_exists "exists"

/*
@name d3.getorient
@description Returns entity orientation quaternion  x y z w
@param Object type: "back", "scene", "front","camera"
@param Entity name
@return Real x
@return Real y
@return Real z
@return Real w
*/

#define D3_getorient "getorient"

/*
@name d3.camlookat
@description Rotates camera in the entity direction
@param name camera name by default "Cam1"
@param name entity name
*/
#define D3_camlookat "camlookat"







/*
@name d3.setorient
@description Set entity orientation quaternion
@param Object type: "back", "scene", "front","camera"
@param Name
@param Real x
@param Real y
@param Real z
@param Real w
*/
#define D3_setorient "setorient"

/*
@name d3.roll
@description Rolls entity
@param Object type: "back", "scene", "front","camera"
@param name
@param Real angle in degree
*/
#define D3_roll "roll"


/*
@name d3.setparent
@description Updates objects hierarchy 
@param Object type: "back", "scene", "front","camera"
@param String Child object name
@param String Parent object name
*/
#define D3_setparent "setparent"


/*
@name d3.yaw
@description Yaws entity
@param Object type: "back", "scene", "front","camera"
@param name
@param Real angle in degree
*/
#define D3_yaw "yaw"

/*
@name d3.pitch
@description Pitches entity
@param Object type: "back", "scene", "front","camera"
@param name
@param Real angle in degree
*/
#define D3_pitch "pitch"



/*
@name d3.rotate
@description Rotates entity
@param Object type: "back", "scene", "front","camera"
@param name
@param Real x in degree
@param Real y in degree
@param Real z in degree
@param Real speed  degree/msec or nil
@param String handler on finish or nil e.g. "remove(\"bullet\")"
*/
#define D3_rotate "rotate"

/*
@name d3.rotatecycle
@description Starts to rotate entity
@param Object type: "back", "scene", "front"
@param name
@param Real x in degree/msec
@param Real y in degree/msec
@param Real z in degree/msec
*/
#define D3_rotatecycle "rotatecycle"


/*
@name d3.setpos
@description Moves object or camera to the position e.g. d3.setpos("Enemy1", 10.0,100.0,0.0 )
@param Object type: "back", "scene", "front","camera"
@param String name
@param Real x 
@param Real y
@param Real y
@param Speed pos/mls
@param String Trigger to execute after finish or nil e.g. "remove(\"bullet\")"
*/
#define D3_setpos "setpos"


/*
@name d3.moveto
@description Moves entity to the position
@param Object type: "back", "scene", "front","camera"
@param name
@param Real x
@param Real y
@param Real z
@param Real speed msec or nil
@param String handler on finish or nil e.g. "remove(\"bullet\")"
*/
#define D3_moveto "moveto"


/*
@name d3.getdestpos
@description returns XYZ values of point from entity on distance
@param Entity name
@param Real angle offset from forward x
@param Real angle offset from forward y
@param Real angle offset from forward z
@param Real Distance
@return Real x
@return Real y
@return Real z
*/
#define D3_getdestpos "getdestpos"


/*
@name d3.moveorient
@description Moves entity depending on the direcftion
@param Object type: "back", "scene", "front","camera"
@param name
@param Real x
@param Real y
@param Real z
@param Real speed msec or nil
@param String handler on finish or nil e.g. "remove(\"bullet\")"
*/
#define D3_moveorient "D3_moveorient"


/*
@name d3.moveon
@description Moves entity along axies 
@param Object type: "back", "scene", "front","camera"
@param name
@param Real x
@param Real y
@param Real z
@param Real speed msec or nil
@param String handler on finish or nil e.g. "remove(\"bullet\")"
*/
#define D3_moveon "moveon"


/*
@name d3.getparent
@description Returns parent object name,if exists or nil
@param Object type: "back", "scene", "front"
@param name
@return Parent object name or nil
*/
#define D3_getparent "getparent"


/*
@name d3.mulscale
@description Multiplies scale on coefficient
@param Object type: "back", "scene", "front"
@param String Name
@param Real x factor
@param Real y factor
@param Real z factor
*/
#define D3_mulscale "mulscale"


/*
@name d3.scale
@description Assign scale
@param Object type: "back", "scene", "front"
@param String Name
@param Real x factor
@param Real y factor
@param Real z factor
*/
#define D3_scale "scale"


/*
@name d3.getscale
@description Return scale factors
@param Тип объекта: "back", "scene", "front"
@param String Name
@return Real x factor
@return Real y factor
@return Real z factor
*/
#define D3_getscale "getscale"


/*
@name d3.status
@description Prints scene status
*/
#define D3_status "status"

/*
@name d3.genname
@param String prefix or nil
@description Generates uniqal name
*/
#define D3_genname "genname"

/*
@name d3.wareframemode
@description Sets wareframemode
@param true - to switch on the wareframe mode
*/
#define D3_wareframemode "wareframemode"

/*
@name d3.imagesize
@description Returns image size
@param Name - image name
@return Uint w
@return Uint h
*/
#define D3_imagesize "imagesize"

/*
@name d3.addplane
@description Adds plane entity
@param name
@param Real w
@param Real h
@param Int w tesselation size
@param Int h tesselation size
@param material name
@param String optional parent name
*/
#define D3_addplane "addplane"


/*
@name d3.addbillboard
@description Adds billboard in to the scene
@param name
@param Real w
@param Real h
@param Int w tesselation size
@param Int h tesselation size
@param material name
*/
#define D3_addbillboard "addbillboard"



/*
@name d3.winsize
@description Returns widow size
@param name
@param Int w
@param Int h
*/
#define D3_winsize "winsize"

/*
@name d3.addfront
@description Adds GUI layer to the scene
@param name
@param Real position x {-1,1}
@param Real position y {-1,1}
@param Real w
@param Real h
@param Int w tesselation size
@param Int h tesselation size
@param material name
*/
#define D3_addGUILayer "addfront"

/*
@name d3.addback
@description Adds GUI layer to the scene
@param name
@param Real position x {-1,1}
@param Real position y {-1,1}
@param Real w
@param Real h
@param Int w tesselation size
@param Int h tesselation size
@param material name
@param Optional. Parrent object name
*/
#define D3_addBackLayer "addback"

/*
@name d3.addbox
@description Adds box entity
@param name
@param Real w
@param Real h
@param Real depth
@param Boolean inside flag, if true - inside box
@param material name
@param String optional parent name
*/
#define D3_addbox "addbox"


/*
@name d3.aspectratio
@description Returns aspect ratio(max/min) or 1
*/
#define D3_aspectratio "aspectratio"


/*
@name d3.addsphere
@description Adds plane entity
@param name
@param Real radius
@param Int z tesselation
@param Int radius tesselation
@param Boolean inside flag, if true - inside box
@param material name
@param String optional parent name
*/
#define D3_addsphere "addsphere"

/*
@name d3.addmesh
@description Adds mesh entity
@param name
@param mesh name
@param material name
@param String optional parent name
*/
#define D3_addmesh "addmesh"

/*
@name d3.setclear
@description Sets clear color
@param Real r
@param Real g
@param Real b
*/
#define D3_setclear "setclear"



/*
@name d3.e
@description executes lua command file
@param name
*/
#define D3_e "e"

/*
@name d3.remove
@description Removes entity
@param Object type: "back", "scene", "front"
@param Boolean Remove all children or nil
@param name
*/
#define D3_remove "remove"

/*
@name d3.removeall
@description Removes all entities
@param Object type: "back", "scene", "front"
*/
#define D3_removeall "removeall"

/*
@name d3.init
@description Trying to reinitialize all unbind resources 
*/
#define D3_init "init"

/*
@name d3.stop
@description Removes all actions from entity
@param Object type: "back", "scene", "front", "camera"
@param name
*/
#define D3_stop "stop"


/*
@name d3.setresolution
@description Changes resulution
@param Int w
@param Int h
@param Boolean fullscreen, true - switch on
*/
#define D3_setresolution "setresolution"

/*
@name d3.screenshot
@description Makes screenshot
@description String optionally prefix 
@description String optionally directory /home/screens
*/
#define D3_screenshot "screenshot"


/*
@name d3.manual
@description Starts manualy render every  with selected period
@param Integer Period of the manual timer
*/
#define D3_manual "manual"


/*
@name d3.layerback
@description Move layer back in stack on 1 position
@param Object type: "back", "front"
@param Object name
*/
#define D3_layerback "layerback"

/*
@name d3.layerfront
@description Move layer front in stack on 1 position
@param Object type: "back", "front"
@param Object name
*/
#define D3_layerfront "layerfront"

/*
@name d3.addtimer
@description Add timer
@param Int Period in milliseconds
@param String Script text or nil, or "". To call d3.settimer later
@return Integer timer id or nil
*/
#define D3_addtimer "addtimer"

/*
@name d3.settimer
@description Updates timer parameters
@param Int timer id
@param String Script text. If nil then the parameter is ignored
@return Int Period in milliseconds. If nil or <= 0 then the parameter is ignored
*/
#define D3_settimer "settimer"

/*
@name d3.deltimer
@description Delete timer
@param Int timer id
*/
#define D3_deltimer "deltimer"

/*
@name d3.deltimers
@description Delete all timers
*/
#define D3_deltimers "deltimers"



/*
@name d3.setchannel
@description Set image channel value
@param Image name(image filename)
@param x position
@param y position
@param channel(0-4)
@param Float value

*/
#define D3_setchannel "setchannel"


/*
@name d3.getchannel
@description Returns image channel value
@param Image name(image filename)
@param x position
@param y position
@param channel(0-4)
@return Float value

*/
#define D3_getchannel "getchannel"


/*
@name d3.delimage
@description Removes texture from video memory and image from resources cache
@param Image name(image filename)
@return true if image was deleted
*/
#define D3_delimage "delimage"

/*
@name d3.deltexture
@description Removes texture from video memory
@param Texture name(image filename)
@return true if texture was deleted
*/
#define D3_deltexture "deltexture"

/*
@name d3.addimage
@description Addes or removes image in to the resource cache
@param Image name(image filename)
*/
#define D3_addimage "addimage"

/*
@name d3.addmaterial
@description Add new material and compile all shaders linked with it
@param Material name
@param Material path or nil
*/
#define D3_addmaterial "addmaterial"


/*
@name d3.loadtexture
@description Loads texture in videomemory and addes image in to the resource cache
@param Texture name(image filename)
*/
#define D3_loadtexture "loadtexture"


/*
@name d3.setcursor
@description Set cursor image
@param Path to the ".cur" file.
*/
#define D3_setcursor "setcursor"



/*
@name d3.copyfont
@description makes copy of hte font
@param unsigned int font id to copy
@return unsigned int new font id
*/
#define D3_copyfont "copyfont"
/*
@name d3.addfont
@description adds new font in stack and makes the font selected
@param string font name
@param int font color r channel or nil
@param int font color g channel or nil
@param int font color b channel or nil
@param scale factor X or nil
@param scale factor Y or nil
@return unsigned int font id
*/
#define D3_addfont  "addfont"
/*
@name d3.selfont
@description select the font
@paramun usigned int font id
*/
#define D3_selfont  "selfont"
/*
@name d3.delfont
@description removes the font from the stack
@param unsigned int font id
@return true if font was deleted
*/
#define D3_delfont   "delfont"
/*
@name d3.delallfont
@description removes all fonts from the stack
*/
#define D3_delallfont  "delallfont"
/*
@name d3.fontcolor
@description changes font color
@param unsigned int font id
@param int font color r channel
@param int font color g channel
@param int font color b channel
*/
#define D3_fontcolor  "fontcolor"
/*
@name d3.rendertext
@description draw text with selected font in to the selected image
@param string text(\n as a new line)
@param int x position of the text
@param int y position of the text
@param int x space between symbols in pixels or nil
@param int y space between symbols in pixels or nil
*/
#define D3_rendertext "rendertext"


/*
@name d3.sbadd
@description add 4 - component sampler buffer
@param string unique name
@param bool true - dynamic, false - static buffer(if nil default)

*/
#define D3_sbadd "sbadd"

/*
@name d3.sbdel
@description Delete sampler buffer
@param string unique name
@treturn true if buffer was deleted
*/
#define D3_sbdel "sbdel"


/*
@name d3.sbresize
@description Resize buffer 
@param string unique name
@param int number of the 4 - component elements
@param bool - if true - fill buffer with 0 value( nil default false)
*/
#define D3_sbresize "sbresize"


/*
@name d3.sbsize
@description Return buffer 4 - component elements quantity
@param string unique name
@return elements quantity
*/
#define D3_sbsize "sbsize"

/*
@name d3.sbsetchannel
@description Set channel value of element in sampler buffer
@param string unique name
@param int element index
@param int channel
@param float  value
*/
#define D3_sbsetchannel "sbsetchannel"

/*
@name d3.sbgetchannel
@description Get channel value of element in sampler buffer
@param string unique name
@param int element index
@param int channel
@return float  value
*/
#define D3_sbgetchannel "sbgetchannel"



/*
@name d3.visible
@description Возвращает признак видимости объекта
@param Тип объекта: "back", "scene", "front"
@param Имя объекта
@return Булевое true - виден
*/

#define D3_visible "visible"



/*
@name d3.gettime
@description
*/
#define D3_gettime "gettime"

/*
@name array.delall
@description Deletes all arrays
@param Array id
*/
#define ARRAY_delall "delall"

/*
@name array.size
@description Array size
@param Array id
@return Array size
*/
#define ARRAY_size "size"


/*
@name array.resize
@description Array resize
@param Array id
*/
#define ARRAY_resize "resize"


/*
@name array.del
@description Deletes vector array
@param Array id
*/
#define ARRAY_del "del"


/*
@name array.addf
@description Adds float vector array
@return Array id
*/
#define ARRAY_addf "addf"


/*
@name array.getvalf
@description (Float values array)Returns array value by index
@param Array id
@param Index
@return float value
*/
#define ARRAY_getvalf "getvalf"

/*
@name array.popf
@description (Float values array)Removes last value
@param Array id
*/
#define ARRAY_popf "popf"


/*
@name array.pushf
@description (Float values array)Adds value in tail
@param Array id
@param float value
*/
#define ARRAY_pushf "pushf"


/*
@name array.setf
@description (Float values array)Sets value in position
@param Array id
@param Index
@param float value
*/
#define ARRAY_setf "setf"


/*
@name array.addi
@description (Integer32 values array)Adds integer32 vector array
@return Array id
*/
#define ARRAY_addi "addi"

/*
@name array.getvali
@description (Integer32 values array)Returns array values by index
@param Array id
@param Index
@return int value
*/
#define ARRAY_getvali "getvali"

/*
@name array.popi
@description (Integer32 values array)Removes the last value
@param Array id
*/
#define ARRAY_popi "popi"


/*
@name array.pushi
@description (Integer32 values array)Adds value in the tail
@param Array id
@param int value
*/
#define ARRAY_pushi "pushi"


/*
@name array.seti
@description (Integer32 values array)Sets value in position
@param Array id
@param Index
@param int value
*/
#define ARRAY_seti "seti"





/*
@name ocl.create
@description Creates gpu program or clears data if the program already exists
@param program name or path
*/
#define OCL_create "create"

/*
@name ocl.platdescrip
@description Returns GPU description
@return GPU platforms description string
*/
#define OCL_platdescrip "platdescrip"

/*
@name ocl.devdescrip
@description Returns GPU description util.msg(ocl.devdescrip(0,60))
@param device id
@param GPUDevAll = 60		// all properties
@return GPU devices description string
*/
#define OCL_devdescrip "devdescrip"

/*
@name ocl.platforms
@description Returns GPU platforms quantity
@return GPU platforms quantity
*/
#define OCL_platforms "platforms"

/*
@name ocl.enabled
@description Returns GPU availability
@return true if GPU functions enabled
*/
#define OCL_enabled "enabled"



/*
@name ocl.setargarray
@description Assigns array argument
@param Program name
@param uint array id
@param uint mem access mask
enNoAccess = 0x00,
enReadWrite = 0x01,   //CL_MEM_READ_WRITE
enWrite = 0x02,		  //CL_MEM_WRITE_ONLY
enRead = 0x04,        //CL_MEM_READ_ONLY
enHostPtr = 0x08,     //CL_MEM_USE_HOST_PTR
enAllocHostPtr = 0x10,//CL_MEM_ALLOC_HOST_PTR
enCopyHostPtr = 0x20  //CL_MEM_COPY_HOST_PTR
*/
#define OCL_setargarray "setargarray"


/*
@name ocl.setvalue
@description Assigns value argument in to the gpu program
@param Program name
@param Value (int8,int16,int32,int64,float,double)
*/
#define OCL_setvalue "setvalue"

/*
@name ocl.execute
@description Execute gpu program
@param String Program name
@param String Script(buffer or file) to execute after finishing
@param uint global width
@param uint global height(2 - dimensional) or nil
@param uint global depth(3 - dimensional) or nil
@param uint position of the output parameter from 0, depends of setvalue and setvalue
@param uint grpup size x or nil default
@param uint grpup size y(2 - dimensional) or nil default
@param uint grpup size z(3 - dimensional) or nil default
*/
#define OCL_execute "execute"

/*
@name ocl.exist
@description Determines whether the program exists or not
@param program name or path
@return true if program exists
*/
#define OCL_exist "exist"


/*
@name ocl.enabled
@description Returns GPU availability
@return true if GPU functions enabled
*/
#define OCL_enabled "enabled"


/*
@name ocl.clear
@description Removes data from gpu program
@param program name
*/
#define OCL_clear "clear"


/*
@name ocl.setarray
@description Задать массив данных вход/выход для расчета
@param Имя программы
@param Id массива
@param Маска параметра
enNoAccess = 0x00,
enReadWrite = 0x01,   //CL_MEM_READ_WRITE
enWrite = 0x02,		  //CL_MEM_WRITE_ONLY
enRead = 0x04,        //CL_MEM_READ_ONLY
enHostPtr = 0x08,     //CL_MEM_USE_HOST_PTR
enAllocHostPtr = 0x10,//CL_MEM_ALLOC_HOST_PTR
enCopyHostPtr = 0x20  //CL_MEM_COPY_HOST_PTR
*/
#define OCL_setarray "setarray"


/*
@name ocl.remove
@description Удаляет программу
@param имя программы
*/
#define OCL_remove "remove"


#endif