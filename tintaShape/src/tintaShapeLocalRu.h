/**  Copyright (C) 2011 - 2020 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  
**/


#ifndef _TINTA_SHAPE_LOCAL_RU_H_
#define _TINTA_SHAPE_LOCAL_RU_H_

/*
@name point.add
@description Adds points layer
@param name - points layer name
@param x - position(window coordinates)
@param y - position(window coordinates)
@param r - 0..1.0 color R value
@param g - 0..1.0 color G value
@param b - 0..1.0 color B value
@param a - 0..1.0 color A value
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
@param a
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
@return a
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
@name point.filllayer
@description Fill layer with points in 0,0 coordinate and color
@param name - points layer name
@param Quantity - points quantity
@param r - channel
@param g - channel
@param b - channel
@param a - channel
*/
#define POINT_filllayer "filllayer"

/*
@name point.swap
@description ������ ���� �������
@param name1 - ������ ���� 
@param name2 - ������ ����
*/
#define POINT_swap "swap"

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




/*
@name d3.getpos
@description ���������� ���������� ������� � ����� ��������: d3.getpos("Enemy1")
@param ��� �������: "back", "scene", "front","camera"
@param ��� �������
@return ����� � ��������� ������ x
@return ����� � ��������� ������ y
@return ����� � ��������� ������ z
*/

#define D3_getpos "getpos"

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
@description ���������� ������� ��������� �������
@param ��� �������: "back", "scene", "front"
@param ��� �������
@return ������� true - �����
*/

#define D3_visible "visible"



/*
@name d3.setmaterial
@description ��������� � ������� ��������, ���� ��� ��� � ������ � ������ ��� ������� ��� �� �������� ����
@param ��� �������: "back", "scene", "front"
@param ��� �������
@param ��� ���������
*/

#define D3_setmaterial "setmaterial"

/*
@name d3.crosspoint
@description ���������� ����������� ����� � ����������� � ��������, �������� ��� ���������( �� ��������� �����)
@param Object type: "back", "scene", "front"
@param Object name
@param x ����������
@param y ����������
@param z ����������
@return true - ���� ����������� �������
*/

#define D3_crosspoint "crosspoint"


/*
@name d3.mousepos
@description ���������� ��������� ���� � ������� �����������
@return x coordinate
@return y coordinate
*/

#define D3_mousepos "mousepos"


/*
@name d3.setvisible
@description ������ ������ � ��� ��� �������� ������� ������� ��� ���
@param Object type: "back", "scene", "front"
@param Object name
@param Boolean visible
*/

#define D3_setvisible "setvisible"

/*
@name d3.setvariable
@description ������ ����� �������� ��� "manual" ���������� � ���������
@param ��� �������: "back", "scene", "front"
@param ��� �������
@param ��� ���������
@param ��� ����������
@param ��� �������� � ����������� �� ���� ����������:
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
@name d3.getorient
@description ���������� ���������� ���������� �������  x y z w
@param ��� �������: "back", "scene", "front","camera"
@param Entity name
@return Real x
@return Real y
@return Real z
@return Real w
*/

#define D3_getorient "getorient"


/*
@name d3.camlookat
@description ������������ ������ � ������� �������, ����� ������������ � ���� � getpos
@param ������ ��� ������ ��� ������ ������. �� ��������� ������� ������ - d3.camlookat("","Enemy1")
@param ������ ��� �������
*/
#define D3_camlookat "camlookat"




/*
@name d3.setorient
@description ������ ���������� ���������� �������
@param ��� �������: "back", "scene", "front","camera"
@param ������ ��� �������
@param ����� � ��������� ������ x
@param ����� � ��������� ������ y
@param ����� � ��������� ������ z
@param ����� � ��������� ������ w
*/
#define D3_setorient "setorient"

/*
@name d3.setparent
@description ������������� �������� ��������
@param  ��� �������: "back", "scene", "front","camera"
@param ������ ��� ��������� �������
@param ������ ��� ������������� �������
*/
#define D3_setparent "setparent"


