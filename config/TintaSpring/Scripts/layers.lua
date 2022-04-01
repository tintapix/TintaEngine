require "paths"

function s_arrlength(T)
  local countOut = 0
  for _ in pairs(T) do countOut = countOut + 1 end
  return countOut
end

function isWhite( r,g, b)
	return r == 255 and g == 255 and b == 255

end

function layers( PathIn , whiteBack, addPixel )


	local ex = 	"png"
	local picImg = image.read( PathIn ,ex )


	local w = image.width()
	local h = image.height()	

	local maxLen = 0

	local colors = {} 

	for h_ = 0, h-1 do	
			for w_ = 0, w-1 do							
			
				local r,g,b,a = image.getpixelb( w_, h_ )
				
				if a > 0 then						
				
					local packed = util.packcolor3( r,g,b )
					if colors[packed] == nil then						
							colors[packed] = packed						
					end
					
				end							
			
			end		
	end

	local count = 0
	util.msg(s_arrlength(colors))
	for i_,v_ in pairs(colors) do 

		
		local outImg = image.create( w, h , "basic" )	
		
		image.fillb(255, 255, 255)
		
		if whiteBack == true then		
			image.fillchannelf( 3,1 )
		else
			image.fillchannelf( 3,0 )
		end
			
		for h_ = 0, h - 1 do	
			for w_ = 0, w - 1 do							
		
				image.select( picImg )
				
				local r,g,b,a = image.getpixelb( w_, h_ )	
				
				if a > 0 then						
				
					local packed = util.packcolor3( r,g,b )
					
					image.select( outImg )
					
					if packed == v_ then						
						image.setpixelb( w_, h_, 0,0,0, 1 )		
						image.setchannelb( w_, h_,3, 255 )		
					end
					
				end							
			
			end		
		end
		
		image.select(outImg)
		
		if addPixel == true then 
		
			local checkImage = image.clone( )
		
			for w_ = 0, w -1 do	
					for h_ = 0, h-1 do				
							
						image.select(checkImage)
						local rCur,gCur,bCur,aCur = image.getpixelb( w_, h_ )									
						
						local check = false
						
						if whiteBack == true then
							check = (rCur == 255 and gCur == 255  and bCur == 255)
						else
							check = (image.getchannelb( w_,h_, 3 ) == 0) 
						end
						
						if  check == true then
							rL,gL,bL,aL = image.getpixelb( math.max(0,w_ - 1), h_ )
							rLU,gLU,bLU,aLU = image.getpixelb( math.max(0,w_ - 1), math.max(0,h_ - 1) )						
							rU,gU,bU,aU = image.getpixelb( w_, math.max(0,h_ - 1) )
							rRU,gRU,bRU,aRU = image.getpixelb( math.min(w - 1, w_ + 1), math.max(0,h_ - 1) )
							rR,gR,bR,aR = image.getpixelb( math.min(w - 1, w_ + 1), h_ )
							rRD,gRD,bRD,aRD = image.getpixelb( math.min(w - 1, w_ + 1), math.min(h - 1,h_ + 1) )
							rD,gD,bD,aD = image.getpixelb( w_, math.min(h - 1,h_ + 1) )
							rLD,gLD,bLD,aLD = image.getpixelb( math.max(0,w_ - 1), math.min(h - 1,h_ + 1) )
							
							
							if isWhite(rL,gL,bL) == false or isWhite(rLU,gLU,bLU) == false or isWhite(rRU,gRU,bRU) == false or isWhite(rR,gR,bR) == false or isWhite(rRD,gRD,bRD) == false or isWhite(rD,gD,bD) == false or isWhite(rLD,gLD,bLD) == false  then
								image.select(outImg)
								if whiteBack == true then		
									image.setpixelb( w_,h_, 0,0,0, 1) 	
								else
									image.setchannelf( w_,h_, 3,1.0 )
									image.setpixelb( w_,h_,rCur,gCur,bCur)
								end
													
							end						
						end		
							
					end		
			end	
			
			image.erase( checkImage )
		end
		
		local PathOut = string.format("%s/%s%d.%s", pic_dir_filtered, "layer_",count, ex )
		image.select(outImg)
		image.save( ex, PathOut )
		image.erase( outImg )
		
		count = count + 1
		
	end

	image.erase( picImg )	

	util.msg("finished")


end

local imgPath = "c:/foto/bitcoin.png"
layers( imgPath , true, false )