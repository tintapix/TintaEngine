require "Scripts/lib/l_util"

--s_addPatternImgVert
--s_addImgRandom
--s_addPatternImg
--s_addPatternImgScale
--s_addPatternImgChannel
--s_addPatternImgInterpImg
--s_circlegradient
--s_vertgradient
--s_addPatternImgVertRand
--s_addPatternImgOther
--s_addPatternImgOtherGradient
--s_addRandomImgOther
--s_addImgOtherFromMarkers



--[[
	adds pattern in vertical order 
	interpolating to the palleteName
]]--
function s_addPatternImgVert( xSize, ySize, countAddIn,  imgDirPath, addImgEx, pathOut, between, maxRandOffset, palleteName, invertIn , newNext )
			
	if newNext == nil then
		newNext = false
	end
	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	local arrColor = s_getpallete(palleteName)
	
	local rezImg = image.create( xSize, ySize , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	
	image.fillb(255,255,255)
	image.fillchannelf(3,1)
	
	local stepX = between
	local stepY = between
	
	
	local posX = 0
	local posY = math.floor(between/2)
	local interpFactor = 1
	
	util.msg(interpFactor)
	local yCount = 0
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )
	
	local fileFullPathOld = nil
	while yCount < countAddIn do			
						
						
			local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )
			
			while newNext == true and fileFullPathOld == fileFullPath and filesMax > 1 do			
				fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )
				--util.msg(fileFullPath)
			end
			
			fileFullPathOld = fileFullPath
			
			local cRRand,cGRand,cBRand = util.unpackcolor3( s_getRandomcolor(arrColor) )		
			
			local picToAdd = 0
			
			picToAdd = s_interpToColor( fileFullPath, nil, cRRand,cGRand,cBRand, 1, false )		 
			
			
			image.select( picToAdd )
			local w = image.width()
			local h = image.height()
			
			stepY = h + between
			
			image.select( rezImg )						
			
			
			local offsetY = maxRandOffset
			if maxRandOffset > 0 then				
				offsetY = main.randint(0,maxRandOffset)
			end
			image.mix( picToAdd, posX, posY + offsetY, 1  )			
			
			
			image.erase( picToAdd )
						
			posY = posY + stepY		
			yCount = yCount + 1
	end		
		
		
	image.select( rezImg )
	image.save( exImageOut, pathOut )
	image.erase( rezImg )
	
	util.msg("***s_addPatternImgVert finished")
end



function s_addImgRandom( width, height, patternsPathIn, addImgEx, pathOutIn, quantity, palleteNameIn, imgId, randAlpha  )
				
	local exImageOut = s_getex(pathOutIn)	
		
	
	local picImg = nil
	if imgId == nil then
		picImg = image.create( width, height , "seamless" ) 			
		image.fillb(255,255,255)	
		image.fillchannelf(3,1)
	else
		picImg = imgId
		image.select( picImg )
	end
	
	local wRead = image.width()
	local hRead = image.height()	
			
	local filesMax  = util.countfiles( patternsPathIn, addImgEx )	
	
	util.msg(filesMax)
	local arrColor = s_getpallete( palleteNameIn )	
	
	for count = 0 , quantity do 				
		
		local fileFullPath = util.getfile( patternsPathIn, main.randintex( 0, filesMax - 1 ), addImgEx , false )		
		local cRRand,cGRand,cBRand = util.unpackcolor3( s_getRandomcolor(arrColor) )
		
		local picToAdd = s_interpToColor( fileFullPath,nil, cRRand,cGRand,cBRand, 1.0, false )
				
		image.select( picImg )			
		
		if randAlpha ~= nil and randAlpha == true then
			image.mix( picToAdd, main.randint(0,wRead), main.randint(0,hRead), main.randfloat()  )										
		else
			image.mix( picToAdd, main.randint(0,wRead), main.randint(0,hRead), 1.0  )										
		end
		
		image.erase( picToAdd )								
		
	end	
	
	image.select( picImg )
	image.save( exImageOut, pathOutIn )
	
	if imgId == nil then
		image.erase( picImg )
	end
	
	util.msg("***s_addImgRandom finished")
end






-------------------------------------------------------

