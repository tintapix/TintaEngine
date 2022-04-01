require "Scripts/lib/l_util"
require "paths"



function outlineToGradient( imgPath, imgOut, layers, palleteName )



	--local nameColors = palleteName
	

	local r = {}

	local arrColor = nil 

	if palleteName ~= nil then
		arrColor = s_getpallete(palleteName)
	end
	
	local arrAllColors = s_buildColorArray( arrColor, layers )
	
	local imgId = image.read( imgPath, s_getex(imgPath) )	
	
	local w = image.width()
	local h = image.height()		
	
	local outImage = image.read( imgPath, s_getex(imgPath) )	
	
	
	for i = 1, layers do
		
		--[[if imgId == nil then
			imgId = outImage
			outImage = image.create( w, h , "basic" )
			image.fillchannelb( 3,0 )	
		end
		]]--
		local boxId = box.create("int32_vector")
		
		for w_ = 0, w -1 do	
				for h_ = 0, h-1 do				
					
					image.select( imgId )
					local rCur,gCur,bCur,aCur = image.getpixelb( w_, h_ )									
					
					if aCur ~= 0 then -- not alpha area
						r,g,b,aL = image.getpixelb( math.max(0,w_ - 1), h_ )
						r,g,b,aLU = image.getpixelb( math.max(0,w_ - 1), math.max(0,h_ - 1) )						
						r,g,b,aU = image.getpixelb( w_, math.max(0,h_ - 1) )
						r,g,b,aRU = image.getpixelb( math.min(w - 1, w_ + 1), math.max(0,h_ - 1) )
						r,g,b,aR = image.getpixelb( math.min(w - 1, w_ + 1), h_ )
						r,g,b,aRD = image.getpixelb( math.min(w - 1, w_ + 1), math.min(h - 1,h_ + 1) )
						r,g,b,aD = image.getpixelb( w_, math.min(h - 1,h_ + 1) )
						r,g,b,aLD = image.getpixelb( math.max(0,w_ - 1), math.min(h - 1,h_ + 1) )
						
						if aL == 0 or aLU == 0 or aU == 0 or aRU == 0 or aR == 0 or aRD == 0 or aD == 0 or aLD == 0 then
							-- edge
							image.select( outImage )													
							local bR,bG,bB = util.unpackcolor3( arrAllColors[i] )													
							image.setpixelb( w_,h_, bR,bG,bB, 1 )							
							--util.msg(" ",i," ",bR," ",bG, " " ,bB)
							--image.select( imgId )
							--image.setchannelb( w_,h_, 3, 0 )													
							box.pushbacki32(boxId, w_  )
							box.pushbacki32(boxId, h_  )
						end					
					
					end				
					
				end		
		end	
		
		local eBox = box.elements(boxId)
		image.select( imgId )
		
		local key = 0
		for i = 0, eBox/2 - 1 do
			image.setchannelb( box.getvali32(boxId,key),box.getvali32(boxId,key + 1), 3, 0 )	
			key = key + 2
		end
		
		box.erase(boxId)		
		
		--image.erase( imgId )
		--imgId = nil
	end
	
	image.select( outImage )
	image.save( s_getex( imgOut ), imgOut )		
	image.erase( imgId )
	image.erase( outImage )		

end



PathIn = "C:/foto/p1_1.png"


local layers = 40
local pallete = "2.png"
PathOut = string.format( "%s/%s.%s", pic_dir_filtered, "outline_gradient_", "png" )
outlineToGradient( PathIn, PathOut, layers, pallete)
	
--PathIn = PathOut



util.msg("*****outlineToGradient finished*****")