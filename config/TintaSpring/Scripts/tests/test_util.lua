require "Scripts/lib/utf8"
require "Scripts/lib/utf8data"

--[[
	Changes color to another by channel factor, using map image
	
	channelIn - 0 - r
	channelIn - 1 - g
	channelIn - 2 - b
	channelIn - 3 - a	
	channelIn - 4 - light
]]--
function s_interpToColorChannelMap( pathIn,pathInMap,pathOut, rIn,gIn,bIn, channelIn, intensity )

	if  s_isstring(pathIn) == true then			
		picImg = image.read( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end	
	
	image.select( picImg )	
	
	local w = image.width()
	local h = image.height()
	
	local picImgMap = nil
	
	if  s_isstring(pathInMap) == true then			
		picImgMap = image.read( pathInMap, s_getex( pathInMap ) )	
	else
		picImgMap = pathInMap
	end
	
	image.select( picImgMap )
	
	local wM = image.width()
	local hM = image.height()
	
	local x = (wM - 1)/(w - 1)
	local y = (hM - 1)/(h - 1)
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				-- take color values
				image.select( picImg )				
				local rVal,gVal,bVal,aVal = image.getpixelf( w_, h_ )				
				
				-- take factor values from map
				image.select( picImgMap )				
				local r,g,b,a = image.getpixelf( math.floor(w_ * x), math.floor(h_ * y) )
				
				image.select( outImage )				
					
				c_setalphaf( w_,h_, aVal )		

				if channelIn ==  0 then
					factor = r					
				elseif channelIn ==  1 then
					factor = g
				elseif channelIn ==  2 then
					factor = b
				elseif channelIn ==  3 then
					factor = a
				elseif channelIn ==  4 then
					local h,s,l = util.rgbtohsl( rVal,gVal,bVal )
					factor = l					
				end		
				factor = factor * intensity
				image.setpixelf( w_,h_, util.cosinterp(r,rInterp,factor),util.cosinterp(g,gInterp,factor),util.cosinterp(b,bInterp,factor), 1 )			
				
			end		
	end
		
	if  s_isstring(pathIn) == true	then		
		image.erase( picImg )			
	end
	if  s_isstring(pathInMap) == true	then		
		image.erase( picImgMap )			
	end	
	
	if pathOut ~= nil then
		image.select( outImage )
		--image.save( s_getex( pathOut ), pathOut )
		image.erase( outImage )
	else
		return outImage
	end
	
	
	util.msg("***s_interpToColorChannelMap finished")
end
