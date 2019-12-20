require "Scripts/lib/l_util"
--[[
s_interpToColor
s_interpToColors
s_interpToColorsLight
s_interpToColorsByChannel
s_interpToImage
s_interpToImageVal
s_interpToImageId
s_interpToImageChannel
s_interpToArrColorsDiscreteChannel
s_interpToImagesByArray
s_replaceColor
]]--

--[[
	toFile - if true saves to the file otherwise returns image id with alpha test (only a > 0)
]]--
function s_interpToColor( srcPath, destPath, bR, bG, bB, factor )

	--util.msg("interpolating file: ", srcPath )
	local picImg = 0
	if s_isstring(srcPath) == true  then		
		picImg = c_readimg( srcPath,  s_getex( srcPath ) )
	else
		picImg = srcPath
	end
	
	c_selimg( picImg )
	
	local w1 = c_getwidth()
	local h1 = c_getheight()
	
	local mainImage = c_createimg( w1, h1 , "basic" )	
	c_fillimgb(255,255,255)
	c_fillalphaf( 1 )

	local rMed = util.bytetofloat( bR )
	local gMed = util.bytetofloat( bG )
	local bMed = util.bytetofloat( bB )

	--local moveFact = 0.3
	local factor = factor

	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				c_selimg( picImg )
				
				local r,g,b,a = c_getpixelf( w_, h_) 
								
				local vr = util.c_cosinterp( r,rMed, factor )
							
				local vg = util.c_cosinterp( g,gMed, factor )
				
				local vb = util.c_cosinterp( b,bMed, factor )
								
				c_selimg( mainImage )
				
				if a > 0.0 then 												
					c_setalphaf( w_,h_, a )
					c_setpixelf( w_,h_, vr,vg,vb, 1 )				
				else					
					c_setalphaf( w_,h_, 0 )
				end
				
			end	
			
	end

	if s_isstring(srcPath) == true  then
		c_delimg( picImg )
	end
	
	if destPath ~= nil then
		c_selimg( mainImage )	
		c_saveimg( s_getex( destPath ), destPath )	
		c_delimg( mainImage )
	else	
		return mainImage;
	end

	util.msg("***Executing s_interpToColor() finished.")

end


