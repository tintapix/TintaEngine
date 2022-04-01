require "Scripts/lib/l_util"
require "paths"


function s_randpixelsFromArray(wIn, hIn, pathdestIn , arrayIn )
	
	local arrLen = s_arrlength( arrayIn )
	local outImage = image.create( wIn, hIn , "basic" )	

	local w = image.width()
	local h = image.height()	
	
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do				
				
				
				local pos = main.randint( 1, arrLen )	
				local color = arrayIn[pos]
				
				local r,g,b,a = util.unpackcolor4( color )
				
				image.setpixelb( w_, h_, r,g,b, 1 )				
									
				image.setchannelb( w_,h_,3, a )						
			end		
	end	
	image.save( s_getex(pathdestIn), pathdestIn )		
	image.erase( outImage )
	
	
	util.msg("s_randpixelsFromArray finished")
end


-- noise from color array
nameColors = "2.png"
local arrColor = s_getpalletea(nameColors)
r = s_buildColorArrayA( arrColor, 2)
local outExt = "png"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
--s_randpixelsFromArray( 500,500, PathOut, r )
--s_randcolorpoins( 2000,2000, PathOut , 255, 255, 255, 255,0,0 , 400 )
--local maskPath = "D:/mya/textures/_A/IMG_6581.JPG" 
--function s_randpixelsFromArrayMask(wIn, hIn, pathdestIn, maskPathIn, factorIn, invertIn, arrayIn )
util.msg("-")
s_randpixelsFromArray( 1000,1000, PathOut, r )


util.msg(a)
--window.redraw()