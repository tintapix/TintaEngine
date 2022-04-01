require "Scripts/lib/l_util"


--[[
	Adds images from the selected directory from red markers image
	imgPathIn - src image path	
	imgDirPath - path to the directory with images to add
	addImgEx - string parameter, file extension: "jpg", "jpeg", "png"
	pathOut - output path for the result image
	markersPath - red markers	image path
]]--
function s_addImgOtherFromMarkers( w,h, imgDirPath, addImgEx, pathOut, markersPath, count , randAlphaIn)


	
	local picImg = image.create( w,h, "seamless" )	
	util.msg(picImg)
	image.fillb(255,255,255)
	image.fillchannelf(3,1)
	
	local w = image.width()

	local h = image.height()	
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )
	
	local boxId = s_boxFromImgRed( markersPath, w, h )
	local q = box.elements(boxId)
	
	local arrIdToAdd = {}

	for i = 0, filesMax -1 do
		local fileFullPath = util.getfile( imgDirPath, i, addImgEx , false )
		local picToAdd = image.read( fileFullPath, addImgEx )	
		table.insert(arrIdToAdd, picToAdd)	
	end
	
	local arrLen = s_arrlength(arrIdToAdd)
	main.lassert( arrLen > 0 )
	
	for i = 0, count - 1  do	
			--local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )			
			local picAdd = arrIdToAdd[ main.randintex( 1, arrLen) ]
					
			-- s_multalpha
			image.select( picImg )
			local randX = main.randintex( 0, q/2)
			
			if randAlphaIn == true then
				image.mix( picAdd, box.getvali32( boxId, randX * 2 ),box.getvali32(boxId,randX * 2  + 1) , main.randfloat() )
			else
				image.mix( picAdd, box.getvali32( boxId, randX * 2 ),box.getvali32(boxId,randX * 2  + 1) )
			end
			--image.erase( picAdd )
	end		
	
	for k,v in spairs( arrIdToAdd ) do
		image.erase(v)
	end
	
	box.erase( boxId )
	
	image.select( picImg )
	image.save( s_getex( pathOut ), pathOut )
	
	image.erase( picImg )	
	
	util.msg("***s_addImgOtherFromMarkers finished")
end