/*
@name d3.rotate
@description ������������ ������ �� ����: d3.rotate("enemy1", 0.3, 0.0, 100.0 )
@param ��� �������: "back", "scene", "front","camera"
@param ������ ��� �������
@param ����� � ��������� ������ ���� �� ��� X
@param ����� � ��������� ������ ���� �� ��� Y
@param ����� � ��������� ������ ���� �� ��� Z
@param ����� � ��������� ������ �������� �������� (�������� / ��������) ��� 0
@param ������ ���������� �� ���������� ��� ������ ������ �������� "remove(\"bullet\")"
*/
#define D3_rotate "rotate"

/*
@name d3.rotatecycle
@description �������� �������� ������� ������: d3.rotatecycle("Enemy1", 0.1,0.1,0.2 )
@param ��� �������: "back", "scene", "front"
@param ������ ��� �������
@param ����� � ��������� ������ ���� �� ��� x (�������� / ��������)
@param ����� � ��������� ������ ���� �� ��� y (�������� / ��������)
@param ����� � ��������� ������ ���� �� ��� z (�������� / ��������)
*/
#define D3_rotatecycle "rotatecycle"



/*
@name d3.moveto
@description ��������� ������ � ���������� d3.setpos("Enemy1", 10.0,100.0,0.0 )
@param ��� �������: "back", "scene", "front","camera"
@param ������
@param ����� � ��������� ������ ���������� x
@param ����� � ��������� ������ ���������� y
@param ����� � ��������� ������ ���������� z
@param ����� � ��������� ������ �������� �����������(�������/����) ��� 0
@param ������ ���������� �� ���������� ��� ������ ������ �������� "remove(\"bullet\")"
*/
#define D3_moveto "moveto"


/*
@name d3.getdestpos
@description ���������� ���������� ��������� ����� � ����������� �� ������� �� ����������: d3.getdestpos("Enemy1", 10.0,5.0,10.0, 35.0 )
@param ��� �������: "back", "scene", "front"
@param ������ ��� �������
@param ����� � ��������� ������ ���� ���������� �� ��������� ������ �� ��� X
@param ����� � ��������� ������ ���� ���������� �� ��������� ������ �� ��� Y
@param ����� � ��������� ������ ���� ���������� �� ��������� ������ �� ��� Z
@param ����� � ��������� ������ ����������
@return ����� � ��������� ������ x
@return ����� � ��������� ������ y
@return ����� � ��������� ������ z
*/
#define D3_getdestpos "getdestpos"


/*
@name d3.moveorient
@description ���������� ������ �� ���������� �������� ����������: d3.moveorient( "Enemy1", 100.0,13.0,5.0 ) ��� d3.moveon( "Enemy1", 100.0,13.0,5.0, 0.0 ), �������� ������ ��� ������������ ��������
@param ��� �������: "back", "scene", "front","camera"
@param ������ ��� �������
@param ����� � ��������� ������ ���������� �� ��� x
@param ����� � ��������� ������ ���������� �� ��� y
@param ����� � ��������� ������ ���������� �� ��� z
@param ����� � ��������� ������ �������� (�������/����) ��� 0
@param ������ ���������� �� ���������� ��� ������ ������ �������� "remove(\"bullet\")"
*/
#define D3_moveorient "moveorient"


/*
@name d3.moveon
@description ���������� ������ �� ���������� ����� ����
@param ��� �������: "back", "scene", "front","camera"
@param ������ ��� �������
@param ����� � ��������� ������ ���������� �� ��� x
@param ����� � ��������� ������ ���������� �� ��� y
@param ����� � ��������� ������ ���������� �� ��� z
@param ����� � ��������� ������ �������� (�������/����) ��� 0
@param ������ ���������� �� ���������� ��� ������ ������ �������� "remove(\"bullet\")"
*/
#define D3_moveon "moveon"

/*
@name d3.mulscale
@description �������� ��������  ��������������� �� �����������
@param ��� �������: "back", "scene", "front"
@param ������ ��� �������
@param ����� � ��������� ���������� �� ��� x
@param ����� � ��������� ���������� �� ��� y
@param ����� � ��������� ���������� �� ��� z
*/
#define D3_mulscale "mulscale"

