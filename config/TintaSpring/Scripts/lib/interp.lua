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
s_injectImageChannel
s_changeAlphaChannel
]]--

--[[
	toFile - if true saves to the file otherwise returns image id with alpha test (only a > 0)
]]--
function s_interpToColor( srcPath, destPath, bR, bG, bB, factor )

	--util.msg("interpolating file: ", srcPath )
	local picImg = 0
	if s_isstring(srcPath) == true  then		
		picImg = image.read( srcPath,  s_getex( srcPath ) )
	else
		picImg = srcPath
	end
	
	image.select( picImg )
	
	local w1 = image.width()
	local h1 = image.height()
	
	local mainImage = image.create( w1, h1 , "basic" )	
	image.fillb(255,255,255)
	image.fillchannelf( 3, 1 )

	local rMed = util.bytetofloat( bR )
	local gMed = util.bytetofloat( bG )
	local bMed = util.bytetofloat( bB )

	--local moveFact = 0.3
	local factor = factor

	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				image.select( picImg )
				
				local r,g,b,a = image.getpixelf( w_, h_) 
								
				local vr = util.cosinterp( r,rMed, factor )
							
				local vg = util.cosinterp( g,gMed, factor )
				
				local vb = util.cosinterp( b,bMed, factor )
								
				image.select( mainImage )
				
				if a > 0.0 then 												
					image.setchannelf( w_,h_,3, a )
					image.setpixelf( w_,h_, vr,vg,vb, 1 )				
				else					
					image.setchannelf( w_,h_,3, 0 )
				end
				
			end	
			
	end

	if s_isstring(srcPath) == true  then
		image.erase( picImg )
	end
	
	if destPath ~= nil then
		image.select( mainImage )	
		image.save( s_getex( destPath ), destPath )	
		image.erase( mainImage )
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

	main.lassert( interpType >=0 and interpType <= 2, "Wrong interpolation type: ", interpType )
	
	local picImg = image.read( srcPath, s_getex( srcPath ) )
	local w1 = image.width()
	local h1 = image.height()
	local destEx = s_getex( srcPath )
	local mainImage = image.create( w1, h1 , "basic" )	

	image.fillchannelf( 3, 1 )

	local rMed1 = util.bytetofloat( b1R )
	local gMed1 = util.bytetofloat( b1G )
	local bMed1 = util.bytetofloat( b1B )
	
	local rMed2 = util.bytetofloat( b2R )
	local gMed2 = util.bytetofloat( b2G )
	local bMed2 = util.bytetofloat( b2B )	
	
	
	if interpType == 0 then -- vert
		for w_ = 0, w1 - 1 do			
				for h_ = 0, h1 - 1 do	
					image.select( picImg )
					
					local r,g,b,a = image.getpixelf( w_, h_) 
					
					local vr = r
					
						
					rMed = util.cosinterp( rMed1,rMed2, h_/h1 )
					vr = util.cosinterp( r,rMed, factor )
					
					
					local vg = g
					
					gMed = util.cosinterp( gMed1,gMed2, h_/h1 )
					vg = util.cosinterp( g,gMed, factor )
					
					
					local vb = b
					
					 bMed = util.cosinterp( bMed1,bMed2, h_/h1 )
					 vb = util.cosinterp( b,bMed, factor )
					
					
					image.select( mainImage )
					
					if a > 0.0 then 												
						image.setchannelf( w_,h_,3, a )
						image.setpixelf( w_,h_, vr,vg,vb, 1 )				
					else					
						image.setchannelf( w_,h_,3, 0 )
					end
					
				end	
				
		end
	elseif  interpType == 1 then -- hor
		for w_ = 0, w1 - 1 do			
				for h_ = 0, h1 - 1 do	
					image.select( picImg )
					
					local r,g,b,a = image.getpixelf( w_, h_) 
					
					local vr = r
					
						
					rMed = util.cosinterp( rMed1,rMed2, w_/w1 )
					vr = util.cosinterp( r,rMed, factor )
					
					
					local vg = g
					
					gMed = util.cosinterp( gMed1,gMed2, w_/w1 )
					vg = util.cosinterp( g,gMed, factor )
					
					
					local vb = b
					
					 bMed = util.cosinterp( bMed1,bMed2, w_/w1 )
					 vb = util.cosinterp( b,bMed, factor )
					
					
					image.select( mainImage )
					
					if a > 0.0 then 												
						image.setchannelf( w_,h_,3, a )
						image.setpixelf( w_,h_, vr,vg,vb, 1 )				
					else					
						image.setchannelf( w_,h_,3, 0 )
					end
					
				end					
		end
	elseif  interpType == 2 then -- circle
	
		
		main.lassert( radius > 0 , "Circle interpolation was chosen and radius == ", radius )
		
		local xCent = w1 / 2
		local yCent = h1 / 2	
		
		for w_ = 0, w1 - 1 do			
				for h_ = 0, h1 - 1 do	
				
					image.select( picImg )
					
					local len = math.min( s_dist2(xCent, yCent, w_, h_) , radius )					
					local r,g,b,a = image.getpixelf( w_, h_) 					
					local vr = r					
						
					rMed = util.cosinterp( rMed1,rMed2, len/radius )
					vr = util.cosinterp( r,rMed, factor )					
					
					local vg = g
					
					gMed = util.cosinterp( gMed1,gMed2, len/radius )
					vg = util.cosinterp( g,gMed, factor )					
					
					local vb = b
					
					 bMed = util.cosinterp( bMed1,bMed2, len/radius )
					 vb = util.cosinterp( b,bMed, factor )					
					
					image.select( mainImage )
					
					if a > 0.0 then 												
						image.setchannelf( w_,h_,3, a )
						image.setpixelf( w_,h_, vr,vg,vb, 1 )				
					else					
						image.setchannelf( w_,h_,3, 0 )
					end					
				end					
		end		
	
	end


	image.select( mainImage )	

	--main_path = "c:/pic"
	--path_ex = "png"
	--folder = "/rez/"

	--string_val = string.format('%s%s%s%s%s%s%s', main_path, folder,"tile_grunge ","_",main.uniqid(),".", path_ex )
	image.save( destEx, destPath )

	image.erase( picImg )
	image.erase( mainImage )

	util.msg("***Executing s_interpToColors() finished.")

