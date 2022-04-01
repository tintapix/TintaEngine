require "Scripts/lib/utf8"
require "Scripts/lib/utf8data"
require "paths"

function s_fixAlpha( imgPathIn, imgRezPathIn, delete, fence  )

	
	local imgId = image.read( imgPathIn, "png" )	
	
	local w = image.width()
	local h = image.height()	
			
	--local outImage = image.create( w, h , "basic" )
	--image.fillchannelf( 3,1 )	
		
		
	--local pixAround = pixAroundIn * 2 + 1
	--local factorMax = ( pixAround * pixAround ) - 1
				
	for w_ = 0, w -1 do	
			for h_ = 0, h-1 do		
							
				local r,g,b,a = image.getpixelb( w_, h_ )					

				if delete == false and (a > fence or a == fence) then																
					image.setchannelb( w_,h_, 3, 255 )	
				else if a < fence then
					image.setchannelb( w_,h_, 3, 0 )
				end

			end		
	end	
	
	
	function save()
		local name = main.uniqid()
		--local name = "out"
		local PathOut = string.format("%s/%s.%s", pic_dir_filtered, name, "png" )
		image.save( "png", PathOut )
		util.msg("saved")
		
		image.eraseall()
	end


	window.setkey( 0x1D, "save()" ) -- ML_LCONTROL    = 0x1D,	

	window.redraw()
	
	
	
	util.msg("s_fixAlpha finished")
	
end
local delete = true

--[[
	уравнивает все не крайние значения a канала
]]--
local edge = 255/2

if delete == true then
	edge = 255
else
	edge = 0
end

s_fixAlpha( "c:/foto/eye_out2.png", "c:/foto/rez/eye_out.png" , delete , edge )