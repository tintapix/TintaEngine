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
		picImg = c_readimg( pathIn,  s_getex( pathIn ) )
	else
		picImg = pathIn
	end	
	
	c_selimg( picImg )	
	
	local w = c_getwidth()
	local h = c_getheight()
	
	local picImgMap = nil
	
	if  s_isstring(pathInMap) == true then			
		picImgMap = c_readimg( pathInMap, s_getex( pathInMap ) )	
	else
		picImgMap = pathInMap
	end
	
	c_selimg( picImgMap )
	
	local wM = c_getwidth()
	local hM = c_getheight()
	
	local x = (wM - 1)/(w - 1)
	local y = (hM - 1)/(h - 1)
	
	local rInterp = util.bytetofloat(rIn)
	local gInterp = util.bytetofloat(gIn)
	local bInterp = util.bytetofloat(bIn)

	local outImage = c_createimg( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				-- take color values
				c_selimg( picImg )				
				local rVal,gVal,bVal,aVal = c_getpixelf( w_, h_ )				
				
				-- take factor values from map
				c_selimg( picImgMap )				
				local r,g,b,a = c_getpixelf( math.floor(w_ * x), math.floor(h_ * y) )
				
				c_selimg( outImage )				
					
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
				c_setpixelf( w_,h_, util.cosinterp(r,rInterp,factor),util.cosinterp(g,gInterp,factor),util.cosinterp(b,bInterp,factor), 1 )			
				
			end		
	end
		
	if  s_isstring(pathIn) == true	then		
		c_delimg( picImg )			
	end
	if  s_isstring(pathInMap) == true	then		
		c_delimg( picImgMap )			
	end	
	
	if pathOut ~= nil then
		c_selimg( outImage )
		--c_saveimg( s_getex( pathOut ), pathOut )
		c_delimg( outImage )
	else
		return outImage
	end
	
	
	util.msg("***s_interpToColorChannelMap finished")
end