--[[
	Interpolates image to colors on vertical with alpha test (only a > 0)
	interpType - 0 - vertical
	interpType - 1 - horisontal
	interpType - 2 - circle
	radius - using only if interpType == 2  
]]--
function s_interpToColors( srcPath, destPath, b1R, b1G, b1B, b2R, b2G, b2B, factor , interpType , radius )

	c_assert( interpType >=0 and interpType <= 2, "Wrong interpolation type: ", interpType )
	
	local picImg = c_readimg( srcPath, s_getex( srcPath ) )
	local w1 = c_getwidth()
	local h1 = c_getheight()
	local destEx = s_getex( srcPath )
	local mainImage = c_createimg( w1, h1 , "basic" )	

	c_fillalphaf( 1 )

	local rMed1 = util.bytetofloat( b1R )
	local gMed1 = util.bytetofloat( b1G )
	local bMed1 = util.bytetofloat( b1B )
	
	local rMed2 = util.bytetofloat( b2R )
	local gMed2 = util.bytetofloat( b2G )
	local bMed2 = util.bytetofloat( b2B )	
	
	
	if interpType == 0 then -- vert
		for w_ = 0, w1 - 1 do			
				for h_ = 0, h1 - 1 do	
					c_selimg( picImg )
					
					local r,g,b,a = c_getpixelf( w_, h_) 
					
					local vr = r
					
						
					rMed = util.c_cosinterp( rMed1,rMed2, h_/h1 )
					vr = util.c_cosinterp( r,rMed, factor )
					
					
					local vg = g
					
					gMed = util.c_cosinterp( gMed1,gMed2, h_/h1 )
					vg = util.c_cosinterp( g,gMed, factor )
					
					
					local vb = b
					
					 bMed = util.c_cosinterp( bMed1,bMed2, h_/h1 )
					 vb = util.c_cosinterp( b,bMed, factor )
					
					
					c_selimg( mainImage )
					
					if a > 0.0 then 												
						c_setalphaf( w_,h_, a )
						c_setpixelf( w_,h_, vr,vg,vb, 1 )				
					else					
						c_setalphaf( w_,h_, 0 )
					end
					
				end	
				
		end
	elseif  interpType == 1 then -- hor
		for w_ = 0, w1 - 1 do			
				for h_ = 0, h1 - 1 do	
					c_selimg( picImg )
					
					local r,g,b,a = c_getpixelf( w_, h_) 
					
					local vr = r
					
						
					rMed = util.c_cosinterp( rMed1,rMed2, w_/w1 )
					vr = util.c_cosinterp( r,rMed, factor )
					
					
					local vg = g
					
					gMed = util.c_cosinterp( gMed1,gMed2, w_/w1 )
					vg = util.c_cosinterp( g,gMed, factor )
					
					
					local vb = b
					
					 bMed = util.c_cosinterp( bMed1,bMed2, w_/w1 )
					 vb = util.c_cosinterp( b,bMed, factor )
					
					
					c_selimg( mainImage )
					
					if a > 0.0 then 												
						c_setalphaf( w_,h_, a )
						c_setpixelf( w_,h_, vr,vg,vb, 1 )				
					else					
						c_setalphaf( w_,h_, 0 )
					end
					
				end					
		end
	elseif  interpType == 2 then -- circle
	
		
		c_assert( radius > 0 , "Circle interpolation was chosen and radius == ", radius )
		
		local xCent = w1 / 2
		local yCent = h1 / 2	
		
		for w_ = 0, w1 - 1 do			
				for h_ = 0, h1 - 1 do	
				
					c_selimg( picImg )
					
					local len = math.min( s_get_length(xCent, yCent, w_, h_) , radius )					
					local r,g,b,a = c_getpixelf( w_, h_) 					
					local vr = r					
						
					rMed = util.c_cosinterp( rMed1,rMed2, len/radius )
					vr = util.c_cosinterp( r,rMed, factor )					
					
					local vg = g
					
					gMed = util.c_cosinterp( gMed1,gMed2, len/radius )
					vg = util.c_cosinterp( g,gMed, factor )					
					
					local vb = b
					
					 bMed = util.c_cosinterp( bMed1,bMed2, len/radius )
					 vb = util.c_cosinterp( b,bMed, factor )					
					
					c_selimg( mainImage )
					
					if a > 0.0 then 												
						c_setalphaf( w_,h_, a )
						c_setpixelf( w_,h_, vr,vg,vb, 1 )				
					else					
						c_setalphaf( w_,h_, 0 )
					end					
				end					
		end		
	
	end


	c_selimg( mainImage )	

	--main_path = "c:/pic"
	--path_ex = "png"
	--folder = "/rez/"

	--string_val = string.format('%s%s%s%s%s%s%s', main_path, folder,"tile_grunge ","_",c_uniqid(),".", path_ex )
	c_saveimg( destEx, destPath )

	c_delimg( picImg )
	c_delimg( mainImage )

	util.msg("***Executing s_interpToColors() finished.")

end

--[[
 Interpolates image to colors by light factor
 good for contrast images	
 reduce == 1 no reduction, reduce == 0.5, if invert == false - reduce color for light or dark otherwise 
 ]]--
function s_interpToColorsLight( srcPath, destPath, bR, bG, bB, bR2, bG2, bB2, reduce, invert  )

	local picImg = c_readimg( srcPath, s_getex( srcPath ) )
	local w1 = c_getwidth()
	local h1 = c_getheight()
	local destEx = s_getex( srcPath )
	local mainImage = c_createimg( w1, h1 , "basic" )	
	
	local catrom = c_creategeom( "geom_catrom" )


	c_fillalphaf( 1 )

	local rMed = util.bytetofloat( bR )
	local gMed = util.bytetofloat( bG )
	local bMed = util.bytetofloat( bB )
	
	

	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				c_selimg( picImg )
							
				local r,g,b,a = c_getpixelf( w_, h_)									
				local h,s,l = util.c_rgbtohsl( r,g,b )				
							
				
				r = util.bytetofloat(bR2)				
				g = util.bytetofloat(bG2)				
				b = util.bytetofloat(bB2)
										
								
				local rl = r
				local gl = g
				local bl = b				
				local al = a
				
				--util.util.msgf(rl, " ", gl," ", bl)
						
				
				local factorEnd = 0
				if invert == true then
					factorEnd = (  1 -  (l * reduce)  ) 
				else
					factorEnd = l * reduce
				end			
				
				
				local vr = util.c_cosinterp( rl,rMed, factorEnd )
				local vg = util.c_cosinterp( gl,gMed, factorEnd )
				local vb = util.c_cosinterp( bl,bMed, factorEnd )
				
				c_selimg( mainImage )
				if al > 0.0 then 												
					c_setalphaf( w_,h_, al )
					c_setpixelf( w_,h_, vr,vg,vb, 1 )				
				else					
					c_setalphaf( w_,h_, 0 )
				end
				
				
			end	
			
	end

	
	c_delgeom(catrom)
	c_delimg( picImg )
	--util.msg("***Executing s_interpToColor() finished.")
	
	if destPath ~= "" then
		c_saveimg( destEx, destPath )	
		c_delimg( mainImage )
	else
		return mainImage
	end

