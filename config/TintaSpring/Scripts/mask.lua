require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"

local PathIn = "c:/foto/rez/3132729095_2020112694957507_7877.png"
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
-- factor < 0.5 -> more sensetive to the light
--local rVal, gVal,bVal = util.c_unpackcolor(0x2d4abf)
--s_maskchannel( pathIn, pathOut , rIn,gIn,bIn, channel, factor, invert, bAlphaTestIn )
s_maskchannel( PathIn, PathOut, 0,0,0, 0, 0.7	, false, true)