--[[
	adds pattern in tile order takes color variant from imgToRead image pixels
	pattern image interpolates to the palleteName color depends on imgToRead channelIn value in current position
	pattern image IS NOT CONVERTING in to the buffer
	arrayOutSize - spread palletete array to this size
]]--
function s_addPatternImgChannel( xSize, ySize, imgChanelPathIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, palleteNameIn, arrayOutSize, channelIn, bInvert, factorInterpIn)
	
	local exImageOut = s_getex(pathOut)		
	
	local picImg = image.create( xSize, ySize , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	
	image.fillb(255,255,255)
	
	local picToGet = image.read( imgChanelPathIn, s_getex(imgChanelPathIn), "basic" )	
	
	image.select( picToGet )
	local wRead = image.width()
	local hRead = image.height()		
	
	local stepX = between
	local stepY = between	
	
	local posX = math.floor(between/2)
	local posY = math.floor(between/2)
	
	local xCount = 0
	local yCount = 0
	
	local filesMax  = util.countfiles( patternsPathIn, addImgEx )
	local factorInterp = 1
	if factorInterpIn ~= nil then
		factorInterp = factorInterpIn
	end
	local arrColor = s_getpallete(palleteNameIn)
	local arrOut = s_buildColorArray( arrColor, arrayOutSize )
	local sizeArr = s_arrlength(arrOut)
	while xCount < wRead do			
		while yCount < hRead do				
						
			image.select( picToGet )			
			local r,g,b,a = image.getpixelf( xCount, yCount) 			
						
			image.select( picToGet )	
			
			local fileFullPath = util.getfile( patternsPathIn, main.randintex( 0, filesMax - 1 ), addImgEx , false )
						
			local f = a
			if channelIn ==  0 then
				f = r
			elseif channelIn ==  1 then
				f = g
			elseif channelIn ==  2 then
				f = b				
			elseif channelIn ==  3 then
				f = a				
			elseif channelIn ==  4 then
				local h,s,l = util.c_rgbtohsl( r,g,b )	
				f = l
			end
						
			if bInvert == true then 
				f = 1 - f
			end
			local pos, color = s_getDiscreteColorByFactor( arrOut, sizeArr, f )
			
			local bR,bG,bB = util.unpackcolor3( color )
				
			local picToAdd = s_interpToColor( fileFullPath, nil, bR,bG,bB, factorInterp, false )
						
			stepX = between				
			stepY = between
			
			image.select( picImg )			
			--a = main.randint(0,1)
			if a > 0 then				
				local offsetX = maxRandOffset
				local offsetY = maxRandOffset
				if maxRandOffset > 0 then
					offsetX = main.randint(0,maxRandOffset)
					offsetY = main.randint(0,maxRandOffset)
				end
				image.select( picImg )
				image.mix( picToAdd, posX + offsetX, posY + offsetY, 1 )		
				image.select( picToAdd )				
			end
			
			image.erase( picToAdd )
						
			posY = posY + stepY		
			yCount = yCount + 1
		end		
		
		posY = math.floor(between/2)
		yCount = 0
		xCount = xCount + 1
		
		posX = posX + stepX		
	end		
	
	image.erase( picToGet )
	image.select( picImg )
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	util.msg("***s_addPatternImgChannel finished")
end











--[[
	Draws vert gradient with image offset
	imageOffsetPath - path to image with offset factor 
	offset - 0,1 offset value
	channel - channel to get a mix factor
]]--
function s_vertgradientOffset( outPathIn, imageOffsetPath, offset, channel, widthIn, heightIn, palleteNameIn, arrayOutSizeIn, bInvetIn )

	main.lassert( channel >  -1 and channel < 4 , "channel >   -1 and channel < 4" )	
		
	local exImageOut = s_getex(outPathIn)	
		
	local arrColor = s_getpallete(palleteNameIn)
	
	local arrOut = s_buildColorArray( arrColor, arrayOutSizeIn )	
	
	local picImg = image.create( widthIn, heightIn , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	
	local offsetImg =  image.read( imageOffsetPath, s_getex(imageOffsetPath) , "seamless")
	
	image.select( offsetImg )
	local wOffset = image.width()
	local hOffset = image.height()
	
	
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
			
			if bInvetIn == true then 
				f = 1 - f
			end
			
			local xOffset = xCount/w * wOffset
			local yOffset = yCount/h * hOffset
			image.select( offsetImg )
			local r,g,b,a = image.getpixelf( math.floor(xOffset), math.floor(yOffset) )	
			
			local chOffset = 0.5
			
			if channel == 0 then
				chOffset = r
			elseif channel == 1 then
				chOffset = g
			elseif channel == 2 then
				chOffset = b
			elseif channel == 3 then
				chOffset = a
			end
			--util.msg(chOffset)			
			image.select( picImg )
			
			f = f + util.cosinterp( -offset, offset, chOffset )
			f = math.min(math.max(f,0.0),1.0)
			
			--util.msg(f)
			local pos, color = s_getDiscreteColorByFactor( arrOut,sizeArr, f )
				
			local bR,bG,bB = util.unpackcolor3( color )
			
			local rMed = util.bytetofloat( bR )
			local gMed = util.bytetofloat( bG )
			local bMed = util.bytetofloat( bB )
			
			image.setchannelf(xCount,yCount,3,1.0)
			image.setpixelf( xCount,yCount, rMed, gMed, bMed, 1.0)						
				
			yCount = yCount + 1
		end		
		yCount = 0
		xCount = xCount + 1		
	end		
	
	image.save( exImageOut, outPathIn )
	image.erase( picImg )
	
	util.msg("***s_vertgradientOffset finished")

end

--[[
	Two pallets interpolates with image channel
]]--
function s_vertgradient2Pallates( outPathIn, imagePath, channel, widthIn, heightIn, palleteNameIn, palleteNameIn2, arrayOutSizeIn, bInvetIn )

	main.lassert( channel >  -1 and channel < 4 , "channel >   -1 and channel < 4" )	
		
	local exImageOut = s_getex(outPathIn)	
		
	local arrColor = s_getpallete(palleteNameIn)
	
	local arrColor2 = s_getpallete(palleteNameIn2)
	
	local arrOut = s_buildColorArray( arrColor, arrayOutSizeIn )

	local arrOut2 = s_buildColorArray( arrColor2, arrayOutSizeIn )	
	
	local picImg = image.create( widthIn, heightIn , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	
	local offsetImg =  image.read( imagePath, s_getex(imagePath) , "seamless")
	
	image.select( offsetImg )
	local wOffset = image.width()
	local hOffset = image.height()
	
	
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
			
			if bInvetIn == true then 
				f = 1 - f
			end
			
			local xOffset = xCount/w * wOffset
			local yOffset = yCount/h * hOffset
			image.select( offsetImg )
			local r,g,b,a = image.getpixelf( math.floor(xOffset), math.floor(yOffset) )	
			
			local factor = 0.5
			
			if channel == 0 then
				factor = r
			elseif channel == 1 then
				factor = g
			elseif channel == 2 then
				factor = b
			elseif channel == 3 then
				factor = a
			end
			--util.msg(chOffset)			
			image.select( picImg )			
			
			--util.msg(factor)
			local pos, color = s_getDiscreteColorByFactor( arrOut,sizeArr, f )
			
			local pos2, color2 = s_getDiscreteColorByFactor( arrOut2,sizeArr, f )
				
			local bR,bG,bB = util.unpackcolor3( color )		
			
			local bR2,bG2,bB2 = util.unpackcolor3( color2 )			
			
			image.setchannelf(xCount,yCount,3,1.0)
						
			image.setchannelb(xCount,yCount, 0, math.floor(util.cosinterp(bR,bR2,factor)) )
			image.setchannelb(xCount,yCount, 1, math.floor(util.cosinterp(bG,bG2,factor)) )
			image.setchannelb(xCount,yCount, 2, math.floor(util.cosinterp(bB,bB2,factor)) )
			
			yCount = yCount + 1
		end		
		yCount = 0
		xCount = xCount + 1		
	end		
	
	image.save( exImageOut, outPathIn )
	image.erase( picImg )
	
	util.msg("***s_vertgradient2Pallates finished")

end


--[[
	adds pattern in vertical order randomly on y axis, x position is fixed on 0
	interpolating to the palleteName
]]--
function s_addPatternImgVertRand( xSize, ySize, countAddIn,  imgDirPath, addImgEx, pathOut,   palleteName, interpFactorIn, channelIn, invertIn)
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex( pathOut )		
	local arrColor = s_getpallete( palleteName )	
	local rezImg = image.create( xSize, ySize , "basic" ) -- image.read( imgPathIn, exImage, imageType )	
	
	image.fillb(255,255,255)
	image.fillchannelf(3,1)
	
	local posX = 0
	
	local interpFactor = 1
	
	if interpFactorIn~= nil then
		interpFactor = interpFactorIn
	end
	util.msg(interpFactor)
	local count = 0
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )
	
	while count < countAddIn do			
						
						
			local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )
			local cRRand,cGRand,cBRand = util.unpackcolor3( s_getRandomcolor(arrColor) )		
						
			local picToAdd = 0
			--[[if channelIn ~= nil then					
				local cRRand2,cGRand2,cBRand2 = util.unpackcolor3( s_getRandomcolor( arrColor ) )
				picToAdd = s_interpToColorsByChannel( fileFullPath, nil, cRRand,cGRand,cBRand, cRRand2,cGRand2,cBRand2, channelIn ,  interpFactor )				
			else]]--
				picToAdd = s_interpToColor( fileFullPath,nil, cRRand,cGRand,cBRand, interpFactor, false )
			--end					 	
		
			image.select( rezImg )			
			image.mix( picToAdd, 0, main.randintex( 50, ySize-50 ), 1  )				
			image.erase( picToAdd )						
			
			count = count + 1
	end		
		
		
	image.select( rezImg )
	image.save( exImageOut, pathOut )
	image.erase( rezImg )
	
	util.msg("***s_addPatternImgVertRand finished")
end


--[[
	Adds images from the selected directory in pattern order: tile, brick without with the 
	original color of the pattern image
	widthIn  - width
	heightIn - height
	imageType - string parameter: "seamless" or "basic"
	imgDirPath  - path to the directory with images to add
	addImgEx 	- string parameter, file extension: "jpg", "jpeg", "png"
	pathOut 	- output path for the result image	
	countAdd    - quantity
	between		- space
	typeArrange - 0 - tile, 1 - brick
	
]]--
function s_addPatternImgOther( widthIn, heightIn, imageType, imgDirPath, addImgEx, pathOut, countAdd, between, typeArrange, maxAlphaIn, bRandAlphaIn )
			
	main.lassert( typeArrange >= 0 and typeArrange < 2 , " Wrong typeArrange value!" )
	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	
	--picImg = image.read( imgPathIn, exImage, imageType )	
	local picImg = image.create( widthIn, heightIn , imageType )

	image.fillb(255,255,255)
	--local w = image.width()
	--local h = image.height()	
	image.fillchannelf(3,1)
	
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
				util.msg(fileFullPath)
				local picAdd = image.read( fileFullPath, addImgEx )	
				local w = image.width()
				local h = image.height()
	
				stepX = w + between				
				stepY = h + between
				-- s_multalpha
				image.select( picImg )					
				if bRandAlphaIn == nil or bRandAlphaIn == false then
					image.mix( picImg, posX, posY, alphaFactor )
				else
					image.mix( picImg, posX, posY, math.min(main.randfloat() , alphaFactor) )
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
					local w = image.width()
					local h = image.height()
					offset = h/2
		
					stepX = w + between				
					stepY = h + between
					-- s_multalpha
					image.select( picImg )					
					if bRandAlphaIn == nil or bRandAlphaIn == false then
						image.mix( picAdd, posX, posY, alphaFactor )
					else
						image.mix( picAdd, posX, posY, main.randfloat() * alphaFactor )
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


--[[
	Adds images from the selected directory in pattern order: tile, brick with gradient scaling for comics
	pattern images converts in to the buffer
	imgPathIn - src image path
	imageType - string parameter: "seamless" or "basic"
	imgDirPath - path to the directory with images to add
	addImgEx - string parameter, file extension: "jpg", "jpeg", "png"
	pathOut - output path for the result image	
	typeArrange - 0 - tile, 1 - brick
	
]]--
function s_addPatternImgOtherGradient( imgPathIn, imageType, imgDirPath, addImgEx, pathOut, countAdd, between, typeArrange )
			
	main.lassert( typeArrange >= 0 and typeArrange < 2 , " Wrong typeArrange value!" )
	local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	picImg = image.read( imgPathIn, exImage, imageType )	

	

	local stepX = between
	local stepY = between
	
	if typeArrange == 0 then 
		local posX = math.floor(between/2)
		local posY = math.floor(between/2)
		
		local xCount = 0
		local yCount = 0
		
		local filesMax  = util.countfiles( imgDirPath, addImgEx )	
		while xCount < countAdd do	
			
			
			while yCount < countAdd do				
				
				local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )			
					--local picAdd = image.read( fileFullPath, addImgEx )					
					
				local srcBuff = s_MaskfromImageAlpha( fileFullPath  )
					
				local xMin, xMax, yMin, yMax = c_extremebitmap(srcBuff)
					
				local xSize = xMax - xMin
				local ySize = yMax - yMin
					
				c_scalebitmap(srcBuff, 1 - xCount/countAdd, 1 - xCount/countAdd )
				
				local picAdd = c_executefunc( "/effects/effect_fillcolor", "s_effect",srcBuff, 255,255,255, 0,0,0, 1, true )
				image.erasebitmap(srcBuff)
				
				-- s_multalpha
				image.select( picImg )					
				
				image.mix( picAdd, posX, posY )
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
			local posX = math.floor(between/2)
			local posY = 0 --math.floor(between/2)
		
			local xCount = 0
			local yCount = 0
			local odd = false
			local filesMax  = util.countfiles( imgDirPath, addImgEx )	
			while xCount < countAdd do				
				while yCount < countAdd do				
					
					local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )			
					--local picAdd = image.read( fileFullPath, addImgEx )					
					
					local srcBuff = s_MaskfromImageAlpha( fileFullPath  )
					
					local xMin, xMax, yMin, yMax = c_extremebitmap(srcBuff)
					
					local xSize = xMax - xMin
					local ySize = yMax - yMin
					
					c_scalebitmap(srcBuff, 1 - xCount/countAdd, 1 - xCount/countAdd )
					
					local picAdd = c_executefunc( "/effects/effect_fillcolor", "s_effect",srcBuff, 255,255,255, 0,0,0, 1, true )
					image.erasebitmap(srcBuff)	

					-- s_multalpha
					image.select( picImg )					
					
					image.mix( picAdd, posX, posY )
					image.erase( picAdd )
					
					posY = posY + stepY		
					yCount = yCount + 1
				end		
				
			posY = math.floor(between/2)
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
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	util.msg("***s_addPatternImgOtherGradient finished")
end



