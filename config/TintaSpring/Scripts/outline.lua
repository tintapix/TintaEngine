require "Scripts/lib/l_util"
require "paths"

PathIn = "C:/foto/pattern/bc/6.png"
PathOut = string.format( "%s/%s.%s", pic_dir_filtered, "bc6", "png" )


-- function outlineAlphaPixel( imgPath, imgOut, rO,gO,bO,  layers )

outlineAlphaPixel( PathIn, PathOut, 255,110,0,  3 )


util.msg("*****outlineAlphaPixel finished*****")