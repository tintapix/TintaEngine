require "Scripts/lib/l_util"
require "Scripts/lib/patterns"

require "paths"

local begTime = os.clock()
s_printTimePoint(begTime, 0)

-- что бы не забивать в генератор случайных чисел опять os.time() делаем это только один раз
if seedInit == nil then
	-- инициализируем генератор случайных чисел
	math.randomseed( os.time() )
	util.msg( os.time() )
	seedInit = true
end


function s_path( imgPathIn, imgRezPathIn )

	
	local imgId = image.read( imgPathIn, s_getex(imgPathIn) )	
	
	local w = image.width()
	local h = image.height()	
	local outImage = image.create( w, h , "basic" )	
	c_fillalphaf( 0 )
	image.fillb( 255,255,255 )
	
	local weigts = {}
	local coord = {key,x,y}
	local positions = { coord }
	
	positions[10] = {}
	positions[10].key = 10
	positions[10].x = 100
	positions[10].y = 200
	
	util.msg(positions[10].key)
	--[[
		for w_ = 0, w -1 do	
				for h_ = 0, h-1 do		
				
					image.select( imgId )
					local r,g,b,a = image.getpixelb( w_, h_ )									
					
					if a == 0 then
						image.select( outImage )
						--image.setpixelb(w_ - 1, h_, r,g,b, 1)
						--c_setalphab(w_ - 1, h_,255)
						
						image.setpixelb(w_ - 1, h_- 1 , r,g,b, 1)
						c_setalphab(w_ - 1, h_- 1,255)
						
						image.setpixelb(w_ + 1, h_- 1 , r,g,b, 1)
						c_setalphab(w_ + 1, h_- 1,255)
						
						image.setpixelb(w_ + 1, h_+ 1 , r,g,b, 1)
						c_setalphab(w_ + 1, h_+ 1,255)
						
						image.setpixelb(w_ - 1, h_+ 1 , r,g,b, 1)
						c_setalphab(w_ - 1, h_+ 1,255)
						
						--image.setpixelb(w_ + 1, h_, r,g,b, 1)
						--c_setalphab(w_ + 1, h_,255)
						
						--image.setpixelb(w_, h_ - 1, r,g,b, 1)
						--c_setalphab(w_, h_ - 1,255)
						
						--image.setpixelb(w_, h_+ 1, r,g,b, 1)
						--c_setalphab(w_, h_+ 1,255)
						
					end
				end		
		end	
	--end
	image.select( outImage )
	image.save( s_getex( imgRezPathIn ), imgRezPathIn )		
	image.erase( outImage )
	
	image.erase( imgId )	
	]]--
	
end

local PathIn = "C:/foto/bat2.png"
local ext = s_getex(PathIn)
local PathOut = string.format( "%s/%s.%s", pic_dir_filtered, "bat_out", ext )

s_path( PathIn, PathOut )

--s_grow(PathIn,PathIn)
s_printTimePoint( begTime, 1 )