/*
@name d3.scale
@description Pflftn ��������  ���������������
@param ��� �������: "back", "scene", "front"
@param ������ ��� �������
@param ����� � ��������� ���������� �� ��� x
@param ����� � ��������� ���������� �� ��� y
@param ����� � ��������� ���������� �� ��� z
*/
#define D3_scale "scale"


/*
@name d3.getscale
@description ������ ���������� ���������������
@param ��� �������: "back", "scene", "front"
@param ������ ��� �������
@return ����� � ��������� ���������� �� ��� x
@return ����� � ��������� ���������� �� ��� y
@return ����� � ��������� ���������� �� ��� z
*/
#define D3_getscale "getscale"



/*
@name d3.status
@description ������� �� ������� ��������� �����
*/
#define D3_status "status"

/*
@name d3.genname
@param ������ ��������� �������: d3.genname("Object")
@description ���������� ���������� ���
@return ������ ���
*/
#define D3_genname "genname"

/*
@name d3.wareframemode
@description �������� ����� ����������� �������� � ���� ��������
@param ������� true - ��������, false - ���������
*/
#define D3_wareframemode "wareframemode"

/*
@name d3.imagesize
@description ���������� ������� �����������
@param Name - ��� �����������
@return Uint w
@return Uint h
*/
#define D3_imagesize "imagesize"

/*
@name d3.addplane
@description ��������� ��������� � �����: d3.addplane( "main_plane", 100000,100000, 10, 10, "tile.material" )
@param ������ ��� ���������
@param ����� � ��������� ������ �����
@param ����� � ��������� ������ ������
@param ����� ����� w ���������� ����� �� �����
@param ����� ����� h ���������� ����� �� ������
@param ������ ��� ���������(�����������)
@param ������ ��� ������������� �������(�����������)
*/
#define D3_addplane "addplane"


/*
@name d3.addbox
@description ��������� �������������� � �����: d3.addbox( "Box1", 10,10,10, false, "lightvert.material" )
@param ������ ���
@param ����� � ��������� ������ ������
@param ����� � ��������� ������ ������
@param ����� � ��������� ������ �������
@param ������� ������ false 
@param ������ ��� ���������(�����������)
@param ������ ��� ������������� �������(�����������)
*/
#define D3_addbox "addbox"


/*
@name d3.addbillboard
@description ��������� ��������� � �����, ������ ���������� �� ���� z � ������: d3.addbillboard( "billboard", 100,100, 5, 5, "color.material" )
@param ������ ��� ���������
@param ����� � ��������� ������ �����
@param ����� � ��������� ������ ������
@param ����� ����� w ���������� ����� �� �����
@param ����� ����� h ���������� ����� �� ������
@param ������ ��� ���������(�����������)
*/
#define D3_addbillboard "addbillboard"


/*
@name d3.winsize
@description ���������� ������� ����
@param name
@param ����� w
@param ����� h
*/
#define D3_winsize "winsize"

/*
@name d3.addsphere
@description ��������� ����� � �����: d3.addsphere( "Sphere1", 5,10,5, false , "fill_black.material" )	
@param ��� ������
@param ����� � ��������� ������ ������
@param ����� ����� ���������� ����� �� z
@param ����� ����� ���������� ����� �� �������
@param ������� ������ false 
@param ������ ��� ���������(�����������)
@param ������ ��� ������������� �������(�����������)
*/
#define D3_addsphere "addsphere"


/*
@name d3.addfront
@description ��������� ���� GUI
@param name
@param ����� � ��������� ��������� x {-1,1}
@param ����� � ��������� ��������� y {-1,1}
@param ����� � ��������� w
@param ����� � ��������� h
@param ����� ���������� ����� �� x 
@param ����� ���������� ����� �� y
@param �������� name
*/
#define D3_addGUILayer "addfront"