--[[
	adds pattern from the masked shape image in tile order.size depends of between parameter	
	colored with pallete colors in random order
	pattern image IS NOT CONVERTING in to the buffer
	imgMaskIn - mask image (with alpha 0,1) xCount depends of width yCount depends of height 
	palleteName - pallete name
]]--
function s_addPatternImgPallete( xSize, ySize, imgMaskIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, randAlphaIn, typeArrange )
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex( pathOut )		
	local arrColor = nil
	if  s_isstring( palleteName ) then
		arrColor = s_getpallete( palleteName )	
	end
	local picImg = image.create( xSize, ySize , "basic" ) 
	
	image.fillb(255,255,255)
	image.fillchannelf(3,1)
	
	-- template 
	local picToGet = 0
	if s_isstring( imgMaskIn ) == true  then		
		picToGet = image.read( imgMaskIn, s_getex( imgMaskIn ), "basic" )	 
	else
		picToGet = imgMaskIn	 
	end
	
	image.select( picToGet )
	local wRead = image.width()
	local hRead = image.height()

	local stepX = between
	local stepY = between
	
	
	local posX = math.floor(between/2)
	local posY = math.floor(between/2)
	
	local xCount = 0
	local yCount = 0
	local offsets = { -maxRandOffset,0, 0,0, maxRandOffset,0 }
	local filesMax  = util.countfiles( patternsPathIn, addImgEx )	
	
	if typeArrange == nil or typeArrange == 0 then
		
		while yCount < hRead do			
			while xCount < wRead do				
							
				image.select( picToGet )			
				local r,g,b,a = image.getpixelb( xCount, yCount) 			
				
				local fileFullPath = util.getfile( patternsPathIn, main.randintex( 0, filesMax - 1 ), addImgEx , false )
				if arrColor ~= nil then
					r,g,b = util.unpackcolor3( s_getRandomcolor(arrColor) )				
				end
							
				local picToAdd = s_interpToColor( fileFullPath, nil, r,g,b, 1.0, false )
				
				image.select( picToAdd )
								
				stepX = between				
				stepY = between
				
				image.select( picImg )			
				
				if a > 0 then			
					local offsetX = 0				
					local offsetY = 0
					
					if maxRandOffset > 0 then		
						local pos = main.randint(1,5)				
						offsetX = offsetX + offsets[pos]					
						offsetY = 0
					end				
					
					if randAlphaIn == true then
						image.mix( picToAdd,  posX + offsetX, posY + offsetY, util.randfloat() )			
					else
						image.mix( picToAdd,  posX + offsetX, posY + offsetY, util.bytetofloat( a )  )								
					end
				end
				
				image.erase( picToAdd )
							
				posX = posX + stepX		
				xCount = xCount + 1
			end		
			
			posX = math.floor(between/2)
			xCount = 0
			yCount = yCount + 1			
			posY = posY + stepY		
		end	
	else -- btick
		local odd = false
				
		while yCount < hRead do			
			while xCount < wRead do				
							
				image.select( picToGet )			
				local r,g,b,a = image.getpixelb( xCount, yCount) 			
				
				local fileFullPath = util.getfile( patternsPathIn, main.randintex( 0, filesMax - 1 ), addImgEx , false )
				if arrColor ~= nil then
					r,g,b = util.unpackcolor3( s_getRandomcolor(arrColor) )				
				end
							
				local picToAdd = s_interpToColor( fileFullPath, nil, r,g,b, 1.0, false )
				
				image.select( picToAdd )					
				local wMask = image.width()				
				
				stepX = between				
				stepY = between
				
				image.select( picImg )			
				
				if a > 0 then			
				
					local offsetX = 0				
					local offsetY = 0
					
					if maxRandOffset > 0 then		
						local pos = main.randint(1,5)				
						offsetX = offsetX + offsets[pos]					
						offsetY = 0
					end			
					
					if randAlphaIn == true then
						image.mix(  picToAdd, posX + offsetX, posY + offsetY, util.randfloat() )			
					else
						image.mix(  picToAdd, posX + offsetX, posY + offsetY, util.bytetofloat( a )  )								
					end
				end
				
				image.erase( picToAdd )
							
				posX = posX + stepX		
				
				xCount = xCount + 1
			end	-- while xCount < wRead
			
			odd = odd ~= true
			if odd == false then
				posX = math.floor(between/2)
			else
				posX = between 
			end
			
			
			
			xCount = 0
			yCount = yCount + 1
			
			posY = posY + stepY		
		end		
		
	end
	
	if s_isstring(imgMaskIn) == true  then	
		image.erase( picToGet )
	end
	image.select( picImg )
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	util.msg("***s_addPatternImgPallete finished")
end


