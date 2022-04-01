require "Scripts/lib/utf8"
require "Scripts/lib/utf8data"
require "Scripts/lib/l_util"
require "paths"


--randomise pixels

function s_randomnearpixels( pathIn, pathDestIn, step  )

	local picImg = 0 

	
	if s_isstring(pathIn) == true  then
		picImg = image.read( pathIn ,s_getex(pathIn) )
	else
		picImg = pathIn
	end
	
	
	local w = image.width()
	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										

				local xPos, yPos = s_posByDirection8( w_, h_, main.randint( 1,8 ), step, w-1, h-1)	
				
				local r,g,b,a = image.getpixelb( xPos, yPos)			
				
				image.select( outImage )
				image.setpixelb( w_, h_, r,g,b, 1 )				
									
				image.setchannelb( w_,h_,3, a )				
				
			end		
	end
	
	image.save( s_getex(pathDestIn), pathDestIn )
	
	if s_isstring(pathIn) == true then
		image.erase( picImg )
	end
	
	image.erase( outImage )
	util.msg("finished")
end

function s_randompixels( pathIn, pathDestIn , rDivMinIn, rDivMaxIn, gDivMinIn, gDivMaxIn, bDivMinIn, bDivMaxIn )

	local picImg = 0 

	
	if s_isstring(pathIn) == true  then
		picImg = image.read( pathIn ,s_getex(pathIn) )
	else
		picImg = pathIn
	end
	
	
	local w = image.width()
	local h = image.height()

	local outImage = image.create( w, h , "basic" )	
	
	for w_ = 0, w-1 do	
			for h_ = 0, h-1 do		
			
				image.select( picImg )										
				
				local r,g,b,a = image.getpixelb( w_, h_) 													
				
				image.select( outImage )
				
				local outr = s_randChannelNear(r, rDivMinIn, rDivMaxIn )
				local outg = s_randChannelNear(g, gDivMinIn, gDivMaxIn )
				local outb = s_randChannelNear(b, bDivMinIn, bDivMaxIn )
				
				image.setpixelb( w_, h_, outr,outg,outb, 1 )				
									
				image.setchannelf( w_,h_,3, util.bytetofloat( a ) )				
				
			end		
	end
	
	image.save( s_getex(pathDestIn), pathDestIn )
	
	if s_isstring(pathIn) == true then
		image.erase( picImg )
	end	
	
	image.erase( outImage )
	util.msg("finished")
end

local PathIn = "C:/foto/abducted.png"
local ext = s_getex(PathIn)
local PathOut = string.format("%s/%s.%s", pic_dir_filtered, main.uniqid(), ext )
--function s_randpixels( pathIn, pathDestIn , rDivMinIn, rDivMaxIn, gDivMinIn, gDivMaxIn, bDivMinIn, bDivMaxIn )
local factor = 20

s_randompixels( PathIn , PathOut, -factor, factor,-factor, factor,-factor, factor )

--s_randomnearpixels(PathIn , PathOut, 4)