--[[
	Adds images from the selected directory in random positions 
	imgPathIn - src image path
	imageType - string parameter: "seamless" or "basic"
	imgDirPath - path to the directory with images to add
	addImgEx - string parameter, file extension: "jpg", "jpeg", "png"
	pathOut - output path for the result image	
	countAdd - how much images to add, if nil or 0 adds one time every image in the folder
]]--
function s_addRandomImgOther( imgPathIn, imageType, imgDirPath, addImgEx, pathOut, countAdd , randAlphaIn)
			
	local exImage = s_getex(imgPathIn)
	
	local exImageOut = s_getex(pathOut)	
	
	local picImg = image.read( imgPathIn, exImage, "seamless" )	
	
	local w = image.width()

	local h = image.height()	
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )
	
	
	if countAdd > 0 then
		for i = 0, countAdd - 1 do 	
				local fileFullPath = util.getfile( imgDirPath, main.randintex( 0, filesMax - 1 ), addImgEx , false )			
				local picAdd = image.read( fileFullPath, addImgEx )
						
				-- s_multalpha
				image.select( picImg )
				if randAlphaIn == true then
					image.mix( picAdd, main.randint(0, w), main.randint(0, h) , main.randfloat() )
				else
					image.mix( picAdd, main.randint(0, w), main.randint(0, h) )
				end
				image.erase( picAdd )
		end		
	else
		
		
		for f = 0, filesMax - 1 do
			local fileFullPath = util.getfile( imgDirPath, f , false )			
			local picAdd = image.read( fileFullPath, addImgEx )
					
			-- s_multalpha
			image.select( picImg )
			if randAlphaIn == true then
				image.mix( picAdd, main.randint(0, w), main.randint(0, h) , main.randfloat() )
			else
				image.mix( picAdd, main.randint(0, w), main.randint(0, h) )
			end
			image.erase( picAdd )
		end
		
	end
	
	--s_drawlogo( picImg )
	
	
	image.select( picImg )
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	
	util.msg("***s_addRandomImgOther finished")
end





