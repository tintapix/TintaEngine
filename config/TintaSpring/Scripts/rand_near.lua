require "Scripts/lib/utf8"
require "Scripts/lib/utf8data"
require "Scripts/lib/l_util"
require "paths"


function s_arrlength(T)
  local countOut = 0
  for _ in pairs(T) do countOut = countOut + 1 end
  return countOut
end


function s_randomNear( PathIn , PathOut )

	local picImg = image.read( PathIn, s_getex(PathIn) )	
	
	local w = image.width()

	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
		for h_ = 0, h-1 do		
		
			image.select( picImg )				
			
			local xPos, yPos = s_posByDirection8( w_, h_, main.randint( 1,8 ), 1, w-1, h-1 )	
			local r,g,b,a = image.getpixelf( xPos, yPos )
			
			image.select( outImage )	
			image.setchannelf( w_, h_, 3, a )
			image.setpixelf( w_, h_ , r,g,b, 1.0)					
			
		end		
	end
		
	image.erase( picImg )	
	
	if PathOut ~= nil then
		image.select( outImage )	
		image.save( s_getex( PathOut ), PathOut )	
		image.erase( outImage )
	else	
		return outImage;
	end	
	
end

function s_randomNear2( PathIn , PathOut )

	local picImg = image.read( PathIn, s_getex(PathIn) )	
	
	local w = image.width()

	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
		for h_ = 0, h-1 do		
		
			image.select( picImg )				
			
			local xPos, yPos = s_posByDirection4( w_, h_, main.randint( 1,4 ), 1, w-1, h-1 )	
			local r,g,b,a = image.getpixelf( xPos, yPos )
			
			image.select( outImage )	
			image.setchannelf( w_, h_, 3, a )
			image.setpixelf( w_, h_ , r,g,b, 1.0)					
			
		end		
	end
		
	image.erase( picImg )	
	
	if PathOut ~= nil then
		image.select( outImage )	
		image.save( s_getex( PathOut ), PathOut )	
		image.erase( outImage )
	else	
		return outImage;
	end	
	
end

local outExt = "png"
local PathIn = "C:/foto/IMG_20190713_165410.jpg"
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, main.uniqid(), outExt )
s_randomNear2( PathIn , PathOut )
util.msg("finished")