/*
@name d3.addback
@description ��������� ���� GUI
@param name
@param ����� � ��������� ��������� x {-1,1}
@param ����� � ��������� ��������� y {-1,1}
@param ����� � ��������� w
@param ����� � ��������� h
@param ����� ���������� ����� �� x
@param ����� ���������� ����� �� y
@param �������� name
@param �����������. ������������ ������ name
*/
#define D3_addBackLayer "addback"


/*
@name d3.addmesh
@description ��������� ������� ������ � �����: d3.addmesh( "Mesh1", "box.mmesh", "lightvert.material" )
@param ������ ��� �������
@param ������ ��� �������
@param ������ ��� ���������(�����������)
@param ������ ��� ������������� �������(�����������)
*/
#define D3_addmesh "addmesh"


/*
@name d3.aspectratio
@description ���������� ����������� ������ ���� �� �������� max/min ��� 1
*/
#define D3_aspectratio "aspectratio"

/*
@name d3.setclear
@description ���������� ���� ������� ����
@param ����� � ��������� ������ r � ��������� �� 0.0 �� 1.0
@param ����� � ��������� ������ g � ��������� �� 0.0 �� 1.0
@param ����� � ��������� ������ b � ��������� �� 0.0 �� 1.0
*/
#define D3_setclear "setclear"



/*
@name d3.e
@description ��������� ���� �������� lua: d3.e("test.lua")
@param ������ ��� ��������
*/
#define D3_e "e"

/*
@name d3.remove
@description ������� ������ �� �����
@param ��� �������: "back", "scene", "front"
@param ������� ������� ��� �������� ������� ��� nil
@param ������ ��� �������
*/
#define D3_remove "remove"


/*
@name d3.getparent
@description ���������� ��� ������������� �������, ���� ������� �������
@param ��� �������: "back", "scene", "front"
@param ��� �������
@return ��� ������������� ������� ��� nil
*/
#define D3_getparent "getparent"

/*
@name d3.removeall
@param ��� �������: "back", "scene", "front"
@description ������� ��� ������� �� �����
*/
#define D3_removeall "removeall"

/*
@name d3.init
@description �������� ��������������������, ��� �� ��������������� �������
*/
#define D3_init "init"

/*
@name d3.stop
@description ������� ��� �������� �� ������� ��� �������
@param ��� �������: "back", "scene", "front","camera"
@param ������ ��� �������
*/
#define D3_stop "stop"


/*
@name d3.setresolution
@description �������� ���������� ������
@param ����� ����� ������ 
@param ����� ����� ������
@param ������� ���������� ������������� �����, true - �������������
*/
#define D3_setresolution "setresolution"

/*
@name d3.screenshot
@description ������� ������ ������ � ��������� � ����
@param ������ �������������� ������� ��� �����
@param ������ �������������� ���������� ���� � ���������� ����  /home/screens
*/
#define D3_screenshot "screenshot"

/*
@name d3.layerback
@description �������� ���� � ������� ��������� ����� �� 1 ��������� 
@param ��� �������: "back", "front"
@param ������ ��� �������
*/
#define D3_layerback "layerback"

/*
@name d3.layerfront
@description �������� ���� � ������� ��������� ������ �� 1 ���������
@param ��� �������: "back", "front"
@param ������ ��� �������
*/
#define D3_layerfront "layerfront"

/*
@name d3.layerswap
@description ������ ������� ���� � ������� ���������
@param ��� �������: "back", "front"
@param ������ ��� ������� ����
@param ������ ��� ������� ����
*/
#define D3_layerswap "layerswap"

/*
@name d3.manual
@description �������� ��������� ������� ����� ������� �� ���������� ������� �������
@param ����� ������ �������
*/
#define D3_manual "manual"

/*
@name d3.gettime
@description 
*/
#define D3_gettime "gettime"

/*
@name d3.addtimer
@description ��������� ������ ��� ���������� �� �������
@param ����� ����� ������ � �������������
@param ������ ����� �������. ����� ���� ������ ������� ��� nil. ������� d3.settimer �����
@return ����� - id ������� ��� nil
*/
#define D3_addtimer "addtimer"


