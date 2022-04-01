require "Scripts/lib/l_util"

require "Scripts/lib/interp"

require "paths"



--[[
	Draws circle gradient
]]--
function s_circlegradient( outPathIn, widthIn, heightIn, xPosIn, yPssIn, palleteNameIn, arrayOutSizeIn, bInvetIn )

	local exImageOut = s_getex(outPathIn)	
	
	local arrColor = s_getpalletea(palleteNameIn)
	
	local arrOut = s_buildColorArrayA( arrColor, arrayOutSizeIn )	
	
	local picImg = image.create( widthIn, heightIn , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	image.select( picImg )
	local w = image.width()
	local h = image.height()
	image.fillb(255,255,255)
	
	local xCount = 0
	local yCount = 0
		
	local lenMax = math.max(w,h)/2
	local sizeArr = s_arrlength(arrOut)
	while xCount < w do			
		while yCount < h do				
						
			local len = s_dist2(xCount, yCount, xPosIn, yPssIn )
			
			local f = math.min(len,lenMax)/lenMax
			--util.util.msgf(f)
			if bInvetIn == true then 
				f = 1 - f
			end
			local pos, color = s_getDiscreteColorByFactor( arrOut,sizeArr, f )				
			local bR,bG,bB,bA = util.unpackcolor4( color )
			
			local rMed = util.bytetofloat( bR )
			local gMed = util.bytetofloat( bG )
			local bMed = util.bytetofloat( bB )
			--util.util.msgf(rMed, " ",gMed," ",bMed)
			image.setchannelb(xCount,yCount,3,bA)
			image.setpixelf( xCount,yCount, rMed, gMed, bMed, 1.0)						
			
			yCount = yCount + 1
		end		
		yCount = 0
		xCount = xCount + 1		
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
	
	util.msg("***s_circlegradient finished")

end

--[[
	Draws vert gradient
]]--
function s_vertgradient( outPathIn, widthIn, heightIn, palleteNameIn, arrayOutSizeIn, bInvetIn )

	local exImageOut = s_getex(outPathIn)	
	
	local arrColor = s_getpalletea(palleteNameIn)
	
	local arrOut = s_buildColorArrayA( arrColor, arrayOutSizeIn )	
	
	local picImg = image.create( widthIn, heightIn , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	image.select( picImg )
	local w = image.width()
	local h = image.height()
	image.fillb(255,255,255)
	
	local xCount = 0
	local yCount = 0
		
	local lenMax = heightIn
	local sizeArr = s_arrlength(arrOut)
	while xCount < w do			
		while yCount < h do				
						
			
			
			local f = yCount/lenMax
			--util.util.msgf(f)
			if bInvetIn == true then 
				f = 1 - f
			end
			local pos, color = s_getDiscreteColorByFactor( arrOut,sizeArr, f )				
			local bR,bG,bB, bA = util.unpackcolor4( color )
			
			local rMed = util.bytetofloat( bR )
			local gMed = util.bytetofloat( bG )
			local bMed = util.bytetofloat( bB )
			
			image.setchannelb(xCount,yCount,3,bA)
			image.setpixelf( xCount,yCount, rMed, gMed, bMed, 1.0)						
				
			yCount = yCount + 1
		end		
		yCount = 0
		xCount = xCount + 1		
	end		
	
	function save()
		local name = main.uniqid()
		--local name = "out"
		local PathOut = string.format("%s/%s.%s", pic_dir_filtered, name, "png" )
		image.save( "png", PathOut )
		util.msg("saved")
		image.eraseall()
	end


	window.setkey(0x1D, "save()") -- ML_LCONTROL    = 0x1D,	

	window.redraw()
	
	util.msg("***s_vertgradient finished")

end


-- GRADIENT and patterns
local outExt = "png"
local palleteName = "3.png"

local PathOut = string.format("%s/%s.%s", pic_dir_filtered, "gradient", outExt )
local arrayOutSize = 200
local w = 2000	
local h = 2000
local xBeg = w * 0.5
local yBeg = h * 0.5	
--local imgPath = "C:/foto/rez/445998174_20169511162756_6739red3.png"
--s_circlegradient( PathOut, w, h, xBeg, yBeg, palleteName, arrayOutSize , false)
s_vertgradient( PathOut, w, h, palleteName, arrayOutSize , false )
