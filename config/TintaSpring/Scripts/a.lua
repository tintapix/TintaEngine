require "Scripts/lib/l_util"
require "paths"

--[[
	Interpolates src image to the InterpImage with alpha test (only a > 0)
	pathIn -  path or id to the src image	
	pathInterpImage - interp image path		
	bRandomPos - if true - position is random
]]--
function s_movecolorMask( pathIdIn,  pathMaskImage ,  rIn,gIn,bIn, factor, bRandomPos )

	image.eraseall()
	local picImg = 0
	
	if s_isstring(pathIdIn) == true  then
		local exImage = s_getex(pathIdIn)	
		picImg = image.read( pathIdIn, exImage )
	else
		picImg = pathIdIn
	end
	local interpImg = 0
	
	if s_isstring(pathMaskImage) == true  then
		interpImg = image.read( pathMaskImage, s_getex(pathMaskImage) )	
	else
		interpImg = pathMaskImage
	end
	
	image.select( picImg )

	local w = image.width()

	local h = image.height()
	
	
	image.select( interpImg )

	local wi = image.width()

	local hi = image.height()	
	
	
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
				--local rb,gb,bb = image.getpixelb( w_, h_)
				
				image.select( interpImg )					
				
				local ri,gi,bi,ai = image.getpixelf( (w_+ offsetW)%wi, (h_+ offsetH)%hi) 			
								
				local outr = math.max(r - ai * r * factor,0.0);
				local outg = math.max(g - ai * r * factor,0.0);			
				local outb = math.max(b - ai * r * factor,0.0); 							
				--util.msg(r," ", ri, "    ", outr , "     ", (1.0 - s_dist3(rIn,gIn,bIn, rb,gb,bb )/maxColorDist))
				image.select( picImg )
				
				image.setpixelf( w_,h_, outr,outg,outb, 1 )		
				
				--[[if a > 0.0 then 												
					image.setchannelf( w_,h_,3, a )
					image.setpixelf( w_,h_, outr,outg,outb, 1 )				
				else					
					image.setchannelf( w_,h_,3, 0 )
				end		]]--		
				
			end		
	end
	
	
	
	
	
	if s_isstring(pathInterpImage) == true then
		image.erase( interpImg )		
	end
	
	function save()
		image.select( picImg )
		local name = main.uniqid()
		--local name = "out"
		local PathOut = string.format("%s/%s.%s", pic_dir_filtered, name,  "png" )
		image.save( "png", PathOut )
		util.msg("saved")
		
		image.eraseall()
	end


	window.setkey(0x1D, "save()") -- ML_LCONTROL    = 0x1D,	

	window.redraw()
	util.msg("finished")
	
end
--441.6729
--util.msg(s_dist3(0.0,0.0,0.0, 255.0, 255.0, 255.0))
s_movecolorMask( "c:/foto/back1.png", "c:/foto/eye_out.png" ,  0,0,0, 0.4,  false )