/*
@name d3.settimer
@description ��������� ������
@param ����� ����� id �������
@param ������ ����� �������. ���� nil, �� �� ��������� ��� ������ 
@return ����� ����� ������ � �������������. ���� nil ��� <= 0, �� �� ��������� ��� ������ 
*/
#define D3_settimer "settimer"


/*
@name d3.delimage
@description ������� �������� �� ����������� � ����������� �� ��������� ��������
@param ������ ��� ����������� (��� �����)
@return true if image was deleted
*/
#define D3_delimage "delimage"

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
@name d3.addmaterial
@description ��������� ����� ��������� � ����������� ��� ��������� ���������, ��������� � ���
@param ��� ����������
@param ���� � ����������. ����� ���� nil(����� �� ����� �� ���������)
*/
#define D3_addmaterial "addmaterial"

/*
@name d3.deltexture
@description ������� �������� �� �����������
@param ������ ��� �������� (��� �����)
@return true ���� �������� ���� �������
*/
#define D3_deltexture "deltexture"

/*
@name d3.loadtexture
@description ��������� �������� � ����� ������ � ��������� ����������� � �������
@param ������ ��� �������� (��� �����)
*/
#define D3_loadtexture "loadtexture"

/*
@name d3.addimage
@description ��������� ��� �������������� ����������� � �������
@param ������ ��� ����������� (��� �����)
*/
#define D3_addimage "addimage"

/*
@name d3.setkey
@description ��������� ������ ��� ���������� �� ������� ��� ������� �������
@param ��� �������:
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
@param ������ ����� �������
@param ������� true - �� ������� ����, false - �����, ��� ������ ����(ML_MOUSE_WEEL) true - ��������� ������
*/
#define D3_setkey "setkey"

/*
@name d3.keypressed
@description ���������� ��������� �������
@param ��� ������� �� setkey
@return ������� true - ������� ������
*/
#define D3_keypressed "keypressed"


/*
@name d3.roll
@description ������  Roll �������
@param Object type: "back", "scene", "front","camera"
@param ��� �������
@param ���� � ��������
*/
#define D3_roll "roll"


/*
@name d3.yaw
@description ������ Yaw �������
@param Object type: "back", "scene", "front","camera"
@param ��� �������
@param ���� � ��������
*/
#define D3_yaw "yaw"

/*
@name d3.pitch
@description ������ Pitch �������
@param Object type: "back", "scene", "front","camera"
@param ��� �������
@param ���� � ��������
*/
#define D3_pitch "pitch"


/*
@name d3.exists
@description ���������� ������� ������� ������� ��� ���� � ����� �� �����
@param @param ��� �������: "back", "scene", "front"
@param ��� �������, ����
@return ������� true - ������������
*/
#define D3_exists "exists"


/*
@name d3.deltimer
@description ������� ������
@param ����� ����� ������������� �������
*/
#define D3_deltimer "deltimer"

/*
@name d3.deltimers
@description ������� ��� �������
*/
#define D3_deltimers "deltimers"


/*
@name d3.setcursor
@description ���������� ����������� �������
@param ���� � ".cur" �����.
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
@name array.del
@description ������� ������(������) ��������
@param Id �������
*/
#define ARRAY_del "del"

/*
@name array.size
@description ���������� ������ �������
@param Id �������
@return ������
*/

#define ARRAY_size "size"

/*
@name array.resize
@description �������� ������ �������
@param Array id
*/
#define ARRAY_resize "resize"

/*
@name array.delall
@description ������� ��� �������
@param Id �������
*/
#define ARRAY_delall "delall"


/*
@name array.addf
@description (������ �������� � ��������� �������) ��������� ������(������) �������� � ��������� �������
@return Id �������
*/
#define ARRAY_addf "addf"

/*
@name array.getf
@description (������ �������� � ��������� �������)���������� �������� �� �������
@param Id �������
@param ������ � �������( 0 < index < array_size)
@return ��������
*/
#define ARRAY_getvalf "getf"

