require "Scripts/lib/l_util"
require "paths"

--[[
	Adds images from the selected directory in pattern order: tile, brick without with the 
	original color of the pattern image	
	imageType - string parameter: "seamless" or "basic"
	imgDirPath  - path to the directory with images to add
	addImgEx 	- string parameter, file extension: "jpg", "jpeg", "png"
	pathOut 	- output path for the result image	
	countAdd    - quantity
	between		- space
	typeArrange - 0 - tile, 1 - brick
	
]]--
function s_addPatternImgOther( imageType, imgDirPath, addImgEx, pathOut, countAdd, between, typeArrange, maxAlphaIn, bRandAlphaIn, randXOffset, randYOffset )
	

	
	main.lassert( typeArrange >= 0 and typeArrange < 2 , " Wrong typeArrange value!" )
	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)		
	
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )	
		
		
	local widthMax = 0
	local heightMax = 0	
	
	for i = 0, filesMax - 1 do
	
			local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )			
			
			local picAdd = image.read( fileFullPath, addImgEx )	
			--util.msg( image.width(), " ", image.height() )			
			widthMax = math.max(widthMax, image.width())
			heightMax = math.max(heightMax, image.height())
			
			image.erase(picAdd)
				
	end	
	

	
	--picImg = image.read( imgPathIn, exImage, imageType )	
	local picImg = image.create( widthMax * countAdd +  countAdd * between, heightMax * countAdd  + countAdd * between , imageType )

	image.fillb(255,255,255)
	
	image.fillchannelf(3,0)
	
	local stepX = between
	local stepY = between
	local alphaFactor = 1
	if maxAlphaIn ~= nil then
		alphaFactor = maxAlphaIn
	end
	
	if typeArrange == 0 then 
		local posX = math.floor(between/2)
		local posY = math.floor(between/2)
		
		local xCount = 0
		local yCount = 0
		
		local filesMax  = util.countfiles( imgDirPath, addImgEx )	
		
		while xCount < countAdd do	
			
			
			while yCount < countAdd do				
				
				local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )			
				--util.msg(fileFullPath)
				local picAdd = image.read( fileFullPath, addImgEx )	
				local w = widthMax
				local h = heightMax
	
				stepX = w + between				
				stepY = h + between
				-- s_multalpha
				image.select( picImg )					
				
				if bRandAlphaIn == nil or bRandAlphaIn == false then
					image.mix( picAdd, posX + math.random( -randXOffset, randXOffset ), posY + math.random( -randYOffset, randYOffset ), alphaFactor )
				else
					image.mix( picAdd, posX + math.random( -randXOffset, randXOffset ), posY + math.random( -randYOffset, randYOffset ), math.min(main.randfloat() , alphaFactor) )
				end
				image.erase( picAdd )
				
				posY = posY + stepY		
				yCount = yCount + 1
			end		
			
			posY = math.floor(between/2)
			yCount = 0
			xCount = xCount + 1
			posX = posX + stepX		
		end		
	elseif typeArrange == 1 then
			local posX = 0 --math.floor(between/2)
			local posY = 0 --math.floor(between/2)
		
			local xCount = 0
			local yCount = 0
			local odd = false
			local offset = 0
			local filesMax  = util.countfiles( imgDirPath, addImgEx )	
			while xCount < countAdd do				
			
				while yCount < countAdd do				
					
					local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )			
					local picAdd = image.read( fileFullPath, addImgEx )	
					local w = widthMax
					local h = heightMax
					offset = h/2
		
					stepX = w + between				
					stepY = h + between
					-- s_multalpha
					image.select( picImg )					
					if bRandAlphaIn == nil or bRandAlphaIn == false then
						image.mix( picAdd, posX + math.random( -randXOffset, randXOffset ), posY + math.random( -randYOffset, randYOffset ), alphaFactor )
					else
						image.mix( picAdd, posX + math.random( -randXOffset, randXOffset ), posY + math.random( -randYOffset, randYOffset ), main.randfloat() * alphaFactor )
					end
					image.erase( picAdd )
					
					posY = posY + stepY		
					yCount = yCount + 1
				end		
				
			posY = offset + math.floor(between/2)
			if odd == true then
				odd = false
				posY = 0
			else
				odd = true
			end
			
			
			yCount = 0
			xCount = xCount + 1
			posX = posX + stepX		
		end		

	
	end
	
	image.select( picImg )
	image.save( "png", pathOut )
	image.erase( picImg )
	
	util.msg("***s_addPatternImgOther finished")
	
	
end
local count = 200
local between = -4
local patterType = 0 -- 0 - tile 1 - btick
local xRand = 1
local yRand = 1
-- s_addPatternImgOther( imageType, imgDirPath, addImgEx, pathOut, countAdd, between, typeArrange, maxAlphaIn, bRandAlphaIn )
s_addPatternImgOther( "seamless", "c:/foto/pattern/dots", "png", "c:/foto/rez/out.png", count, between, patterType, 1.0, false, xRand, yRand )