end

--[[
 Interpolates image to colors by light factor
 good for contrast images	
 reduce == 1 no reduction, reduce == 0.5, if invert == false - reduce color for light or dark otherwise 
 ]]--
function s_interpToColorsLight( srcPath, destPath, bR, bG, bB, bR2, bG2, bB2, reduce, invert  )

	local picImg = image.read( srcPath, s_getex( srcPath ) )
	local w1 = image.width()
	local h1 = image.height()
	local destEx = s_getex( srcPath )
	local mainImage = image.create( w1, h1 , "basic" )	
	
	local catrom = c_creategeom( "geom_catrom" )


	image.fillchannelf( 3, 1 )

	local rMed = util.bytetofloat( bR )
	local gMed = util.bytetofloat( bG )
	local bMed = util.bytetofloat( bB )
	
	

	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				image.select( picImg )
							
				local r,g,b,a = image.getpixelf( w_, h_)									
				local h,s,l = util.c_rgbtohsl( r,g,b )				
							
				
				r = util.bytetofloat(bR2)				
				g = util.bytetofloat(bG2)				
				b = util.bytetofloat(bB2)
										
								
				local rl = r
				local gl = g
				local bl = b				
				local al = a
				
				--util.msgf(rl, " ", gl," ", bl)
						
				
				local factorEnd = 0
				if invert == true then
					factorEnd = (  1 -  (l * reduce)  ) 
				else
					factorEnd = l * reduce
				end			
				
				
				local vr = util.cosinterp( rl,rMed, factorEnd )
				local vg = util.cosinterp( gl,gMed, factorEnd )
				local vb = util.cosinterp( bl,bMed, factorEnd )
				
				image.select( mainImage )
				if al > 0.0 then 												
					image.setchannelf( w_,h_,3, al )
					image.setpixelf( w_,h_, vr,vg,vb, 1 )				
				else					
					image.setchannelf( w_,h_,3, 0 )
				end
				
				
			end	
			
	end

	
	image.erasegeom(catrom)
	image.erase( picImg )
	--util.msg("***Executing s_interpToColor() finished.")
	
	if destPath ~= "" then
		image.save( destEx, destPath )	
		image.erase( mainImage )
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


	main.lassert( channel >=0 and channel <= 4, "Wrong channel value: ", channel )
	local picImg = image.read( srcPath, s_getex( srcPath ) )
	local w1 = image.width()
	local h1 = image.height()
	local destEx = s_getex( srcPath )
	
	local mainImage = image.create( w1, h1 , "basic" )

	image.fillchannelf( 3,1 )

	local rMed1 = util.bytetofloat( b1R )
	local gMed1 = util.bytetofloat( b1G )
	local bMed1 = util.bytetofloat( b1B )
	
	local rMed2 = util.bytetofloat( b2R )
	local gMed2 = util.bytetofloat( b2G )
	local bMed2 = util.bytetofloat( b2B )	
	

	for w_ = 0, w1 - 1 do			
			for h_ = 0, h1 - 1 do	
				image.select( picImg )
				
				local r,g,b,a = image.getpixelf( w_, h_) 
							

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
					chFactor = main.randfloat()
				end		
								
				local rMed = util.cosinterp( rMed1,rMed2, factor * chFactor )						
				vr = util.cosinterp( r,rMed, factor )
					
				local gMed = util.cosinterp( gMed1,gMed2, factor * chFactor )						
				vg = util.cosinterp( g,gMed, factor )

				local bMed = util.cosinterp( bMed1,bMed2, factor * chFactor )						
				vb = util.cosinterp( b,bMed, factor )				
				
				image.select( mainImage )
				
				if a > 0.0 then 												
					image.setchannelf( w_,h_,3, a )
					image.setpixelf( w_,h_, vr,vg,vb, 1 )				
				else					
					image.setchannelf( w_,h_,3, 0 )
				end				
			end				
	end

	
	image.erase( picImg )
	
	if destPath ~= "" then
		image.select( mainImage )	
		image.save( destEx, destPath )
		image.erase( mainImage )
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
		picImg = image.read( pathIn, s_getex( pathIn ) )
	else
		picImg = pathIn
	end
	
	local interpImg = 0
	
	if s_isstring(pathInterpImage) == true  then
		interpImg = image.read( pathInterpImage, s_getex(pathInterpImage) )	
	else
		interpImg = pathInterpImage
	end
	
	image.select( picImg )

	local w = image.width()

	local h = image.height()	
	
	image.select( interpImg )

	local wi = image.width()

	local hi = image.height()
	
	
	
	local outImage = image.create( w, h , "basic" )
	
	local offsetW = 0
	local offsetH = 0
	if bRandomPos == nil or bRandomPos == true then
		offsetW = main.randint(0, wi - w)
		offsetH = main.randint(0, hi - h)
	end	
		
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelf( w_, h_) 
				local rb,gb,bb,ab = image.getpixelb( w_, h_)
								
				
				image.select( interpImg )					
				
				local outr = r				
				local outg = g				
				local outb = b	
					
				if rb == rIn and  gb == gIn and bb == bIn then					
					local ri,gi,bi = image.getpixelf( (w_+ offsetW)%wi, (h_+ offsetH)%hi) 												
					outr = util.cosinterp( r,ri, factor )				
					outg = util.cosinterp( g,gi, factor )				
					outb = util.cosinterp( b,bi, factor )				
				end
				
				image.select( outImage )
				
				if ab > 0.0 then 												
					image.setchannelf( w_,h_,3, a )
					image.setpixelf( w_,h_, outr,outg,outb, 1 )				
				else					
					image.setchannelf( w_,h_,3, 0 )
				end			
			end					
	end	
	
	if pathOut ~= nil then
		image.save( s_getex(pathOut), pathOut )		
		image.erase( outImage )
	end
	
	image.erase( picImg )
	
	image.erase( interpImg )	
	
	
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
		picImg = image.read( pathIdIn, exImage )
	else
		picImg = pathIdIn
	end
	local interpImg = 0
	
	if s_isstring(pathInterpImage) == true  then
		interpImg = image.read( pathInterpImage, s_getex(pathInterpImage) )	
	else
		interpImg = pathInterpImage
	end
	
	image.select( picImg )

	local w = image.width()

	local h = image.height()
	
	
	image.select( interpImg )

	local wi = image.width()

	local hi = image.height()
		
	local outImage = image.create( w, h , "basic" )
	
	local offsetW = 0
	local offsetH = 0
	if bRandomPos == nil or bRandomPos == true then
		offsetW = main.randint(0, wi - w)
		offsetH = main.randint(0, hi - h)
	end	
	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelf( w_, h_) 
				
				image.select( interpImg )					
				
				local ri,gi,bi = image.getpixelf( (w_+ offsetW)%wi, (h_+ offsetH)%hi) 			
								
				local outr = util.cosinterp( r,ri, factor )				
				local outg = util.cosinterp( g,gi, factor )				
				local outb = util.cosinterp( b,bi, factor )				
				
				image.select( outImage )
				
				if a > 0.0 then 												
					image.setchannelf( w_,h_,3, a )
					image.setpixelf( w_,h_, outr,outg,outb, 1 )				
				else					
					image.setchannelf( w_,h_,3, 0 )
				end				
				
			end		
	end
	
	
	if pathOut ~= nil then
		image.save( s_getex(pathOut)	, pathOut )		
		image.erase( outImage )
	end
	
	if s_isstring(pathIdIn) == true then
		image.erase( picImg )
	end
	
	if s_isstring(pathInterpImage) == true then
		image.erase( interpImg )		
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
	
	local picImg = image.read( pathIn, exImage )	
	
	local w = image.width()

	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	local sizeArr = s_arrlength( colorsIn )
	local channel = channelIn
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelf( w_, h_ ) 
				
					
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
					f = main.randfloat()				
				end
				
				if invertIn == true then
					f = 1 - f
				end
				if bAlphaTestIn == false or a > 0.0 then 				
					image.select( outImage )			

					local pos, color = s_getDiscreteColorByFactor( colorsIn,sizeArr, f )				
					
					local bR,bG,bB = util.unpackcolor3( color )
					
					local rMed = util.bytetofloat( bR )
					local gMed = util.bytetofloat( bG )
					local bMed = util.bytetofloat( bB )
		
					local vr = util.cosinterp( r,rMed, factorIn )				
					local vg = util.cosinterp( g,gMed, factorIn )
					local vb = util.cosinterp( b,bMed, factorIn )
					
					image.setpixelf( w_, h_, vr,vg,vb, 1 )					
										
					image.setchannelf( w_, h_,3, a )	
				end	
			end		
	end
	image.select( outImage )
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
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
		
		local interpImg = image.read( pathInterp, imgExIn )
		wi = image.width()
		hi = image.height()
		util.msg(interpImg)
		interpImgIds[i] = interpImg
	end
	
	
	local exImage = s_getex( pathIn )
	
	local exImageOut = s_getex( pathOut )	
	
	local picImg = image.read( pathIn, exImage )	
	
	local w = image.width()
	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	local channel = channelIn
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelf( w_, h_ ) 			
					
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
					f = main.randfloat()				
				end
				
				if invertIn == true then
					f = 1 - f
				end
				if bAlphaTestIn == false or a > 0.0 then 				
								

					local pos, color = s_getDiscreteColorByFactor( colorsIn,sizeArr, f )				
					local key = math.max((pos+1)%filesMax,1)
					
					image.select( interpImgIds[key] )
					local ri,gi,bi,ai = image.getpixelf( w_ % wi, h_ % hi )
					
					local vr = util.cosinterp( r,ri, factorIn )				
					local vg = util.cosinterp( g,gi, factorIn )
					local vb = util.cosinterp( b,bi, factorIn )
					
					image.select( outImage )
					image.setpixelf( w_, h_, vr,vg,vb, 1 )					
										
					image.setchannelf( w_, h_,3, a )	
				end	
			end		
	end
	image.select( outImage )
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
	
	
	for i = 1, #interpImgIds do
		image.erase( i )
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
		
		local interpImg = image.read( pathInterp, imgExIn )
		wi = image.width()
		hi = image.height()
		util.msg(interpImg)
		interpImgIds[i] = interpImg
	end
	
	
	local exImage = s_getex( pathIn )
	
	local exImageOut = s_getex( pathOut )	
	
	local picImg = image.read( pathIn, exImage )	
	
	local w = image.width()
	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	local alphaFore = 1	
	local alphaBack = 0
	
	local channel = channelIn
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelb( w_, h_ ) 			
										
				
				if bAlphaTestIn == false or a > 0 then 				
					local packed = util.c_packcolor(r,g,b)		
					
					local pos = s_findValue(colorsIn, packed)
					--local pos, color = s_getDiscreteColorByFactor( colorsIn,sizeArr, f )				
					if pos ~= -1 then
						local key = math.max((pos+1)%filesMax,1)
						
						image.select( interpImgIds[key] )
						local ri,gi,bi,ai = image.getpixelf( w_ % wi, h_ % hi )
						
						local vr = util.cosinterp( util.bytetofloat(r),ri, factorIn )				
						local vg = util.cosinterp( util.bytetofloat(g),gi, factorIn )
						local vb = util.cosinterp( util.bytetofloat(b),bi, factorIn )
						
						image.select( outImage )
						image.setpixelf( w_, h_, vr,vg,vb, 1 )					
											
						image.setchannelf( w_, h_,3, util.bytetofloat(a) )
					end					
				end	
			end		
	end
	image.select( outImage )
	image.save( exImageOut, pathOut )
	image.erase( picImg )
	
	image.erase( outImage )
	
	
	for i = 1, #interpImgIds do
		image.erase( i )
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

		
	local interpImg = image.read(  toInjectIn, s_getex(toInjectIn) )
	
	local picImageId = 0
	
	if s_isstring(mainImgIn) == true  then		
		picImageId = image.read( mainImgIn, exImage )
	else
		picImageId = mainImgIn
	end
	image.select( picImageId )
	
	local w = image.width()
	local h = image.height()		
	
	image.select( interpImg )
	local wi = image.width()
	local hi = image.height()		
	
	local offsetX = xPosIn
	local offsetY = yPosIn
	
	for w_ = 0, wi-1 do	
			for h_ = 0, hi-1 do		
			
				image.select( picImageId )		
				local posX =  w_ + offsetX
				local posY =  h_ + offsetY
				if posX <  w - 1 and posY <  h - 1 then
				
					local r,g,b,a = image.getpixelf( posX, posY ) 
					
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
					
					image.select( interpImg )					
					
					local ri,gi,bi,af = image.getpixelf(  (w_ + offsetX)%wi, (h_ + offsetY)%hi ) 							
					
					local factorEnd = chFactor * factorIn * af
					
					local outr = util.cosinterp( r, ri, factorEnd )
					local outg = util.cosinterp( g, gi, factorEnd )
					local outb = util.cosinterp( b, bi, factorEnd )
					
					image.select( picImageId )				
					
					image.setpixelf( w_ + offsetX, h_ + offsetY, outr,outg,outb, 1 )	
				end				
				
				
			end		
	end
	
	image.erase( interpImg )	
	util.msg( "finished" )
	
	if s_isstring( mainImgIn ) == true then
		image.select( picImageId )
		image.save( "png", pathOutIn )
		image.erase( picImageId )
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

		
	local channeImgId = image.read(  channelImageIn, s_getex( channelImageIn ) )
	
	local picImageId = 0
	
	--if s_isstring(mainImgIn) == true  then		
		picImageId = image.read( mainImgIn, exImage )
	--else
	--	picImageId = mainImgIn
	--end
	image.select( picImageId )
	
	local w = image.width()
	local h = image.height()		
	
	image.select( channeImgId )
	
	local wi = image.width()
	local hi = image.height()		
	
	local offsetX = xPosIn
	local offsetY = yPosIn
	
	for w_ = 0, w - 1 do	
			for h_ = 0, h - 1 do		
			
				image.select( picImageId )		
				
				local r,g,b,a = image.getpixelf( w_, h_ ) 					
				
				image.select( channeImgId )					
				
				local ri,gi,bi,ai = image.getpixelf(  ( w_ + offsetX ) % wi, ( h_ + offsetY ) % hi ) 		

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
				
				local aout = util.cosinterp( a, chFactor, factorEnd )				
				
				image.select( picImageId )				
				
				image.setpixelf( w_, h_, r,g,b, 1 )	
				image.setchannelf( w_, h_,3, aout )					
			end		
	end
	
	image.erase( channeImgId )	
	util.msg( "s_changeAlphaChannel finished" )
	
	--if s_isstring( mainImgIn ) == true then
		image.select( picImageId )
		image.save( "png", pathOutIn )
		image.erase( picImageId )
	--lse
	--	return picImageId
	--end	
end



