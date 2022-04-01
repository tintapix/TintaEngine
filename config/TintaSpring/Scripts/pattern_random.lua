require "Scripts/lib/l_util"
require "Scripts/lib/patterns"
require "Scripts/lib/interp"

require "paths"

local w = 2000	
local h = 2000

local Dir = "C:/foto/pattern/p"

local quantity = 10
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, "gradient", outExt )
s_addRandomImgOther( w, h, "seamless", Dir, "png", PathOut, quantity, false )