/*
@name array.popf
@description (������ �������� � ��������� �������)������� ��������� ������ ������� �� 1 ���������� ��������� ��������
@param Id �������
*/
#define ARRAY_popf "popf"


/*
@name array.pushf
@description (������ �������� � ��������� �������)��������� �������� � ����� �������, ���������� ��� �� 1
@param Id �������
@param ��������
*/
#define ARRAY_pushf "pushf"


/*
@name array.setf
@description (������ �������� � ��������� �������)������ �������� �� �������
@param Id �������
@param ������ � �������( 0 < index < array_size)
@param ��������
*/
#define ARRAY_setf "setf"


/*
@name array.addi
@description (������ ������������� ��������) ��������� ������(������) ������������� ��������
@return Id �������
*/
#define ARRAY_addi "addi"

/*
@name array.geti
@description (������ ������������� ��������)���������� �������� �� �������
@param Id �������
@param ������ � �������( 0 < index < array_size)
@return ��������
*/
#define ARRAY_getvali "geti"

/*
@name array.popi
@description (������ ������������� ��������)������� ��������� ������ ������� �� 1 ���������� ��������� ��������
@param Id �������
*/
#define ARRAY_popi "popi"


/*
@name array.pushi
@description (������ ������������� ��������)��������� �������� � ����� �������, ���������� ��� �� 1
@param Id �������
@param ��������
*/
#define ARRAY_pushi "pushi"


/*
@name array.seti
@description (������ ������������� ��������)������ �������� �� �������
@param Id �������
@param ������ � �������( 0 < index < array_size)
@param ��������
*/
#define ARRAY_seti "seti"




/*
@name ocl.create
@description ������� OCL ��������� ��� ������� ������ ���� ��� ��� �����������
@param ��� ��������� ��� ���� � ����� � ���
*/
#define OCL_create "create"

/*
@name ocl.platdescrip
@description ���������� �������� ���������
@return �������� ���������
*/
#define OCL_platdescrip "platdescrip"


/*
@name ocl.exist
@description ���������� ������� ��� ��������� ���������
@param ��� ���������
@return true - ���� ��� �������
*/
#define OCL_exist "exist"

/*
@name ocl.devdescrip
@description ���������� �������� ���������� GPU util.msg(ocl.devdescrip(0,60))
@param device id
@param ���� �������������� ��������, ������ �������� ������� - GPUDevAll = 60		
@return ��������
*/
#define OCL_devdescrip "devdescrip"

/*
@name ocl.platforms
@description ���������� ���������� GPU ��������
@return ���������� GPU ��������
*/
#define OCL_platforms "platforms"

/*
@name ocl.enabled
@description ���������� ������� ����������� GPU �������
@return true - ���� ��������
*/
#define OCL_enabled "enabled"

/*
@name ocl.clear
@description ������� ������ �� ���������
@param ��� ���������
*/
#define OCL_clear "clear"


/*
@name ocl.remove
@description ������� ���������
@param ��� ���������
*/
#define OCL_remove "remove"


/*
@name ocl.setarray
@description ������ ������ ������ ����/����� ��� �������
@param ��� ���������
@param Id �������
@param ����� ���������
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
@name ocl.setvalue
@description ������ ���� ������ ����/����� ��� �������
@param ��� ���������
@param �������� ����: int8,int16,int32,int64,float,double
*/
#define OCL_setvalue "setvalue"

/*
@name ocl.execute
@description ��������� ��������� � ������� ���������
@param ��� ���������
@param uint ���������� ������ ������� ������
@param uint ���������� ������ ������� ������(2 - ������ ������) ��� nil
@param uint ���������� ������� ������� ������(3 - ������ ������) ��� nil
@param uint ��������� ���������� ����� ��������� � ��������� �������, ��������� setarray ��� setvalue
@param uint ������ ������ �� x ��� nil �� ���������
@param uint ������ ������ �� y(2 - ������ ������) ��� nil �� ���������
@param uint ������ ������ �� z(3 - ������ ������) ��� nil �� ���������
*/
#define OCL_execute "execute"




#endif