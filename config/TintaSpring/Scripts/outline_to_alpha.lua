require "Scripts/lib/l_util"
require "paths"



--[[
	draws the outline on the alpha edge  rO,gO,bO byte 255
]]--
function outlineToAlpha( imgPath, imgOut, layers )


	local imgId = image.read( imgPath, s_getex(imgPath) )	
	
	
	
	local w = image.width()
	local h = image.height()		
	local outImage = image.create( w, h , "basic" )
	image.fillchannelb( 3,0 )	
	
	
	for i = 1, layers do
		
		if imgId == nil then
			imgId = outImage
			outImage = image.create( w, h , "basic" )
			image.fillchannelb( 3,0 )	
		end
		
		for w_ = 0, w -1 do	
				for h_ = 0, h-1 do				
						
					image.select( imgId )
					local rCur,gCur,bCur,aCur = image.getpixelb( w_, h_ )									
					
					if aCur ~= 0 then
						r,g,b,aL = image.getpixelb( math.max(0,w_ - 1), h_ )
						r,g,b,aLU = image.getpixelb( math.max(0,w_ - 1), math.max(0,h_ - 1) )						
						r,g,b,aU = image.getpixelb( w_, math.max(0,h_ - 1) )
						r,g,b,aRU = image.getpixelb( math.min(w - 1, w_ + 1), math.max(0,h_ - 1) )
						r,g,b,aR = image.getpixelb( math.min(w - 1, w_ + 1), h_ )
						r,g,b,aRD = image.getpixelb( math.min(w - 1, w_ + 1), math.min(h - 1,h_ + 1) )
						r,g,b,aD = image.getpixelb( w_, math.min(h - 1,h_ + 1) )
						r,g,b,aLD = image.getpixelb( math.max(0,w_ - 1), math.min(h - 1,h_ + 1) )
						
						if aL == 0 or aLU == 0 or aU == 0 or aRU == 0 or aR == 0 or aRD == 0 or aD == 0 or aLD == 0 then
							--util.msg(i)
							image.select( outImage )											
							image.setpixelb( w_,h_, rCur,gCur,bCur, 1 )
							image.setchannelb( w_,h_, 3, 0 )							
						else
							image.select( outImage )							
							image.setchannelb( w_,h_, 3, 255 )			
							image.setpixelb( w_,h_, rCur,gCur,bCur, 1 )
						end
					else
							image.select( outImage )												
							image.setpixelb( w_,h_, rCur,gCur,bCur, 1 )
					end				
					
				end		
		end	
		
		image.erase( imgId )
		imgId = nil
	end
	
	image.select( outImage )
	image.save( s_getex( imgOut ), imgOut )		
	
	image.erase( outImage )	

end



PathIn = "C:/foto/3132729095_202181051655667_29611_p.png"

local stages = 4
local layers = 1
for i = 0 , stages  - 1 do
	PathOut = string.format( "%s/%s%d.%s", pic_dir_filtered, "outline_",i, "png" )
	outlineToAlpha( PathIn, PathOut, layers )
	
	PathIn = PathOut
end


util.msg("*****outlineToAlpha finished*****")