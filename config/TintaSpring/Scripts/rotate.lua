require "Scripts/lib/l_util"
require "paths"

function s_rotateImage(imgSrcIn, imgPathRezIn, angleDegree )

	
	local imgId = 0
	
	if s_isstring(imgSrcIn) == true  then
		imgId = image.read( imgSrcIn, s_getex(imgSrcIn) )		
	else
		imgId = imgSrcIn
	end
	
	local w = image.width()
	local h = image.height()
	
	local r = math.ceil( math.sqrt(w*w + h*h) )
	
	
	
	util.msg(w, " ", h," ", r )
	
	local outImage = image.create( r, r, "basic" )		
	
	local xOffset = math.ceil(w / 2)
	local yOffset = math.ceil(h / 2)
	
	
	for w_ = 0, w - 1 do	
	
		for h_ = 0, h - 1 do	
		
				image.select( imgId )
				r,g,b,a = image.getpixelb( w_, h_ )
				
				s = math.sin( angleDegree * 3.14159265358979 / 180.0 )
				c = math.cos( angleDegree * 3.14159265358979 / 180.0 )		

				xOrigin = w_ - xOffset
				yOrigin = yOffset - h_ 
				
				xNew = xOrigin * c  + yOrigin * s
				yNew = -xOrigin * s + yOrigin * c
				
				image.select( outImage )		
				--util.msg( xNew ," ",yNew )
				
				
				xEnd = math.floor(xNew + w)				
				yEnd = math.floor(yNew + h)
				
				image.setpixelb( xEnd, yEnd, r,g,b, 1 )
				image.setchannelb( xEnd, yEnd, 3 , a )
				
				
				xEnd = math.ceil(xNew + w)				
				yEnd = math.ceil(yNew + h)
				
				image.setpixelb( xEnd, yEnd, r,g,b, 1 )
				image.setchannelb( xEnd, yEnd, 3 , a )
				
				
				
			
				
		end			
		
	end
	
	
	
	--[[
	for w_ = 0, xSizeIn - 1 do	
			for h_ = 0, ySizeIn - 1 do		
			
				image.select( imgId )	
				
				local r,g,b,a = image.getpixelb( math.floor(w_ * factorX), math.floor(h_ * factorY) )												
				image.select( outImage )
				
				image.setpixelb( w_,h_, r,g,b, 1 )
				image.setchannelb( w_,h_, 3, a )				
							
			end		
	end	
	
	
	
	]]--
	
	--image.select( imgId )
	c_priorcolor( 4	, 0, 1,true  )
	
	if imgPathRezIn ~= nil then
		image.select( outImage )
		image.save( s_getex( imgPathRezIn ), imgPathRezIn )
		image.erase(outImage)
	else
		return outImage
	end
	
	
	
	if s_isstring(imgSrcIn) == true  then
		image.erase(imgId)
	end
	
	
	util.msg("s_rotateImage finished")
end




local path = "c:/foto/test2.png" 
local PathOut = "c:/foto/rez/rotate.png"


s_rotateImage(path, PathOut, 15 )



util.msg("*****finished*****")