require "Scripts/lib/l_util"




--[[
	adds pattern in vertical order 
	interpolating to the palleteName
]]--
function s_addPatternImgVert( xSize, ySize, countAddIn,  imgDirPath, addImgEx, pathOut, between, maxRandOffset, palleteName, invertIn)
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	local arrColor = s_getpallete(palleteName)
	
	local rezImg = c_createimg( xSize, ySize , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	
	c_fillimgb(255,255,255)
	c_fillalphaf(1)
	
	local stepX = between
	local stepY = between
	
	
	local posX = 0
	local posY = math.floor(between/2)
	local interpFactor = 1
	
	util.msg(interpFactor)
	local yCount = 0
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )
	
	while yCount < countAddIn do			
						
						
			local fileFullPath = util.getfile( imgDirPath, c_randintex( 0, filesMax - 1 ), addImgEx , false )
			local cRRand,cGRand,cBRand = util.unpackcolor( s_getRandomcolor(arrColor) )		
			
			local picToAdd = 0
			
			picToAdd = s_interpToColor( fileFullPath, nil, cRRand,cGRand,cBRand, 1, false )		 
			
			
			c_selimg( picToAdd )
			local w = c_getwidth()
			local h = c_getheight()
			
			stepY = h + between
			
			c_selimg( rezImg )						
			
			
			local offsetY = maxRandOffset
			if maxRandOffset > 0 then				
				offsetY = c_randint(0,maxRandOffset)
			end
			c_injectimg( picToAdd, rezImg, posX, posY + offsetY, 1  )			
			
			
			c_delimg( picToAdd )
						
			posY = posY + stepY		
			yCount = yCount + 1
	end		
		
		
	c_selimg( rezImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( rezImg )
	
	util.msg("***s_addPatternImgVert finished")
end



function s_addImgRandom( width, height, patternsPathIn, addImgEx, pathOutIn, quantity, palleteNameIn, imgId, randAlpha  )
				
	local exImageOut = s_getex(pathOutIn)	
		
	
	local picImg = nil
	if imgId == nil then
		picImg = c_createimg( width, height , "seamless" ) 			
		c_fillimgb(255,255,255)	
		c_fillalphaf( 1 )
	else
		picImg = imgId
		c_selimg( picImg )
	end
	
	local wRead = c_getwidth()
	local hRead = c_getheight()	
			
	local filesMax  = util.countfiles( patternsPathIn, addImgEx )	
	
	util.msg(filesMax)
	local arrColor = s_getpallete( palleteNameIn )	
	
	for count = 0 , quantity do 				
		
		local fileFullPath = util.getfile( patternsPathIn, c_randintex( 0, filesMax - 1 ), addImgEx , false )		
		local cRRand,cGRand,cBRand = util.unpackcolor( s_getRandomcolor(arrColor) )
		
		local picToAdd = s_interpToColor( fileFullPath,nil, cRRand,cGRand,cBRand, 1.0, false )
				
		c_selimg( picImg )			
		
		if randAlpha ~= nil and randAlpha == true then
			c_injectimg( picToAdd, picImg, c_randint(0,wRead), c_randint(0,hRead), c_randfloat()  )										
		else
			c_injectimg( picToAdd, picImg, c_randint(0,wRead), c_randint(0,hRead), 1.0  )										
		end
		
		c_delimg( picToAdd )								
		
	end	
	
	c_selimg( picImg )
	c_saveimg( exImageOut, pathOutIn )
	
	if imgId == nil then
		c_delimg( picImg )
	end
	
	util.msg("***s_addImgRandom finished")
end




--[[
	adds pattern from the masked shape image in tile order.size depends of between parameter	
	colored with pallete colors in random order
	pattern image IS NOT CONVERTING in to the buffer
	imgMaskIn - mask image (with alpha 0,1) xCount depends of width yCount depends of height 
	palleteName - pallete name
]]--
function s_addPatternImg( xSize, ySize, imgMaskIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, palleteName, randAlphaIn, typeArrange )
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex( pathOut )		
	local arrColor = nil
	if  s_isstring( palleteName ) then
		arrColor = s_getpallete( palleteName )	
	end
	local picImg = c_createimg( xSize, ySize , "basic" ) 
	
	c_fillimgb(255,255,255)
	
	-- template 
	local picToGet = 0
	if s_isstring( imgMaskIn ) == true  then		
		picToGet = c_readimg( imgMaskIn, s_getex( imgMaskIn ), "basic" )	 
	else
		picToGet = imgMaskIn	 
	end
	
	c_selimg( picToGet )
	local wRead = c_getwidth()
	local hRead = c_getheight()

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
							
				c_selimg( picToGet )			
				local r,g,b,a = c_getpixelb( xCount, yCount) 			
				
				local fileFullPath = util.getfile( patternsPathIn, c_randintex( 0, filesMax - 1 ), addImgEx , false )
				if arrColor ~= nil then
					r,g,b = util.unpackcolor( s_getRandomcolor(arrColor) )				
				end
							
				local picToAdd = s_interpToColor( fileFullPath, nil, r,g,b, 1.0, false )
				
				c_selimg( picToAdd )
								
				stepX = between				
				stepY = between
				
				c_selimg( picImg )			
				
				if a > 0 then			
					local offsetX = 0				
					local offsetY = 0
					
					if maxRandOffset > 0 then		
						local pos = c_randint(1,5)				
						offsetX = offsetX + offsets[pos]					
						offsetY = 0
					end				
					
					if randAlphaIn == true then
						c_injectimg( picToAdd, picImg, posX + offsetX, posY + offsetY, util.randfloat() )			
					else
						c_injectimg( picToAdd, picImg, posX + offsetX, posY + offsetY, util.bytetofloat( a )  )								
					end
				end
				
				c_delimg( picToAdd )
							
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
							
				c_selimg( picToGet )			
				local r,g,b,a = c_getpixelb( xCount, yCount) 			
				
				local fileFullPath = util.getfile( patternsPathIn, c_randintex( 0, filesMax - 1 ), addImgEx , false )
				if arrColor ~= nil then
					r,g,b = util.unpackcolor( s_getRandomcolor(arrColor) )				
				end
							
				local picToAdd = s_interpToColor( fileFullPath, nil, r,g,b, 1.0, false )
				
				c_selimg( picToAdd )					
				local wMask = c_getwidth()				
				
				stepX = between				
				stepY = between
				
				c_selimg( picImg )			
				
				if a > 0 then			
				
					local offsetX = 0				
					local offsetY = 0
					
					if maxRandOffset > 0 then		
						local pos = c_randint(1,5)				
						offsetX = offsetX + offsets[pos]					
						offsetY = 0
					end			
					
					if randAlphaIn == true then
						c_injectimg( picToAdd, picImg, posX + offsetX, posY + offsetY, util.randfloat() )			
					else
						c_injectimg( picToAdd, picImg, posX + offsetX, posY + offsetY, util.bytetofloat( a )  )								
					end
				end
				
				c_delimg( picToAdd )
							
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
		c_delimg( picToGet )
	end
	c_selimg( picImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	util.msg("***s_addPatternImg finished")
end

-------------------------------------------------------

--[[
	adds pattern from the masked shape image in tile order.size depends of between parameter
	with scaling pattern image(converting in buffer by alpha value) depends on channelIn value
	pattern image IS CONVERTING in to the buffer
	channelIn = 0 r
	channelIn = 1 g
	channelIn = 2 b
	channelIn = 3 alpha	
	channelIn = 4 lightness	
	colored with pallete colors in random order
	imgMaskIn - mask image (with alpha 0,1)
	palleteName - pallete name
]]--
function s_addPatternImgScale( xSize, ySize, imgMaskIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, palleteName, channelIn, minFactorIn )
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	local arrColor = s_getpallete(palleteName)
	
	local picImg = c_createimg( xSize, ySize , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	
	c_fillimgb(255,255,255)
	
	-- template 
	local picToGet = c_readimg( imgMaskIn, s_getex(imgMaskIn), "basic" )	 
	c_selimg( picToGet )
	local wRead = c_getwidth()
	local hRead = c_getheight()

	local stepX = between
	local stepY = between
	
	
	local posX = math.floor(between/2)
	local posY = math.floor(between/2)
	
	local xCount = 0
	local yCount = 0
	
	local filesMax  = util.countfiles( patternsPathIn, addImgEx )
	
	if minFactorIn == nil then
		minFactorIn = 0
	end
	
	while xCount < wRead do			
		while yCount < hRead do				
						
			c_selimg( picToGet )			
			local r,g,b,a = c_getpixelf( xCount, yCount) 			
			
			if a > 0 then
				local fileFullPath = util.getfile( patternsPathIn, c_randintex( 0, filesMax - 1 ), addImgEx , false )
				local cRRand,cGRand,cBRand = util.unpackcolor( s_getRandomcolor(arrColor) )
								
				stepX = between				
				stepY = between						
				
				local srcBuff = s_MaskfromImageAlpha( fileFullPath  )
				
				local f = a -- default
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
					
				c_scalebitmap(srcBuff, math.max(f, minFactorIn ), math.max(f, minFactorIn ) )
				
						
				local picToAdd = c_executefunc( "/effects/effect_fillcolor", "s_effect",srcBuff, 255,255,255, cRRand,cGRand,cBRand, 1, true )
				local offsetX = maxRandOffset
				local offsetY = maxRandOffset
				if maxRandOffset > 0 then
					offsetX = c_randint(0,maxRandOffset)
					offsetY = c_randint(0,maxRandOffset)
				end
					
				c_injectimg( picToAdd, picImg, posX + offsetX, posY + offsetY, 1 )			
				
				c_delbitmap(srcBuff)
				
				c_delimg( picToAdd )
			end		
			posY = posY + stepY		
			yCount = yCount + 1
		end		
		
		posY = math.floor(between/2)
		yCount = 0
		xCount = xCount + 1
		
		posX = posX + stepX		
	end		
	
	
	c_delimg( picToGet )
	c_selimg( picImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	util.msg("***s_addPatternImg finished")
end

--[[
	adds pattern in tile order takes color variant from imgToRead image pixels
	pattern image interpolates to the palleteName color depends on imgToRead channelIn value in current position
	pattern image IS NOT CONVERTING in to the buffer
	arrayOutSize - spread palletete array to this size
]]--
function s_addPatternImgChannel( xSize, ySize, imgChanelPathIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, palleteNameIn, arrayOutSize, channelIn, bInvert, factorInterpIn)
	
	local exImageOut = s_getex(pathOut)		
	
	local picImg = c_createimg( xSize, ySize , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	
	c_fillimgb(255,255,255)
	
	local picToGet = c_readimg( imgChanelPathIn, s_getex(imgChanelPathIn), "basic" )	
	
	c_selimg( picToGet )
	local wRead = c_getwidth()
	local hRead = c_getheight()		
	
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
						
			c_selimg( picToGet )			
			local r,g,b,a = c_getpixelf( xCount, yCount) 			
						
			c_selimg( picToGet )	
			
			local fileFullPath = util.getfile( patternsPathIn, c_randintex( 0, filesMax - 1 ), addImgEx , false )
						
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
			
			local bR,bG,bB = util.unpackcolor( color )
				
			local picToAdd = s_interpToColor( fileFullPath, nil, bR,bG,bB, factorInterp, false )
						
			stepX = between				
			stepY = between
			
			c_selimg( picImg )			
			--a = c_randint(0,1)
			if a > 0 then				
				local offsetX = maxRandOffset
				local offsetY = maxRandOffset
				if maxRandOffset > 0 then
					offsetX = c_randint(0,maxRandOffset)
					offsetY = c_randint(0,maxRandOffset)
				end
				c_injectimg( picToAdd, picImg, posX + offsetX, posY + offsetY, 1 )			
			end
			
			c_delimg( picToAdd )
						
			posY = posY + stepY		
			yCount = yCount + 1
		end		
		
		posY = math.floor(between/2)
		yCount = 0
		xCount = xCount + 1
		
		posX = posX + stepX		
	end		
	
	c_delimg( picToGet )
	c_selimg( picImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	util.msg("***s_addPatternImgChannel finished")
end


--[[
	adds pattern in tile order using mask image
	pattern image interpolates to the imagePathToInterpIn in random position
	pallete is not uses
	pattern image IS NOT CONVERTING in to the buffer
	maskImgPathIn - mask image		
]]--
function s_addPatternImgInterpImg( xSize, ySize, maskImgPathIn, patternsPathIn, addImgEx, pathOut, between, maxRandOffset, imagePathToInterpIn, randOffset )
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	
	local picImg = c_createimg( xSize, ySize , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	
	c_fillimgb(255,255,255)
	
	local picToGet = c_readimg( maskImgPathIn, s_getex(maskImgPathIn), "basic" )	 
	c_selimg( picToGet )
	local wRead = c_getwidth()
	local hRead = c_getheight()

	local stepX = between
	local stepY = between	
	
	local posX = math.floor(between/2)
	local posY = math.floor(between/2)
	
	local xCount = 0
	local yCount = 0
	
	local filesMax  = util.countfiles( patternsPathIn, addImgEx )	
	local interpId  = c_readimg( imagePathToInterpIn, s_getex(imagePathToInterpIn), "basic" )
	
	while xCount < wRead do			
		while yCount < hRead do				
						
			c_selimg( picToGet )			
			local r,g,b,a = c_getpixelb( xCount, yCount) 			
			
			local fileFullPath = util.getfile( patternsPathIn, c_randintex( 0, filesMax - 1 ), addImgEx , false )
						
			local picToAdd = s_interpToImageId( fileFullPath, "", interpId, 1, true )			
						
			local w = c_getwidth()
			local h = c_getheight()

			stepX = w + between				
			stepY = h + between
			
			c_selimg( picImg )			
			
			if a > 0 then				
				local offsetX = 0
				local offsetY = 0
				if randOffset == true then
					offsetX = c_randint(0,maxRandOffset)
					offsetY = c_randint(0,maxRandOffset)
				end
					c_injectimg( picToAdd, picImg, posX + offsetX, posY + offsetY, util.bytetofloat( a )  )											
			end
			
			c_delimg( picToAdd )
						
			posY = posY + stepY		
			yCount = yCount + 1
		end		
		
		posY = math.floor(between/2)
		yCount = 0
		xCount = xCount + 1
		
		posX = posX + stepX		
	end		
	
	c_delimg( interpId )	
	c_delimg( picToGet )
	c_selimg( picImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	util.msg("***s_addPatternImgInterpImg finished")
end







--[[
	Draws circle gradient
]]--
function s_circgradient( outPathIn, widthIn, heightIn, xPosIn, yPssIn, palleteNameIn, arrayOutSizeIn, bInvetIn )

	local exImageOut = s_getex(outPathIn)	
	
	local arrColor = s_getpallete(palleteNameIn)
	
	local arrOut = s_buildColorArray( arrColor, arrayOutSizeIn )	
	
	local picImg = c_createimg( widthIn, heightIn , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	c_selimg( picImg )
	local w = c_getwidth()
	local h = c_getheight()
	c_fillimgb(255,255,255)
	
	local xCount = 0
	local yCount = 0
		
	local lenMax = s_get_length( 0, h - 1, xPosIn, yPssIn )
	local sizeArr = s_arrlength(arrOut)
	while xCount < w do			
		while yCount < h do				
						
			local len = s_get_length(xCount, yCount, xPosIn, yPssIn )
			
			local f = math.min(len,lenMax)/lenMax
			--util.util.msgf(f)
			if bInvetIn == true then 
				f = 1 - f
			end
			local pos, color = s_getDiscreteColorByFactor( arrOut,sizeArr, f )				
			local bR,bG,bB = util.unpackcolor( color )
			
			local rMed = util.bytetofloat( bR )
			local gMed = util.bytetofloat( bG )
			local bMed = util.bytetofloat( bB )
			--util.util.msgf(rMed, " ",gMed," ",bMed)
			c_setalphaf(xCount,yCount,1.0)
			c_setpixelf( xCount,yCount, rMed, gMed, bMed, 1.0)						
			
			yCount = yCount + 1
		end		
		yCount = 0
		xCount = xCount + 1		
	end		
	
	c_saveimg( exImageOut, outPathIn )
	c_delimg( picImg )
	
	util.msg("***s_gradient finished")

end


--[[
	Draws circle gradient
]]--
function s_circlegradient( outPathIn, widthIn, heightIn, xPosIn, yPssIn, palleteNameIn, arrayOutSizeIn, bInvetIn )

	local exImageOut = s_getex(outPathIn)	
	
	local arrColor = s_getpallete(palleteNameIn)
	
	local arrOut = s_buildColorArray( arrColor, arrayOutSizeIn )	
	
	local picImg = c_createimg( widthIn, heightIn , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	c_selimg( picImg )
	local w = c_getwidth()
	local h = c_getheight()
	c_fillimgb(255,255,255)
	
	local xCount = 0
	local yCount = 0
		
	local lenMax = s_get_length( 0, h - 1, xPosIn, yPssIn )
	local sizeArr = s_arrlength(arrOut)
	while xCount < w do			
		while yCount < h do				
						
			local len = s_get_length(xCount, yCount, xPosIn, yPssIn )
			
			local f = math.min(len,lenMax)/lenMax
			--util.util.msgf(f)
			if bInvetIn == true then 
				f = 1 - f
			end
			local pos, color = s_getDiscreteColorByFactor( arrOut,sizeArr, f )				
			local bR,bG,bB = util.unpackcolor( color )
			
			local rMed = util.bytetofloat( bR )
			local gMed = util.bytetofloat( bG )
			local bMed = util.bytetofloat( bB )
			--util.util.msgf(rMed, " ",gMed," ",bMed)
			c_setalphaf(xCount,yCount,1.0)
			c_setpixelf( xCount,yCount, rMed, gMed, bMed, 1.0)						
			
			yCount = yCount + 1
		end		
		yCount = 0
		xCount = xCount + 1		
	end		
	
	c_saveimg( exImageOut, outPathIn )
	c_delimg( picImg )
	
	util.msg("***s_circlegradient finished")

end

--[[
	Draws vert gradient
]]--
function s_vertgradient( outPathIn, widthIn, heightIn, palleteNameIn, arrayOutSizeIn, bInvetIn )

	local exImageOut = s_getex(outPathIn)	
	
	local arrColor = s_getpallete(palleteNameIn)
	
	local arrOut = s_buildColorArray( arrColor, arrayOutSizeIn )	
	
	local picImg = c_createimg( widthIn, heightIn , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	c_selimg( picImg )
	local w = c_getwidth()
	local h = c_getheight()
	c_fillimgb(255,255,255)
	
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
			local bR,bG,bB = util.unpackcolor( color )
			
			local rMed = util.bytetofloat( bR )
			local gMed = util.bytetofloat( bG )
			local bMed = util.bytetofloat( bB )
			
			c_setalphaf(xCount,yCount,1.0)
			c_setpixelf( xCount,yCount, rMed, gMed, bMed, 1.0)						
			
			yCount = yCount + 1
		end		
		yCount = 0
		xCount = xCount + 1		
	end		
	
	c_saveimg( exImageOut, outPathIn )
	c_delimg( picImg )
	
	util.msg("***s_vertgradient finished")

end





--[[
	adds pattern in vertical order randomly on y axis, x position is fixed on 0
	interpolating to the palleteName
]]--
function s_addPatternImgVertRand( xSize, ySize, countAddIn,  imgDirPath, addImgEx, pathOut,   palleteName, interpFactorIn, channelIn, invertIn)
			

	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex( pathOut )		
	local arrColor = s_getpallete( palleteName )	
	local rezImg = c_createimg( xSize, ySize , "basic" ) -- c_readimg( imgPathIn, exImage, imageType )	
	
	c_fillimgb(255,255,255)
	c_fillalphaf(1)
	
	local posX = 0
	
	local interpFactor = 1
	
	if interpFactorIn~= nil then
		interpFactor = interpFactorIn
	end
	util.msg(interpFactor)
	local count = 0
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )
	
	while count < countAddIn do			
						
						
			local fileFullPath = util.getfile( imgDirPath, c_randintex( 0, filesMax - 1 ), addImgEx , false )
			local cRRand,cGRand,cBRand = util.unpackcolor( s_getRandomcolor(arrColor) )		
						
			local picToAdd = 0
			--[[if channelIn ~= nil then					
				local cRRand2,cGRand2,cBRand2 = util.unpackcolor( s_getRandomcolor( arrColor ) )
				picToAdd = s_interpToColorsByChannel( fileFullPath, nil, cRRand,cGRand,cBRand, cRRand2,cGRand2,cBRand2, channelIn ,  interpFactor )				
			else]]--
				picToAdd = s_interpToColor( fileFullPath,nil, cRRand,cGRand,cBRand, interpFactor, false )
			--end					 	
		
			c_selimg( rezImg )			
			c_injectimg( picToAdd, rezImg, 0, c_randintex( 50, ySize-50 ), 1  )				
			c_delimg( picToAdd )						
			
			count = count + 1
	end		
		
		
	c_selimg( rezImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( rezImg )
	
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
			
	c_assert( typeArrange >= 0 and typeArrange < 2 , " Wrong typeArrange value!" )
	--local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	
	--picImg = c_readimg( imgPathIn, exImage, imageType )	
	local picImg = c_createimg( widthIn, heightIn , imageType )

	c_fillimgb(255,255,255)
	--local w = c_getwidth()
	--local h = c_getheight()	
	c_fillalphaf(1)
	
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
				
				local fileFullPath = util.getfile( imgDirPath, c_randintex( 0, filesMax - 1 ), addImgEx , false )			
				--util.msg(fileFullPath)
				local picAdd = c_readimg( fileFullPath, addImgEx )	
				local w = c_getwidth()
				local h = c_getheight()
	
				stepX = w + between				
				stepY = h + between
				-- s_multalpha
				c_selimg( picImg )					
				if bRandAlphaIn == nil or bRandAlphaIn == false then
					c_injectimg( picAdd, picImg, posX, posY, alphaFactor )
				else
					c_injectimg( picAdd, picImg, posX, posY, math.min(c_randfloat() , alphaFactor) )
				end
				c_delimg( picAdd )
				
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
					
					local fileFullPath = util.getfile( imgDirPath, c_randintex( 0, filesMax - 1 ), addImgEx , false )			
					local picAdd = c_readimg( fileFullPath, addImgEx )	
					local w = c_getwidth()
					local h = c_getheight()
					offset = h/2
		
					stepX = w + between				
					stepY = h + between
					-- s_multalpha
					c_selimg( picImg )					
					if bRandAlphaIn == nil or bRandAlphaIn == false then
						c_injectimg( picAdd, picImg, posX, posY, alphaFactor )
					else
						c_injectimg( picAdd, picImg, posX, posY, c_randfloat() * alphaFactor )
					end
					c_delimg( picAdd )
					
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
	
	c_selimg( picImg )
	c_saveimg( "png", pathOut )
	c_delimg( picImg )
	
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
			
	c_assert( typeArrange >= 0 and typeArrange < 2 , " Wrong typeArrange value!" )
	local exImage 	 = s_getex(imgPathIn)		
	local exImageOut = s_getex(pathOut)	
	
	picImg = c_readimg( imgPathIn, exImage, imageType )	

	

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
				
				local fileFullPath = util.getfile( imgDirPath, c_randintex( 0, filesMax - 1 ), addImgEx , false )			
					--local picAdd = c_readimg( fileFullPath, addImgEx )					
					
				local srcBuff = s_MaskfromImageAlpha( fileFullPath  )
					
				local xMin, xMax, yMin, yMax = c_extremebitmap(srcBuff)
					
				local xSize = xMax - xMin
				local ySize = yMax - yMin
					
				c_scalebitmap(srcBuff, 1 - xCount/countAdd, 1 - xCount/countAdd )
				
				local picAdd = c_executefunc( "/effects/effect_fillcolor", "s_effect",srcBuff, 255,255,255, 0,0,0, 1, true )
				c_delbitmap(srcBuff)
				
				-- s_multalpha
				c_selimg( picImg )					
				
				c_injectimg( picAdd, picImg, posX, posY )
				c_delimg( picAdd )
				
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
					
					local fileFullPath = util.getfile( imgDirPath, c_randintex( 0, filesMax - 1 ), addImgEx , false )			
					--local picAdd = c_readimg( fileFullPath, addImgEx )					
					
					local srcBuff = s_MaskfromImageAlpha( fileFullPath  )
					
					local xMin, xMax, yMin, yMax = c_extremebitmap(srcBuff)
					
					local xSize = xMax - xMin
					local ySize = yMax - yMin
					
					c_scalebitmap(srcBuff, 1 - xCount/countAdd, 1 - xCount/countAdd )
					
					local picAdd = c_executefunc( "/effects/effect_fillcolor", "s_effect",srcBuff, 255,255,255, 0,0,0, 1, true )
					c_delbitmap(srcBuff)	

					-- s_multalpha
					c_selimg( picImg )					
					
					c_injectimg( picAdd, picImg, posX, posY )
					c_delimg( picAdd )
					
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
	
	c_selimg( picImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
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
	
	local picImg = c_readimg( imgPathIn, exImage, "seamless" )	
	
	local w = c_getwidth()

	local h = c_getheight()	
	
	local filesMax  = util.countfiles( imgDirPath, addImgEx )
	
	
	if countAdd > 0 then
		for i = 0, countAdd - 1 do 	
				local fileFullPath = util.getfile( imgDirPath, c_randintex( 0, filesMax - 1 ), addImgEx , false )			
				local picAdd = c_readimg( fileFullPath, addImgEx )
						
				-- s_multalpha
				c_selimg( picImg )
				if randAlphaIn == true then
					c_injectimg( picAdd, picImg, c_randint(0, w), c_randint(0, h) , c_randfloat() )
				else
					c_injectimg( picAdd, picImg, c_randint(0, w), c_randint(0, h) )
				end
				c_delimg( picAdd )
		end		
	else
		
		
		for f = 0, filesMax - 1 do
			local fileFullPath = util.getfile( imgDirPath, f , false )			
			local picAdd = c_readimg( fileFullPath, addImgEx )
					
			-- s_multalpha
			c_selimg( picImg )
			if randAlphaIn == true then
				c_injectimg( picAdd, picImg, c_randint(0, w), c_randint(0, h) , c_randfloat() )
			else
				c_injectimg( picAdd, picImg, c_randint(0, w), c_randint(0, h) )
			end
			c_delimg( picAdd )
		end
		
	end
	
	--s_drawlogo( picImg )
	
	
	c_selimg( picImg )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	
	util.msg("***s_addRandomImgOther finished")
end


