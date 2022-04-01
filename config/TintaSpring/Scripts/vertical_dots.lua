require "Scripts/lib/l_util"
require "paths"

	
--[[
	Draws vertical dots
]]--
function s_vertical( heightIn, palleteNameIn, arrayOutSizeIn, randomPallete )

	local arrColor = nil
	
	if randomPallete == false then
		arrColor = s_getpallete( palleteNameIn )
	else 
		arrColor = s_genColorsRandTable(0,255, arrayOutSizeIn ) 
	end	
	
	local arrOut = s_buildColorArray( arrColor, arrayOutSizeIn )	
	
	local picImg = image.create( 1, heightIn , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	
	image.select( picImg )
	
	local w = image.width()
	local h = image.height()
	
	image.fillb(255,255,255)
	image.fillchannelb(3, 255)		
	
	local sizeArr = s_arrlength( arrOut )	
	
	for i = 1, 	heightIn do										
	
		local color = arrOut[main.randint( 1, sizeArr )]
		
		local bR,bG,bB = util.unpackcolor3( color )
		
		image.setpixelb( 0,i - 1, bR, bG, bB, 1.0)								
		
	end		
	
	function save()
		local name = main.uniqid()
		--local name = "out"
		local PathOut = string.format("%s/%s.%s", pic_dir_filtered, name, "png" )
		image.save( "png", PathOut )
		util.msg("saved")
	end


	window.setkey(0x1D, "save()") -- ML_LCONTROL    = 0x1D,	

	window.redraw()
	
	util.msg("***s_vertical finished")

end


s_vertical( 300, "5.png", 10, false )