end


--[[
Interpolates by channel intensity
channel == 0 - by r
channel == 1 - by g
channel == 2 - by b
channel == 3 - alpha
channel == 4 - lought
channel == 5 - rand
]]--
function s_interpToColorsByChannel( srcPath, destPath,  b1R, b1G, b1B, b2R, b2G, b2B, channel, factor )


	c_assert( channel >=0 and channel <= 4, "Wrong channel value: ", channel )
	local picImg = c_readimg( srcPath, s_getex( srcPath ) )
	local w1 = c_getwidth()
	local h1 = c_getheight()
	local destEx = s_getex( srcPath )
	
	local mainImage = c_createimg( w1, h1 , "basic" )

	c_fillalphaf( 1 )

	local rMed1 = util.bytetofloat( b1R )
	local gMed1 = util.bytetofloat( b1G )
	local bMed1 = util.bytetofloat( b1B )
	
	local rMed2 = util.bytetofloat( b2R )
	local gMed2 = util.bytetofloat( b2G )
	local bMed2 = util.bytetofloat( b2B )	
	

	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				c_selimg( picImg )
				
				local r,g,b,a = c_getpixelf( w_, h_) 
							

				local chFactor = 0
				if channel ==  0 then
					chFactor = r
				elseif channel ==  1 then
					chFactor = g
				elseif channel ==  2 then
					chFactor = b				
				elseif channel ==  3 then
					chFactor = a				
				elseif channel ==  4 then
					local h,s,l = util.c_rgbtohsl( r,g,b )	
					chFactor = l
				elseif channel ==  5 then					
					chFactor = c_randfloat()
				end		
								
				local rMed = util.c_cosinterp( rMed21,rMed2, factor * chFactor )						
				vr = util.c_cosinterp( r,rMed, factor )
					
				local gMed = util.c_cosinterp( gMed21,gMed2, factor * chFactor )						
				vg = util.c_cosinterp( g,gMed, factor )

				local bMed = util.c_cosinterp( bMed21,bMed2, factor * chFactor )						
				vb = util.c_cosinterp( b,bMed, factor )				
				
				c_selimg( mainImage )
				
				if a > 0.0 then 												
					c_setalphaf( w_,h_, a )
					c_setpixelf( w_,h_, vr,vg,vb, 1 )				
				else					
					c_setalphaf( w_,h_, 0 )
				end				
			end				
	end

	
	c_delimg( picImg )
	
	if destPath ~= "" then
		c_selimg( mainImage )	
		c_saveimg( destEx, destPath )
		c_delimg( mainImage )
	else
		return mainImage
	end

	util.msg("***Executing s_interpToColor() finished.")
end




