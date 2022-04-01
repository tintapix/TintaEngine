require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"



pathIn = "C:/foto/test5.png"
local ext = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )

local picImg1 = image.read( pathIn, ext )
pathIn = "C:/foto/3132729095_202021995333343_20631.png"

local picImg2 = image.read( pathIn, ext )

image.select(picImg1)
image.mix(picImg2, 0,0 )

image.save( "png", PathOut )

image.erase(picImg1)
image.erase(picImg2)