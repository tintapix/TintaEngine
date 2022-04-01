require "Scripts/lib/l_util"
require "paths"


function s_corktexture( width, height, pathDestIn, points, maxStep, offsetXIn, offsetYIn, arrColor )

	local picImg = 0 
	
	local offsetX = 0
	local offsetY = 0
	
	if offsetXIn ~= nil then
		offsetX = offsetXIn
	end
	
	if offsetYIn ~= nil then
		offsetY = offsetYIn
	end

	local sizePallete = s_arrlength(arrColor)
	
	local picImg = image.create( width, height , "seamless" )
	
	image.fillb(255,255,255)
	image.fillchannelf( 3,1 )
	
	local w = image.width()
	local h = image.height()	
	
	for i = 1, points do		

		local xPos = main.randint(offsetX, w - offsetX)
		local yPos = main.randint(offsetY, h - offsetY)
		--image.setpixelb( xPos,yPos, 0,0,0, 1 )		
		local rVal, gVal,bVal = util.unpackcolor3( arrColor[main.randintex(1, sizePallete)] )
		
		
		for r = 1, maxStep do		
			
			xPos, yPos = s_posByDirection4( xPos, yPos, main.randint( 1,4 ), 1, w, h )	
			
			
			image.setpixelb( xPos, yPos, rVal, gVal,bVal, 1 )											
		end
		--image.setchannelf( w_,h_, util.bytetofloat( a ) )				
	end
				
	--image.save( s_getex(pathDestIn), pathDestIn )
	--image.erase( picImg )	
	
	function save()
		
		image.save( "png", pathDestIn )
		util.msg("saved")
		
		image.eraseall()
	end


	window.setkey(0x1D, "save()") -- ML_LCONTROL    = 0x1D,	

	window.redraw()
	util.msg("finished")	
	
end


local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )

local pallete = "1.png"
local arrColor =  s_getpallete( pallete ) --s_genColorsRandTable(0,255, 3 )  --
--function s_corktexture( width, height, pathDestIn, points, maxStep, edgeOffsetX, edgeOffsetY )
s_corktexture( 1000,1000, PathOut,4000, 50, 10,10, arrColor )