--[[
	adds pattern from the masked shape image in tile order.size depends of between parameter	
	colored with pallete colors in random order
	pattern image IS NOT CONVERTING in to the buffer
	imgMaskIn - mask image (with alpha 0,1) xCount depends of width yCount depends of height 
	palleteName - pallete name
]]--
function s_addPatternImg( xSize, ySize, imgMaskIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset,  randAlphaIn, typeArrange )
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex( pathOut )		
	
	local picImg = image.create( xSize, ySize , "basic" ) 
	
	image.fillb(255,255,255)
	image.fillchannelf(3,1)
	
	-- template 
	local picToGet = 0
	if s_isstring( imgMaskIn ) == true  then		
		picToGet = image.read( imgMaskIn, s_getex( imgMaskIn ), "basic" )	 
	else
		picToGet = imgMaskIn	 
	end
	
	image.select( picToGet )
	local wRead = image.width()
	local hRead = image.height()

	local stepX = between
	local stepY = between
	
	
	local posX = math.floor(between/2)
	local posY = math.floor(between/2)
	
	local xCount = 0
	local yCount = 0
	local offsets = { -maxRandOffset,0, 0,0, maxRandOffset,0 }
	local filesMax  = util.countfiles( patternsPathIn, addImgEx )	
	local arrIdToAdd = {}

	for i = 0, filesMax -1 do
		local fileFullPath = util.getfile( patternsPathIn, i, addImgEx , false )
		local picToAdd = image.read( fileFullPath, addImgEx )	
		table.insert(arrIdToAdd, picToAdd)	
	end
	
	local arrLen = s_arrlength(arrIdToAdd)
	main.lassert( arrLen > 0 )
	
	if typeArrange == nil or typeArrange == 0 then
		
		while yCount < hRead do			
			while xCount < wRead do				
							
				image.select( picToGet )			
				local r,g,b,a = image.getpixelb( xCount, yCount) 			
				
				
								
							
				local picToAdd = arrIdToAdd[ main.randintex( 1, arrLen) ]
				
				image.select( picToAdd )
								
				stepX = between				
				stepY = between
				
				image.select( picImg )			
				
				if a > 0 then			
					local offsetX = 0				
					local offsetY = 0
					
					if maxRandOffset > 0 then		
						local pos = main.randint(1,5)				
						offsetX = offsetX + offsets[pos]					
						offsetY = 0
					end				
					
					if randAlphaIn == true then
						image.mix( picToAdd,  posX + offsetX, posY + offsetY, util.randfloat() )			
					else
						image.mix( picToAdd,  posX + offsetX, posY + offsetY, util.bytetofloat( a )  )								
					end
				end
				
				--image.erase( picToAdd )
							
				posX = posX + stepX		
				xCount = xCount + 1
			end		
			
			posX = math.floor(between/2)
			xCount = 0
			yCount = yCount + 1			
			posY = posY + stepY		
		end	
	else -- btick
		local odd = false
				
		while yCount < hRead do			
			while xCount < wRead do				
							
				image.select( picToGet )			
				local r,g,b,a = image.getpixelb( xCount, yCount) 			
				
				
							
				local picToAdd = image.read( fileFullPath, addImgEx )	
				
				image.select( picToAdd )
				
				local wMask = image.width()				
				
				stepX = between				
				stepY = between
				
				image.select( picImg )			
				
				if a > 0 then			
				
					local offsetX = 0				
					local offsetY = 0
					
					if maxRandOffset > 0 then		
						local pos = main.randint(1,5)				
						offsetX = offsetX + offsets[pos]					
						offsetY = 0
					end			
					
					if randAlphaIn == true then
						image.mix(  picToAdd, posX + offsetX, posY + offsetY, util.randfloat() )			
					else
						image.mix(  picToAdd, posX + offsetX, posY + offsetY, util.bytetofloat( a )  )								
					end
				end
				
				image.erase( picToAdd )
							
				posX = posX + stepX		
				
				xCount = xCount + 1
			end	-- while xCount < wRead
			
			odd = odd ~= true
			if odd == false then
				posX = math.floor(between/2)
			else
				posX = between 
			end
			
			
			
			xCount = 0
			yCount = yCount + 1
			
			posY = posY + stepY		
		end		
		
	end
	
	if s_isstring(imgMaskIn) == true  then	
		image.erase( picToGet )
	end
	
	for k,v in spairs( arrIdToAdd ) do
		image.erase(v)
	end
	
	image.select( picImg )
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	util.msg("***s_addPatternImg finished")
end




local PathRadPixels = "C:/foto/bat.png"

local pallete = "1.png"
randomMoveStep = 5
local Dir = "C:/foto/pattern/1"	
between = 100
maxRandOffset = 10
local outExt = "png"
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, "gradient", outExt )

-- function s_addPatternImg( xSize, ySize, imgMaskIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, palleteName, randAlphaIn, typeArrange )
--s_addPatternImgPallete( 2000,2000, PathRadPixels, Dir, "png", PathOut, 4, 0 , pallete, false , 0 )
s_addPatternImg( 2000,2000, PathRadPixels, Dir, "png", PathOut, 4, 0, false , 0 )

local PathMarkers = "C:/foto/bat.png"
local imgDirPath = "C:/foto/pattern/bc"

local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), "png" )
local count = 500
--s_addImgOtherFromMarkers( 4000,4000, imgDirPath, "png", PathOut, PathMarkers,count,  false)