--[[
	Interpolates src image to the InterpImage only with pixels with value rIn, gIn, bIn
	with alpha test (only a > 0)
	pathIn -  path or id to the src image
	pathOut - dest image or nil
	pathInterpImage - interp image path	
	bToFile - if true saves to file, if false - returns image id
	bRandomPos - if true - position is random
]]--
function s_interpToImageVal( pathIn, pathOut,  pathInterpImage , rIn, gIn, bIn, factor,  bRandomPos)
	
	local picImg = 0
	if s_isstring(pathIn) == true  then
		picImg = c_readimg( pathIn, s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	
	local interpImg = 0
	
	if s_isstring(pathInterpImage) == true  then
		interpImg = c_readimg( pathInterpImage, s_getex(pathInterpImage) )	
	else
		interpImg = pathInterpImage
	end
	
	c_selimg( picImg )

	local w = c_getwidth()

	local h = c_getheight()	
	
	c_selimg( interpImg )

	local wi = c_getwidth()

	local hi = c_getheight()
	
	
	
	local outImage = c_createimg( w, h , "basic" )
	
	local offsetW = 0
	local offsetH = 0
	if bRandomPos == nil or bRandomPos == true then
		offsetW = c_randint(0, wi - w)
		offsetH = c_randint(0, hi - h)
	end	
		
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelf( w_, h_) 
				local rb,gb,bb,ab = c_getpixelb( w_, h_)
								
				
				c_selimg( interpImg )					
				
				local outr = r				
				local outg = g				
				local outb = b	
					
				if rb == rIn and  gb == gIn and bb == bIn then					
					local ri,gi,bi = c_getpixelf( (w_+ offsetW)%wi, (h_+ offsetH)%hi) 												
					outr = util.c_cosinterp( r,ri, factor )				
					outg = util.c_cosinterp( g,gi, factor )				
					outb = util.c_cosinterp( b,bi, factor )				
				end
				
				c_selimg( outImage )
				
				if ab > 0.0 then 												
					c_setalphaf( w_,h_, a )
					c_setpixelf( w_,h_, outr,outg,outb, 1 )				
				else					
					c_setalphaf( w_,h_, 0 )
				end			
			end					
	end	
	
	if pathOut ~= nil then
		c_saveimg( s_getex(pathOut), pathOut )		
		c_delimg( outImage )
	end
	
	c_delimg( picImg )
	
	c_delimg( interpImg )	
	
	
	if pathOut == nil then
		return outImage
	end
end


--[[
	Interpolates src image to the InterpImage with alpha test (only a > 0)
	pathIn -  path or id to the src image
	pathOut - dest image
	pathInterpImage - interp image path		
	bRandomPos - if true - position is random
]]--
function s_interpToImage( pathIdIn, pathOut,  pathInterpImage ,  factor, bRandomPos )

	local picImg = 0
	
	if s_isstring(pathIdIn) == true  then
		local exImage = s_getex(pathIdIn)	
		picImg = c_readimg( pathIdIn, exImage )
	else
		picImg = pathIdIn
	end
	local interpImg = 0
	
	if s_isstring(pathInterpImage) == true  then
		interpImg = c_readimg( pathInterpImage, s_getex(pathInterpImage) )	
	else
		interpImg = pathInterpImage
	end
	
	c_selimg( picImg )

	local w = c_getwidth()

	local h = c_getheight()
	
	
	c_selimg( interpImg )

	local wi = c_getwidth()

	local hi = c_getheight()
		
	local outImage = c_createimg( w, h , "basic" )
	
	local offsetW = 0
	local offsetH = 0
	if bRandomPos == nil or bRandomPos == true then
		offsetW = c_randint(0, wi - w)
		offsetH = c_randint(0, hi - h)
	end	
	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelf( w_, h_) 
				
				c_selimg( interpImg )					
				
				local ri,gi,bi = c_getpixelf( (w_+ offsetW)%wi, (h_+ offsetH)%hi) 			
								
				local outr = util.c_cosinterp( r,ri, factor )				
				local outg = util.c_cosinterp( g,gi, factor )				
				local outb = util.c_cosinterp( b,bi, factor )				
				
				c_selimg( outImage )
				
				if a > 0.0 then 												
					c_setalphaf( w_,h_, a )
					c_setpixelf( w_,h_, outr,outg,outb, 1 )				
				else					
					c_setalphaf( w_,h_, 0 )
				end				
				
			end		
	end
	
	
	if pathOut ~= nil then
		c_saveimg( s_getex(pathOut)	, pathOut )		
		c_delimg( outImage )
	end
	
	if s_isstring(pathIdIn) == true then
		c_delimg( picImg )
	end
	
	if s_isstring(pathInterpImage) == true then
		c_delimg( interpImg )		
	end
	
	
	if pathOut == nil then
		return outImage
	end
end



--[[
	channel - 0 - r
	channel - 1 - g
	channel - 2 - b
	channel - 3 - a
	channel - 4 - lightness
	bAlphaTestIn - if true ignore all alpha == 0
]]--
function s_interpToArrColorsDiscreteChannel( pathIn, pathOut , colorsIn, factorIn, invertIn, channelIn, bAlphaTestIn )	
	
	local exImage = s_getex( pathIn )
	
	local exImageOut = s_getex( pathOut )	
	
	local picImg = c_readimg( pathIn, exImage )	
	
	local w = c_getwidth()

	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	local sizeArr = s_arrlength( colorsIn )
	local channel = channelIn
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelf( w_, h_ ) 
				
					
				local f = 0
				if channel ==  0 then
					f = r
				elseif channel ==  1 then
					f = g
				elseif channel ==  2 then
					f = b				
				elseif channel ==  3 then
					f = a				
				elseif channel ==  4 then
					local h,s,l = util.c_rgbtohsl( r,g,b )	
					f = l
				elseif channel ==  5 then					
					f = c_randfloat()				
				end
				
				if invertIn == true then
					f = 1 - f
				end
				if bAlphaTestIn == false or a > 0.0 then 				
					c_selimg( outImage )			

					local pos, color = s_getDiscreteColorByFactor( colorsIn,sizeArr, f )				
					
					local bR,bG,bB = util.c_unpackcolor( color )
					
					local rMed = util.bytetofloat( bR )
					local gMed = util.bytetofloat( bG )
					local bMed = util.bytetofloat( bB )
		
					local vr = util.c_cosinterp( r,rMed, factorIn )				
					local vg = util.c_cosinterp( g,gMed, factorIn )
					local vb = util.c_cosinterp( b,bMed, factorIn )
					
					c_setpixelf( w_, h_, vr,vg,vb, 1 )					
										
					c_setalphaf( w_, h_, a )	
				end	
			end		
	end
	c_selimg( outImage )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
	util.msg("***s_interpToArrColorsDiscreteChannel finished")
end

--[[
	Works like s_interpToArrColorsDiscreteChannel instead of color from  the array gets image from dir by index
]]--
function s_interpToImagesByArray( pathIn, pathOut , colorsIn, factorIn, invertIn, channelIn, bAlphaTestIn , imgDirIn, imgExIn )	
	
	
	local filesMax  = util.c_countfiles( imgDirIn, imgExIn )
	local sizeArr = s_arrlength(colorsIn)
	
	
	local interpImgIds = {}
	local wi = 0
	local hi = 0
	
	for i = 1, sizeArr do
	
		local filePos = math.max(i%filesMax,1)		
		local pathInterp = util.c_getfile( imgDirIn, filePos - 1 , imgExIn , false ) 
		
		local interpImg = c_readimg( pathInterp, imgExIn )
		wi = c_getwidth()
		hi = c_getheight()
		util.msg(interpImg)
		interpImgIds[i] = interpImg
	end
	
	
	local exImage = s_getex( pathIn )
	
	local exImageOut = s_getex( pathOut )	
	
	local picImg = c_readimg( pathIn, exImage )	
	
	local w = c_getwidth()
	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	local channel = channelIn
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelf( w_, h_ ) 			
					
				local f = 0
				if channel ==  0 then
					f = r
				elseif channel ==  1 then
					f = g
				elseif channel ==  2 then
					f = b				
				elseif channel ==  3 then
					f = a				
				elseif channel ==  4 then
					local h,s,l = util.c_rgbtohsl( r,g,b )	
					f = l
				elseif channel ==  5 then					
					f = c_randfloat()				
				end
				
				if invertIn == true then
					f = 1 - f
				end
				if bAlphaTestIn == false or a > 0.0 then 				
								

					local pos, color = s_getDiscreteColorByFactor( colorsIn,sizeArr, f )				
					local key = math.max((pos+1)%filesMax,1)
					
					c_selimg( interpImgIds[key] )
					local ri,gi,bi,ai = c_getpixelf( w_ % wi, h_ % hi )
					
					local vr = util.c_cosinterp( r,ri, factorIn )				
					local vg = util.c_cosinterp( g,gi, factorIn )
					local vb = util.c_cosinterp( b,bi, factorIn )
					
					c_selimg( outImage )
					c_setpixelf( w_, h_, vr,vg,vb, 1 )					
										
					c_setalphaf( w_, h_, a )	
				end	
			end		
	end
	c_selimg( outImage )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
	
	
	for i = 1, #interpImgIds do
		c_delimg( i )
	end
	
	
	
	util.msg("***s_interpToImagesByArray finished")
end

--[[
	Replaces colors from pallete with texture from the folder in tile mode 
	every color in pallete replaces with pixel from texture with the same position in the directory
]]--
function s_replaceColor( pathIn, pathOut , colorsIn, factorIn, bAlphaTestIn , imgDirIn, imgExIn )	
	
	
	local filesMax  = util.c_countfiles( imgDirIn, imgExIn )
	local sizeArr = s_arrlength(colorsIn)
	util.msg(filesMax)
	util.msg(sizeArr)
	
	
	local interpImgIds = {}
	local wi = 0
	local hi = 0
	
	for i = 1, sizeArr do
	
		local filePos = math.max(i%filesMax,1)		
		local pathInterp = util.c_getfile( imgDirIn, filePos - 1 , imgExIn , false ) 
		
		local interpImg = c_readimg( pathInterp, imgExIn )
		wi = c_getwidth()
		hi = c_getheight()
		util.msg(interpImg)
		interpImgIds[i] = interpImg
	end
	
	
	local exImage = s_getex( pathIn )
	
	local exImageOut = s_getex( pathOut )	
	
	local picImg = c_readimg( pathIn, exImage )	
	
	local w = c_getwidth()
	local h = c_getheight()

	local outImage = c_createimg( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	local channel = channelIn
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				c_selimg( picImg )										
				
				local r,g,b,a = c_getpixelb( w_, h_ ) 			
										
				
				if bAlphaTestIn == false or a > 0 then 				
					local packed = util.c_packcolor(r,g,b)		
					
					local pos = s_findValue(colorsIn, packed)
					--local pos, color = s_getDiscreteColorByFactor( colorsIn,sizeArr, f )				
					if pos ~= -1 then
						local key = math.max((pos+1)%filesMax,1)
						
						c_selimg( interpImgIds[key] )
						local ri,gi,bi,ai = c_getpixelf( w_ % wi, h_ % hi )
						
						local vr = util.c_cosinterp( util.bytetofloat(r),ri, factorIn )				
						local vg = util.c_cosinterp( util.bytetofloat(g),gi, factorIn )
						local vb = util.c_cosinterp( util.bytetofloat(b),bi, factorIn )
						
						c_selimg( outImage )
						c_setpixelf( w_, h_, vr,vg,vb, 1 )					
											
						c_setalphaf( w_, h_, util.bytetofloat(a) )
					end					
				end	
			end		
	end
	c_selimg( outImage )
	c_saveimg( exImageOut, pathOut )
	c_delimg( picImg )
	
	c_delimg( outImage )
	
	
	for i = 1, #interpImgIds do
		c_delimg( i )
	end
	
	
	
	util.msg("***s_interpToImagesByArray finished")
end



--[[
	Injects one image in to the another using channelIn factor from src image
	mainImgIn -  src image
	pathOut - dest image
	toInjectIn - image path to inject
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a
	channelIn - 4 - lightness	
	factorIn - reduce factor default == 1
	xPosIn - x pos
	yPosIn - y pos
]]--
function s_injectImageChannel( mainImgIn, pathOutIn,  toInjectIn, channelIn, factorIn, xPosIn, yPosIn )

		
	local interpImg = c_readimg(  toInjectIn, s_getex(toInjectIn) )
	
	local picImageId = 0
	
	if s_isstring(mainImgIn) == true  then		
		picImageId = c_readimg( mainImgIn, exImage )
	else
		picImageId = mainImgIn
	end
	c_selimg( picImageId )
	
	local w = c_getwidth()
	local h = c_getheight()		
	
	c_selimg( interpImg )
	local wi = c_getwidth()
	local hi = c_getheight()		
	
	local offsetX = xPosIn
	local offsetY = yPosIn
	
	for w_ = 0, wi-1 do	
			for h_ = 0, hi-1 do		
			
				c_selimg( picImageId )		
				local posX =  w_ + offsetX
				local posY =  h_ + offsetY
				if posX <  w - 1 and posY <  h - 1 then
				
					local r,g,b,a = c_getpixelf( posX, posY ) 
					
					local chFactor = 0
					if channelIn ==  0 then
						chFactor = r
					elseif channelIn ==  1 then
						chFactor = g
					elseif channelIn ==  2 then
						chFactor = b				
					elseif channelIn ==  3 then
						chFactor = a				
					elseif channelIn ==  4 then
						local h,s,l = util.c_rgbtohsl( r,g,b )	
						chFactor = l				
					end						
					
					c_selimg( interpImg )					
					
					local ri,gi,bi,af = c_getpixelf(  (w_ + offsetX)%wi, (h_ + offsetY)%hi ) 							
					
					local factorEnd = chFactor * factorIn * af
					
					local outr = util.c_cosinterp( r, ri, factorEnd )
					local outg = util.c_cosinterp( g, gi, factorEnd )
					local outb = util.c_cosinterp( b, bi, factorEnd )
					
					c_selimg( picImageId )				
					
					c_setpixelf( w_ + offsetX, h_ + offsetY, outr,outg,outb, 1 )	
				end				
				
				
			end		
	end
	
	c_delimg( interpImg )	
	util.msg( "finished" )
	
	if s_isstring( mainImgIn ) == true then
		c_selimg( picImageId )
		c_saveimg( "png", pathOutIn )
		c_delimg( picImageId )
	else
		return picImageId
	end	
end


--[[
	Cahnges alpha value with channel value from another image
	mainImgIn -  src image, id or path
	pathOut   -  dest image if mainImgIn id path
	channelImageIn - image path with channel value
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a
	channelIn - 4 - lightness	
	factorIn - reduce factor default == 1
	minValIn - lower bound, default == 0
	maxValIn - upper bound, default == 1
	xPosIn - x pos
	yPosIn - y pos
]]--
function s_changeAlphaChannel( mainImgIn, pathOutIn, channelImageIn, channelIn, factorIn, minValIn, maxValIn, xPosIn, yPosIn )

		
	local channeImgId = c_readimg(  channelImageIn, s_getex( channelImageIn ) )
	
	local picImageId = 0
	
	if s_isstring(mainImgIn) == true  then		
		picImageId = c_readimg( mainImgIn, exImage )
	else
		picImageId = mainImgIn
	end
	c_selimg( picImageId )
	
	local w = c_getwidth()
	local h = c_getheight()		
	
	c_selimg( channeImgId )
	
	local wi = c_getwidth()
	local hi = c_getheight()		
	
	local offsetX = xPosIn
	local offsetY = yPosIn
	
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do		
			
				c_selimg( picImageId )		
				
				local r,g,b,a = c_getpixelf( w_, h_ ) 					
				
				c_selimg( channeImgId )					
				
				local ri,gi,bi,ai = c_getpixelf(  ( w_ + offsetX ) % wi, ( h_ + offsetY ) % hi ) 		

				local chFactor = 0
				if channelIn ==  0 then
					chFactor = ri
				elseif channelIn ==  1 then
					chFactor = gi
				elseif channelIn ==  2 then
					chFactor = bi				
				elseif channelIn ==  3 then
					chFactor = ai				
				elseif channelIn ==  4 then
					local h,s,l = util.c_rgbtohsl( ri,gi,bi )	
					chFactor = l				
				end				
				chFactor = math.min( math.max( minValIn, chFactor ), maxValIn ) 
				local factorEnd = factorIn * a
				
				local aout = util.c_cosinterp( a, chFactor, factorEnd )				
				
				c_selimg( picImageId )				
				
				c_setpixelf( w_, h_, r,g,b, 1 )	
				c_setalphaf( w_, h_, aout )					
			end		
	end
	
	c_delimg( channeImgId )	
	util.msg( "s_changeAlphaChannel finished" )
	
	if s_isstring( mainImgIn ) == true then
		c_selimg( picImageId )
		c_saveimg( "png", pathOutIn )
		c_delimg( picImageId )
	else
		return picImageId
	end	
end



